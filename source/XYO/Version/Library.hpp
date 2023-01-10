// Version
// Copyright (c) 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_VERSION_LIBRARY_HPP
#define XYO_VERSION_LIBRARY_HPP

#ifndef XYO_VERSION_DEPENDENCY_HPP
#	include <XYO/Version/Dependency.hpp>
#endif

namespace XYO::Version {

	XYO_VERSION_EXPORT String get(String versionFile, String projectName);
	XYO_VERSION_EXPORT bool set(String versionFile, String projectName, String version);
	XYO_VERSION_EXPORT bool buildBump(String versionFile, String projectName);
	XYO_VERSION_EXPORT bool patchBump(String versionFile, String projectName);
	XYO_VERSION_EXPORT bool minorBump(String versionFile, String projectName);
	XYO_VERSION_EXPORT bool majorBump(String versionFile, String projectName);
	XYO_VERSION_EXPORT bool processTemplate(String versionFile, String projectName, String templateIn, String fileOut, size_t maxLineSize = 32768);

};

#endif
