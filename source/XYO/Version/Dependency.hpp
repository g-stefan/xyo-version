// Version
// Copyright (c) 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_VERSION_DEPENDENCY_HPP
#define XYO_VERSION_DEPENDENCY_HPP

#ifndef XYO_FILEJSON_HPP
#	include <XYO/FileJSON.hpp>
#endif

// -- Export

#ifdef XYO_VERSION_INTERNAL
#	define XYO_VERSION_EXPORT XYO_LIBRARY_EXPORT
#else
#	define XYO_VERSION_EXPORT XYO_LIBRARY_IMPORT
#endif

namespace XYO::Version {
	using namespace XYO::System;
};

#endif
