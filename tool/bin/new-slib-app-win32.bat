@echo off

set CURRENT_PATH=%cd%
if defined SLIB_PATH goto setup_path_end
set BAT_PATH=%~dp0
set BAT_PATH=%BAT_PATH:~0,-1%
call %BAT_PATH%/../../setup-path.bat quick
cd %CURRENT_PATH%

:setup_path_end
set APP_NAME=%1
if not "%APP_NAME%"=="" goto input_app_name_end
set /p APP_NAME=Please input the application name:
if "%APP_NAME%"=="" goto :eof

:input_app_name_end
set APP_NAME=%APP_NAME:-=%
if "%APP_NAME%"=="" goto :eof
set APP_NAME=%APP_NAME:_=%
if "%APP_NAME%"=="" goto :eof
set APP_NAME=%APP_NAME: =%
if "%APP_NAME%"=="" goto :eof

echo New SLib Desktop App (Win32): %APP_NAME%
xcopy /h "%SLIB_PATH%\tool\template\Win32\.gitignore" "%CURRENT_PATH%"
copy "%SLIB_PATH%\tool\template\Win32\SLIB_TEMPLATE_APP_NAME.vcxproj.filters" "%CURRENT_PATH%\%APP_NAME%.vcxproj.filters"
"%SLIB_PATH%\bin\Windows\ReplaceTextInFile.exe" "%SLIB_PATH%\tool\template\Win32\SLIB_TEMPLATE_APP_NAME.sln" "%CURRENT_PATH%\%APP_NAME%.sln" SLIB_TEMPLATE_APP_NAME %APP_NAME%
"%SLIB_PATH%\bin\Windows\ReplaceTextInFile.exe" "%SLIB_PATH%\tool\template\Win32\SLIB_TEMPLATE_APP_NAME.vcxproj" "%CURRENT_PATH%\%APP_NAME%.vcxproj" SLIB_TEMPLATE_APP_NAME %APP_NAME%
"%SLIB_PATH%\bin\Windows\ReplaceTextInFile.exe" "%SLIB_PATH%\tool\template\Win32\main.cpp" "%CURRENT_PATH%\main.cpp" SLIB_TEMPLATE_APP_NAME %APP_NAME%
"%SLIB_PATH%\bin\Windows\ReplaceTextInFile.exe" "%SLIB_PATH%\tool\template\desktop\app.h" "%CURRENT_PATH%\app.h" SLIB_TEMPLATE_APP_NAME %APP_NAME%
"%SLIB_PATH%\bin\Windows\ReplaceTextInFile.exe" "%SLIB_PATH%\tool\template\desktop\app.cpp" "%CURRENT_PATH%\app.cpp" SLIB_TEMPLATE_APP_NAME %APP_NAME%
