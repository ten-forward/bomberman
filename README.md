bomberman
=========

Bomberman!

[![Build Status](https://travis-ci.org/davidsiaw/bomberman.png)](https://travis-ci.org/davidsiaw/bomberman)

Building
========

For all environments, in order to build this project, you will need the Android SDK and NDK downloadable here:

http://developer.android.com/sdk/index.html
http://developer.android.com/tools/sdk/ndk/index.html

You will need to install the Build Tools, Platform Tools and Android API Level 10

Linux
-----

You can refer to the .travis.yml file if you encounter problems.

You will need to set the environment variables
  ANDROID_HOME=.../android-sdk-linux
  ANDROID_NDK=.../android-ndk-r8e

Then simply run

  make all
  
It will create the .apk for you.

Windows
-------

Install the SDK and NDK, then set PATH variables to these folders: 

D:\adt-bundle-windows-x86_64-20130522\android-ndk-r8e-windows-x86_64
D:/adt-bundle-windows-x86_64-20130522/sdk/tools
D:/adt-bundle-windows-x86_64-20130522/sdk/platform-tools

After this, in the main directory of this repository, run 

  ndk-build

to compile the native sources.

You have an option of compiling using eclipse or installing the JDK and compiling using ant by typing

  ant debug
