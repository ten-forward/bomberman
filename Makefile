OS := $(firstword $(shell uname -s))

ifeq ($(OS),Darwin)
-include Makefile.macosx
else
-include Makefile.linux
endif