CC := gcc
CFLAGS := -fPIC -Wall -Wextra -g
LDFLAGS := -shared

SOURCES := common.c seccomp.c
OBJECTS := $(SOURCES:.c=.o)

TARGET := libctf_preload.so

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: all clean

.DEFAULT_GOAL = all
