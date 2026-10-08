// Version
// Copyright (c) 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <XYO/Version/Library.hpp>

namespace XYO::Version {
	using namespace XYO::FileJSON;

	static bool has(Value *json, String projectName) {
		VAssociativeArray *jsonInfo = TDynamicCast<VAssociativeArray *>(json);
		if (!jsonInfo) {
			return false;
		};
		int k, m;
		String key;
		Value *item;
		VAssociativeArray *vAssociativeArray;
		for (k = 0; k < jsonInfo->value->length(); ++k) {
			key = jsonInfo->value->arrayKey->index(k);
			if (key == projectName) {
				return true;
			};
		};
		return false;
	};

	static bool get(Value *json, String projectName, String propertyName, String &value) {
		VAssociativeArray *jsonInfo = TDynamicCast<VAssociativeArray *>(json);
		if (!jsonInfo) {
			return false;
		};
		int k, m;
		String key;
		Value *item;
		VAssociativeArray *vAssociativeArray;
		for (k = 0; k < jsonInfo->value->length(); ++k) {
			key = jsonInfo->value->arrayKey->index(k);
			if (key == projectName) {
				item = jsonInfo->value->arrayValue->index(k);
				vAssociativeArray = TDynamicCast<VAssociativeArray *>(item);
				if (vAssociativeArray) {
					String vKey;
					Value *vItem;
					VString *vString;
					for (m = 0; m < vAssociativeArray->value->length(); ++m) {
						vKey = vAssociativeArray->value->arrayKey->index(m);
						if (vKey == propertyName) {
							vItem = vAssociativeArray->value->arrayValue->index(m);
							vString = TDynamicCast<VString *>(vItem);
							if (vString) {
								value = vString->value;
								return true;
							};
							return false;
						}
					};
				};
			};
		};
		return false;
	};

	static bool set(Value *json, String projectName, String propertyName, String value) {
		VAssociativeArray *jsonInfo = TDynamicCast<VAssociativeArray *>(json);
		if (!jsonInfo) {
			return false;
		};
		int k, m;
		String key;
		Value *item;
		VAssociativeArray *vAssociativeArray;
		for (k = 0; k < jsonInfo->value->length(); ++k) {
			key = jsonInfo->value->arrayKey->index(k);
			if (key == projectName) {
				break;
			};
		};
		if (k == jsonInfo->value->length()) {
			jsonInfo->value->set(projectName, TMemory<VAssociativeArray>::newMemory());
		};

		item = jsonInfo->value->arrayValue->index(k);
		vAssociativeArray = TDynamicCast<VAssociativeArray *>(item);
		if (!vAssociativeArray) {
			vAssociativeArray = TMemory<VAssociativeArray>::newMemory();
			jsonInfo->value->arrayValue->index(k) = vAssociativeArray;
		};
		String vKey;
		Value *vItem;
		VString *vString;
		for (m = 0; m < vAssociativeArray->value->length(); ++m) {
			vKey = vAssociativeArray->value->arrayKey->index(m);
			if (vKey == propertyName) {
				vItem = vAssociativeArray->value->arrayValue->index(m);
				vString = TDynamicCast<VString *>(vItem);
				if (vString) {
					vString->value = value;
				} else {
					vAssociativeArray->value->arrayValue->index(m) = VString::fromString(value);
				};
				return true;
			}
		};
		vAssociativeArray->set(propertyName, VString::fromString(value));
		return true;
	};

	String get(
	    String versionFile,
	    String projectName) {
		TPointer<Value> json;
		String value;
		if (load(versionFile, json)) {
			if (!get(json, projectName, "version", value)) {
				value = "0.0.0";
			};
			return value;
		};
		return "0.0.0";
	};

	bool set(
	    String versionFile,
	    String projectName,
	    String version) {
		TPointer<Value> json;
		char buf[32];
		DateTime now;
		if (load(versionFile, json)) {
			if (has(json, projectName)) {
				if (!set(json, projectName, "version", version)) {
					return false;
				};
				sprintf(buf, "%04u-%02u-%02u", now.getYear(), now.getMonth(), now.getDay());
				if (!set(json, projectName, "date", buf)) {
					return false;
				};
				sprintf(buf, "%02u:%02u:%02u", now.getHour(), now.getMinute(), now.getSecond());
				if (!set(json, projectName, "time", buf)) {
					return false;
				};
				return save(versionFile, json, Mode::IndentationTab);
			};
		} else {
			json = TMemory<VAssociativeArray>::newMemory();
		};
		if (!set(json, projectName, "version", version)) {
			return false;
		};
		if (!set(json, projectName, "build", "0")) {
			return false;
		};
		sprintf(buf, "%04u-%02u-%02u", now.getYear(), now.getMonth(), now.getDay());
		if (!set(json, projectName, "date", buf)) {
			return false;
		};
		sprintf(buf, "%02u:%02u:%02u", now.getHour(), now.getMinute(), now.getSecond());
		if (!set(json, projectName, "time", buf)) {
			return false;
		};
		return save(versionFile, json, Mode::IndentationTab);
	};

	bool buildBump(
	    String versionFile,
	    String projectName) {
		TPointer<Value> json;
		String value;
		int versionBuild;
		char buf[32];
		DateTime now;
		if (load(versionFile, json)) {
			if (has(json, projectName)) {
				if (!get(json, projectName, "build", value)) {
					value = "0";
				};
				if (sscanf(value.value(), "%d", &versionBuild) != 1) {
					versionBuild = 0;
				}
				++versionBuild;
				sprintf(buf, "%d", versionBuild);
				if (!set(json, projectName, "build", buf)) {
					return false;
				};
				sprintf(buf, "%04u-%02u-%02u", now.getYear(), now.getMonth(), now.getDay());
				if (!set(json, projectName, "date", buf)) {
					return false;
				};
				sprintf(buf, "%02u:%02u:%02u", now.getHour(), now.getMinute(), now.getSecond());
				if (!set(json, projectName, "time", buf)) {
					return false;
				};
				return save(versionFile, json, Mode::IndentationTab);
			};
		};
		return false;
	};

	bool patchBump(
	    String versionFile,
	    String projectName) {
		TPointer<Value> json;
		String value;
		int versionPatch;
		int versionMinor;
		int versionMajor;
		char buf[32];
		DateTime now;
		if (load(versionFile, json)) {
			if (has(json, projectName)) {
				if (!get(json, projectName, "version", value)) {
					value = "0.0.0";
				};
				if (sscanf(value.value(), "%d.%d.%d", &versionMajor, &versionMinor, &versionPatch) != 3) {
					versionMajor = 0;
					versionMinor = 0;
					versionPatch = 0;
				};
				++versionPatch;
				sprintf(buf, "%d.%d.%d", versionMajor, versionMinor, versionPatch);
				if (!set(json, projectName, "version", buf)) {
					return false;
				};
				sprintf(buf, "%04u-%02u-%02u", now.getYear(), now.getMonth(), now.getDay());
				if (!set(json, projectName, "date", buf)) {
					return false;
				};
				sprintf(buf, "%02u:%02u:%02u", now.getHour(), now.getMinute(), now.getSecond());
				if (!set(json, projectName, "time", buf)) {
					return false;
				};
				return save(versionFile, json, Mode::IndentationTab);
			};
		};
		return false;
	};

	bool minorBump(
	    String versionFile,
	    String projectName) {
		TPointer<Value> json;
		String value;
		int versionPatch;
		int versionMinor;
		int versionMajor;
		char buf[32];
		DateTime now;
		if (load(versionFile, json)) {
			if (has(json, projectName)) {
				if (!get(json, projectName, "version", value)) {
					value = "0.0.0";
				};
				if (sscanf(value.value(), "%d.%d.%d", &versionMajor, &versionMinor, &versionPatch) != 3) {
					versionMajor = 0;
					versionMinor = 0;
					versionPatch = 0;
				};
				versionPatch = 0;
				++versionMinor;
				sprintf(buf, "%d.%d.%d", versionMajor, versionMinor, versionPatch);
				if (!set(json, projectName, "version", buf)) {
					return false;
				};
				sprintf(buf, "%04u-%02u-%02u", now.getYear(), now.getMonth(), now.getDay());
				if (!set(json, projectName, "date", buf)) {
					return false;
				};
				sprintf(buf, "%02u:%02u:%02u", now.getHour(), now.getMinute(), now.getSecond());
				if (!set(json, projectName, "time", buf)) {
					return false;
				};
				return save(versionFile, json, Mode::IndentationTab);
			};
		};
		return false;
	};

	bool majorBump(
	    String versionFile,
	    String projectName) {
		TPointer<Value> json;
		String value;
		int versionPatch;
		int versionMinor;
		int versionMajor;
		char buf[32];
		DateTime now;
		if (load(versionFile, json)) {
			if (has(json, projectName)) {
				if (!get(json, projectName, "version", value)) {
					value = "0.0.0";
				};
				if (sscanf(value.value(), "%d.%d.%d", &versionMajor, &versionMinor, &versionPatch) != 3) {
					versionMajor = 0;
					versionMinor = 0;
					versionPatch = 0;
				};
				versionPatch = 0;
				versionMinor = 0;
				++versionMajor;
				sprintf(buf, "%d.%d.%d", versionMajor, versionMinor, versionPatch);
				if (!set(json, projectName, "version", buf)) {
					return false;
				};
				sprintf(buf, "%04u-%02u-%02u", now.getYear(), now.getMonth(), now.getDay());
				if (!set(json, projectName, "date", buf)) {
					return false;
				};
				sprintf(buf, "%02u:%02u:%02u", now.getHour(), now.getMinute(), now.getSecond());
				if (!set(json, projectName, "time", buf)) {
					return false;
				};
				return save(versionFile, json, Mode::IndentationTab);
			};
		};
		return false;
	};

	bool processTemplate(
	    String versionFile,
	    String projectName,
	    String templateIn,
	    String fileOut,
	    size_t maxLineSize) {
		TPointer<Value> json;
		TDynamicArray<TDynamicArray<String>> replace;
		String value;
		String datetime;
		char buf[64];
		DateTime now;
		replace[0][0] = "#{VERSION_ABCD}";
		replace[1][0] = "#{VERSION_VERSION}";
		replace[2][0] = "#{VERSION_BUILD}";
		replace[3][0] = "#{VERSION_DATETIME}";
		replace[0][1] = "0,0,0,0";
		replace[1][1] = "0.0.0";
		replace[2][1] = "0";

		int a, b, c, d;

		a = 0;
		b = 0;
		c = 0;
		d = 0;

		sprintf(buf, "%04u-%02u-%02u %02u:%02u:%02u",
		        now.getYear(), now.getMonth(), now.getDay(),
		        now.getHour(), now.getMinute(), now.getSecond());
		replace[3][1] = buf;

		if (load(versionFile, json)) {
			if (has(json, projectName)) {
				if (get(json, projectName, "version", value)) {
					replace[1][1] = value;
					if (sscanf(value.value(), "%d.%d.%d", &a, &b, &c) != 3) {
						a = 0;
						b = 0;
						c = 0;
					};
				};
				if (get(json, projectName, "build", value)) {
					replace[2][1] = value;
					if (sscanf(value.value(), "%d", &d) != 1) {
						d = 0;
					};
				};

				sprintf(buf, "%d,%d,%d,%d", a, b, c, d);
				replace[0][1] = buf;

				if (!get(json, projectName, "date", value)) {
					sprintf(buf, "%04u-%02u-%02u", now.getYear(), now.getMonth(), now.getDay());
					value = buf;
				};
				datetime = value;
				datetime += " ";
				if (!get(json, projectName, "time", value)) {
					sprintf(buf, "%02u:%02u:%02u", now.getHour(), now.getMinute(), now.getSecond());
					value = buf;
				};
				datetime += value;
				replace[3][1] = datetime;
				return Shell::fileReplaceText(
				    templateIn,
				    fileOut,
				    replace,
				    maxLineSize);
			};
		};
		return false;
	};

	int compare(String versionA, String versionB) {
		int versionAPatch;
		int versionAMinor;
		int versionAMajor;
		int versionBPatch;
		int versionBMinor;
		int versionBMajor;
		if (sscanf(versionA.value(), "%d.%d.%d", &versionAMajor, &versionAMinor, &versionAPatch) != 3) {
			return -1;
		};
		if (sscanf(versionB.value(), "%d.%d.%d", &versionBMajor, &versionBMinor, &versionBPatch) != 3) {
			return 1;
		};
		if(versionAMajor==versionBMajor) {
			if(versionAMinor==versionBMinor) {
				return (versionAPatch-versionBPatch);
			}
			return (versionAMinor-versionBMinor);
		}
				
		return (versionAMajor-versionBMajor);
	};

	//
	// Version specificity, nodejs (npm semver range) like.
	//
	// Every supported specificity is reduced to a half open interval
	// [low, high) and a specificity may be a list of intervals
	// intersected by " " (and) or unified by "||" (or).
	//

	static const int specificityInfinite = 0x7FFFFFFF;

	struct SpecificityVersion {
			int major;
			int minor;
			int patch;
			int level; // number of parsed components, 0 = any
	};

	static int specificityCompare(SpecificityVersion &versionA, SpecificityVersion &versionB) {
		if (versionA.major == versionB.major) {
			if (versionA.minor == versionB.minor) {
				return (versionA.patch - versionB.patch);
			};
			return (versionA.minor - versionB.minor);
		};
		return (versionA.major - versionB.major);
	};

	static void specificitySet(SpecificityVersion &version, int major, int minor, int patch) {
		version.major = major;
		version.minor = minor;
		version.patch = patch;
		version.level = 3;
	};

	static const char *specificitySkipSpace(const char *scan) {
		while ((*scan == ' ') || (*scan == '\t') || (*scan == ',')) {
			++scan;
		};
		return scan;
	};

	//
	// Parse [v]major[.minor[.patch]][-prerelease][+build]
	// where a component may be "x", "X" or "*".
	// Prerelease and build metadata are accepted and ignored.
	//

	static const char *specificityParseVersion(const char *scan, SpecificityVersion &version) {
		int index;
		int value;

		version.major = 0;
		version.minor = 0;
		version.patch = 0;
		version.level = 0;

		if ((*scan == 'v') || (*scan == 'V')) {
			++scan;
		};

		for (index = 0; index < 3; ++index) {
			if ((*scan == 'x') || (*scan == 'X') || (*scan == '*')) {
				++scan;
				break;
			};
			if ((*scan < '0') || (*scan > '9')) {
				break;
			};
			value = 0;
			while ((*scan >= '0') && (*scan <= '9')) {
				value = value * 10 + (*scan - '0');
				++scan;
			};
			switch (index) {
				case 0:
					version.major = value;
					break;
				case 1:
					version.minor = value;
					break;
				case 2:
					version.patch = value;
					break;
			};
			version.level = index + 1;
			if (*scan != '.') {
				break;
			};
			++scan;
		};

		// skip prerelease, build metadata or an unknown tag like "latest"
		while ((*scan != 0) && (*scan != ' ') && (*scan != '\t') && (*scan != ',') && (*scan != '|')) {
			++scan;
		};

		return scan;
	};

	//
	// Expand one comparator, given as operator + partial version,
	// into the interval [low, high)
	//

	static void specificityInterval(
	    const char *operator_,
	    SpecificityVersion &version,
	    SpecificityVersion &low,
	    SpecificityVersion &high) {

		specificitySet(low, 0, 0, 0);
		specificitySet(high, specificityInfinite, specificityInfinite, specificityInfinite);

		if (version.level == 0) {
			// "*", "x" or any tag, match all
			return;
		};

		if (operator_[0] == '^') {
			specificitySet(low, version.major, version.minor, version.patch);
			if (version.major > 0 || version.level == 1) {
				specificitySet(high, version.major + 1, 0, 0);
				return;
			};
			if (version.minor > 0 || version.level == 2) {
				specificitySet(high, 0, version.minor + 1, 0);
				return;
			};
			specificitySet(high, 0, 0, version.patch + 1);
			return;
		};

		if (operator_[0] == '~') {
			specificitySet(low, version.major, version.minor, version.patch);
			if (version.level == 1) {
				specificitySet(high, version.major + 1, 0, 0);
				return;
			};
			specificitySet(high, version.major, version.minor + 1, 0);
			return;
		};

		if (operator_[0] == '>') {
			if (operator_[1] == '=') {
				specificitySet(low, version.major, version.minor, version.patch);
				return;
			};
			// greater than any version matched by the partial version
			switch (version.level) {
				case 1:
					specificitySet(low, version.major + 1, 0, 0);
					break;
				case 2:
					specificitySet(low, version.major, version.minor + 1, 0);
					break;
				default:
					specificitySet(low, version.major, version.minor, version.patch + 1);
					break;
			};
			return;
		};

		if (operator_[0] == '<') {
			if (operator_[1] == '=') {
				// less or equal to any version matched by the partial version
				switch (version.level) {
					case 1:
						specificitySet(high, version.major + 1, 0, 0);
						break;
					case 2:
						specificitySet(high, version.major, version.minor + 1, 0);
						break;
					default:
						specificitySet(high, version.major, version.minor, version.patch + 1);
						break;
				};
				return;
			};
			specificitySet(high, version.major, version.minor, version.patch);
			return;
		};

		// exact or partial version, "1.2.3", "1.2", "1", "=1.2.3"
		specificitySet(low, version.major, version.minor, version.patch);
		switch (version.level) {
			case 1:
				specificitySet(high, version.major + 1, 0, 0);
				break;
			case 2:
				specificitySet(high, version.major, version.minor + 1, 0);
				break;
			default:
				specificitySet(high, version.major, version.minor, version.patch + 1);
				break;
		};
	};

	//
	// Check if version is matched by versionSpecificity
	//

	static bool specificityMatch(SpecificityVersion &version, String versionSpecificity) {
		const char *scan = versionSpecificity.value();
		char operator_[4];
		size_t operatorLn;
		SpecificityVersion parsed;
		SpecificityVersion parsedHigh;
		SpecificityVersion low;
		SpecificityVersion high;
		SpecificityVersion groupLow;
		SpecificityVersion groupHigh;

		if (scan == nullptr) {
			return true;
		};

		for (;;) {
			// begin of an "or" group
			specificitySet(groupLow, 0, 0, 0);
			specificitySet(groupHigh, specificityInfinite, specificityInfinite, specificityInfinite);

			for (;;) {
				scan = specificitySkipSpace(scan);
				if ((*scan == 0) || (*scan == '|')) {
					break;
				};

				operatorLn = 0;
				while ((operatorLn < 2) &&
				       ((*scan == '>') || (*scan == '<') || (*scan == '=') || (*scan == '~') || (*scan == '^'))) {
					operator_[operatorLn] = *scan;
					++operatorLn;
					++scan;
				};
				operator_[operatorLn] = 0;
				operator_[operatorLn + 1] = 0;

				scan = specificitySkipSpace(scan);
				scan = specificityParseVersion(scan, parsed);

				// hyphen range, "1.2.3 - 2.3.4"
				if (operatorLn == 0) {
					const char *scanHyphen = specificitySkipSpace(scan);
					if ((scanHyphen[0] == '-') && ((scanHyphen[1] == ' ') || (scanHyphen[1] == '\t'))) {
						scan = specificitySkipSpace(scanHyphen + 1);
						scan = specificityParseVersion(scan, parsedHigh);
						specificitySet(low, parsed.major, parsed.minor, parsed.patch);
						specificitySet(high, specificityInfinite, specificityInfinite, specificityInfinite);
						switch (parsedHigh.level) {
							case 0:
								break;
							case 1:
								specificitySet(high, parsedHigh.major + 1, 0, 0);
								break;
							case 2:
								specificitySet(high, parsedHigh.major, parsedHigh.minor + 1, 0);
								break;
							default:
								specificitySet(high, parsedHigh.major, parsedHigh.minor, parsedHigh.patch + 1);
								break;
						};
						if (specificityCompare(low, groupLow) > 0) {
							groupLow = low;
						};
						if (specificityCompare(high, groupHigh) < 0) {
							groupHigh = high;
						};
						continue;
					};
				};

				specificityInterval(operator_, parsed, low, high);

				// intersect with the group interval
				if (specificityCompare(low, groupLow) > 0) {
					groupLow = low;
				};
				if (specificityCompare(high, groupHigh) < 0) {
					groupHigh = high;
				};
			};

			if ((specificityCompare(version, groupLow) >= 0) && (specificityCompare(version, groupHigh) < 0)) {
				return true;
			};

			if (*scan == 0) {
				break;
			};

			// end of the "or" group
			while (*scan == '|') {
				++scan;
			};
			if (*scan == 0) {
				break;
			};
		};

		return false;
	};

	//
	// Return true if version does not match versionSpecificity,
	// the package needs to be updated.
	//

	bool specificity(String version, String versionSpecificity) {
		SpecificityVersion version_;

		if (version.value() == nullptr) {
			return true;
		};
		specificityParseVersion(version.value(), version_);
		if (version_.level == 0) {
			// unknown installed version
			return true;
		};
		version_.level = 3;

		return !specificityMatch(version_, versionSpecificity);
	};

};
