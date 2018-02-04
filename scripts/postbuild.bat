@echo off
@REM usage: ./postbuild.bat path/to/solution path/to/build platform
SET SOLUTION_DIR=%1
SET BUILD_DIR=%2
SET PLATFORM=%3

echo Performing postbuild actions...

pushd %SOLUTION_DIR%

IF "%PLATFORM%" == "Win32" goto :PLATFORM86
IF "%PLATFORM%" == "x64" goto :PLATFORM64
ECHO ERROR Unknown platform, please use Win32 or x64
GOTO :ERROR

:PLATFORM86
ECHO Copying SDL2.dll (x86)
copy %SOLUTION_DIR%\thirdparty\SDL2\lib\x86\SDL2.dll %BUILD_DIR%\SDL2.dll
GOTO END

:PLATFORM64
ECHO Copying SDL2.dll (x64)
copy %SOLUTION_DIR%\thirdparty\SDL2\lib\x64\SDL2.dll %BUILD_DIR%\SDL2.dll
GOTO END

:ERROR
popd
exit /b 1

:END
popd