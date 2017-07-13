TARGET = backlight_gui

CC = gcc
CFLAGS = -Wall -Wextra `pkg-config --cflags --libs gtk+-2.0`

SRCC = $(wildcard ./*.c)
OBJ = $(SRCC:.c=.o)

default: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $(TARGET) $(OBJ) $(CFLAGS) 

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean: 
	rm -f $(TARGET) $(OBJ)
