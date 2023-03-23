// Version
// Copyright (c) 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_VERSION_COPYRIGHT_HPP
#define XYO_VERSION_COPYRIGHT_HPP

#ifndef XYO_VERSION_DEPENDENCY_HPP
#	include <XYO/Version/Dependency.hpp>
#endif

namespace XYO::Version::Copyright {

	XYO_VERSION_EXPORT const char *copyright();
	XYO_VERSION_EXPORT const char *publisher();
	XYO_VERSION_EXPORT const char *company();
	XYO_VERSION_EXPORT const char *contact();

};

#endif
