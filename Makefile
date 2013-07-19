
ifeq ($(ANDROID_HOME),)
NDK ?= $(ANDROID_NDK)
else
NDK ?= $(lastword $(sort $(wildcard $(dir $(ANDROID_HOME))/android-ndk*)))
endif

ifeq ($(NDK),)
$(error no ndk found - set ANDROID_HOME or ANDROID_NDK)
endif

.PHONY: all clean

all:
	$(NDK)/ndk-build V=1
	ant debug

clean:
	$(NDK)/ndk-build clean
	ant clean