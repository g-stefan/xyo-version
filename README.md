# Version

Utility and C++ library for software version numbers
- Keeps the version of every project in a `version.json` file:
`version` (`major.minor.patch`), `build`, `date`, `time`.
- `xyo-version` command line tool: `--get`, `--bump` (build),
`--bump-patch`, `--bump-minor`, `--bump-major`, and templates
(`Version.Template.rh` to `Version.rh`) with `#{VERSION_VERSION}`,
`#{VERSION_BUILD}`, `#{VERSION_ABCD}`, `#{VERSION_DATETIME}`.
- Library `XYO::Version`: `get`, `set`, `buildBump`, `patchBump`,
`minorBump`, `majorBump`, `processTemplate`, `compare`, and `specificity`
(npm semver range check: `^1.2.3`, `~1.2`, `>=1.2.7 <1.3.0`, `||`, hyphen).

Built on `xyo-system` and `file-json`; used by `fabricare`
(`fabricare version`) to version every XYO C++ project.

## Documentation

- [Overview](docs/README.md) - purpose and design
- [Getting started](docs/getting-started.md) - build, depend on it, include, first program
- [Version file](docs/version-file.md) - `version.json`, version and build numbers, templates, fabricare
- [Command line](docs/command-line.md) - `xyo-version` options, file discovery, exit codes, examples
- [Library](docs/library.md) - `get`, `set`, bumps, `processTemplate`, `compare`, embedding the tool
- [Version specificity](docs/version-specificity.md) - npm semver like range check
- [API reference](docs/reference.md)

A Claude Code skill for this library is in
[.claude/skills/xyo-version](.claude/skills/xyo-version/SKILL.md).

## License

Copyright (c) 2016-2026 Grigore Stefan
Licensed under the [MIT](LICENSE) license.
