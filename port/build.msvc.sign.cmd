@echo off
rem Public domain
rem http://unlicense.org/
rem Created by Grigore Stefan <g_stefan@yahoo.com>

echo -^> sign xyo-version

pushd bin
for /r %%i in (*.exe) do call grigore-stefan.sign "XYO Version" "%%i"
popd
