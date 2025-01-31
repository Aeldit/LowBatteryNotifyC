CC=gcc
CFLAGS=-W -Wall -Werror -std=c11 -pedantic

TARGET=low-battery-notifier

all: clean $(TARGET)
	./$(TARGET)

$(TARGET):
	$(CC) $(CFLAGS) main.c -o $(TARGET)

clean:
	if [ -f "$(TARGET)" ]; then rm $(TARGET); fi

