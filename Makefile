CFLAGS = -O2 -std=c17 -Wall -Werror
LIB_SRCS = AmigaROMUtil.c teeny-sha1.c
MAIN_SRC = main.c
LIB_OBJS = $(LIB_SRCS:.c=.o)
MAIN_OBJ = $(MAIN_SRC:.c=.o)
MAIN = AmigaROMUtil
SO_FILE = $(MAIN).so
A_FILE = $(MAIN).a

all:		$(MAIN)

$(MAIN):	$(LIB_OBJS) $(MAIN_OBJ)
			$(AR) -rv $(A_FILE) $(LIB_OBJS)
			$(CC) $(CFLAGS) -o $(SO_FILE) $(LIB_OBJS) -shared
			$(CC) $(CFLAGS) -o $(MAIN) $(LIB_OBJS) $(MAIN_OBJ)

.c.o:
			$(CC) $(CFLAGS) -c $< -o $@

clean:
			$(RM) $(LIB_OBJS) $(MAIN_OBJ) $(SO_FILE) $(A_FILE) *~ $(MAIN)