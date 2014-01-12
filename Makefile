OS := $(firstword $(shell uname -s))

ifeq ($(OS),Darwin)
-include Makefile.macosx
else
-include Makefile.linux
endif

run: all
	cd assets && ./bomberman
	
android:
	ndk-build
	ant debug
	