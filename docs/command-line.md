# Command line

```
xyo-version [options] [@file ...]
```

## Options

| Option | Effect |
|--------|--------|
| `--help`, `--usage` | print usage, exit 0 |
| `--license` | print the license, exit 0 |
| `--version` | print the tool's own version, exit 0 |
| `--project=name` | the entry of the version file to work on |
| `--version-file=file` | the version file, default `version.json` |
| `--get` | print the `version` of the project (no newline), exit 0 |
| `--bump`, `--bump-build` | increment `build` |
| `--bump-patch` | increment patch, `a.b.c` → `a.b.(c+1)` |
| `--bump-minor` | increment minor, `a.b.c` → `a.(b+1).0` |
| `--bump-major` | increment major, `a.b.c` → `(a+1).0.0` |
| `--no-bump` | cancel an earlier `--bump` / `--bump-build` (not the others) |
| `--file-in=file` | template to expand (needs `--file-out`) |
| `--file-out=file` | output of the template, its folder is created |
| `--max-line-size=size` | longest template line, default `32768` |
| `@file` | read more arguments from `file` (a response file) |

`--help`, `--usage`, `--license` and `--version` act as soon as they are
seen. Arguments that do not start with `--` (other than `@file`) and
unknown options are ignored. An empty `--project=`, `--version-file=`,
`--file-in=` or `--file-out=` is an error.

## Order of work

1. If no `--project` is given, choose the version file and the project (see
   below).
2. `--get`: print the version and stop; bumps and templates are skipped.
3. Bumps, in this fixed order whatever the order on the command line:
   build, patch, minor, major. Each bump of a project that is **not in the
   file** (or of a file that does not exist) creates the entry with version
   `0.0.0` and build `0` instead of bumping.
4. Template: if both `--file-in` and `--file-out` are given, expand the
   template with the (bumped) values.

Because minor resets patch and major resets minor, `--bump-major
--bump-minor` gives `(a+1).0.0`, not `(a+1).1.0`.

## Version file discovery

Without `--project`:

- if the version file (`version.json`, or `--version-file`) does not exist,
  the first `*.version.json` of the current directory is used, else
  `version.json`; none found → `Error: project version file not found`;
- the **first entry** of that file is the project.

With `--project`, the version file is used as given and created by the
first bump if missing.

## Exit codes

| Code | Meaning |
|------|---------|
| `0` | success, also for `--get` of an unknown project (prints `0.0.0`) |
| `1` | bad option value, response file not found, no version file / project, write failed, template failed (project not in file, input missing) |

## Response files

`@file` is replaced by the arguments read from `file`, split like a command
line (quotes group words). Several `@file` and normal arguments can be
mixed:

```
--project=xyo-system
--bump-build
--file-in=source/XYO/System/Version.Template.rh
--file-out=source/XYO/System/Version.rh
```

```
xyo-version @version.args
```

## Examples

New project, then a few builds:

```
> xyo-version --project=my-tool --bump
> xyo-version --project=my-tool --bump
> xyo-version --project=my-tool --get
0.0.0
> type version.json
{
	"my-tool": {
		"version": "0.0.0",
		"build": "1",
		"date": "2026-10-03",
		"time": "22:34:16"
	}
}
```

Start a release line and regenerate the header:

```
xyo-version --project=my-tool --bump-minor ^
            --file-in=source/XYO/MyTool/Version.Template.rh ^
            --file-out=source/XYO/MyTool/Version.rh
```

Use the version in a script (the output has no trailing newline):

```bash
VERSION=$(xyo-version --project=my-tool --get)
```

```bat
for /f %%v in ('xyo-version --project=my-tool --get') do set VERSION=%%v
```

A repository with several version files:

```
xyo-version --version-file=tools.version.json --project=packer --bump-patch
```
