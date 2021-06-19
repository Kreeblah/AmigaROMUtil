CFLAGS = -O2 -std=c17 -Wall -Werror
LIB_SRCS = AmigaROMUtil.c teeny-sha1.c
MAIN_SRC = main.c
LIB_OBJS = $(LIB_SRCS:.c=.o)
MAIN_OBJ = $(MAIN_SRC:.c=.o)
MAIN = AmigaROMUtil
SHARED_LIB = $(MAIN).so
STATIC_LIB = $(MAIN).a

all:			$(SHARED_LIB) $(STATIC_LIB) $(MAIN)

shared:			$(SHARED_LIB)

$(SHARED_LIB):	$(LIB_OBJS)
				$(CC) $(CFLAGS) -o $(SHARED_LIB) $(LIB_OBJS) -shared

static:			$(STATIC_LIB)

$(STATIC_LIB):	$(LIB_OBJS)
				$(AR) -rv $(STATIC_LIB) $(LIB_OBJS)

app:			$(MAIN)

$(MAIN):		$(LIB_OBJS) $(MAIN_OBJ)
				$(CC) $(CFLAGS) -o $(MAIN) $(LIB_OBJS) $(MAIN_OBJ)

clean:
				$(RM) $(LIB_OBJS) $(MAIN_OBJ) $(SHARED_LIB) $(STATIC_LIB) *~ $(MAIN)