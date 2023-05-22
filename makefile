CC = gcc
CFLAGS = -Wall -Wextra

SRCS = wish.c utils.c
OBJS = $(SRCS:.c=.o)
HEADER = utils.h
TARGET = wish

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
