// Version
// Copyright (c) 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
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
		replace[0][0] = "${VERSION_ABCD}";
		replace[1][0] = "${VERSION_VERSION}";
		replace[2][0] = "${VERSION_BUILD}";
		replace[3][0] = "${VERSION_DATETIME}";
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

};
