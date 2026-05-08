# Настройки
CC = gcc
CFLAGS = -Wall -Wextra -g -lpthread -lncurses -lm
DEPFLAGS = -MMD -MP

# Имя программы
TARGET = main

# Списки файлов
SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)
DEPS = $(SRCS:.c=.d)

# Список команд (хороший тон)
.PHONY: all clean run

# 1. Собрать всё
all: $(TARGET)

# 2. Собрать и сразу запустить
run: $(TARGET)
	./$(TARGET)

# Сборка исполняемого файла
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(CFLAGS)

# Компиляция объектов
%.o: %.c
	$(CC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

# Подключение зависимостей
-include $(DEPS)

# Очистка
clean:
	rm -f *.o *.d $(TARGET)