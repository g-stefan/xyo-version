// Version
// Copyright (c) 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_VERSION_VERSION_HPP
#define XYO_VERSION_VERSION_HPP

#ifndef XYO_VERSION_DEPENDENCY_HPP
#	include <XYO/Version/Dependency.hpp>
#endif

namespace XYO::Version::Version {

	XYO_VERSION_EXPORT const char *version();
	XYO_VERSION_EXPORT const char *build();
	XYO_VERSION_EXPORT const char *versionWithBuild();
	XYO_VERSION_EXPORT const char *datetime();

};

#endif
