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

 * Xcode 8.0 or higher for compiling macOS/iOS packages
 * Android Studio 2.3 or higher for compiling Android packages
 * Visual Studio 2017 or higher for compiling Win32 packages
 * Tizen Studio 1.1.1 or higher for compiling Tizen packages
 * KDevelop 4.7 or higher for compiling Linux packages

### Clone the repository from Github.

 ```bash
  git clone https://github.com/SLIBIO/SLib.git
 ```

### Build the static libraries
 
Build Configurations Debug/Release and Platforms arm/arm64/i386/x86/x86_64 for the each project in `build` directory using the appropriate IDE.
 
You can see the following projects in `build` directory.

| Project| Description |
| ----|----|
| AndroidStudio | Android Studio project for Android |
| iOS-Xcode | Xcode project for iOS |
| macOS-Xcode | Xcode project for macOS |
| Win32-VC | Visual Studio solution for Win32 |
| TizenStudio | Tizen Studio project for Tizen |
| Linux-KDevelop | KDevelop project for Linux |
 
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

After setting up the environment variables, please close all the running IDEs(Xcode, Android Studio, Tizen Studio,...) and reopen them. (on macOS, press Command+Q to completely close the IDEs)

## Integrating SLib with existing C++ projects

It is very easy to integrate `SLib.io` into your existing C++ project. You just need to setup `include` and `lib` directories into your existing C++ project.

| Platform | IDE | Include Directory | Link Directory |
| ----|----|----|----|
| Android | Android Studio (CMake) | ${SLIB_PATH}/include | ${SLIB_PATH}/lib/Android/${CMAKE_BUILD_TYPE}-${ANDROID_ABI} |
| iOS | Xcode | $(SLIB_PATH)/include | $(SLIB_PATH)/lib/iOS/$(CONFIGURATION)$(EFFECTIVE_PLATFORM_NAME) |
| Tizen | Tizen Studio | ${SLIB_PATH}/include | ${SLIB_PATH}/lib/Tizen/${ConfigName}-${SDK_ARCH} |
| macOS | Xcode | $(SLIB_PATH)/include | $(SLIB_PATH)/lib/macOS/$(CONFIGURATION) |
| Win32 | Visual Studio | $(SLIB_PATH)/include | $(SLIB_PATH)/lib/Win32/$(Configuration)-$(Platform) |
| Linux | KDevelop, CMake | ${SLIB_PATH}/include | ${SLIB_PATH}/lib/Linux/${CMAKE_BUILD_TYPE}-${CMAKE_HOST_SYSTEM_PROCESSOR} |

You can also integrate SLib into any types of C++ projects using similar `include` and `lib` directory rules.

`Important:` Your C++ project must be compiled with C++11 support.

After setup directories, link `slib` library via IDE or set `-lslib` option to the linker.

Your project can also link to the subsets of SLib: `slib-core` (core+math+crypto) , `slib-network`, `slib-graphics`, `slib-render`, `slib-ui`, `slib-media`, `slib-device`, `slib-db`, `slib-web`, `slib-geo`

To make your project more portable, you can copy the include directory and the precompiled static libraries into your project and use the relative path instead of the environment variable.

### Android Studio (using CMake)

Firstly, open and build the project in the `build/AndroidStudio` directory in the SLib source. After completion of build, you can see the precompiled static libraries and `slib.aar` in the `lib/Android` directory.

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


### Xcode (iOS, macOS)

Firstly, open and build the project in the `build/iOS-Xcode` (or `build/macOS-Xcode` for macOS) directory in the SLib source. After completion of build, you can see the precompiled static libraries in the `lib/iOS` (or `lib/macOS` for macOS) directory.

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

Firstly, open and build the project in the `build/TizenStudio` directory in the SLib source. After completion of build, you can see the precompiled static libraries in the `lib/Tizen` directory.

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

Firstly, open and build `SLib.sln` solution in the `build/Win32-VC` directory in SLib source. After completion of build, you can see the precompiled static libraries in the `lib/Win32` directory.

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


### KDevelop or CMake

Firstly, open and build `SLib.kdev4` project in the `build/Linux-KDevelop` directory in SLib source. After completion of build, you can see the precompiled static libraries in the `lib/Linux` directory.

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
