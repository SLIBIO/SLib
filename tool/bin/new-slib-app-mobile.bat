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

echo New SLib App (Mobile): %APP_NAME% (%APP_ID%)

mkdir "%CURRENT_PATH%\src"
xcopy /h /e "%SLIB_PATH%\tool\template\mobile-sapp\src" "%CURRENT_PATH%\src\"
"%SLIB_PATH%\bin\Windows\ReplaceTextInFile.exe" "%CURRENT_PATH%\src\cpp\app.h" "%CURRENT_PATH%\src\cpp\app.h" SLIB_TEMPLATE_APP_NAME %APP_NAME%
"%SLIB_PATH%\bin\Windows\ReplaceTextInFile.exe" "%CURRENT_PATH%\src\cpp\app.cpp" "%CURRENT_PATH%\src\cpp\app.cpp" SLIB_TEMPLATE_APP_NAME %APP_NAME%
rename "%CURRENT_PATH%\src\template_gen" gen

mkdir "%CURRENT_PATH%\iOS"
xcopy /h /e "%SLIB_PATH%\tool\template\mobile-sapp\iOS" "%CURRENT_PATH%\iOS\"
"%SLIB_PATH%\bin\Windows\ReplaceTextInFile.exe" "%CURRENT_PATH%\iOS\Project.xcodeproj\project.pbxproj" "%CURRENT_PATH%\iOS\Project.xcodeproj\project.pbxproj" SLIB_TEMPLATE_APP_NAME %APP_NAME%
"%SLIB_PATH%\bin\Windows\ReplaceTextInFile.exe" "%CURRENT_PATH%\iOS\Project.xcodeproj\project.pbxproj" "%CURRENT_PATH%\iOS\Project.xcodeproj\project.pbxproj" SLIB-TEMPLATE-APP-ID %APP_ID%
rename "%CURRENT_PATH%\iOS\Project.xcodeproj" "%APP_NAME%.xcodeproj"
"%SLIB_PATH%\bin\Windows\ReplaceTextInFile.exe" "%CURRENT_PATH%\iOS\project\Base.lproj\LaunchScreen.storyboard" "%CURRENT_PATH%\iOS\project\Base.lproj\LaunchScreen.storyboard" SLIB_TEMPLATE_APP_NAME %APP_NAME%
"%SLIB_PATH%\bin\Windows\ReplaceTextInFile.exe" "%CURRENT_PATH%\iOS\project\main.mm" "%CURRENT_PATH%\iOS\project\main.mm" SLIB_TEMPLATE_APP_NAME %APP_NAME%

mkdir "%CURRENT_PATH%\Android"
xcopy /h /e "%SLIB_PATH%\tool\template\mobile-sapp\Android" "%CURRENT_PATH%\Android\"
"%SLIB_PATH%\bin\Windows\ReplaceTextInFile.exe" "%CURRENT_PATH%\Android\settings.gradle" "%CURRENT_PATH%\Android\settings.gradle" SLIB_TEMPLATE_APP_NAME %APP_NAME%
"%SLIB_PATH%\bin\Windows\ReplaceTextInFile.exe" "%CURRENT_PATH%\Android\app\build.gradle" "%CURRENT_PATH%\Android\app\build.gradle" SLIB_TEMPLATE_APP_ID %APP_ID%
"%SLIB_PATH%\bin\Windows\ReplaceTextInFile.exe" "%CURRENT_PATH%\Android\app\src\main\AndroidManifest.xml" "%CURRENT_PATH%\Android\app\src\main\AndroidManifest.xml" SLIB_TEMPLATE_APP_ID %APP_ID%
"%SLIB_PATH%\bin\Windows\ReplaceTextInFile.exe" "%CURRENT_PATH%\Android\app\src\main\res\values\strings.xml" "%CURRENT_PATH%\Android\app\src\main\res\values\strings.xml" SLIB_TEMPLATE_APP_NAME %APP_NAME%
"%SLIB_PATH%\bin\Windows\ReplaceTextInFile.exe" "%CURRENT_PATH%\Android\app\src\main\java\MainActivity.java" "%CURRENT_PATH%\Android\app\src\main\java\MainActivity.java" SLIB_TEMPLATE_APP_ID %APP_ID%
set PACKAGE_DIR=%APP_ID:.=\%
mkdir "%CURRENT_PATH%\Android\app\src\main\java\%PACKAGE_DIR%"
copy "%CURRENT_PATH%\Android\app\src\main\java\MainActivity.java" "%CURRENT_PATH%\Android\app\src\main\java\%PACKAGE_DIR%\"
del "%CURRENT_PATH%\Android\app\src\main\java\MainActivity.java"
"%SLIB_PATH%\bin\Windows\ReplaceTextInFile.exe" "%CURRENT_PATH%\Android\app\src\main\cpp\native-lib.cpp" "%CURRENT_PATH%\Android\app\src\main\cpp\native-lib.cpp" SLIB_TEMPLATE_APP_NAME %APP_NAME%
