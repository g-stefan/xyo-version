# Version file

## Format

A version file is a JSON object with **one entry per project name**. Every
entry has four string fields:

```json
{
	"xyo-version.library": {
		"version": "8.6.0",
		"build": "12",
		"date": "2026-09-16",
		"time": "23:03:07"
	},
	"xyo-version": {
		"version": "8.6.0",
		"build": "12",
		"date": "2026-09-16",
		"time": "23:03:07"
	}
}
```

| Field | Meaning |
|-------|---------|
| `version` | `major.minor.patch`, decimal numbers |
| `build` | build counter, decimal number, independent of `version` |
| `date` | `YYYY-MM-DD` of the last change, local time |
| `time` | `HH:MM:SS` of the last change, local time |

All values are **strings**. The file is written back with tab indentation,
entries and fields keep their order, unknown fields are kept.

The default name is `version.json` in the repository root. A repository may
use another name (`--version-file=...`); the command line tool also finds a
`*.version.json` when `version.json` is missing (see
[Command line](command-line.md#version-file-discovery)).

## How the numbers move

| Operation | `version` | `build` | `date` / `time` |
|-----------|-----------|---------|-----------------|
| new project (`set`, or the tool's first bump) | given (`0.0.0` for the tool) | `0` | now |
| `set` on an existing project | given | kept | now |
| build bump | kept | `+1` | now |
| patch bump | `a.b.(c+1)` | kept | now |
| minor bump | `a.(b+1).0` | kept | now |
| major bump | `(a+1).0.0` | kept | now |

The build number is **never reset**: it counts every build of the project
for its whole life, so `8.6.0.12` and `8.7.0.13` are consecutive builds. A
`version` that is not `a.b.c` is treated as `0.0.0` by the bumps.

## Templates

`processTemplate` (tool: `--file-in` / `--file-out`) copies a text file line
by line and replaces four placeholders with the values of one project:

| Placeholder | Value | Example |
|-------------|-------|---------|
| `#{VERSION_VERSION}` | `version` | `8.6.0` |
| `#{VERSION_BUILD}` | `build` | `12` |
| `#{VERSION_ABCD}` | `major,minor,patch,build` (Windows `VERSIONINFO` form) | `8,6,0,12` |
| `#{VERSION_DATETIME}` | `date time` | `2026-09-16 23:03:07` |

The output folder is created. A missing field falls back to `0.0.0`, `0`
or the current date and time. A placeholder split over two lines, or over a
line longer than the maximum line size, is not replaced.

The standard XYO template, `source/XYO/<Name>/Version.Template.rh`:

```cpp
#ifndef XYO_VERSION_VERSION_RH
#define XYO_VERSION_VERSION_RH

#ifndef XYO_VERSION_NO_VERSION
#define XYO_VERSION_VERSION_ABCD #{VERSION_ABCD}
#define XYO_VERSION_VERSION_STR "#{VERSION_VERSION}"
#define XYO_VERSION_VERSION_STR_BUILD "#{VERSION_BUILD}"
#define XYO_VERSION_VERSION_STR_DATETIME "#{VERSION_DATETIME}"
#define XYO_VERSION_VERSION_STR_WITH_BUILD "#{VERSION_VERSION}.#{VERSION_BUILD}"
#else
#define XYO_VERSION_VERSION_ABCD 0,0,0,0
#define XYO_VERSION_VERSION_STR "0.0.0"
#define XYO_VERSION_VERSION_STR_BUILD "0"
#define XYO_VERSION_VERSION_STR_DATETIME "0000-00-00"
#define XYO_VERSION_VERSION_STR_WITH_BUILD "0.0.0.0"
#endif

#endif
```

`Version.rh` is generated and committed; it is included by `Version.cpp`
(the `Version::version()`, `build()`, `versionWithBuild()`, `datetime()`
functions every XYO library has) and by the Windows `.rc` file
(`XYO_PLATFORM_VERSION_INFO`). Edit the template, never `Version.rh`.
`.rh` is used instead of `.h` because the file must stay valid for the
resource compiler.

## How fabricare uses it

`fabricare version` runs, in an `xyo-cpp` solution, for every project of
category `make` in `fabricare.json` that has neither `"noVersion": true`
nor a `"linkVersion"`:

```
xyo-version --project=<versionName or name> --bump-build
            --file-in=source/<first sourcePath>/Version.Template.rh
            --file-out=source/<first sourcePath>/Version.rh
```

So `fabricare version` increments the build number and regenerates
`Version.rh`; run it before `fabricare make` for a new build. `make` itself
does not touch `version.json`. The other version commands:

| fabricare command | Runs |
|-------------------|------|
| `fabricare version` | `--bump-build` + template |
| `fabricare version-patch` | `--bump-patch` + template |
| `fabricare version-minor` | `--bump-minor` + template |
| `fabricare version-major` | `--bump-major` + template |

Outside an `xyo-cpp` solution the same commands only bump, without a
template.

fabricare links the tool in (`xyo-version.application.static`) and calls it
as the internal function `xyoVersion(...)`, so no `xyo-version` executable
has to be on the `PATH` for a build.
