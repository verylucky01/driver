#!/usr/bin/env python3
# -*- coding: UTF-8 -*-
# ------------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ------------------------------------------------------------------------------------------------------------

import copy
import glob
import hashlib
import itertools
import re
import xml.etree.ElementTree as ET
import os
import sys
from argparse import Namespace
from functools import partial
from io import StringIO
from itertools import chain, filterfalse
from operator import attrgetter, itemgetter, methodcaller
from typing import (
    Any, Callable, Dict, Iterable, Iterator, List, NamedTuple, Optional, Set, Tuple, Union
)

from .utils import pkg_utils
from .filelist import FileItem, FileList, fill_is_common_path
from .utils.pkg_utils import (
    ContainAsteriskError, FAIL, BLOCK_CONFIG_PATH,
    BlockConfigError, EnvNotSupported, IllegalVersionDir, PackageError,
    ParseOsArchError, config_feature_to_set,
    flatten, star_pipe, merge_dict, yield_if, ErrMsgs, each_file_line, strip_lines
)
from .utils.funcbase import constant, dispatch, invoke, pipe, star_apply
from .utils.comm_log import CommLog
from .version_info import (
    VersionInfo, VersionXml, VersionFormatNotMatch, is_multi_version, VersionInfoFile
)

EnvDict = Dict[str, str]

FileInfo = Dict[str, str]

PackageAttr = Dict[str, Union[str, bool]]

GenerateInfo = Dict[str, str]


class ParseOption(NamedTuple):
    os_arch: Optional[str]
    pkg_version: Optional[str]
    build_type: Optional[str]
    package_check: bool
    ext_name: str = ''


def parse_os_arch(os_arch: str) -> Tuple[str, str, str]:
    match = re.match("^([a-z]+)(\\d+(\\.\\d+)*)?[.-]?(\\S*)", os_arch)
    if match:
        os_name = match.group(1)
        os_ver = match.group(2)
        if match.group(4):
            arch = match.group(4)
        else:
            arch = 'aarch64'

        return os_name, os_ver, arch

    raise ParseOsArchError()


def replace_env(env_dict: EnvDict, in_str: str):
    env_list = re.findall(".*?\\$\\((.*?)\\).*?", in_str)
    for env in env_list:
        if env == 'FILE':
            continue
        if env in env_dict:
            if env_dict[env] is not None:
                in_str = in_str.replace(f"$({env})", env_dict[env])
            else:
                in_str = in_str.replace(f"$({env})", '')
        else:
            raise EnvNotSupported(f"Error: {env} not supported.")
    return in_str


class ParseEnv(NamedTuple):
    env_dict: EnvDict
    parse_option: ParseOption
    delivery_dir: str
    top_dir: str


class BlockElement(NamedTuple):
    name: str
    block_conf_path: str
    dst_path: str
    chips: Set[str]
    features: Set[str]
    attrs: Dict[str, str]

BLOCK_ELEMENT_PASS_THROUGH_ARGS = ['dst_path', 'chips', 'features', 'attrs']


class LoadedBlockElement(NamedTuple):
    root_ele: ET.Element
    use_move: bool
    dst_path: str
    chips: Set[str]
    features: Set[str]
    attrs: Dict[str, str]


class FileInfoParsedResult(NamedTuple):
    file_info: FileInfo
    move_infos: List[FileInfo]
    dir_infos: List[Dict[str, str]]
    expand_infos: List[Dict[str, str]]


class BlockConfig(NamedTuple):

    dir_install_list: List[Dict]
    move_files: List[FileInfo]
    expand_content_list: List[Dict]
    package_content_list: List[Dict]
    generate_infos: List[GenerateInfo]


class PackerConfig(NamedTuple):
    fill_is_common_path: Callable[[FileList], Iterator[FileItem]]


class XmlConfig(NamedTuple):
    default_config: Dict[str, str]
    package_attr: PackageAttr
    version_info: VersionInfo
    blocks: List[BlockConfig]
    version: str
    version_xml: Optional[VersionXml]
    packer_config: PackerConfig

    def _collect_list(self, list_name):
        result = []
        for block in self.blocks:
            result.extend(getattr(block, list_name))
        return result

    @property
    def dir_install_list(self):
        return self._collect_list('dir_install_list')

    @property
    def move_content_list(self):
        return self._collect_list('move_files')

    @property
    def expand_content_list(self):
        return self._collect_list('expand_content_list')

    @property
    def package_content_list(self):
        return self._collect_list('package_content_list')

    @property
    def generate_infos(self) -> List[GenerateInfo]:
        return self._collect_list('generate_infos')

DEFAULT_PACKAGE_ATTR = {
    'gen_version_info': True,
}


def parse_package_info(package_info_ele: Optional[ET.Element]) -> Dict:
    def get_package_info_attrs(ele: ET.Element) -> Iterator[Tuple[str, Union[str, bool]]]:

        bool_attrs = (
            'expand_asterisk', 'parallel', 'parallel_limit', 'package_check', 'check_features',
            'use_move', 'gen_version_info'
        )
        bool_values = ('t', 'true', 'y', 'yes')
        if ele.tag in bool_attrs:
            if ele.text.lower() in bool_values:
                yield ele.tag, True
            else:
                yield ele.tag, False
        else:
            yield ele.tag, ele.text

    if not package_info_ele:
        return {}

    attr = dict(
        chain.from_iterable(map(get_package_info_attrs, list(package_info_ele)))
    )

    return attr

skip_empty_lines = partial(filter, bool)

skip_comment_lines = partial(filterfalse, methodcaller('startswith', '#'))

skip_empty_and_comment_lines = pipe(
    skip_empty_lines,
    skip_comment_lines,
)


def load_itf_version_conf(itf_conf: str,
                          replace_env_func: Callable[[str], str]) -> List[str]:
    load_func = pipe(
        each_file_line,
        strip_lines,
        skip_empty_and_comment_lines,
        partial(map, replace_env_func),
        list,
    )
    return load_func(itf_conf)


def make_loaded_block_element(root_ele: ET.Element,
                              dst_path: str = None) -> LoadedBlockElement:
    if not dst_path:
        new_dst_path = ''
    else:
        new_dst_path = dst_path

    return LoadedBlockElement(root_ele, False, new_dst_path, set(), set(), set(), {})


def parse_install_version_info(attr_info: Dict[str, str]) -> Tuple[bool, Dict]:
    if attr_info.get('install') == 'true':
        return True, attr_info
    return False, None


def parse_itf_version_paths(version_info_ele: Optional[ET.Element]
                            ) -> Tuple[ErrMsgs, List[str]]:
    def has_itf_version(ele: ET.Element) -> bool:
        return bool(ele.attrib.get('itf_version'))

    if version_info_ele is None:
        return [], []

    interface_eles = version_info_ele.findall('./interface')

    err_msgs = [
        'interface element doesn\'t have itf_version attribute!'
        for ele in interface_eles if not has_itf_version(ele)
    ]

    itf_version_paths = [
        ele.attrib['itf_version']
        for ele in interface_eles if has_itf_version(ele)
    ]

    return err_msgs, itf_version_paths


def parse_itf_versions(version_info_ele: Optional[ET.Element],
                       top_dir: str,
                       replace_env_func: Callable[[str], str]) -> Tuple[ErrMsgs, List[str]]:

    def itf_conf_exists(path: str) -> bool:
        return os.path.isfile(os.path.join(top_dir, path))

    err_msgs, itf_version_paths = parse_itf_version_paths(version_info_ele)
    if err_msgs:
        return err_msgs, []

    err_msgs = [
        f'{path} doesn\'t exist!'
        for path in itf_version_paths if not itf_conf_exists(path)
    ]
    if err_msgs:
        return err_msgs, []

    itf_versions = flatten([
        load_itf_version_conf(os.path.join(top_dir, path), replace_env_func)
        for path in itf_version_paths
    ])

    return err_msgs, itf_versions


def parse_version_info_by_root(root_ele: ET.Element,
                               env_dict: EnvDict,
                               version: str,
                               version_dir: Optional[str],
                               version_xml: Optional[VersionXml],
                               timestamp: str,
                               ) -> Tuple[ErrMsgs, VersionInfo]:
    version_info_ele = root_ele.find('version_info')
    evaluate_info_func = partial(
        evaluate_info, loaded_block=make_loaded_block_element(root_ele), env_dict=env_dict
    )

    if version_info_ele is None:
        err_msgs, itf_versions = [], []
        attr_info = {}
        dst_path = ''
    else:
        err_msgs, itf_versions = parse_itf_versions(
            version_info_ele, pkg_utils.TOP_DIR, partial(replace_env, env_dict)
        )
        attr_info = evaluate_info_func(version_info_ele.attrib)
        dst_path = attr_info.get('dst_path', '')

    version_info = VersionInfo(
        *parse_install_version_info(attr_info),
        itf_versions,
        version,
        version_xml,
        timestamp
    )

    return err_msgs, version_info


def parse_package_attr_by_args(args: Namespace) -> Dict:

    def pairs():
        if hasattr(args, 'chip_name') and args.chip_name:
            yield 'chip_name', args.chip_name
        if hasattr(args, 'suffix') and args.suffix:
            yield 'suffix', args.suffix
        if hasattr(args, 'func_name') and args.func_name:
            yield 'func_name', args.func_name

    return dict(pairs())


def parse_package_attr(root_ele: ET.Element, args: Namespace) -> Dict:
    package_info_ele = root_ele.find("package_info")
    return merge_dict(
        DEFAULT_PACKAGE_ATTR,
        parse_package_info(package_info_ele),
        parse_package_attr_by_args(args),
    )


def render_cann_version(a_ver: int,
                        b_ver: int,
                        c_ver: Optional[int],
                        d_ver: Optional[int],
                        e_ver: Optional[int],
                        f_ver: Optional[int]) -> str:
    buffer = StringIO()
    buffer.write('(')
    buffer.write(f'({a_ver + 1} * 100000000) + ({b_ver + 1} * 1000000)')
    if c_ver is not None:
        buffer.write(f' + ({c_ver + 1} * 10000)')
    if d_ver is not None:
        buffer.write(f' + (({d_ver + 1} * 100) + 5000)')
    if e_ver is not None:
        buffer.write(f' + ({e_ver + 1} * 100)')
    if f_ver is not None:
        buffer.write(f' + {f_ver}')
    buffer.write(')')
    return buffer.getvalue()


def get_cann_version(version_dir: str) -> str:
    if not version_dir:
        return ""

    release_pattern = re.compile(r'(\d+)\.(\d+)\.(\d+)', re.IGNORECASE)
    matched = release_pattern.fullmatch(version_dir)
    if matched:
        return render_cann_version(
            int(matched.group(1)), int(matched.group(2)), int(matched.group(3)), None, None, None
        )

    release_alpha_pattern = re.compile(r'(\d+)\.(\d+)\.(\d+)\.[a-z]*(\d+)', re.IGNORECASE)
    matched = release_alpha_pattern.fullmatch(version_dir)
    if matched:
        return render_cann_version(
            int(matched.group(1)), int(matched.group(2)), int(matched.group(3)), None, None,
            int(matched.group(4))
        )

    rc_pattern = re.compile(r'(\d+)\.(\d+)\.RC(\d+)', re.IGNORECASE)
    matched = rc_pattern.fullmatch(version_dir)
    if matched:
        return render_cann_version(
            int(matched.group(1)), int(matched.group(2)), None, int(matched.group(3)), None, None
        )

    test_pattern = re.compile(r'(\d+)\.(\d+)\.T(\d+)', re.IGNORECASE)
    matched = test_pattern.fullmatch(version_dir)
    if matched:
        return render_cann_version(
            int(matched.group(1)), int(matched.group(2)), None, None, int(matched.group(3)), None
        )

    alpha_pattern = re.compile(r'(\d+)\.(\d+)\.RC(\d+)\.[a-z]*(\d+)', re.IGNORECASE)
    matched = alpha_pattern.fullmatch(version_dir)
    if matched:
        return render_cann_version(
            int(matched.group(1)), int(matched.group(2)), None, int(matched.group(3)), None,
            int(matched.group(4))
        )

    cann_pattern = re.compile(r'CANN-(\d+)\.(\d+)', re.IGNORECASE)
    matched = cann_pattern.fullmatch(version_dir)
    if matched:
        return render_cann_version(
            int(matched.group(1)), int(matched.group(2)), None, None, None, None
        )

    raise IllegalVersionDir(version_dir)


def get_cann_version_info(name: str, version_dir: str) -> List[Tuple[str, str]]:
    version_info = []

    package_name = name[:-8]

    if not version_dir:
        version_str = '0'
    elif version_dir.startswith('CANN-'):
        version_str = version_dir[5:]
    else:
        version_str = version_dir

    version_info.append((f'{package_name}_VERSION_STR', f'"{version_str}"'))

    version = get_cann_version(version_dir)
    version_info.append((f'{package_name}_VERSION', version))

    return version_info


def get_default_env_items() -> Iterator[Tuple[str, str]]:
    yield 'VERSION_DIR', ''
    yield 'HOME', os.environ.get('HOME')


def get_env_items_by_version(version: Optional[str]) -> Iterator[Tuple[str, str]]:
    if version:
        yield 'ASCEND_VER', version

        version_parts = version.split('.')
        for idx in range(1, len(version_parts) + 1):
            yield f'CUR_VER[{idx}]', '.'.join(version_parts[:idx])
        yield 'CUR_VER', version
        yield 'LOWER_CUR_VER', version.lower()


def get_env_items_by_version_dir(version_dir: Optional[str]) -> Iterator[Tuple[str, str]]:
    if version_dir:
        yield 'VERSION_DIR', version_dir


def get_os_arch_default_env_items() -> Iterator[Tuple[str, str]]:
    yield 'OS_NAME', 'linux'
    yield 'OS_VER', ''
    yield 'ARM', 'aarch64'
    yield 'TARGET_ENV', '$(TARGET_ENV)'


def get_env_items_by_os_arch(os_arch: str) -> Iterator[Tuple[str, str]]:
    if os_arch:
        os_name, os_ver, arch = parse_os_arch(os_arch)
        yield 'OS_NAME', os_name
        yield 'OS_VER', os_ver
        yield 'ARCH', arch
        yield 'OS_ARCH', os_arch
        if arch in ('arm', 'sw_64'):
            yield 'ARM', arch
        else:
            yield 'ARM', 'aarch64'
        yield 'TARGET_ENV', f"{arch}-linux"
    else:
        yield from get_os_arch_default_env_items()


def get_env_items_by_timestamp(timestamp: Optional[str]) -> Iterator[Tuple[str, str]]:
    if timestamp:
        yield 'TIMESTAMP', timestamp
        yield 'TIMESTAMP_NO', timestamp.replace('_', '')
    else:
        yield 'TIMESTAMP', '0'
        yield 'TIMESTAMP_NO', '0'


def parse_env_dict(os_arch: str,
                   package_attr: PackageAttr,
                   version: Optional[str],
                   version_dir: Optional[str],
                   timestamp: Optional[str]) -> EnvDict:
    env_dict = dict(
        chain(
            get_default_env_items(),
            yield_if(('ARCH', package_attr.get('default_arch')), itemgetter(1)),
            get_env_items_by_os_arch(os_arch),
            get_env_items_by_version(version),
            get_env_items_by_version_dir(version_dir),
            yield_if(('VERSION_DIR', version_dir), constant(version_dir)),
            get_env_items_by_timestamp(timestamp),
        )
    )

    return env_dict


def get_timestamp(args: Namespace) -> Optional[str]:
    if 'tag' not in args:
        return None

    tag = args.tag
    if tag:
        timestamp_re = r"\d{8}_\d{9}"
        timestamp_list = re.findall(timestamp_re, tag)
        if not timestamp_list:
            raise PackageError("The {} format is incorrect.".format(tag))
        timestamp = timestamp_list[-1]
    else:
        timestamp = None
    return timestamp


def extract_element_attrib(ele: ET.Element) -> Dict:
    return ele.attrib.copy()


def extract_generate_info_content(generate_info_ele: ET.Element, env_dict: EnvDict) -> Dict:
    file_content = {
        sub_item.tag: replace_env(env_dict, sub_item.text)
        for sub_item in list(generate_info_ele)
    }
    return {
        'content': file_content
    }


def parse_generate_infos_by_loaded_block(loaded_block: LoadedBlockElement,
                                         default_config: Dict[str, str],
                                         env_dict: EnvDict) -> List[Dict]:
    return invoke(
        pipe(
            partial(
                map, pipe(
                    dispatch(
                        pipe(
                            extract_element_attrib,
                            partial(merge_dict, default_config),
                            partial(evaluate_info, loaded_block=loaded_block, env_dict=env_dict),
                        ),
                        partial(extract_generate_info_content, env_dict=env_dict),
                    ),
                    star_apply(merge_dict),
                )
            ),
            list,
        ),
        loaded_block.root_ele.findall('generate_info')
    )


def join_pkg_inner_softlink(link_str_list: List[str]) -> str:
    path = "/".join(link_str_list)
    return os.path.normpath(path)


def check_contain_asterisk(value: str) -> bool:
    if '*' in value:
        return True
    return False


def check_value(value: str,
                package_check: bool,
                package_attr: PackageAttr):
    if package_check and package_attr.get('suffix') == 'run':
        if check_contain_asterisk(value):
            raise ContainAsteriskError(value)


def get_dst_prefix(file_info: FileInfo, env: ParseEnv) -> str:
    return os.path.join(env.delivery_dir, file_info['dst_path'])


def get_dst_target(file_info: FileInfo, env: ParseEnv) -> str:
    dst_prefix = get_dst_prefix(file_info, env)
    return os.path.join(dst_prefix, os.path.basename(file_info.get('value')))


def make_hash(filepath: str) -> str:
    sha256_hash = hashlib.sha256()
    with open(filepath, "rb") as file:
        sha256_hash.update(file.read())

    return sha256_hash.hexdigest()


def config_hash(parsed_result: FileInfoParsedResult, env: ParseEnv):
    file_info = parsed_result.file_info
    if file_info and file_info['configurable'] == 'TRUE':
        src_target = get_dst_target(file_info, env)
        hash_value = make_hash(src_target)
        file_info['hash'] = hash_value
    return parsed_result


def apply_func(func: Callable[[str], str],
               value: Union[List[str], Set[str], str]
               ) -> Union[List[str], Set[str], str]:
    if isinstance(value, list):
        return list(map(func, value))
    if isinstance(value, set):
        return set(map(func, value))
    return func(value)

REAL_PREFIX = 'real:'


def join_dst_path(base: str, other: str) -> str:
    if other.startswith('real:'):
        other = other[len(REAL_PREFIX):]
        return other
    return os.path.join(base, other)


def evaluate_info(info: Dict[str, str],
                  loaded_block: LoadedBlockElement,
                  env_dict: EnvDict) -> Dict[str, str]:
    dst_keys = ('dst_path',)

    replace_env_func = partial(replace_env, env_dict)
    add_dst_path_func = partial(join_dst_path, loaded_block.dst_path)

    def upper_value(key: str, value: str) -> Tuple[str, str]:
        if key == 'configurable':
            return key, value.upper()
        return key, value

    def add_dst_path(key: str, value: str) -> Tuple[str, str]:
        if key in dst_keys:
            return key, apply_func(add_dst_path_func, value)
        return key, value

    def replace_pkg_inner_softlink(key: str, value: str) -> Tuple[str, str]:
        if key == 'pkg_inner_softlink':
            inner_softlink_new = [
                join_pkg_inner_softlink(link_str.split(':'))
                for link_str in value.split(';')
                if ':' not in link_str or link_str.split(':')[0] == loaded_block.dst_path
            ]
            if inner_softlink_new:
                return key, ';'.join(inner_softlink_new)
            return key, 'NA'
        return key, value

    def merge_feature(key: str, value: str) -> Tuple[str, str]:
        if key in ('chip', 'feature'):
            config_features = config_feature_to_set(value, key)
            return key, config_features | getattr(loaded_block, f'{key}s')
        return key, value

    def eval_value(_key: str, value: str) -> str:
        if value is not None:
            return apply_func(replace_env_func, value)

    eval_value_func = star_pipe(
        upper_value, add_dst_path, replace_pkg_inner_softlink,
        merge_feature, eval_value,
    )

    return {
        key: eval_value_func(key, value)
        for key, value in
        itertools.chain(
            [
                ('dst_path', ''), ('configurable', 'FALSE'),
                ('chip', None), ('feature', None), ('pkg_feature', None),
            ],
            info.items()
        )
    }


def parse_dir_info_elements(loaded_block: LoadedBlockElement,
                            default_config: Dict[str, str],
                            package_attr: PackageAttr,
                            env: ParseEnv) -> List[Dict[str, str]]:
    dir_info_elements: List[ET.Element] = loaded_block.root_ele.findall('dir_info')
    dir_infos = []
    for item in dir_info_elements:
        dir_config = default_config.copy()
        dir_config.update(item.attrib)
        dir_config['module'] = dir_config.get('value')
        for sub_item in list(item):
            dir_info = dir_config.copy()
            dir_info.update(sub_item.attrib)
            dir_info = evaluate_info(dir_info, loaded_block, env.env_dict)
            check_value(
                dir_info['value'], env.parse_option.package_check, package_attr
            )
            dir_infos.append(dir_info)

    return dir_infos


def expand_dir(file_info: FileInfo, get_dst_target_func: Callable[[FileInfo], str]):
    file_info_list = []
    dir_info_list = []
    dst_target = get_dst_target_func(file_info)

    value_list = file_info.get('value').split('/')
    target_name = value_list[-1] if value_list[-1] else value_list[-2]

    dir_info_copy = file_info.copy()
    dir_info_copy['module'] = file_info.get('value')
    dir_info_copy['value'] = os.path.join(
        file_info.get('install_path', ''), target_name
    )

    subdir_mod = file_info.get("subdir_mod", None)
    if subdir_mod is not None:
        dir_info_copy['install_mod'] = subdir_mod
    dir_info_copy['install_softlink'] = 'NA'
    dir_info_copy['pkg_inner_softlink'] = 'NA'
    dir_info_list.append(dir_info_copy)

    for root, dirs, files in os.walk(dst_target, followlinks=True):
        dirs.sort()
        files.sort()

        dirs_to_remove = []
        for name in dirs:
            dirname = os.path.join(root, name)
            if os.path.islink(dirname) and not need_dereference(file_info):
                copy_file_info = create_file_info(dirname, dst_target, file_info, name, target_name)
                copy_file_info['install_softlink'] = 'NA'
                copy_file_info['pkg_inner_softlink'] = 'NA'
                file_info_list.append(copy_file_info)
                dirs_to_remove.append(name)
                continue
            relative_dirname = os.path.relpath(dirname, dst_target)
            dir_info_copy = file_info.copy()
            dir_info_copy['module'] = file_info.get('value')
            dir_info_copy['value'] = os.path.join(
                file_info.get('install_path', ''), target_name, relative_dirname
            )
            dir_info_copy['install_softlink'] = 'NA'
            dir_info_copy['pkg_inner_softlink'] = 'NA'
            subdir_mod = file_info.get("subdir_mod", None)
            if subdir_mod is not None:
                dir_info_copy['install_mod'] = subdir_mod
            dir_info_list.append(dir_info_copy)
        for name in files:
            filename = os.path.join(root, name)
            copy_file_info = create_file_info(filename, dst_target, file_info, name, target_name)
            file_info_list.append(copy_file_info)

        for name in dirs_to_remove:
            dirs.remove(name)
    return file_info_list, dir_info_list


def create_file_info(dirname, dst_target, file_info, name, target_name):
    relative_filename = os.path.relpath(dirname, dst_target)
    relative_dir_name = os.path.split(relative_filename)[0]
    copy_file_info = file_info.copy()
    copy_file_info['value'] = name
    copy_file_info['src_path'] = os.path.join(
        file_info['src_path'], file_info['value'], relative_dir_name
    )
    copy_file_info['dst_path'] = os.path.join(
        file_info['dst_path'], target_name, relative_dir_name
    )
    copy_file_info['install_path'] = os.path.join(
        file_info.get('install_path', ''), target_name, relative_dir_name
    )
    return copy_file_info


def expand_file_info_asterisk(parsed_result: FileInfoParsedResult,
                              env: ParseEnv) -> Iterator[FileInfoParsedResult]:
    file_info = parsed_result.file_info
    if check_contain_asterisk(file_info.get('value', '')):
        dst_prefix = get_dst_prefix(file_info, env)
        dst_targets = sorted(glob.glob(get_dst_target(file_info, env)))
        if 'exclude' in file_info:
            exclude = list(map(methodcaller('strip'), file_info['exclude'].split(';')))
        else:
            exclude = []
        for dst_target in dst_targets:
            value = os.path.relpath(dst_target, dst_prefix)
            if value in exclude:
                continue
            new_file_info = file_info.copy()
            new_file_info['value'] = value
            if 'pkg_inner_softlink' in new_file_info:
                pkg_inner_softlink = new_file_info['pkg_inner_softlink']
                new_file_info['pkg_inner_softlink'] = pkg_inner_softlink.replace(
                    '$(FILE)', os.path.basename(dst_target)
                )
            yield parsed_result._replace(file_info=new_file_info)
    else:
        yield parsed_result


def trans_to_stream(item: Any) -> Iterator[Any]:
    yield item


def need_dereference(file_info: FileInfo) -> bool:
    if 'dereference' in file_info:
        return True
    return False


def need_expand(file_info: FileInfo, get_dst_target_func: Callable[[FileInfo], str]) -> bool:
    if file_info.get('entity') == 'true':
        return False
    dst_target = get_dst_target_func(file_info)
    if os.path.isdir(dst_target):
        if need_dereference(file_info):
            return True
        if os.path.islink(dst_target):
            return False
        return True
    return False


def expand_file_info(parsed_result: FileInfoParsedResult,
                     use_move: bool,
                     get_dst_target_func: Callable[[FileInfo], str]) -> FileInfoParsedResult:
    file_info = parsed_result.file_info
    if need_expand(file_info, get_dst_target_func):
        expand_infos, dir_infos = expand_dir(file_info, get_dst_target_func)
        return FileInfoParsedResult(
            merge_dict(file_info, {'is_dir': True}), [], dir_infos, expand_infos
        )

    if use_move:
        return FileInfoParsedResult(
            {}, [file_info], parsed_result.dir_infos, parsed_result.expand_infos
        )

    return parsed_result


def trans_file_info_to_result(file_info: FileInfo) -> FileInfoParsedResult:
    return FileInfoParsedResult(file_info, [], [], [])


def parse_file_element(file_ele: ET.Element,
                       file_config: Dict[str, str],
                       loaded_block: LoadedBlockElement,
                       package_attr: PackageAttr,
                       env: ParseEnv) -> Iterator[FileInfoParsedResult]:
    file_info = merge_dict(file_config, file_ele.attrib)
    file_info = evaluate_info(file_info, loaded_block, env.env_dict)

    if package_attr.get('expand_asterisk', False):
        expand_asterisk_func = partial(expand_file_info_asterisk, env=env)
    else:
        expand_asterisk_func = trans_to_stream

    if 'install_path' not in file_info:
        file_info['install_path'] = ''

    trans_file_info_func = pipe(
        trans_file_info_to_result,
        expand_asterisk_func,
        partial(map, partial(config_hash, env=env)),
        partial(
            map,
            partial(
                expand_file_info,
                use_move=loaded_block.use_move,
                get_dst_target_func=partial(get_dst_target, env=env)
            )
        ),
    )

    yield from trans_file_info_func(file_info)


def parse_file_info_elements(loaded_block: LoadedBlockElement,
                             default_config: Dict[str, str],
                             package_attr: PackageAttr,
                             env: ParseEnv) -> Iterator[FileInfoParsedResult]:
    file_info_elements: List[ET.Element] = loaded_block.root_ele.findall('file_info')
    for file_info_ele in file_info_elements:
        file_config = merge_dict(
            default_config,
            file_info_ele.attrib,
            {'module': file_info_ele.attrib.get('value')}
        )

        for sub_item in list(file_info_ele):
            yield from parse_file_element(
                sub_item, file_config, loaded_block, package_attr, env
            )


def unique_infos(infos: Iterable) -> List[Dict[str, str]]:
    cache: Set[str] = set()
    new_infos = []
    for info in infos:
        if info['value'] in cache:
            continue
        cache.add(info['value'])
        new_infos.append(info)

    return new_infos


def parse_block_config(loaded_block: LoadedBlockElement,
                       package_attr: PackageAttr,
                       parse_env: ParseEnv):
    default_config = copy.copy(loaded_block.attrs)
    default_config.update(loaded_block.root_ele.attrib)

    dir_infos = parse_dir_info_elements(
        loaded_block,
        default_config,
        package_attr,
        parse_env,
    )
    file_info_results = list(
        chain(
            parse_file_info_elements(
                loaded_block,
                default_config,
                package_attr,
                parse_env,
            )
        )
    )

    generate_infos = parse_generate_infos_by_loaded_block(
        loaded_block, default_config, parse_env.env_dict
    )

    return BlockConfig(
        unique_infos(
            itertools.chain(dir_infos,
                            flatten(result.dir_infos for result in file_info_results)
                            )
        ),
        list(flatten(map(attrgetter('move_infos'), file_info_results))),
        list(flatten(map(attrgetter('expand_infos'), file_info_results))),
        [result.file_info for result in file_info_results if result.file_info],
        generate_infos,
    )


def make_loaded_block_element(root_ele: ET.Element,
                              dst_path: str = '') -> LoadedBlockElement:
    return LoadedBlockElement(root_ele, False, dst_path, set(), set(), {})


def parse_block_element(block_ele: ET.Element,
                        block_info_attr: Dict[str, str]) -> BlockElement:

    def filter_attrs(attrs: Dict[str, str]) -> Dict[str, str]:
        return {
            key: value
            for key, value in attrs.items()
            if key not in ('dst_path', 'block_conf_path')
        }

    def with_merged_attrs(attrs: Dict[str, str]) -> BlockElement:
        name = attrs.get('name')
        block_conf_path = attrs.get('block_conf_path')

        if not name:
            raise BlockConfigError("block's name is not set!")

        if not block_conf_path:
            raise BlockConfigError("block's conf_path is not set!")

        return BlockElement(
            name=name,
            block_conf_path=block_conf_path,
            dst_path=attrs.get('dst_path', ''),
            chips=config_feature_to_set(attrs.get('chip'), 'chip'),
            features=config_feature_to_set(attrs.get('feature'), 'feature'),
            attrs=filter_attrs(attrs)
        )

    return with_merged_attrs(merge_dict(block_info_attr, block_ele.attrib))


def parse_block_info(block_info: ET.Element) -> List[BlockElement]:
    def parse_block_elements(block_elements: List[ET.Element]) -> List[BlockElement]:
        return [
            parse_block_element(block_ele, block_info.attrib) for block_ele in block_elements
        ]

    return parse_block_elements(list(block_info))


def get_block_filepath(block_element: BlockElement) -> str:
    return os.path.join(
        pkg_utils.TOP_SOURCE_DIR, BLOCK_CONFIG_PATH, block_element.block_conf_path,
        f'{block_element.name}.xml'
    )


def load_block_element(package_attr: PackageAttr,
                       block_element: BlockElement) -> LoadedBlockElement:

    def with_filepath(block_xml: str):
        if not os.path.exists(block_xml):
            raise BlockConfigError(f"block's config xml {block_xml} does not exist!")

        try:
            return LoadedBlockElement(
                root_ele=ET.parse(block_xml).getroot(),
                use_move=package_attr.get('use_move', False),
                **{
                    name: getattr(block_element, name)
                    for name in BLOCK_ELEMENT_PASS_THROUGH_ARGS
                }
            )
        except Exception:
            raise BlockConfigError(f"dependent block configuration {block_xml} parse failed!")

    return with_filepath(get_block_filepath(block_element))


def parse_blocks(root_ele: ET.Element,
                 package_attr: PackageAttr,
                 parse_env: ParseEnv) -> List[BlockConfig]:
    return [
        parse_block_config(
            loaded_block, package_attr, parse_env
        )
        for loaded_block in itertools.chain(
            [make_loaded_block_element(root_ele)],
            map(
                partial(load_block_element, package_attr),
                chain.from_iterable(
                    map(parse_block_info, root_ele.findall("block_info"))
                )
            )
        )
    ]


def read_version_info(package_attr: PackageAttr) -> Tuple[str, str]:
    version_path = os.path.join(pkg_utils.TOP_DIR,  package_attr.get('version'))
    with open(version_path, 'r') as file:
        version = file.readline().strip()
    version_dir=""
    m = re.match(r'[.a-zA-Z0-9]+$', version)
    if not m:
        raise VersionFormatNotMatch()

    return version, version_dir


def parse_xml_config(filepath: str,
                     delivery_dir: str,
                     parse_option: ParseOption,
                     args: Namespace) -> XmlConfig:
    try:
        tree = ET.parse(filepath)
        xml_root = tree.getroot()
    except ET.ParseError as ex:
        CommLog.cilog_error("xml parse %s failed: %s!", filepath, ex)
        sys.exit(FAIL)

    default_config = xml_root.attrib.copy()

    package_attr = parse_package_attr(xml_root, args)
    version, version_dir = read_version_info(package_attr)
    if args.disable_multi_version:
        version_dir = None
    timestamp = get_timestamp(args)
    try:
        env_dict = parse_env_dict(
            parse_option.os_arch, package_attr, version, version_dir, timestamp
        )
    except ParseOsArchError:
        CommLog.cilog_error(
            "os_arch %s is not correctly configured: %s!",
            parse_option.os_arch, filepath
        )
        sys.exit(FAIL)

    err_msgs, version_info = parse_version_info_by_root(
        xml_root, env_dict, version, version_dir, None,
        timestamp
    )

    parse_env = ParseEnv(
        env_dict, parse_option, delivery_dir, pkg_utils.TOP_SOURCE_DIR
    )

    blocks = parse_blocks(
        xml_root, package_attr, parse_env
    )

    if is_multi_version(version_dir):
        fill_is_common_path_func = partial(
            fill_is_common_path, target_env=env_dict.get('TARGET_ENV')
        )
    else:
        fill_is_common_path_func = iter

    return XmlConfig(
        default_config, package_attr, version_info, blocks, version, None,
        PackerConfig(fill_is_common_path_func)
    )
