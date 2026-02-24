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
ROOT = ..

include $(ROOT)/linux_gcc_rules.mak

TARGET = $(ROOT)/lib/$(BIN_PATH)/$(CC)/$(BUILD_TARGET)/libuastack.a

DIRS = core stackcore securechannel transport/tcp transport/https \
       proxystub/serverstub platforms/linux

CFLAGS = -Wall -Wextra -Wshadow -Wundef -Wmissing-prototypes -Wstrict-prototypes -pthread \
         $(patsubst %,-I%,$(DIRS)) $(EXTRA_CFLAGS)

SOURCES = $(wildcard $(patsubst %,%/*.c,$(DIRS)))

OBJECTS = $(SOURCES:%.c=./$(BIN_PATH)/$(CC)/$(BUILD_TARGET)/%.o)

DEPS = $(OBJECTS:%.o=%.d)

all: $(TARGET)

ifneq ($(MAKECMDGOALS),clean)
-include $(DEPS)
$(OBJECTS): linux_gcc.mak $(ROOT)/linux_gcc_rules.mak
endif

strip:
	$(STRIP) -g $(TARGET)

clean:
	$(RM) $(OBJECTS)
	$(RM) $(DEPS)
	$(RM) $(TARGET)

$(TARGET): $(OBJECTS)
	$(MKDIR) $(dir $@)
	$(AR) rcs $@ $^

./$(BIN_PATH)/$(CC)/$(BUILD_TARGET)/%.o : %.c
	$(MKDIR) $(dir $@)
	$(CC) -c $(CFLAGS) -MMD -MP -MT $@ -MF $(@:%.o=%.d) -o $@ $<
