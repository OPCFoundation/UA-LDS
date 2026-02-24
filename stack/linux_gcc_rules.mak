# ========================================================================
# Copyright (c) 2005-2026 The OPC Foundation, Inc. All rights reserved.
#
# OPC Foundation MIT License 1.00
#
# Permission is hereby granted, free of charge, to any person
# obtaining a copy of this software and associated documentation
# files (the "Software"), to deal in the Software without
# restriction, including without limitation the rights to use,
# copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the
# Software is furnished to do so, subject to the following
# conditions:
#
# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
# OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
# HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
# WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
# OTHER DEALINGS IN THE SOFTWARE.
#
# The complete license agreement can be found here:
# http://opcfoundation.org/License/MIT/1.00/
# ======================================================================*/
#
# Global rules for all GCC builds

CC = $(CROSS_COMPILE)gcc
CXX = $(CROSS_COMPILE)g++
AR = $(CROSS_COMPILE)ar
STRIP = $(CROSS_COMPILE)strip
MKDIR = mkdir -p

ifndef MACHINE_TYPE
ifndef CROSS_COMPILE
	TMP_MACHINE_TYPE = $(shell uname -i)
	ifeq ($(TMP_MACHINE_TYPE),x86_64)
		MACHINE_TYPE = x86_64
	else
	ifeq ($(TMP_MACHINE_TYPE),i386)
		MACHINE_TYPE = i386
	else
	ifeq ($(TMP_MACHINE_TYPE),i686)
		MACHINE_TYPE = i386
	else
		MACHINE_TYPE = unknown
	endif
	endif
	endif
else
	MACHINE_TYPE = unknown
endif
endif

ifndef MACHINE_OPT
ifndef CROSS_COMPILE
	ifeq ($(MACHINE_TYPE),x86_64)
		MACHINE_OPT = -m64
	else
	ifeq ($(MACHINE_TYPE),i386)
		MACHINE_OPT = -m32
	endif
	endif
endif
endif

BIN_PATH = linux/$(MACHINE_TYPE)

ifndef BUILD_TARGET
	BUILD_TARGET = release
endif

ifeq ($(findstring debug,$(BUILD_TARGET)),debug)
	EXTRA_CFLAGS = -D_DEBUG -g $(MACHINE_OPT)
else
	EXTRA_CFLAGS = -O3 $(MACHINE_OPT)
endif
