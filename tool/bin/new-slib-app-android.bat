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
set APP_NAME=%APP_NAME: =%
if "%APP_NAME%"=="" goto :eof

set APP_ID=%2
if not "%APP_ID%"=="" goto input_app_id_end
set /p APP_ID=Please input the application id:
if "%APP_ID%"=="" goto :eof

:input_app_id_end
set APP_ID=%APP_ID:-=%
if "%APP_ID%"=="" goto :eof
set APP_ID=%APP_ID: =%
if "%APP_ID%"=="" goto :eof

echo New SLib Mobile App (Android): %APP_NAME% (%APP_ID%)

xcopy /h /e "%SLIB_PATH%\tool\template\Android" "%CURRENT_PATH%\"
"%SLIB_PATH%\bin\Windows\ReplaceTextInFile.exe" "%CURRENT_PATH%\settings.gradle" "%CURRENT_PATH%\settings.gradle" SLIB_TEMPLATE_APP_NAME %APP_NAME%
"%SLIB_PATH%\bin\Windows\ReplaceTextInFile.exe" "%CURRENT_PATH%\app\build.gradle" "%CURRENT_PATH%\app\build.gradle" SLIB_TEMPLATE_APP_ID %APP_ID%
"%SLIB_PATH%\bin\Windows\ReplaceTextInFile.exe" "%CURRENT_PATH%\app\src\main\AndroidManifest.xml" "%CURRENT_PATH%\app\src\main\AndroidManifest.xml" SLIB_TEMPLATE_APP_ID %APP_ID%
"%SLIB_PATH%\bin\Windows\ReplaceTextInFile.exe" "%CURRENT_PATH%\app\src\main\res\values\strings.xml" "%CURRENT_PATH%\app\src\main\res\values\strings.xml" SLIB_TEMPLATE_APP_NAME %APP_NAME%
"%SLIB_PATH%\bin\Windows\ReplaceTextInFile.exe" "%CURRENT_PATH%\app\src\main\java\MainActivity.java" "%CURRENT_PATH%\app\src\main\java\MainActivity.java" SLIB_TEMPLATE_APP_ID %APP_ID%
set PACKAGE_DIR=%APP_ID:.=\%
mkdir "%CURRENT_PATH%\app\src\main\java\%PACKAGE_DIR%"
copy "%CURRENT_PATH%\app\src\main\java\MainActivity.java" "%CURRENT_PATH%\app\src\main\java\%PACKAGE_DIR%\"
del "%CURRENT_PATH%\app\src\main\java\MainActivity.java"
"%SLIB_PATH%\bin\Windows\ReplaceTextInFile.exe" "%CURRENT_PATH%\app\src\main\cpp\native-lib.cpp" "%CURRENT_PATH%\app\src\main\cpp\native-lib.cpp" SLIB_TEMPLATE_APP_NAME %APP_NAME%

"%SLIB_PATH%\bin\Windows\ReplaceTextInFile.exe" "%SLIB_PATH%\tool\template\mobile\app.h" "%CURRENT_PATH%\app\src\main\cpp\app.h" SLIB_TEMPLATE_APP_NAME %APP_NAME%
"%SLIB_PATH%\bin\Windows\ReplaceTextInFile.exe" "%SLIB_PATH%\tool\template\mobile\app.cpp" "%CURRENT_PATH%\app\src\main\cpp\app.cpp" SLIB_TEMPLATE_APP_NAME %APP_NAME%
