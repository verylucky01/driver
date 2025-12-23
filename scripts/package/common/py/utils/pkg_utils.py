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
from functools import partial
from itertools import chain, tee
from operator import methodcaller
from pathlib import Path
from typing import Callable, Dict, Iterator, List, Optional, TypeVar, Set

TOP_DIR = str(Path(__file__).resolve().parents[5])
TOP_SOURCE_DIR = TOP_DIR + '/scripts/'
DELIVERY_PATH = "build/_CPack_Packages/makeself_staging"
CONFIG_SCRIPT_PATH = 'package'
BLOCK_CONFIG_PATH = 'package/module'

SUCCESS = 0
FAIL = -1

ErrMsgs = List[str]

A = TypeVar('A')


class PackageError(Exception):
    """Base class for packaging exceptions."""


class PackageConfigError(PackageError):
    """Packaging configuration error exception."""


class BlockConfigError(PackageError):
    """Block configuration error exception."""


class ParseOsArchError(PackageError):
    """Failed to parse os_arch exception."""


class EnvNotSupported(PackageError):
    """Environment variables do not support exceptions."""


class ContainAsteriskError(PackageError):

    def __init__(self, value: str):
        super().__init__()
        self.value = value


class FilelistError(PackageError):
    """File list is abnormal."""


class UnknownOperateTypeError(PackageError):
    """Unknown operation type."""


class PackageNameEmptyError(PackageError):
    """Package name is empty error."""


class GenerateFilelistError(PackageError):
    def __init__(self, filename: str):
        super().__init__()
        self.filename = filename


class IllegalVersionDir(PackageError):
    """version_dir configuration error."""


class CompressError(PackageError):

    def __init__(self, package_name: Optional[str]):
        super().__init__(package_name)
        self.package_name = package_name


def flatten(list_of_lists):
    """Flatten one level of nesting"""
    return chain.from_iterable(list_of_lists)


def merge_dict(base: Dict, *news: Dict):
    result = base.copy()
    for new in news:
        result.update(new)
    return result


def star_pipe(*funcs):
    def pipe_func(*args, **k_args):
        result = funcs[0](*args, **k_args)
        for func in funcs[1:]:
            result = func(*result)
        return result

    return pipe_func


def swap_args(func):
    def inner(fst, snd, *args, **k_args):
        return func(snd, fst, *args, **k_args)
    return inner


def conditional_apply(predicate, func):
    def conditional_apply_func(arg):
        if predicate(arg):
            return func(arg)
        return arg

    return conditional_apply_func


def pairwise(iterable):
    """s -> (s0,s1), (s1,s2), (s2, s3), ..."""
    a, b = tee(iterable)
    next(b, None)
    return zip(a, b)


def path_join(base: Optional, *others: str) -> Optional:
    if base is None:
        return None
    return os.path.join(base, *others)


def yield_if(data, predicate: Callable) -> Iterator:
    if predicate(data):
        yield data


def config_feature_to_set(feature_str: str, feature_type: str = 'feature') -> Set[str]:
    if feature_str is None:
        return set()

    if isinstance(feature_str, set):
        return feature_str

    if feature_str == '':
        raise PackageConfigError(f"Not allow to config {feature_type} empty.")

    features = set(feature_str.split(';'))
    if 'all' in features:
        raise PackageConfigError(f"Not allow to config {feature_type} all.")
    return features


def config_feature_to_string(features: Set[str]) -> str:
    if not features:
        return 'all'
    return ';'.join(sorted(features))


def each_file_line(filepath: str) -> Iterator[str]:
    with open(filepath, encoding='utf-8') as file:
        yield from file

strip_lines = partial(map, methodcaller('strip'))