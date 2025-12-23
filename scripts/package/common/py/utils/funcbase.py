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

import operator
from typing import Callable, Iterator, TypeVar

A = TypeVar('A')


def constant(value: A) -> Callable[..., A]:
    def constant_inner(*_args, **_kwargs) -> A:
        return value

    return constant_inner


def dispatch(*funcs):
    def dispatch_inner(*args, **kwargs) -> Iterator:
        return (func(*args, **kwargs) for func in funcs)

    return dispatch_inner


def pipe(*funcs):
    def pipe_func(*args, **k_args):
        result = funcs[0](*args, **k_args)
        for func in funcs[1:]:
            result = func(result)
        return result

    return pipe_func


def identity(value: A) -> A:
    return value


def invoke(func, *args, **kwargs):
    return func(*args, **kwargs)


def side_effect(*funcs):
    def side_effect_func(arg):
        for func in funcs:
            func(arg)
        return arg

    return side_effect_func


def star_apply(func):
    def star_apply_func(arg):
        return func(*arg)

    return star_apply_func


def any_(*funcs) -> Callable:
    return pipe(
        dispatch(*funcs),
        any,
    )


def not_(func) -> Callable:
    return pipe(func, operator.not_)
