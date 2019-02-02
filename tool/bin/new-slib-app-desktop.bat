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

echo New SLib App (Desktop): %APP_NAME%

mkdir "%CURRENT_PATH%\src"
xcopy /h /e "%SLIB_PATH%\tool\template\desktop-sapp\src" "%CURRENT_PATH%\src\"
"%SLIB_PATH%\bin\Windows\ReplaceTextInFile.exe" "%CURRENT_PATH%\src\cpp\app.h" "%CURRENT_PATH%\src\cpp\app.h" SLIB_TEMPLATE_APP_NAME %APP_NAME%
"%SLIB_PATH%\bin\Windows\ReplaceTextInFile.exe" "%CURRENT_PATH%\src\cpp\app.cpp" "%CURRENT_PATH%\src\cpp\app.cpp" SLIB_TEMPLATE_APP_NAME %APP_NAME%
rename "%CURRENT_PATH%\src\template_gen" gen

mkdir "%CURRENT_PATH%\macOS"
xcopy /h /e "%SLIB_PATH%\tool\template\desktop-sapp\macOS" "%CURRENT_PATH%\macOS\"
"%SLIB_PATH%\bin\Windows\ReplaceTextInFile.exe" "%CURRENT_PATH%\macOS\Project.xcodeproj\project.pbxproj" "%CURRENT_PATH%\macOS\Project.xcodeproj\project.pbxproj" SLIB_TEMPLATE_APP_NAME %APP_NAME%
rename "%CURRENT_PATH%\macOS\Project.xcodeproj" "%APP_NAME%.xcodeproj"
"%SLIB_PATH%\bin\Windows\ReplaceTextInFile.exe" "%CURRENT_PATH%\macOS\project\Base.lproj\MainMenu.xib" "%CURRENT_PATH%\macOS\project\Base.lproj\MainMenu.xib" SLIB_TEMPLATE_APP_NAME %APP_NAME%
"%SLIB_PATH%\bin\Windows\ReplaceTextInFile.exe" "%CURRENT_PATH%\macOS\project\main.mm" "%CURRENT_PATH%\macOS\project\main.mm" SLIB_TEMPLATE_APP_NAME %APP_NAME%

mkdir "%CURRENT_PATH%\Win32"
xcopy /h /e "%SLIB_PATH%\tool\template\desktop-sapp\Win32" "%CURRENT_PATH%\Win32\"
"%SLIB_PATH%\bin\Windows\ReplaceTextInFile.exe" "%CURRENT_PATH%\Win32\SLIB_TEMPLATE_APP_NAME.sln" "%CURRENT_PATH%\Win32\SLIB_TEMPLATE_APP_NAME.sln" SLIB_TEMPLATE_APP_NAME %APP_NAME%
rename "%CURRENT_PATH%\Win32\SLIB_TEMPLATE_APP_NAME.sln" "%APP_NAME%.sln"
"%SLIB_PATH%\bin\Windows\ReplaceTextInFile.exe" "%CURRENT_PATH%\Win32\SLIB_TEMPLATE_APP_NAME.vcxproj" "%CURRENT_PATH%\Win32\SLIB_TEMPLATE_APP_NAME.vcxproj" SLIB_TEMPLATE_APP_NAME %APP_NAME%
rename "%CURRENT_PATH%\Win32\SLIB_TEMPLATE_APP_NAME.vcxproj" "%APP_NAME%.vcxproj"
rename "%CURRENT_PATH%\Win32\SLIB_TEMPLATE_APP_NAME.vcxproj.filters" %APP_NAME%.vcxproj.filters"
"%SLIB_PATH%\bin\Windows\ReplaceTextInFile.exe" "%CURRENT_PATH%\Win32\main.cpp" "%CURRENT_PATH%\Win32\main.cpp" SLIB_TEMPLATE_APP_NAME %APP_NAME%
