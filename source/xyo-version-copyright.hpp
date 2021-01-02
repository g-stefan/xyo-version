//
// XYO Version
//
// Copyright (c) 2020-2021 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef XYO_VERSION_COPYRIGHT_HPP
#define XYO_VERSION_COPYRIGHT_HPP

#define XYO_VERSION_COPYRIGHT            "Copyright (c) Grigore Stefan"
#define XYO_VERSION_PUBLISHER            "Grigore Stefan"
#define XYO_VERSION_COMPANY              XYO_VERSION_PUBLISHER
#define XYO_VERSION_CONTACT              "g_stefan@yahoo.com"
#define XYO_VERSION_FULL_COPYRIGHT       XYO_VERSION_COPYRIGHT " <" XYO_VERSION_CONTACT ">"

#ifndef XYO_RC

namespace XYOVersion {
	namespace Copyright {
		const char *copyright();
		const char *publisher();
		const char *company();
		const char *contact();
		const char *fullCopyright();
	};
};

#endif
#endif
