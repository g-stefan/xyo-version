//
// XYO Version
//
// Copyright (c) 2020-2021 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#include "xyo-version-version.hpp"

namespace XYOVersion {
	namespace Version {

		static const char *version_ = "3.1.0";
		static const char *build_ = "13";
		static const char *versionWithBuild_ = "3.1.0.13";
		static const char *datetime_ = "2021-07-06 15:11:43";

		const char *version() {
			return version_;
		};
		const char *build() {
			return build_;
		};
		const char *versionWithBuild() {
			return versionWithBuild_;
		};
		const char *datetime() {
			return datetime_;
		};

	};
};

