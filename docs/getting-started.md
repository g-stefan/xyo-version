# Getting started

## 1. Build and install

The library and the tool are built with
[fabricare](https://github.com/g-stefan/fabricare). `xyo-platform`,
`xyo-managed-memory`, `xyo-data-structures`, `xyo-multithreading`,
`xyo-encoding`, `xyo-system` and `file-json` must be installed to the SDK
first. From the repository root:

```bash
fabricare make       # build into output/
fabricare test       # run output/bin/xyo-version on output/test/version.json
fabricare install    # copy output/{bin,include,lib} to ~/.fabricare/<platform>
fabricare clean      # remove output/ and temp/
```

Four projects are produced (`fabricare.json`):

| Project | Kind | Use it when |
|---------|------|-------------|
| `xyo-version` | DLL / shared library (`dll-or-lib`), the library | default, shared between several programs |
| `xyo-version.static` | static library, static CRT | self-contained executables |
| `xyo-version.application.static` | static library with the command line tool, no `main` | embed `xyo-version` into another tool (fabricare does) |
| `xyo-version` | executable | the `xyo-version` command |

The DLL and the executable share the name `xyo-version`, so the library
keeps its version under the key `xyo-version.library` in `version.json`
(`"versionName"`); the static variants reuse it (`"linkVersion"`).

## 2. Depend on it from another fabricare project

In the consumer's `fabricare.json`:

```json
{
	"name": "my-installer",
	"make": "exe",
	"sourcePath": "XYO/MyInstaller",
	"dependency": [
		"xyo-version"
	]
}
```

For the static variant use `"xyo-version.static"`; it exports
`XYO_VERSION_LIBRARY` to the consumer (`dependencyDefines`), which turns
`XYO_VERSION_EXPORT` into nothing. To embed the command line tool use
`"xyo-version.application.static"` (exports
`XYO_VERSION_APPLICATION_LIBRARY`, which leaves out `main`). `file-json`,
`xyo-system` and the layers below come in as transitive dependencies.

## 3. Include

```cpp
#include <XYO/Version.hpp>               // the library
#include <XYO/Version.Application.hpp>   // the library + the command line tool class
```

Namespace `XYO::Version` contains `using namespace XYO::System;`, so
`String`, `TDynamicArray`, `Shell::`, `DateTime`, ... are visible inside it.

The functions have short, generic names (`get`, `set`, `compare`). Call them
**qualified**, `XYO::Version::get(...)`, instead of
`using namespace XYO::Version;`: `compare` and `set` collide easily with
other names, and `XYO::System::ApplicationVersion::compare` is a different
function with a different signature.

The metadata namespaces exist in every XYO layer. Here the nested one is
`XYO::Version::Version`, so the library version is
`XYO::Version::Version::version()`.

## 4. First program

A tool that reads the version of a project from `version.json`, refuses to
run an older one and stamps a new build:

```cpp
#include <XYO/Version.hpp>

using namespace XYO::System;

class Application : public virtual IApplication {
		XYO_PLATFORM_DISALLOW_COPY_ASSIGN_MOVE(Application);

	public:
		inline Application(){};

		int main(int cmdN, char *cmdS[]);
};

int Application::main(int cmdN, char *cmdS[]) {
	String installed = XYO::Version::get("version.json", "my-library");
	printf("my-library %s\n", installed.value());

	if (XYO::Version::specificity(installed, "^2.1.0")) {
		printf("* Error: my-library ^2.1.0 required\n");
		return 1;
	};

	if (XYO::Version::compare(installed, "2.4.0") < 0) {
		printf("older than 2.4.0, using the compatibility path\n");
	};

	if (!XYO::Version::buildBump("version.json", "my-library")) {
		printf("* Error: my-library not found in version.json\n");
		return 1;
	};
	return 0;
};

XYO_APPLICATION_MAIN(Application);
```

`XYO_APPLICATION_MAIN` comes from `xyo-system`: it initializes the managed
memory registry and calls `Application::main`.

## 5. Conventions used by the whole library

- **`bool` means success.** `set`, the bump functions and
  `processTemplate` return `false` on any failure, there are no exceptions.
  Check the result.
- **Paths are relative to the current directory.** Nothing is searched; the
  version file discovery (`*.version.json`, `version.json`) belongs to the
  command line tool only.
- **The file is read and rewritten on every call.** There is no cache and
  no locking: one writer per version file at a time.
- **Only `major.minor.patch` is a version.** `build` is a separate counter,
  never part of `compare` or `specificity`.
