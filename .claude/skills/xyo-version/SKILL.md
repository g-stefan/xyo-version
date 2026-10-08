---
name: xyo-version
description: >-
  How to use xyo-version, the XYO version number tool and C++ library
  (namespace XYO::Version) on top of xyo-system and file-json: the
  version.json file (one entry per project: version, build, date, time); the
  xyo-version command line (--project, --version-file, --get, --bump /
  --bump-build / --bump-patch / --bump-minor / --bump-major, --no-bump,
  --file-in / --file-out / --max-line-size, @response files,
  *.version.json discovery); the Version.Template.rh -> Version.rh
  templates with #{VERSION_VERSION}, #{VERSION_BUILD}, #{VERSION_ABCD},
  #{VERSION_DATETIME}; the functions get, set, buildBump, patchBump,
  minorBump, majorBump, processTemplate, compare and specificity (npm semver
  ranges ^ ~ >= < || hyphen x); embedding the tool through
  xyo-version.application.static and XYO::Version::Application::Application;
  how fabricare version / version-patch / version-minor / version-major and
  versionName / linkVersion / noVersion use it. Use when writing or
  reviewing code that includes <XYO/Version.hpp> or
  <XYO/Version.Application.hpp>, depends on "xyo-version" in
  fabricare.json, edits a version.json or a Version.Template.rh, bumps or
  reads a project version, checks a version against a range, or when
  working inside the xyo-version repository.
---

# xyo-version

Keeps the version of each project in a JSON file (`version.json`) and
writes it into the sources through templates. Two faces:

- the **`xyo-version` command line tool** (run by `fabricare version*`);
- the **C++ library** `XYO::Version` with the same operations plus
  `compare` and `specificity` (npm semver range check) for installers.

Built on `xyo-system` (see the `xyo-system` skill and the layers below it:
their rules apply) and `file-json`.

Full documentation: `docs/` in the xyo-version repository
(`X:\Storage\XYO\Gitea\CPP\xyo-version\docs` on this machine): README,
getting-started, **version-file**, **command-line**, library,
**version-specificity**, reference. Read the matching page when you need
more than this summary. The whole implementation is
`source/XYO/Version/Library.cpp` and
`source/XYO/Version.Application/Application.cpp`.

## Pick a tool

| Need | Command line | C++ |
|------|--------------|-----|
| Read a version | `xyo-version --project=p --get` (no newline) | `XYO::Version::get(file, p)` |
| Create entry / set version | first `--bump` of a new name → `0.0.0` build `0` | `XYO::Version::set(file, p, "1.0.0")` |
| Next build | `--bump` / `--bump-build` | `buildBump(file, p)` |
| Next patch / minor / major | `--bump-patch` / `--bump-minor` / `--bump-major` | `patchBump` / `minorBump` / `majorBump` |
| Generate `Version.rh` | `--file-in=…/Version.Template.rh --file-out=…/Version.rh` | `processTemplate(file, p, in, out, 32768)` |
| Order two versions | | `XYO::Version::compare(a, b)` (sign only) |
| Is installed version in range | | `XYO::Version::specificity(v, "^1.2.3")` (`true` = **needs update**) |
| From fabricare | `fabricare version` (build), `version-patch`, `version-minor`, `version-major` | |

## version.json

```json
{
	"my-tool": {
		"version": "1.4.2",
		"build": "37",
		"date": "2026-10-03",
		"time": "22:34:16"
	}
}
```

One entry per project name; all values are strings; written back with tabs,
order and unknown fields kept. `build` counts every build for the life of
the project and is **never reset** by patch / minor / major bumps.
Bumps: patch `a.b.(c+1)`, minor `a.(b+1).0`, major `(a+1).0.0`; each sets
`date` / `time` to now (local).

## Hard rules

1. **`specificity` is inverted**: it returns `true` when the version does
   **not** satisfy the range (update needed), `false` when it is already
   inside. An unparsable installed version (`""`) returns `true`.
2. **`compare` returns a difference, not -1/0/1**: test `< 0`, `== 0`,
   `> 0`. It needs `a.b.c` at the start of both strings (`"1.2"`,
   `"v1.2.3"`, `""` do not parse: A unparsable → `-1`, else B unparsable →
   `1`). Suffixes after the third number are ignored. For partial versions
   or `v` prefixes use `specificity`. Not the same as
   `XYO::System::ApplicationVersion::compare` (4 parts, result by reference).
3. **Library bumps never create a project**: `buildBump` / `patchBump` /
   `minorBump` / `majorBump` / `processTemplate` return `false` if the file
   or the project is missing. Do what the tool does: on `false`, call
   `set(file, p, "0.0.0")`.
4. **`set` on an unreadable or invalid JSON file rewrites it** with only
   that project — the others are lost. Check the file first when that
   matters.
5. **`get` never fails**: `"0.0.0"` means version `0.0.0` *or* unknown.
6. **Qualify the calls**: `XYO::Version::get(...)`, not
   `using namespace XYO::Version;` (`get`, `set`, `compare` are generic
   names). The library's own metadata is
   `XYO::Version::Version::version()` (nested namespace `Version`), from
   `<XYO/Version/Version.hpp>` which `<XYO/Version.hpp>` does not include.
7. **Tool bump order is fixed**: build, patch, minor, major, whatever the
   command line order; `--bump-major --bump-minor` gives `(a+1).0.0`.
   `--get` prints and exits before any bump or template. `--no-bump` only
   cancels `--bump` / `--bump-build`.
8. **Tool without `--project`**: uses the first entry of `version.json`
   (or `--version-file`); if that file does not exist, the first
   `*.version.json` in the current directory. With `--project`, a missing
   file is created by the first bump.
9. **Templates are line based** (`Shell::fileReplaceText`): a placeholder
   must be on one line, shorter than `maxLineSize` (library default 4096,
   tool 32768). The output folder is created; in and out must differ.
   Edit `Version.Template.rh`, never the generated `Version.rh` (it is
   committed). `.rh` because the Windows resource compiler includes it.
10. **Placeholders**: `#{VERSION_VERSION}` `1.4.2`, `#{VERSION_BUILD}`
    `37`, `#{VERSION_ABCD}` `1,4,2,37` (Windows `VERSIONINFO`),
    `#{VERSION_DATETIME}` `2026-10-03 22:34:16`.
11. **fabricare**: `fabricare version*` runs, per `make` project without
    `"noVersion": true` and without `"linkVersion"`,
    `--project=<versionName or name> --bump-* --file-in=source/<first
    sourcePath>/Version.Template.rh --file-out=…/Version.rh`. `fabricare
    make` does **not** bump. When a DLL and an exe share a name, give the
    DLL `"versionName": "<name>.library"`; static variants use
    `"linkVersion"` to reuse another project's version.
12. **Not thread or process safe**: every call loads and rewrites the whole
    file, no locking; one writer per version file at a time.

## Specificity ranges

| Form | Example | Matches |
|------|---------|---------|
| any | `*`, `x`, `""`, `latest` | everything |
| exact / partial | `1.2.3`, `=1.2.3`, `1.2`, `1.2.x`, `1` | `1.2.3`; `1.2.*`; `1.*` |
| caret | `^1.2.3`, `^0.2.3`, `^0.0.3` | `<2.0.0`, `<0.3.0`, `<0.0.4` |
| tilde | `~1.2.3`, `~1` | `<1.3.0`, `<2.0.0` |
| comparators | `>=1.2.7`, `>1.2`, `<=1.2`, `<1.2.3` | npm meaning |
| and | `>=1.2.7 <1.3.0` (space or comma) | intersection |
| or | `1.2.7 \|\| >=1.2.9 <2.0.0` | union |
| hyphen | `1.2.3 - 2.3.4` (spaces around `-` required) | `>=1.2.3 <2.3.5` |

Leading `v` and `-prerelease` / `+build` suffixes are accepted and ignored;
only `major.minor.patch` is compared.

## Depend on it

```json
"dependency": [ "xyo-version" ]                    // DLL
"dependency": [ "xyo-version.static" ]             // static, defines XYO_VERSION_LIBRARY
"dependency": [ "xyo-version.application.static" ] // embed the tool, no main
```

```cpp
#include <XYO/Version.hpp>

String installed = XYO::Version::get("version.json", "my-library");
if (XYO::Version::specificity(installed, "^2.1.0")) {
	printf("* Error: my-library ^2.1.0 required, found %s\n", installed.value());
	return 1;
};
if (!XYO::Version::buildBump("version.json", "my-library")) {
	if (!XYO::Version::set("version.json", "my-library", "0.0.0")) {
		return 1;
	};
};
```

Embedded tool (as fabricare's `xyoVersion(...)`): build a plain `char *`
array (`cmdS[0]` = program name, `TDynamicArray` is not contiguous) and call
`XYO::Version::Application::Application application;
application.main(cmdN, cmdS)` — it returns the exit code (`0` ok, `1`
error) and prints to stdout.

## Working inside this repository

- Four projects in `fabricare.json`: `xyo-version` (DLL, version key
  `xyo-version.library`), `xyo-version.static`,
  `xyo-version.application.static`, `xyo-version` (exe).
- `fabricare make`, then `fabricare test` (runs `output/bin/xyo-version`
  on `output/test/version.json`).
- Keep the docs in `docs/` and this skill in step with
  `Library.hpp` / `Application.cpp` when behavior changes.
- Licensing follows REUSE: `source/`, `docs/` and `README.md` are MIT
  (source files also carry SPDX headers); build scripts, config,
  `version.json` and `.claude/` are Unlicense. Every new top level file or
  folder needs a `Files:` entry in `.reuse/dep5` (check with `reuse lint`).
