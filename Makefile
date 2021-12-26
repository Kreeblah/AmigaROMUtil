# MIT License
#
# Copyright (c) 2021 Christopher Gelatt
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

CFLAGS = -O2 -std=c17 -Wall -Wextra -Werror -pedantic-errors
LIB_SRCS = AmigaROMUtil.c teeny-sha1.c
MAIN_SRC = main.c
LIB_OBJS = $(LIB_SRCS:.c=.o)
MAIN_OBJ = $(MAIN_SRC:.c=.o)
ifneq ($(OS),Windows_NT)
	MAIN = AmigaROMUtil
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Darwin)
		SHARED_LIB = libamigarom.dylib
		SO_LIB_CMD = $(CC) $(CFLAGS) -dynamiclib -o $(SHARED_LIB) $(LIB_OBJS)
	else
		SHARED_LIB = libamigarom.so
		SO_LIB_CMD = $(CC) $(CFLAGS) -o $(SHARED_LIB) $(LIB_OBJS) -shared
	endif
	STATIC_LIB = libamigarom.a
else
	MAIN = AmigaROMUtil.exe
	SHARED_LIB = amigarom.dll
	STATIC_LIB = amigarom.lib
	SO_LIB_CMD = $(CC) $(CFLAGS) -o $(SHARED_LIB) $(LIB_OBJS) -shared
endif

all:			$(SHARED_LIB) $(STATIC_LIB) $(MAIN)

libs:			$(SHARED_LIB) $(STATIC_LIB)

shared:			$(SHARED_LIB)

$(SHARED_LIB):	$(LIB_OBJS)
				$(SO_LIB_CMD)

static:			$(STATIC_LIB)

$(STATIC_LIB):	$(LIB_OBJS)
				$(AR) -rv $(STATIC_LIB) $(LIB_OBJS)

app:			$(MAIN)

$(MAIN):		$(LIB_OBJS) $(MAIN_OBJ)
				$(CC) $(CFLAGS) -o $(MAIN) $(LIB_OBJS) $(MAIN_OBJ)

clean:
				$(RM) $(LIB_OBJS) $(MAIN_OBJ) $(SHARED_LIB) $(STATIC_LIB) *~ $(MAIN)