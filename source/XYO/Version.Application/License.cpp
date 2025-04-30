// Version
// Copyright (c) 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <XYO/Version.Application/License.hpp>

namespace XYO::Version::Application::License {

	std::string license() {
		return XYO::ManagedMemory::License::license();
	};

	std::string shortLicense() {
		return XYO::ManagedMemory::License::shortLicense();
	};

};
