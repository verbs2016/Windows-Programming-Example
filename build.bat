@echo off
rem 2020-12-01

setlocal enabledelayedexpansion
set CURRENTDIR=%~dp0

cd /d %CURRENTDIR% || exit /b 1

if "%1" == "x86_debug" goto build_x86_debug
if "%1" == "x86_release" goto build_x86_release

:build_x86_debug
    if not exist build ( mkdir build )
    cd build
	cmake -G "Visual Studio 14 2015" -T v140_xp .. || goto :error
	cmake --build . --target ALL_BUILD --config Debug || goto :error
	goto :success

:build_x86_release
    if not exist build ( mkdir build )
    cd build
	cmake -G "Visual Studio 14 2015" -A x64 -T v140_xp .. || goto :error
	cmake --build . --target ALL_BUILD --config Release || goto :error
	goto :success

:success
	echo [+] Success. build project.
	exit /b 0

:error
	echo [-] Failed with error #%errorlevel%.
	cmd /c exit %errorlevel%
