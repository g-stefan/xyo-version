// Version
// Copyright (c) 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_VERSION_DEPENDENCY_HPP
#define XYO_VERSION_DEPENDENCY_HPP

#ifndef XYO_FILEJSON_HPP
#	include <XYO/FileJSON.hpp>
#endif

// -- Export

#ifdef XYO_VERSION_DLL_INTERNAL
#	define XYO_VERSION_EXPORT XYO_PLATFORM_LIBRARY_EXPORT
#else
#	define XYO_VERSION_EXPORT XYO_PLATFORM_LIBRARY_IMPORT
#endif
#ifdef XYO_VERSION_LIBRARY
#	undef XYO_VERSION_EXPORT
#	define XYO_VERSION_EXPORT
#endif

namespace XYO::Version {
	using namespace XYO::System;
};

#endif
