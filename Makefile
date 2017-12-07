# Makefile

TARGET = main

SRC = src
INC = inc
BIN = bin

SOURCE = $(wildcard $(SRC)/*.c)
OBJECT = $(patsubst %,$(BIN)/%, $(notdir $(SOURCE:.c=.o)))

CC = gcc
CFLAGS = -Wall -I$(INC)


# Build the project

$(BIN)/$(TARGET): $(OBJECT)
	$(CC) -o $@ $^


# Link the project dependencies

$(BIN)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) -c $< -o $@


# Run the project

run: $(BIN)/$(TARGET)
	$(BIN)/$(TARGET)

clean: rm -f $(OBJECT) $(BIN)/$(TARGET)
