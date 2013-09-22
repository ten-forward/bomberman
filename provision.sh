#!/bin/sh

# prepare the computer for building bomberman

if [ $THE_MAKEFILE == "Makefile.linux" ]
then 
    sudo apt-get update -qq
    if [ `uname -m` = x86_64 ]; then sudo apt-get install -qq --force-yes libgd2-xpm ia32-libs ia32-libs-multiarch; fi
fi
  
if [ $THE_MAKEFILE == "Makefile.android" ]
then 
    wget --timeout=30 http://dl.google.com/android/ndk/android-ndk-r9-linux-x86.tar.bz2 -O ndk.tar.bz2
    tar -xf ndk.tar.bz2
    export ANDROID_HOME=$(pwd)/android-sdk-linux 
fi
