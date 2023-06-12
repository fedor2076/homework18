CC = g++
CFLAGS = -std=c++17 -o
SRC = src/*.cpp
TARGET = chat
PREFIX = /usr/local/bin

#Упрощенный вариант Makefile без LIB
#также нет необходимости в clean
chat:
	echo "Console chat"
	$(CC) $(CFLAGS) $(TARGET) $(SRC)
clean:
	rm -rf *.o *.a
install:
	install $(TARGET) $(PREFIX)
uninstall:
	rm -rf  $(PREFIX)/$(TARGET)
