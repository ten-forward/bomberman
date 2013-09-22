#!/bin/bash

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
    wget -q http://dl.google.com/android/android-sdk_r22.0.1-linux.tgz
    tar xzvf android-sdk_r22.0.1-linux.tgz > /dev/null
    $ANDROID_HOME/tools/android list sdk
    echo "y" | $ANDROID_HOME/tools/android -s update sdk --filter 13,1,2,3 --no-ui
    $ANDROID_HOME/tools/android list targets
    $ANDROID_HOME/tools/android -s update project --path . --target "android-10"
fi
