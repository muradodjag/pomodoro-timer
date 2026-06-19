CC      = gcc
TARGET  = pomodoro-timer
SRCS    = main.c timer.c
CFLAGS  = $(shell pkg-config --cflags raylib libnotify)
LIBS    = $(shell pkg-config --libs raylib libnotify)

$(TARGET): $(SRCS)
	$(CC) $(SRCS) -o $(TARGET) $(CFLAGS) $(LIBS)

clean:
	rm -f $(TARGET)

.PHONY: clean
