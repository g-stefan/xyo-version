// Version
// Copyright (c) 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_VERSION_LICENSE_HPP
#define XYO_VERSION_LICENSE_HPP

#ifndef XYO_VERSION_DEPENDENCY_HPP
#	include <XYO/Version/Dependency.hpp>
#endif

namespace XYO::Version::License {

	XYO_VERSION_EXPORT const char *license();
	XYO_VERSION_EXPORT const char *shortLicense();

};

#endif
