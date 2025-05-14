# Piqi

Unofficial async Qt-based library for interacting with the official pixiv Android app API.

**Will I get banned?**\
Probably not, it's still a risk though, as with any 3rd party library

**Is this compatible with QtQuick?**\
Yes. All classes are registered `QML_ELEMENT`s and they can be used in QtQuick projects with no further setup, you only need to add `import org.kde.piqi` in your *.qml* files.\
However this compatibility with QtQuick means, that QtQuick is a dependency of any project using piqi, whether it's using QtQuick or not.

## Installation

There are no packages currently.

## Usage (CMake only)

```cmake
find_package(piqi REQUIRED)
# or
add_subdirectory(piqi) # if you use piqi from source

target_link_libraries(your_target_name PRIVATE piqi::piqi)
```

## Dependencies
- QtDeclarative
- QCoro

## Contributing

Contributions are welcome, as writing piqi is quite a repetetive job. The style of the API (most likely) won't change, as it works well in practice.

I recommend to use Http Toolkit and Waydroid, it's a *fairly* easy setup without rooting your phone

1. Install Waydroid, Http Toolkit and Android SDK Platform Tools (for ADB)\
   Arch - `yay -S waydroid httptoolkit-bin android-sdk-platform-tools`
2. Initialise waydroid\
   `sudo waydroid init`
3. Follow the instructions on installing Magisk\
   (https://github.com/mistrmochov/magiskWaydroid)
4. Restart Waydroid
   `systemctl stop waydroid-container.service`
5. Acquire pixiv apk (I recommend from Apkpure, alternatively you can extract the APK from your phone)
6. Install the apk\
   `waydroid app install <path-to-apk>`
7. Start Waydroid\
   `waydroid show-full-ui`
8. In HTTP Toolkit, choose "Android App via Frida" and then select pixiv
