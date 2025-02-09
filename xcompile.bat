:: This file builds Pulse for many of the systems needed for our Pulse Unity Asset
:: Run this script from your Pulse source directory
:: It will create a new directory next to your source directory called 'dockcross-builds'
:: All builds will be placed here, and the binaries you need will be in each install directory
:: Note that for Android builds, you need to already have a completed build
:: The android protoc compiler cannot run on our linux based docker host
:: So we need to get those autogenerated files from a different build


set cwd=%CD%
:: Get the source directory name (folder only)
for %%I in (.) do set srcDir=%%~nxI
cd ..
mkdir dockcross-builds


call %cwd%\dockcross.bat manylinux2014-x64 .\%srcDir%\ .\dockcross-builds\
call %cwd%\dockcross.bat android-arm .\%srcDir%\ .\dockcross-builds\ .\dockcross-builds\pulse-engine-manylinux2014-x64
call wsl ./android-arm cp /usr/arm-linux-androideabi/sysroot/usr/lib/arm-linux-androideabi/libc++_shared.so /work/dockcross-builds/pulse-engine-android-arm/install/bin
call %cwd%\dockcross.bat android-arm64 .\%srcDir%\ .\dockcross-builds\ .\dockcross-builds\pulse-engine-manylinux2014-x64
call wsl ./android-arm64 cp /usr/aarch64-linux-android/sysroot/usr/lib/aarch64-linux-android/libc++_shared.so /work/dockcross-builds/pulse-engine-android-arm64/install/bin
call %cwd%\dockcross.bat linux-x64-clang .\%srcDir%\ .\dockcross-builds\

cd %cwd%