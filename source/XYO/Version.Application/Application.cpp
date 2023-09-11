// Version
// Copyright (c) 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <XYO/Version.hpp>
#include <XYO/Version.Application/Application.hpp>
#include <XYO/Version.Application/Copyright.hpp>
#include <XYO/Version.Application/License.hpp>
#include <XYO/Version.Application/Version.hpp>

namespace XYO::Version::Application {

	void Application::showUsage() {
		printf("XYO Version\n");
		showVersion();
		printf("%s\n\n", ::XYO::Version::Application::Copyright::copyright());

		printf("%s",
		       "options:\n"
		       "    --help                  this info\n"
		       "    --usage                 this info\n"
		       "    --license               show license\n"
		       "    --version               show version\n"
		       "    --project=name          project name\n"
		       "    --no-bump               do not increment version\n"
		       "    --bump                  increment version build number\n"
		       "    --bump-build            increment version build number\n"
		       "    --bump-patch            increment version patch number\n"
		       "    --bump-minor            increment version minor number\n"
		       "    --bump-major            increment version major number\n"
		       "    --version-file=file     use file for version info\n"
		       "    --file-in=file          input file for replace\n"
		       "    --file-out=file         output file for replace\n"
		       "    --max-line-size=size    line size for replace\n"
		       "    --get                   show project version info\n");
		printf("\n");
	};

	void Application::showLicense() {
		printf("%s", ::XYO::Version::Application::License::license().c_str());
	};

	void Application::showVersion() {
		printf("version %s build %s [%s]\n", ::XYO::Version::Application::Version::version(), ::XYO::Version::Application::Version::build(), ::XYO::Version::Application::Version::datetime());
	};

	void Application::initMemory() {
		String::initMemory();
		TDynamicArray<String>::initMemory();
	};

	int Application::main(int cmdN, char *cmdS[]) {
		int i;
		String opt;
		size_t optIndex;
		String optValue;
		TDynamicArray<String> cmdLine;

		// ---

		String versionFile = "version.json";
		bool bumpVersionBuild = false;
		bool bumpVersionPatch = false;
		bool bumpVersionMinor = false;
		bool bumpVersionMajor = false;
		bool hasBumpVersion = false;
		String projectName;
		String fileIn;
		String fileOut;
		int maxLineSize = 32768;
		bool getVersion = false;

		// ---

		for (i = 1; i < cmdN; ++i) {
			if (StringCore::beginWith(cmdS[i], "@")) {
				String content;
				if (Shell::fileGetContents(&cmdS[i][1], content)) {
					int cmdNX;
					char **cmdSX;
					int m;
					Shell::mainArgsSet(content, cmdNX, cmdSX);
					for (m = 0; m < cmdNX; ++m) {
						cmdLine.push(cmdSX[m]);
					};
					Shell::mainArgsDelete(cmdNX, cmdSX);
					continue;
				};
				printf("Error: file not found - %s\n", &cmdS[i][1]);
				return 1;
			};
			cmdLine.push(cmdS[i]);
		};

		for (i = 0; i < cmdLine.length(); ++i) {
			if (StringCore::beginWith(cmdLine[i], "--")) {
				opt = cmdLine[i].index(2);
				optValue = "";
				if (opt.indexOf("=", 0, optIndex)) {
					optValue = opt.substring(optIndex + 1);
					opt = opt.substring(0, optIndex);
				};
				if (opt == "help") {
					showUsage();
					return 0;
				};
				if (opt == "usage") {
					showUsage();
					return 0;
				};
				if (opt == "license") {
					showLicense();
					return 0;
				};
				if (opt == "version") {
					showVersion();
					return 0;
				};
				if (opt == "project") {
					if (optValue.length() == 0) {
						printf("Error: project is empty\n");
						return 1;
					};
					projectName = optValue;
					continue;
				};
				if (opt == "no-bump") {
					bumpVersionBuild = false;
					hasBumpVersion = false;
					continue;
				};
				if (opt == "bump") {
					bumpVersionBuild = true;
					hasBumpVersion = true;
					continue;
				};
				if (opt == "bump-build") {
					bumpVersionBuild = true;
					hasBumpVersion = true;
					continue;
				};
				if (opt == "bump-patch") {
					bumpVersionPatch = true;
					hasBumpVersion = true;
					continue;
				};
				if (opt == "bump-minor") {
					bumpVersionMinor = true;
					hasBumpVersion = true;
					continue;
				};
				if (opt == "bump-major") {
					bumpVersionMajor = true;
					hasBumpVersion = true;
					continue;
				};
				if (opt == "version-file") {
					if (optValue.length() == 0) {
						printf("Error: version-file is empty\n");
						return 1;
					};
					versionFile = optValue;
					continue;
				};
				if (opt == "file-in") {
					if (optValue.length() == 0) {
						printf("Error: file-in is empty\n");
						return 1;
					};
					fileIn = optValue;
					continue;
				};
				if (opt == "file-out") {
					if (optValue.length() == 0) {
						printf("Error: file-out is empty\n");
						return 1;
					};
					fileOut = optValue;
					continue;
				};
				if (opt == "max-line-size") {
					sscanf(optValue.value(), "%d", &maxLineSize);
					continue;
				};
				if (opt == "get") {
					getVersion = true;
					continue;
				};
				continue;
			};
		};

		if (cmdLine.length() == 0) {
			showUsage();
			return 0;
		};

		// ---

		//
		// If no project given, search for a ".version.json" or "version.json" file
		// and use first section as project
		//

		if (projectName.length() == 0) {
			bool isOk = false;
			if (!Shell::fileExists(versionFile)) {
				TDynamicArray<String> fileList;
				Shell::getFileList("*.version.json", fileList);
				if (fileList.length() == 0) {
					Shell::getFileList("version.json", fileList);
					if (fileList.length() == 0) {
						printf("Error: project version file not found\n");
						return 1;
					};
				};
				versionFile = fileList[0];
			};
			TPointer<FileJSON::Value> json;
			if (FileJSON::load(versionFile, json)) {
				FileJSON::VAssociativeArray *jsonInfo = TDynamicCast<FileJSON::VAssociativeArray *>(json);
				if (jsonInfo) {
					if (jsonInfo->value->length()) {
						projectName = jsonInfo->value->arrayKey->index(0);
						if (projectName.length()) {
							isOk = true;
						};
					};
				};
			};
			if (!isOk) {
				printf("Error: project not found or not specified\n");
				return 1;
			};
		};

		// ---

		if (getVersion) {
			String projectVersion = get(versionFile, projectName);
			printf("%s", projectVersion.value());
			return 0;
		};

		if (bumpVersionBuild) {
			if (!buildBump(versionFile, projectName)) {
				if (!set(versionFile, projectName, "0.0.0")) {
					return 1;
				};
			};
		};

		if (bumpVersionPatch) {
			if (!patchBump(versionFile, projectName)) {
				if (!set(versionFile, projectName, "0.0.0")) {
					return 1;
				};
			};
		};

		if (bumpVersionMinor) {
			if (!minorBump(versionFile, projectName)) {
				if (!set(versionFile, projectName, "0.0.0")) {
					return 1;
				};
			};
		};

		if (bumpVersionMajor) {
			if (!majorBump(versionFile, projectName)) {
				if (!set(versionFile, projectName, "0.0.0")) {
					return 1;
				};
			};
		};

		if (fileIn.length() > 0) {
			if (fileOut.length() > 0) {
				if (!processTemplate(versionFile, projectName, fileIn, fileOut, maxLineSize)) {
					return 1;
				};
			};
		};

		return 0;
	};
};

#ifndef XYO_VERSION_APPLICATION_LIBRARY
XYO_APPLICATION_MAIN(::XYO::Version::Application::Application);
#endif
