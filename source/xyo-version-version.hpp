//
// XYO Version
//
// Copyright (c) 2020-2021 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef XYO_VERSION_VERSION_HPP
#define XYO_VERSION_VERSION_HPP

#define XYO_VERSION_VERSION_ABCD                3,4,0,28
#define XYO_VERSION_VERSION_STR                 "3.4.0"
#define XYO_VERSION_VERSION_STR_BUILD           "28"
#define XYO_VERSION_VERSION_STR_DATETIME        "2021-11-15 16:51:47"

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

