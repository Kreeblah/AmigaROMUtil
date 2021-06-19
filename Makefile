CFLAGS = -O2 -std=c17 -Wall -Werror
LIB_SRCS = AmigaROMUtil.c teeny-sha1.c
MAIN_SRC = main.c
LIB_OBJS = $(LIB_SRCS:.c=.o)
MAIN_OBJ = $(MAIN_SRC:.c=.o)
MAIN = AmigaROMUtil

all:		$(MAIN)

$(MAIN):	$(LIB_OBJS) $(MAIN_OBJ)
			$(AR) -rv $(MAIN).a $(LIB_OBJS)
			$(CC) $(CFLAGS) -o $(MAIN).so $(LIB_OBJS) -shared
			$(CC) $(CFLAGS) -o $(MAIN) $(LIB_OBJS) $(MAIN_OBJ)

.c.o:
			$(CC) $(CFLAGS) -c $< -o $@

clean:
			$(RM) *.o *.so *.a *~ $(MAIN)