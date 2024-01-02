// Version
// Copyright (c) 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <XYO/Version/Copyright.hpp>
#include <XYO/Version/Copyright.rh>

namespace XYO::Version::Copyright {

	static const char *copyright_ = XYO_VERSION_COPYRIGHT;
	static const char *publisher_ = XYO_VERSION_PUBLISHER;
	static const char *company_ = XYO_VERSION_COMPANY;
	static const char *contact_ = XYO_VERSION_CONTACT;

	const char *copyright() {
		return copyright_;
	};

	const char *publisher() {
		return publisher_;
	};

	const char *company() {
		return company_;
	};

	const char *contact() {
		return contact_;
	};

};
