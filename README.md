# SLib

SLib is a universal cross-platform library, specially designed for the various UI components, OpenGL rendering and networking solutions.

SLib enables software developers to build multi-platform(Android/iOS/MacOS/Tizen/Win32) applications and games under the most effective workflows and the coding conventions of C++, while providing easy-to-use toolkits to develop HTTP/Socket servers on the Linux/MacOS/Win32 platforms.


## Modules

* core

    Auto Reference Counting, String, List, Map, File, Thread, Synchronizing, Time, Variant, Json, ...

* crypto
   
    AES, RSA, GZIP, SHA1/2, GCM, MD5, Blowfish, ...

* db

    SQLite, MySQL

* device
    
    Sensor, Vibrator

* graphics

    Bitmap, Image, Canvas, Font, ...

* math

    BigInt, Uint128, Matrix, Vector, Transform, ...

* media

    MediaPlayer, AudioRecorder, AudioPlayer, Camera, ...

* network

    UrlRequest, Socket, Asynchronous I/O, HttpService, ...

* render

    RenderEngine, RenderProgram, VertexBuffer, IndexBuffer, Texture, ...

* ui

    Window, View, ViewGroup, LinearView, ScrollView, Animation, Button, EditView, TabView, WebView, ...

* web

    WebController, WebService

* geo

    LatLon, GeoLocation, GeoLine, GeoRectangle, Globe, Earth


## Getting Started

### Prerequisites

 * Xcode 9.4 or higher for compiling macOS/iOS packages
 * Android Studio 3.0 or higher for compiling Android packages
 * Visual Studio 2017 or higher for compiling Win32 packages
 * Tizen Studio 1.1.1 or higher for compiling Tizen packages
 * CMake (>=3.0), GCC/C++ (>=4.8.1) for compiling Linux packages. Optional: KDevelop(>=4.7)

### Clone the repository from Github.

 ```bash
  git clone https://github.com/SLIBIO/SLib.git
 ```

### Build the static libraries

You can see the following projects in `build` directory.

| Project| Description |
| ----|----|
| Android | Android Studio project for Android |
| iOS | Xcode project for iOS |
| macOS | Xcode project for macOS |
| Win32 | Visual Studio solution for Win32 |
| Tizen | Tizen Studio project for Tizen |
| Linux | CMake/KDevelop project for Linux |
 
 After compiling the projects, you can find the static libraries in the `lib` directory.

### Setup Environment

It's time to setup the environment variables. It is a bit different depending on the platforms.

On macOS and Linux, you'll need to run the `setup-path` script on Terminal or Finder.

 ```bash
  ./setup-path
 ```

On Windows, you'll need to run the `setup-path.bat` batch file on Command Prompt or File Explorer.

 ```bash
  setup-path.bat
 ```

`setup-path` will register the current source directory as `SLIB-PATH` environment variable depending on the Operating Systems and IDEs(XCode and KDevelop).

After setting up the environment variables, please close all the running IDEs(Xcode, Android Studio, Tizen Studio,...) and terminal (or konsole) windows, and then reopen them. (on macOS, press Command+Q to completely close the IDEs).

## Integrating SLib with existing C++ projects

It is very easy to integrate `SLib` into your existing C++ project. You just need to setup `include` and `lib` directories into your existing C++ project.

| Platform | IDE | Include Directory | Link Directory |
| ----|----|----|----|
| Android | Android Studio (CMake) | ${SLIB_PATH}/include | ${SLIB_PATH}/lib/Android/${CMAKE_BUILD_TYPE}-${ANDROID_ABI} |
| iOS | Xcode | $(SLIB_PATH)/include | $(SLIB_PATH)/lib/iOS/$(CONFIGURATION)$(EFFECTIVE_PLATFORM_NAME) |
| Tizen | Tizen Studio | ${SLIB_PATH}/include | ${SLIB_PATH}/lib/Tizen/${ConfigName}-${SDK_ARCH} |
| macOS | Xcode | $(SLIB_PATH)/include | $(SLIB_PATH)/lib/macOS/$(CONFIGURATION) |
| Win32 | Visual Studio | $(SLIB_PATH)/include | $(SLIB_PATH)/lib/Win32/$(Configuration)-$(Platform) |
| Linux | CMake, KDevelop | ${SLIB_PATH}/include | ${SLIB_PATH}/lib/Linux/${CMAKE_BUILD_TYPE}-${CMAKE_HOST_SYSTEM_PROCESSOR} |

You can also integrate SLib into any types of C++ projects using similar `include` and `lib` directory rules.

`Important:` Your C++ project must be compiled with C++11 support.

After setup directories, link `slib` library via IDE or set `-lslib` option to the linker.

Your project can also link to the core subsets of SLib: `slib-core` (core+math+crypto+network).

To make your project more portable, you can copy the include directory and the precompiled static libraries into your project and use the relative path instead of the environment variable.

### Android Studio (using CMake)

Firstly, open and build the project in the `build/Android` directory in the SLib source. After completion of build, you can see the precompiled static libraries and `slib.aar` in the `lib/Android` directory.

Secondly, open your Android project created by yourself which will use SLib.

`Important:` Use gradle version 2.3.1 or higher.

Edit the `build.gradle` in your app module as following.

```
  ...
  android {
    ...
    defaultConfig {
      ...
      externalNativeBuild {
        ...
        cmake {
          cppFlags "-std=c++11"
          arguments "-DSLIB_PATH=${System.env.SLIB_PATH}"
        }
        ...
      }
      ndk {
        abiFilters 'armeabi-v7a', 'arm64-v8a', 'x86'
      }

      ...
    }
  }
  ...
  repositories {
    ...
    flatDir{
       ...
       dirs "${System.env.SLIB_PATH}/lib/Android"
    }
  }
  dependencies {
    ...
    compile ':slib@aar'
  }
  
```


Edit `CMakeLists.txt` in your app module as following.

```
  ...
  include_directories (${SLIB_PATH}/include)
  link_directories (${SLIB_PATH}/lib/Android/${CMAKE_BUILD_TYPE}-${ANDROID_ABI})
  ...
  target_link_libraries (
    your-native-module-name
    ...
    slib
    log GLESv2 OpenSLES
    ...
  )
  ...
```

Edit the main cpp file (for example, `native-lib.cpp`), and insert the following code snippet.

```
#include <slib/core/platform_android.h>

JNIEXPORT jint JNI_OnLoad(JavaVM* jvm, void* reserved)
{
	slib::Android::initialize(jvm);
	return JNI_VERSION_1_4;
}
```

If you already defined `JNI_OnLoad` in somewhere, please insert `slib::Android::initialize(jvm);` in the existing definition, instead of inserting above code snippet.


### Xcode (iOS, macOS)

Firstly, open and build the project in the `build/iOS` (or `build/macOS` for macOS) directory in the SLib source. After completion of build, you can see the precompiled static libraries in the `lib/iOS` (or `lib/macOS` for macOS) directory.

Secondly, open your Xcode project created by yourself which will use SLib.

1. Click on the project icon in the inspector (on the left side)

2. Click on the `Build Settings` tab on the right side

3. Find `Search Paths` section

    * Add following path to `Header Search Paths`

        `$(SLIB_PATH)/include`

    * Add following path to `Library Search Paths`

        on iOS

        `$(SLIB_PATH)/lib/iOS/$(CONFIGURATION)$(EFFECTIVE_PLATFORM_NAME)`

        on macOS

        `$(SLIB_PATH)/lib/macOS/$(CONFIGURATION)`

4. Find `Linking` section

    Add following linker flag to `Other Linker Flags`

    `-lslib`


### Tizen Studio

Firstly, open and build the project in the `build/Tizen` directory in the SLib source. After completion of build, you can see the precompiled static libraries in the `lib/Tizen` directory.

Secondly, open your Tizen project created by yourself which will use SLib.

1. Right click on the project in the `Project Explorer`

2. Click on `Properties` on the popup menu.

    Then, `Properties` popup window will be shown.

3. Find `C/C++ General` in the left tree and then open it

    Select the subitem: `Paths and Symbols`. Then, `Paths and Symbols` property page will be shown on the right side.

    * Select `Includes` tab on the property page
    
        * Select `GNU C++` in `Languages` list on the left side of the property page.
        
        * Click on the `Add` button on the right side of the property page.
        
        * In the `Add directory path` dialog, input the following path under the `Directory:` and click on `OK` button.
        
            `${SLIB_PATH}/include`
        
    * Select `Library Paths` tab on the property page
    
        * Click on the `Add...` button on the right side of the property page.
        
        * In the `Add...` dialog, input the following path under the `Directory:` and click on `OK` button.
        
            `${SLIB_PATH}/lib/Tizen/${ConfigName}-${SDK_ARCH}`

    * Select `Libraries` tab on the property page
    
        * Click on the `Add...` button on the right side of the property page.
        
        * In the `Add...` dialog, input the following name under the `File:` and click on `OK` button.
        
            `slib`

4. Find `C/C++ Build` in the left tree and then open it

    Select the subitem: `Settings`. Then, `Settings` property page will be shown on the right side.
    
    * Select `Tool Settings` tab in the property page
    
    * Select `C++ Compiler` in the tree under the tab button
    
    * Select the subitem: `Dialect`
    
    * On the right side, select one of the following options for `Language standard` item.
    
        `ISO C++11 (-std=c++0x)`
        
        `ISO C++1y (-std=c++1y)`


### Visual Studio

Firstly, open and build `SLib.sln` solution in the `build/Win32` directory in SLib source. After completion of build, you can see the precompiled static libraries in the `lib/Win32` directory.

Secondly, open your VC project created by yourself which will use SLib.

1. Right click on the project in the `Solution Explorer`

2. Click on `Properties` on the popup menu.
    
    Then, `... Property Pages` dialog will be shown.

3. Select `VC++ Directories` under the `Configuration Properties` in the left tree.

    On the right side,
    
    * Add following path to the `Include Directories` option
    
        `$(SLIB_PATH)/include`
        
    * Add following path to the `Library Directories` option
    
        `$(SLIB_PATH)/lib/Win32/$(Configuration)-$(Platform)`

4. Select `Linker` under the `Configuration Properties` in the left tree.

    * Select subitem: `Input`

    * On the right side, add following file to the `Additional Dependencies`
    
        `slib.lib`


### CMake or KDevelop

Firstly, run `build-release.sh` (or `build-debug.sh`) in the `build/Linux` directory in SLib source. (You can also use KDevelop project - `SLib.kdev4` to compile SLib). After completion of build, you can see the precompiled static libraries in the `lib/Linux` directory.

Secondly, setup your project which will use SLib.

Edit `CMakeLists.txt` as following
    
```
    ...
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    
    set (SLIB_PATH $ENV{SLIB_PATH})
    include_directories (${SLIB_PATH}/include) 
    link_directories(${SLIB_PATH}/lib/Linux/${CMAKE_BUILD_TYPE}-${CMAKE_HOST_SYSTEM_PROCESSOR})
    ...
    target_link_libraries (
        your-executable-name
        ...
        slib
        dl z pthread ...
    )
```


## Contributing

We sincerely appreciate your support and suggestions. For contributing pull requests, please make sure to contact us at contact@slib.io.


## License

SLib is dual-licensed. It is currently licensed under the Mozilla Public License v2.0, but also under a separate proprietary license. If you feel like you need to purchase a proprietary license for SLib, please contact us at contact@slib.io.
