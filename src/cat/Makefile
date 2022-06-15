$(CC)=gcc
$(CFLAGS)=-Wall -Werror -Wextra
$(RM)=rm -rf
$(TARGET)=s21_cat

all: build

build: s21_cat.o
	$(CC) $(CFLAGS) $^ -o $(TARGET)

%.o: %.c
	$(CC) $< -o $@

clean:
	$(RM) *.o

.PHONY: all build clear