#!/usr/bin/env -S make -f

CC = gcc
LD = gcc

PROJECT_NAME = serum-cli
PROJECT_SOURCE = ./src
PROJECT_DIST = ./dist

CFLAGS_EXTRA = -fno-diagnostics-show-caret -DSERUM_BUILD_MUTE=1
INCLUDES = ../serum/include

DEFAULT_TARGET = serum

include build/makeframe/bootstrap.mk




serum: $(PROJECT_DIST)/serum

$(PROJECT_DIST)/serum: $(OBJECTS)
	$(LD) $(LFLAGS) -o $@ $^ ../serum/dist/libserum.so
