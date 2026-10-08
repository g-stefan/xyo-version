# Version Specificity

A nodejs (npm semver range) like version specificity check.

```cpp
namespace XYO::Version {
	bool specificity(String version, String versionSpecificity);
};
```

Returns `true` when `version` does **not** satisfy `versionSpecificity`, that is
when the package needs to be updated, and `false` when the installed version is
already inside the requested range.

## Supported specificity

| Form | Example | Range |
| --- | --- | --- |
| any | `*`, `x`, `""`, `latest` | everything |
| exact / partial | `1.2.3`, `1.2`, `1.2.x`, `1` | `1.2.3`, `1.2.*`, `1.*` |
| caret | `^1.2.3`, `^0.2.3`, `^0.0.3`, `^0.0` | `<2.0.0`, `<0.3.0`, `<0.0.4`, `<0.1.0` |
| tilde | `~1.2.3`, `~1.2`, `~1` | `<1.3.0`, `<1.3.0`, `<2.0.0` |
| comparators | `>=1.2.7`, `>1.2`, `<=1.2`, `<1.2.3`, `=1.2.3` | npm equivalent |
| and | `>=1.2.7 <1.3.0` | intersection |
| or | `1.2.7 \|\| >=1.2.9 <2.0.0` | union |
| hyphen | `1.2.3 - 2.3.4`, `1.2 - 2.3` | `<2.3.5`, `<2.4.0` |

Comparators separated by space or comma are intersected (and), comparators
separated by `||` are unified (or). A hyphen range requires a space on both
sides of the hyphen, `1.2.3 - 2.3.4`.

## Version format

A version is `major[.minor[.patch]]`, a missing component is `0`.

A leading `v` is accepted and ignored, `v1.2.3` is `1.2.3`.

A `-prerelease` or `+build` suffix is accepted and ignored, `1.2.3-alpha` is
`1.2.3`. Only `major.minor.patch` is compared, the same as `compare`.

In a specificity, a component may be `x`, `X` or `*` and truncates the version
at that position, `1.2.x` is the partial version `1.2`.

An installed `version` that can not be parsed returns `true`, the package needs
to be updated.

## Examples

```cpp
specificity("1.2.3", "^1.2.3");                 // false, up to date
specificity("1.9.9", "^1.2.3");                 // false, up to date
specificity("2.0.0", "^1.2.3");                 // true, needs update
specificity("1.2.2", "^1.2.3");                 // true, needs update

specificity("0.2.9", "^0.2.3");                 // false, up to date
specificity("0.3.0", "^0.2.3");                 // true, needs update

specificity("1.2.9", "~1.2.3");                 // false, up to date
specificity("1.3.0", "~1.2.3");                 // true, needs update

specificity("1.2.99", ">=1.2.7 <1.3.0");        // false, up to date
specificity("1.3.0", ">=1.2.7 <1.3.0");         // true, needs update

specificity("1.4.6", "1.2.7 || >=1.2.9 <2.0.0");// false, up to date
specificity("1.2.8", "1.2.7 || >=1.2.9 <2.0.0");// true, needs update

specificity("2.3.4", "1.2.3 - 2.3.4");          // false, up to date
specificity("2.3.5", "1.2.3 - 2.3.4");          // true, needs update

specificity("1.2.3", "*");                      // false, up to date
specificity("", "*");                           // true, needs update
```

## Implementation

Every comparator is reduced to a half open interval `[low, high)`. Because a
prerelease is ignored, `>` and `<=` fold into `>=` and `<` by incrementing the
patch number, so only a lower and an upper bound are needed.

The specificity is a list of intervals, intersected by ` ` (and) or unified by
`||` (or). The parser consumes at least one character for every token, so a
malformed specificity terminates instead of looping.
