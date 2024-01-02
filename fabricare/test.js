// Created by Grigore Stefan <g_stefan@yahoo.com>
// Public domain (Unlicense) <http://unlicense.org>
// SPDX-FileCopyrightText: 2022-2024 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: Unlicense

messageAction("test");

// ---

Shell.mkdirRecursivelyIfNotExists("output/test");
Shell.mkdirRecursivelyIfNotExists("temp");

// ---

Shell.setenv("PATH", Shell.realPath(Shell.getcwd()) + "\\output\\bin;" + Shell.getenv("PATH"));
runInPath("output/test", function() {
	exitIf(Shell.execute("../bin/xyo-version --project=project1 --bump"));
	exitIf(Shell.execute("../bin/xyo-version --project=project2 --bump"));
});
