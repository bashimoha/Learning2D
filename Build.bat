@echo off

REM chekc options   
IF "%1"=="" goto help
IF "%1"=="compile" goto compile

REM help
:help
echo.
echo Usage: Build.bat [option]
echo.
echo Options:
echo.
echo compile - compile the project
GOTO end
REM compile
:compile
echo.
echo Compiling...
echo.
REM open vs command prompt
call .\3rdparty\premake\premake5.exe vs2022
if not defined DevEnvDir (
        call "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat"
)
set SoltuionFile = "Learning2d.sln"
set Configuration = "Debug"
set Platform = "x64"
msbuild /t:build /p:Configuration=Debug /p:Platform=x64 %SoltuionFile%
REM if success run the project
if %errorlevel% == 0 (
        echo.
        echo Running...
        echo.
       REM  C:\Dev\Learning2D\bin\Debug-x86_64\2DGame\2DGame.exe
        start bin\Debug-x86_64\2DGame\2DGame.exe
)
:end
echo.
echo Done.
echo.
pause
