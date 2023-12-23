include CONFIG.cfg

CC = gcc
LD = gcc

CCFLAGS = -Wall -O2

EXEC = $(BUILD_DIR)/$(NAME)
OBJECTS = $(BUILD_DIR)/main.o $(BUILD_DIR)/communication.o
DEPS = $(OBJECTS:.o=.d)

.PHONY: all clean

all: $(EXEC)

$(EXEC): $(OBJECTS)
	$(LD) $^ -o $@

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CCFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $@

clean:
	$(RM) $(EXEC) $(OBJECTS) $(DEPS)