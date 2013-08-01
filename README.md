bomberman
=========

Bomberman!

[![Build Status](https://travis-ci.org/davidsiaw/bomberman.png)](https://travis-ci.org/davidsiaw/bomberman)

Building
========

For all environments, in order to build this project, you will need the Android SDK and NDK downloadable here:

* http://developer.android.com/sdk/index.html
* http://developer.android.com/tools/sdk/ndk/index.html

You will need to install the Build Tools, Platform Tools and Android API Level 10.

With the Android SDK and NDK installed, clone this repository (making sure to include submodules):

    git clone --recursive https://github.com/davidsiaw/bomberman.git

Linux
-----

You can refer to the .travis.yml file if you encounter problems.

You will need to set the environment variables

	ANDROID_HOME=.../android-sdk-linux
	ANDROID_NDK=.../android-ndk-r8e

Then simply run

	$ANDROID_HOME/tools/android -s update project --path . --target "android-10"
	make all
  
It will create the .apk for you.


Mac OS X
--------

Assuming you unzipped the ADT and the NDK to ~/Development:

	export PATH=$PATH:~/Development/adt-bundle-mac-x86_64/sdk/tools
	export PATH=$PATH:~/Development/adt-bundle-mac-x86_64/sdk/platform-tools
	export PATH=$PATH:~/Development/android-ndk-r8e/

You will also need to install API Level 10 using

	android update sdk

Then in the directory run

	android update project --path . --target "android-10"
	ndk-build
	ant debug

Windows
-------

Install the SDK and NDK, then set PATH variables to these folders: 

	...\adt-bundle-windows-x86_64-20130522\android-ndk-r8e-windows-x86_64
	...\adt-bundle-windows-x86_64-20130522\sdk\tools
	...\adt-bundle-windows-x86_64-20130522\sdk/platform-tools

After this, in the main directory of this repository, run 

	ndk-build

to compile the native sources.

You have an option of compiling using eclipse or installing the JDK and compiling using ant by typing

	android update project --path . --target "android-10"
	ant debug
