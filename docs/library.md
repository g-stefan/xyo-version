# Library

```cpp
#include <XYO/Version.hpp>

namespace XYO::Version {
	String get(String versionFile, String projectName);
	bool set(String versionFile, String projectName, String version);
	bool buildBump(String versionFile, String projectName);
	bool patchBump(String versionFile, String projectName);
	bool minorBump(String versionFile, String projectName);
	bool majorBump(String versionFile, String projectName);
	bool processTemplate(String versionFile, String projectName, String templateIn, String fileOut,
	                     size_t maxLineSize = XYO_SYSTEM_CONFIG_BUFFER_SIZE);
	int compare(String versionA, String versionB);
	bool specificity(String version, String versionSpecificity);
};
```

Every function that takes a `versionFile` loads it with `file-json`, works
on the entry `projectName` and, if it changes something, saves the whole
file back (tab indentation, order kept). See [Version file](version-file.md)
for the format.

## get

```cpp
String version = XYO::Version::get("version.json", "my-tool");   // "1.4.2"
```

Returns the `version` field of the project, or `"0.0.0"` if the file can
not be read, the project is missing or it has no `version`. It never fails,
so `"0.0.0"` also means "unknown": check the file yourself if the difference
matters.

## set

```cpp
if (!XYO::Version::set("version.json", "my-tool", "2.0.0")) {
	// write failed
};
```

| Situation | Result |
|-----------|--------|
| project exists | `version` replaced, `date` / `time` now, `build` kept |
| file exists, project missing | entry added: `version`, `build` `"0"`, `date`, `time` |
| file missing **or not valid JSON** | a new file with only this project |

The last row means a damaged version file is **replaced**, the other
projects in it are lost. `set` does not check the version string; pass
`major.minor.patch`.

## buildBump, patchBump, minorBump, majorBump

```cpp
XYO::Version::buildBump("version.json", "my-tool");   // build 7 -> 8
XYO::Version::patchBump("version.json", "my-tool");   // 1.4.2 -> 1.4.3
XYO::Version::minorBump("version.json", "my-tool");   // 1.4.3 -> 1.5.0
XYO::Version::majorBump("version.json", "my-tool");   // 1.5.0 -> 2.0.0
```

Each one updates `date` / `time` and saves. They return `false` when the
file can not be read **or the project is not in it** — they never create an
entry. Create it with `set` first, the way the command line tool does:

```cpp
if (!XYO::Version::buildBump(file, project)) {
	if (!XYO::Version::set(file, project, "0.0.0")) {
		return false;
	};
};
```

A `version` that is not three numbers is read as `0.0.0` before the bump; a
`build` that is not a number is read as `0`. The version bumps leave `build`
alone.

## processTemplate

```cpp
bool ok = XYO::Version::processTemplate(
    "version.json", "my-tool",
    "source/XYO/MyTool/Version.Template.rh",
    "source/XYO/MyTool/Version.rh",
    32768);
```

Expands `#{VERSION_VERSION}`, `#{VERSION_BUILD}`, `#{VERSION_ABCD}` and
`#{VERSION_DATETIME}` (table in [Version file](version-file.md#templates))
with `Shell::fileReplaceText`. Returns `false` if the version file can not
be read, the project is not in it, or the template can not be read / the
output written. The output folder is created. `maxLineSize` is the longest
line expected (default `XYO_SYSTEM_CONFIG_BUFFER_SIZE`, 4096; the tool
uses 32768); a placeholder across a longer line may be missed. Input and
output must be different files.

## compare

```cpp
int r = XYO::Version::compare("1.10.0", "1.9.3");   // > 0
if (XYO::Version::compare(installed, required) < 0) {
	// installed is older
};
```

Compares `major`, then `minor`, then `patch` as numbers. The result is the
**difference** of the first part that differs (`compare("1.10.0", "1.9.3")`
is `1`, `compare("3.0.0", "1.0.0")` is `2`), so test only its sign.

Both strings must start with three numbers `a.b.c`; anything after them is
ignored (`"1.2.3-beta"` and `"1.2.3.45"` are `1.2.3`). `"1.2"`, `"v1.2.3"`
or `""` do not parse: if `versionA` does not parse the result is `-1`,
else if `versionB` does not parse it is `1`. For partial versions, a `v`
prefix and ranges use `specificity`.

`XYO::System::ApplicationVersion::compare` (from `xyo-system`) is a
different function: it compares up to four parts, accepts partial versions
and returns the result through a reference.

## specificity

```cpp
if (XYO::Version::specificity(installed, "^1.2.3")) {
	// installed does not satisfy ^1.2.3, update it
};
```

An npm semver range check. Returns **`true` when the version does not
satisfy the range** (the package needs an update) and `false` when it is
already inside. An installed version that can not be parsed returns `true`.
The full syntax is in [Version specificity](version-specificity.md).

## Embedding the command line tool

Depend on `xyo-version.application.static` and call the tool class with an
argument vector, the way fabricare does:

```cpp
#include <XYO/Version.Application.hpp>

int runXYOVersion(TDynamicArray<String> &arguments) {
	int cmdN = (int)arguments.length() + 1;
	char **cmdS = new char *[cmdN];
	cmdS[0] = const_cast<char *>("xyo-version");
	for (int k = 1; k < cmdN; ++k) {
		cmdS[k] = const_cast<char *>(arguments[k - 1].value());
	};
	int retV;
	{
		XYO::Version::Application::Application application;
		retV = application.main(cmdN, cmdS);
	};
	delete[] cmdS;
	return retV;
};
```

`cmdS[0]` is the program name and is skipped. `TDynamicArray` is not
contiguous memory, so build a plain `char *` array. The tool prints to `stdout`
and returns the exit code; it never calls `exit`.

## Library metadata

As in every XYO library (the headers are not included by
`<XYO/Version.hpp>`):

```cpp
#include <XYO/Version/Version.hpp>
#include <XYO/Version/Copyright.hpp>
#include <XYO/Version/License.hpp>

XYO::Version::Version::version();            // "8.6.0"
XYO::Version::Version::build();              // "12"
XYO::Version::Version::versionWithBuild();   // "8.6.0.12"
XYO::Version::Version::datetime();           // "2026-09-16 23:03:07"
XYO::Version::Copyright::copyright();
XYO::Version::License::license();            // std::string
```

The tool has the same set in `XYO::Version::Application::Version`,
`Copyright` and `License`.
