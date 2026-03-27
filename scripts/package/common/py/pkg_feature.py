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

from enum import IntEnum
from itertools import groupby
from pathlib import Path
from typing import Dict, Iterable, Optional, Set, Tuple, Union
from abc import ABC, abstractmethod
from dataclasses import dataclass

from .utils.pkg_utils import PackageConfigError


def pkg_feature_to_set(feature: str) -> Set[str]:
    if not feature:
        return set()

    return set(feature.strip().split(','))


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


@dataclass
class PkgFeature(ABC):
    excludes: Set[str]
    exclude_all: bool
    includes: Set[str]

    @abstractmethod
    def _matched(self, config_features: Set[str]) -> bool:
        """ Match or not """

    def matched(self, config_features: Set[str]) -> bool:
        if bool(config_features & self.excludes):
            return False

        if self._matched(config_features):
            return True

        return bool(not config_features and not self.exclude_all)


@dataclass
class NormalPkgFeature(PkgFeature):
    def _matched(self, config_features: Set[str]) -> bool:
        return bool(
            config_features & self.includes
        )


@dataclass
class AllPkgFeature(PkgFeature):
    def _matched(self, config_features: Set[str]) -> bool:
        return True


def make_pkg_feature(features: Set[str], exclude_all: bool = False) -> PkgFeature:
    class FeatureType(IntEnum):
        INCLUDE = 1
        EXCLUDE = 2

    def feature_keyfunc(feature: str) -> int:
        if feature.startswith('-'):
            return FeatureType.EXCLUDE
        return FeatureType.INCLUDE

    def group_features_to_set(key: FeatureType,
                             group_features: Iterable[str]) -> Set[str]:
        if key == FeatureType.INCLUDE:
            return set(group_features)

        return {feature[1:] for feature in group_features}

    def get_features_dict(features: Set[str]) -> Dict[int, Set[str]]:
        sorted_features = sorted(features, key=feature_keyfunc)
        grouped = groupby(
                sorted_features,
                key=feature_keyfunc
            )
        return {
            key: group_features_to_set(key, group_features)
            for key, group_features in grouped
        }

    def classify_features(features: Set[str]) -> Tuple[Set[str], Set[str]]:
        feature_dict = get_features_dict(features)
        return (
            feature_dict.get(FeatureType.INCLUDE, set()),
            feature_dict.get(FeatureType.EXCLUDE, set())
        )

    def with_features(includes: Set[str], excludes: Set[str]) -> PkgFeature:
        if not includes or 'all' in includes:
            return AllPkgFeature(excludes, exclude_all, set())

        return NormalPkgFeature(excludes, exclude_all, includes)

    return with_features(
        *classify_features(features)
    )


def feature_compatible(left: PkgFeature, right: PkgFeature) -> bool:
    if bool(left.includes & right.excludes) or bool(left.excludes & right.includes):
        return False

    if left.exclude_all and not right.includes:
        return False

    if right.exclude_all and not left.includes:
        return False

    if not left.includes or not right.includes:
        return True
    return bool(left.includes & right.includes)


def load_feature_list(filepath: Optional[Union[Path, str]]) -> Set[str]:
    if not filepath:
        return set()

    with Path(filepath).open(encoding='utf-8') as file:
        return {
            line.strip() for line in file
            if line.strip() and not line.startswith('#')
        }


def combine_feature_and_feature_list(feature: str, feature_list_path: Optional[str]) -> Set[str]:
    return pkg_feature_to_set(feature) | load_feature_list(feature_list_path)