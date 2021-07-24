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

#define XYO_VERSION_VERSION_ABCD                3,2,0,15
#define XYO_VERSION_VERSION_STR                 "3.2.0"
#define XYO_VERSION_VERSION_STR_BUILD           "15"
#define XYO_VERSION_VERSION_STR_DATETIME        "2021-07-23 16:34:00"

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

