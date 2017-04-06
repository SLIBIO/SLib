# SLib.io

SLib.io is a universal cross-platform library, specially designed for the various UI components, OpenGL rendering and networking solutions.

SLib.io enables software developers to build multi-platform(Android/iOS/MacOS/Tizen/Win32) applications and games under the most effective workflows and the coding conventions of C++, while providing easy-to-use toolkits to develop HTTP/Socket servers on the Linux/MacOS/Win32 platforms.


## Modules

- core: Auto Reference Counting, String, List, Map, File, Thread, Synchronizing, Time, Variant, Json, ...

- crypto: AES, RSA, GZIP, SHA1/2, GCM, MD5, Blowfish, ...

- db: SQLite, MySQL

- device: Sensor, Vibrator

- graphics: Bitmap, Image, Canvas, Font, ...

- math: BigInt, Uint128, Matrix, Vector, Transform, ...

- media: MediaPlayer, AudioRecorder, AudioPlayer, Camera, ...

- network: UrlRequest, Socket, Asynchronous I/O, HttpService, ...

- render: RenderEngine, RenderProgram, VertexBuffer, IndexBuffer, Texture, ...

- ui: Window, View, ViewGroup, LinearView, ScrollView, Animation, Button, EditView, TabView, WebView, ...

- web: WebController, WebService

- geo: LatLon, GeoLocation, GeoLine, GeoRectangle, Globe, Earth


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
 
 Open the projects in `build` directory using IDEs and then compile for configurations (Debug, Release) and architectures (arm, arm64, i386, x86_64).
 
 Following directories are listed in `build` directory for supported platforms.

| Directory| Description |
| ----|----|
| AndroidStudio | Android Studio project for Android |
| iOS-Xcode | Xcode project for iOS |
| macOS-Xcode | Xcode project for macOS |
| Win32-VC | Visual Studio solution for Win32 |
| TizenStudio | Tizen Studio project for Tizen |
| Linux-KDevelop | KDevelop project for Linux |
 
 After compiling the projects, you can find the static libraries in the `lib` directory in the source tree.

### Setup Environment

In macOS and Linux, run the `setup-path` script using Terminal or Finder.

 ```bash
  ./setup-path
 ```

In Windows, run the `setup-path.bat` batch file using Command Prompt or File Explorer.

 ```bash
  setup-path.bat
 ```

`setup-path` will register current source directory as `SLIB_PATH` environment variable on the Operating System, XCode, and KDevelop.


## Integrating SLib.io with existing C++ projects

Using `SLib.io` with C++ projects is very easy. You just need to setup include and link directories to your existing C++ project.

| Platform | IDE | Include Directory | Link Directory |
| ----|----|----|----|
| Android | Android Studio (CMake) | ${SLIB_PATH}/include | ${SLIB_PATH}/lib/Android/${CMAKE_BUILD_TYPE}-${ANDROID_ABI} |
| iOS | Xcode | $(SLIB_PATH)/include | $(SLIB_PATH)/lib/iOS/$(CONFIGURATION)$(EFFECTIVE_PLATFORM_NAME) |
| Tizen | Tizen Studio | ${SLIB_PATH}/include | ${SLIB_PATH}/lib/Tizen/${ConfigName}-${SDK_ARCH} |
| macOS | Xcode | $(SLIB_PATH)/include | $(SLIB_PATH)/lib/macOS/$(CONFIGURATION) |
| Win32 | Visual Studio | $(SLIB_PATH)/include | $(SLIB_PATH)/lib/Win32/$(Configuration)-$(Platform) |
| Linux | KDevelop, CMake | ${SLIB_PATH}/include | ${SLIB_PATH}/lib/Linux/${CMAKE_BUILD_TYPE}-${CMAKE_HOST_SYSTEM_PROCESSOR} |

You can also integrate SLib.io into any type of C++ projects using similar include and link directory rules.

`Important` Your C++ project must be compiled with C++11 support.

After setup directories, link `slib` library via IDE or set `-lslib` option to the linker.

Your project can also link to the subsets of SLib.io: `slib-core` (core+math+crypto) , `slib-network`, `slib-graphics`, `slib-render`, `slib-ui`, `slib-media`, `slib-device`, `slib-db`, `slib-web`, `slib-geo`

 
### Android Studio (using CMake)

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
          ...
          cppFlags "-std=c++11"
          arguments "-DSLIB_PATH=${System.env.SLIB_PATH}"
          ...
        }
        ...
      }
      ...
    }
    ...
  }
  ...
  repositories {
    ...
    flatDir{
       ...
       dirs "${System.env.SLIB_PATH}/lib/Android"
       ...
    }
    ...
  }
  ...
  dependencies {
    ...
    compile ':slib@aar'
    ...
  }
  ...
  
```

Edit `CMakeLists.txt` in your app module as following.

```
  ...
  include_directories (${SLIB_PATH}/include)
  link_directories (${SLIB_PATH}/lib/Android/${CMAKE_BUILD_TYPE}-${ANDROID_ABI})
  ...
  target_link_libraries (
    your-project-module-name
    ...
    slib
    log GLESv2 OpenSLES
    ...
  )
  ...
```


## Contributing

We sincerely appreciate your support and suggestions. For contributing pull requests, please make sure to contact us at contact@slib.io.


## License

SLib.io is dual-licensed. It is currently licensed under the Mozilla Public License v2.0, but also under a separate proprietary license. If you feel like you need to purchase a proprietary license for SLib.io, please contact us at contact@slib.io.
