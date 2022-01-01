//
// XYO Version
//
// Copyright (c) 2020-2022 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef XYO_VERSION_VERSION_HPP
#define XYO_VERSION_VERSION_HPP

#define XYO_VERSION_VERSION_ABCD                3,6,0,29
#define XYO_VERSION_VERSION_STR                 "3.6.0"
#define XYO_VERSION_VERSION_STR_BUILD           "29"
#define XYO_VERSION_VERSION_STR_DATETIME        "2022-01-01 21:19:32"

#ifndef XYO_RC

namespace XYOVersion {
	namespace Version {
		const char *version();
		const char *build();
		const char *versionWithBuild();
		const char *datetime();
	};
};

#endif
#endif

