CC = gcc
CFLAGS = -Wall -Wextra -I include

TARGET = mini_cpp

SRC = src/main.c \
      src/preprocessor.c \
      src/comment.c \
      src/macro.c \
      src/include.c \
      src/conditional.c

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)