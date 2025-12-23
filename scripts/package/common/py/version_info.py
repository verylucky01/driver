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

import os
import re
import sys
import xml.etree.ElementTree as ET
from functools import total_ordering
from pathlib import Path
from typing import Dict, List, NamedTuple, Optional, Tuple, Union
from .utils.comm_log import CommLog


class VersionInfoError(Exception):
    """Version Information Exception Base Class."""


class VersionFormatNotMatch(VersionInfoError):
    """The version format does not match."""


class IntervalFormatNotMatch(VersionInfoError):
    """The range format does not match."""


class DuplicatedPkgConfig(VersionInfoError):

    def __init__(self, pkg_name):
        super().__init__(pkg_name)
        self.pkg_name = pkg_name


class ParseVersionFailed(VersionInfoError):
    """Failed to parse version."""


class CollectRequiresFailed(VersionInfoError):

    def __init__(self, pkg_name, version_str, msg):
        super().__init__(pkg_name, version_str, msg)
        self.pkg_name = pkg_name
        self.version_str = version_str
        self.msg = msg


@total_ordering
class Version:

    def __init__(self, version):
        self.version = version

    @classmethod
    def match(cls, input_str):
        m = re.match(r'[.a-zA-Z0-9]+$', input_str)
        return bool(m)

    @classmethod
    def parse(cls, input_str):
        if not cls.match(input_str):
            raise VersionFormatNotMatch()

        return cls(input_str)

    @classmethod
    def try_convert_to_int_list(cls, str_list):
        for idx, item in enumerate(str_list):
            try:
                int_item = int(item)
                str_list[idx] = int_item
            except ValueError:
                pass

    def to_required_list(self):
        return [self.version]

    def __eq__(self, other):
        if not isinstance(other, self.__class__):
            return False
        return self.version == other.version

    def __lt__(self, other):
        if not isinstance(other, self.__class__):
            return True

        self_list = self.version.split('.')
        other_list = other.version.split('.')

        self.try_convert_to_int_list(self_list)
        self.try_convert_to_int_list(other_list)

        self_tuple = tuple(self_list)
        other_tuple = tuple(other_list)

        return self_tuple < other_tuple

    def __str__(self):
        return self.version

    def __repr__(self):
        return repr(self.version)


class Point(NamedTuple):
    type_: int
    value: Version


class Interval(NamedTuple):
    low: Point
    high: Point

    @classmethod
    def match(cls, input_str: str) -> bool:
        if not input_str.startswith('(') and not input_str.startswith('['):
            return False
        if not input_str.endswith(')') and not input_str.endswith(']'):
            return False
        input_str = input_str[1:-1]
        if input_str.count(',') > 1:
            return False
        return True

    @classmethod
    def parse(cls, input_str):
        if not cls.match(input_str):
            raise IntervalFormatNotMatch()

        if input_str[0] == '[':
            low_type = 0
        elif input_str[0] == '(':
            low_type = 1
        else:
            assert False, 'should not go here.'

        if input_str[-1] == ']':
            high_type = 0
        elif input_str[-1] == ')':
            high_type = 1
        else:
            assert False, 'should not go here.'

        input_str = input_str[1:-1]
        input_list = input_str.split(',')
        low = input_list[0].strip()
        if len(input_list) > 1:
            high = input_list[1].strip()
        else:
            high = None

        if low:
            low_version = Point(low_type, Version(low))
        else:
            low_version = None

        if high:
            high_version = Point(high_type, Version(high))
        else:
            high_version = None

        return cls(low=low_version, high=high_version)

    def to_required_list(self):
        result = []

        if self.low:
            if self.low.type_ == 0:
                operator = '>='
            else:
                operator = '>'
            required_str = '{0}{1}'.format(operator, self.low.value.version)
            result.append(required_str)

        if self.high:
            if self.high.type_ == 0:
                operator = '<='
            else:
                operator = '<'
            required_str = '{0}{1}'.format(operator, self.high.value.version)
            result.append(required_str)

        return result


class Require(NamedTuple):
    pkg_name: str
    versions: List

    @classmethod
    def _sort_key(cls, item) -> Tuple:
        if isinstance(item, Interval):
            if item.low:
                return item.low.value, item.low.type_
            return item.high.value, -item.high.type_

        return item, 0

    @classmethod
    def _sort_versions(cls, versions: List) -> bool:
        versions.sort(key=cls._sort_key)
        return True

    @classmethod
    def _to_required_list(cls, versions: List) -> List[str]:
        result = []
        for version in versions:
            requires = version.to_required_list()
            result.extend(requires)

        return result

    @classmethod
    def _to_required_str(cls, versions: List) -> str:
        requires = cls._to_required_list(versions)
        required_str = ', '.join(requires)

        return required_str

    def sort_versions(self) -> bool:
        return self._sort_versions(self.versions)

    def to_required_full_str(self) -> str:
        required_str = self._to_required_str(self.versions)
        required_full_str = 'required_package_{0}_version="{1}"'.format(self.pkg_name, required_str)
        return required_full_str


class ItemElement(NamedTuple):
    name: str
    version: str

    @classmethod
    def parse(cls, item_ele: ET.Element, cur_ver: str):
        name = item_ele.attrib['name']
        version = item_ele.attrib['version'].replace("$(CUR_VER)", cur_ver)
        return cls(name=name, version=version)

    @classmethod
    def skip(cls, item_ele: ET.Element):
        version = item_ele.attrib['version']
        return version.strip() == ''


class CompatibleElement(NamedTuple):
    items: List

    @classmethod
    def parse(cls, compatible_ele: ET.Element, cur_ver: str):
        items = []
        for item_ele in compatible_ele.findall("./item"):
            if ItemElement.skip(item_ele):
                continue
            item = ItemElement.parse(item_ele, cur_ver)
            items.append(item)
        return cls(items=items)


def is_version_number(version: str) -> bool:
    has_slash = '/' in version
    return not has_slash and len(version.split(".")) >= 3


class VersionXml(NamedTuple):
    release_version: str
    version_dir: str
    packages: Dict

    @classmethod
    def match(cls, filepath: Union[Path, str]) -> bool:
        return str(filepath).endswith('.xml')

    @classmethod
    def parse_version(cls, version_str: str):
        ret = Interval.match(version_str)
        if ret:
            result = Interval.parse(version_str)
            return result

        ret = Version.match(version_str)
        if ret:
            result = Version.parse(version_str)
            return result

        raise ParseVersionFailed()

    def get_release_version(self):
        return self.release_version

    def get_version_dir(self):
        return self.version_dir

    def collect_requires(self, package: str) -> List[Require]:
        requires = {}

        if package not in self.packages:
            return []

        compatible = self.packages[package]

        for item in compatible.items:
            pkg_name = item.name
            if pkg_name not in requires:
                requires[pkg_name] = Require(pkg_name=pkg_name, versions=[])

            version_str = item.version
            try:
                version = self.parse_version(version_str)
            except ParseVersionFailed as ex:
                msg = f'parse pkg {pkg_name} version {version_str} failed'
                raise CollectRequiresFailed(pkg_name, version_str, msg) from ex

            requires[pkg_name].versions.append(version)

        result = []
        for pkg_name in sorted(requires.keys()):
            requires[pkg_name].sort_versions()
            result.append(requires[pkg_name])

        return result


def get_version_dir(version_xml: Optional[VersionXml],
                    disable_multi_version: bool,
                    version_dir: Optional[str]) -> Optional[str]:
    if disable_multi_version:
        return None

    if version_dir:
        return version_dir

    if version_xml and version_xml.get_version_dir():
        return version_xml.get_version_dir()

    return None


def is_multi_version(version_dir: str) -> bool:
    return bool(version_dir)


class VersionInfo(NamedTuple):
    install_version_info: bool
    install_version_info_attrib: Optional[Dict[str, str]]
    itf_versions: List[str]
    version: str
    version_xml: Optional[VersionXml]
    timestamp: Optional[str]


class VersionInfoFile(NamedTuple):
    version: str
    itf_version_info: Optional[str] = None
    requires: Optional[List[Require]] = None
    version_dir: Optional[str] = None
    timestamp: Optional[str] = None

    def _get_content(self) -> str:
        lines = ['Version={0}'.format(self.version)]
        if self.version_dir:
            lines.append('version_dir={0}'.format(self.version_dir))
        if self.timestamp:
            lines.append('timestamp={0}'.format(self.timestamp))
        if self.itf_version_info:
            lines.append(self.itf_version_info)

        if self.requires:
            requires_str = [require.to_required_full_str() for require in self.requires]
            lines.extend(requires_str)

        lines.append('')

        return '\n'.join(lines)

    def save(self, target_path: Union[Path, str]):
        content = self._get_content()

        target_dir = os.path.dirname(target_path)
        if not os.path.exists(target_dir):
            os.makedirs(target_dir)

        with open(target_path, 'w') as file:
            file.write(content)
