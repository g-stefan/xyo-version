//
// XYO Version
//
// Copyright (c) 2020-2022 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "xyo.hpp"
#include "xyo-version-application.hpp"
#include "xyo-version-copyright.hpp"
#include "xyo-version-license.hpp"
#ifndef XYO_VERSION_NO_VERSION
#include "xyo-version-version.hpp"
#endif

#include "xyo-cc-compiler.hpp"

namespace XYOVersion {

	using namespace XYO;
	using namespace XYOCC;

	void Application::showUsage() {
		printf("xyo-version - Utility for generating software version numbers\n");
		showVersion();
		printf("%s\n\n", XYOVersion::Copyright::fullCopyright());

		printf("%s",
			"options:\n"
			"    --usage                 this info\n"
			"    --license               show license\n"
			"    --version               show version\n"
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
			"    --get                   show project version info\n"
		);
		printf("\n");
	};

	void Application::showLicense() {
		printf("%s", XYOVersion::License::content());
	};

	void Application::showVersion() {
#ifndef XYO_VERSION_NO_VERSION
		printf("version %s build %s [%s]\n", XYOVersion::Version::version(), XYOVersion::Version::build(), XYOVersion::Version::datetime());
#endif
	};

	int Application::main(int cmdN, char *cmdS[]) {
		int i;
		String opt;
		size_t optIndex;
		String optValue;

		String versionFile = "version.ini";
		bool bumpVersionBuild = false;
		bool bumpVersionPatch = false;
		bool bumpVersionMinor = false;
		bool bumpVersionMajor = false;
		bool hasBumpVersion = false;
		String projectName;
		String fileIn;
		String fileOut;
		int maxLineSize = 16384;
		bool getVersion = false;

		if(cmdN < 1) {
			showUsage();
			return 0;
		};

		for (i = 1; i < cmdN; ++i) {
			if (StringCore::beginWith(cmdS[i], "--")) {
				opt = &cmdS[i][2];
				optValue = "";
				if(String::indexOf(opt, "=", 0, optIndex)) {
					optValue = String::substring(opt, optIndex + 1);
					opt = String::substring(opt, 0, optIndex);
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
					versionFile = optValue;
					if(versionFile.length() == 0) {
						printf("Error: version-file is empty\n");
						return 1;
					};
					continue;
				};
				if (opt == "file-in") {
					fileIn = optValue;
					if(fileIn.length() == 0) {
						printf("Error: file-in is empty\n");
						return 1;
					};
					continue;
				};
				if (opt == "file-out") {
					fileOut = optValue;
					if(fileOut.length() == 0) {
						printf("Error: file-out is empty\n");
						return 1;
					};
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
			projectName = cmdS[i];
		};

		//
		// If no project name given, search for a ".version.ini" or "version.ini" file
		// and use first section as project name
		//

		if(projectName.length() == 0) {
			bool isOk = false;
			if(!Shell::fileExists(versionFile)) {
				TDynamicArray<String> fileList;
				Shell::getFileList("*.version.ini", fileList);
				if(fileList.length() == 0) {
					Shell::getFileList("version.ini", fileList);
					if(fileList.length() == 0) {
						printf("Error: project version file not found\n");
						return 1;
					};
				};
				versionFile = fileList[0];
			};
			INIFile iniFile;
			if(INIFileX::load(versionFile, iniFile)) {
				if(INIFileX::getSection(iniFile, 0, projectName)) {
					if(projectName.length() > 0) {
						isOk = true;
					};
				};
			};
			if(!isOk) {
				printf("Error: project not found or not specified\n");
				return 1;
			};
		};

		if(getVersion) {
			String projectVersion=Compiler::getVersion(versionFile, projectName);
			printf("%s", projectVersion.value());
			return 0;
		};

		if(bumpVersionBuild) {
			if(!Compiler::versionBuildBump(versionFile, projectName)) {
				if(!Compiler::versionSetVersion(versionFile, projectName, "0.0.0")) {
					return 1;
				};
			};
		};

		if(bumpVersionPatch) {
			if(!Compiler::versionPatchBump(versionFile, projectName)) {
				if(!Compiler::versionSetVersion(versionFile, projectName, "0.0.0")) {
					return 1;
				};
			};
		};

		if(bumpVersionMinor) {
			if(!Compiler::versionMinorBump(versionFile, projectName)) {
				if(!Compiler::versionSetVersion(versionFile, projectName, "0.0.0")) {
					return 1;
				};
			};
		};

		if(bumpVersionMajor) {
			if(!Compiler::versionMajorBump(versionFile, projectName)) {
				if(!Compiler::versionSetVersion(versionFile, projectName, "0.0.0")) {
					return 1;
				};
			};
		};

		if(fileIn.length() > 0) {
			if(fileOut.length() > 0) {
				if(!Compiler::versionProcessTemplate(versionFile,
						projectName,
						fileIn,
						fileOut,
						maxLineSize)) {
					return 1;
				};
			};
		};

		return 0;
	};

};

#ifndef XYO_VERSION_LIBRARY
XYO_APPLICATION_MAIN_STD(XYOVersion::Application);
#endif

