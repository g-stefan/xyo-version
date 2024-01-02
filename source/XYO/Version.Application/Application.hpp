// Version
// Copyright (c) 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_VERSION_APPLICATION_APPLICATION_HPP
#define XYO_VERSION_APPLICATION_APPLICATION_HPP

#ifndef XYO_VERSION_DEPENDENCY_HPP
#	include <XYO/Version/Dependency.hpp>
#endif

namespace XYO::Version::Application {

	class Application : public virtual IApplication {
			XYO_DISALLOW_COPY_ASSIGN_MOVE(Application);

		public:
			inline Application(){};

			void showUsage();
			void showLicense();
			void showVersion();

			int main(int cmdN, char *cmdS[]);

			static void initMemory();
	};

};

#endif
