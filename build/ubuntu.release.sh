#!/bin/sh
# Public domain
# http://unlicense.org/
# Created by Grigore Stefan <g_stefan@yahoo.com>

echo "-> release xyo-version"

cmdX(){
	if ! "$@" ; then
		echo "Error: release"
		exit 1
	fi
}

export XYO_PATH_RELEASE=release

if ! xyo-cc xyo-version --has-archived-release --version-file=version.ini; then
	exit 0
fi
cmdX /bin/sh ./build/ubuntu.sh make
cmdX /bin/sh ./build/ubuntu.sh install
cmdX /bin/sh ./build/ubuntu.sh install-release
cmdX xyo-cc xyo-version --archive-release-sha512 --version-file=version.ini
