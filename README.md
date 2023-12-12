# External ImGui Android
An External ImGui modmenu for android.
![](https://media.discordapp.net/attachments/934836097807167519/1183884783361736725/Screenshot_20231211_214904_KGO_Multi_Space.png)

## Features
- Built with [ImGUI](https://github.com/ocornut/imgui).
- Memory Tools for Unreal Engine games
- Math & Structs for Unreal Engine games

## To-Do
- implement ptrace to hook and call functions externally

## Requirements
The following dependencies are required to build the library from source.
- [NDK Android](https://developer.android.com/ndk).
- [APK Easy Tools](https://forum.xda-developers.com/android/software-hacking/tool-apk-easy-tool-v1-02-windows-gui-t3333960).
- Any text editor. We use [Notepad++](https://notepad-plus-plus.org/).

For AIDE users.
- [AIDE Download](https://cdn.discordapp.com/attachments/1059996036069871686/1181367274955882516/base.apk).
- [AIDE NDK arm64](https://cdn.discordapp.com/attachments/1059996036069871686/1181368161883398204/aide_ndk_arm64.tar.gz).

## Usage 
- [KGO Virtual Space](https://cdn.discordapp.com/attachments/1059996036069871686/1182052573897838683/base.apk).

## Building the source
- For AIDE users: Run Project / Build Project
- For Android Studio users: Build & Run

## Inject into your Game
- In MainActivity.java, replace ARKs package name with your games package name
- in the gles3jni.cpp file also replace ARKs package name with the one of your game and the offsets with your own

