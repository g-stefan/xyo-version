# API reference

## Headers

| Header | Contents |
|--------|----------|
| `<XYO/Version.hpp>` | the library: `Dependency.hpp` + `Library.hpp` |
| `<XYO/Version/Copyright.hpp>`, `License.hpp`, `Version.hpp` | library metadata (not included by `Version.hpp`) |
| `<XYO/Version.Application.hpp>` | the library + `XYO::Version::Application::Application` |

## Macros

| Macro | Meaning |
|-------|---------|
| `XYO_VERSION_EXPORT` | `dllexport` / `dllimport` / nothing |
| `XYO_VERSION_DLL_INTERNAL` | defined while building the DLL (exports) |
| `XYO_VERSION_LIBRARY` | static use, `XYO_VERSION_EXPORT` is empty (set by `xyo-version.static`) |
| `XYO_VERSION_APPLICATION_LIBRARY` | build the tool without `main` (set by `xyo-version.application.static`) |
| `XYO_VERSION_NO_VERSION` | `Version.rh` gives `0.0.0` instead of the generated values |
| `XYO_VERSION_VERSION_ABCD`, `_STR`, `_STR_BUILD`, `_STR_DATETIME`, `_STR_WITH_BUILD` | generated in `Version.rh` |

## namespace XYO::Version

`using namespace XYO::System;`

| Function | Returns |
|----------|---------|
| `String get(String versionFile, String projectName)` | `version` of the project, `"0.0.0"` if unknown |
| `bool set(String versionFile, String projectName, String version)` | set `version`; creates the project (build `0`) or the file |
| `bool buildBump(String versionFile, String projectName)` | `build + 1`; `false` if project missing |
| `bool patchBump(String versionFile, String projectName)` | `a.b.(c+1)`; `false` if project missing |
| `bool minorBump(String versionFile, String projectName)` | `a.(b+1).0`; `false` if project missing |
| `bool majorBump(String versionFile, String projectName)` | `(a+1).0.0`; `false` if project missing |
| `bool processTemplate(String versionFile, String projectName, String templateIn, String fileOut, size_t maxLineSize = XYO_SYSTEM_CONFIG_BUFFER_SIZE)` | expand `#{VERSION_*}`; `false` if project missing or I/O failed |
| `int compare(String versionA, String versionB)` | `< 0`, `0`, `> 0` on `major.minor.patch`; `-1` if A unparsable, `1` if B unparsable |
| `bool specificity(String version, String versionSpecificity)` | `true` if `version` is **not** in the npm range (needs update) |

All bump / set functions also set `date` and `time` to now (local).

## Template placeholders

| Placeholder | Value |
|-------------|-------|
| `#{VERSION_VERSION}` | `major.minor.patch` |
| `#{VERSION_BUILD}` | build |
| `#{VERSION_ABCD}` | `major,minor,patch,build` |
| `#{VERSION_DATETIME}` | `YYYY-MM-DD HH:MM:SS` |

## namespace XYO::Version::Version / Copyright / License

```cpp
const char *XYO::Version::Version::version();
const char *XYO::Version::Version::build();
const char *XYO::Version::Version::versionWithBuild();
const char *XYO::Version::Version::datetime();

const char *XYO::Version::Copyright::copyright();
const char *XYO::Version::Copyright::publisher();
const char *XYO::Version::Copyright::company();
const char *XYO::Version::Copyright::contact();

std::string XYO::Version::License::license();
std::string XYO::Version::License::shortLicense();
```

The same functions exist for the tool in `XYO::Version::Application::Version`,
`Copyright` and `License`.

## class XYO::Version::Application::Application

```cpp
class Application : public virtual IApplication {
	public:
		void showUsage();
		void showLicense();
		void showVersion();
		int main(int cmdN, char *cmdS[]);
		static void initMemory();
};
```

The `xyo-version` command line tool; `main` returns the exit code. See
[Command line](command-line.md).

## Command line options

`--help`, `--usage`, `--license`, `--version`, `--project=name`,
`--version-file=file`, `--get`, `--bump`, `--bump-build`, `--bump-patch`,
`--bump-minor`, `--bump-major`, `--no-bump`, `--file-in=file`,
`--file-out=file`, `--max-line-size=size`, `@file`.

## Specificity syntax

| Form | Example |
|------|---------|
| any | `*`, `x`, `""`, `latest` |
| exact / partial | `1.2.3`, `=1.2.3`, `1.2`, `1.2.x`, `1` |
| caret | `^1.2.3`, `^0.2.3`, `^0.0.3` |
| tilde | `~1.2.3`, `~1.2`, `~1` |
| comparators | `>=1.2.7`, `>1.2`, `<=1.2`, `<1.2.3` |
| and | `>=1.2.7 <1.3.0` (space or comma) |
| or | `1.2.7 \|\| >=1.2.9 <2.0.0` |
| hyphen | `1.2.3 - 2.3.4` (spaces required) |

Details: [Version specificity](version-specificity.md).
