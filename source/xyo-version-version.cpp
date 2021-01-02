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

		static const char *version_ = "2.0.0";
		static const char *build_ = "7";
		static const char *versionWithBuild_ = "2.0.0.7";
		static const char *datetime_ = "2020-09-21 17:38:55";

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

