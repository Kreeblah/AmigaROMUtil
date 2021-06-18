CFLAGS = -O2 -std=c17 -Wall -Werror
SRCS = AmigaROMUtil.c teeny-sha1.c main.c
OBJS = $(SRCS:.c=.o)
MAIN = AmigaROMUtil

all:		$(MAIN)

$(MAIN):	$(OBJS)
			$(CC) $(CFLAGS) -o $(MAIN) $(OBJS)

.c.o:
			$(CC) $(CFLAGS) -c $< -o $@

clean:
			$(RM) *.o *~ $(MAIN)