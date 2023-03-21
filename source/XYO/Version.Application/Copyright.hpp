// Version
// Copyright (c) 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_VERSION_APPLICATION_COPYRIGHT_HPP
#define XYO_VERSION_APPLICATION_COPYRIGHT_HPP

#ifndef XYO_VERSION_DEPENDENCY_HPP
#	include <XYO/Version/Dependency.hpp>
#endif

namespace XYO::Version::Application::Copyright {
	std::string copyright();
	std::string publisher();
	std::string company();
	std::string contact();
};

#endif
