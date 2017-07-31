DEBUG = yes
ifeq ($(DEBUG),yes)
	CFLAGS=
	LDFLAGS=
else
	CFLAGS=-W -Wall -ansi -pedantic
	LDFLAGS=
endif

CC   = gcc
NASM = nasm
EXEC = packer
SRCS = srcs/main.c \
	   srcs/map_file.c \
	   srcs/generate_elf.c \
	   srcs/inject_payload.c \
	   srcs/binary_tools.c
SRCS_ASM = srcs/payload.asm
CFLAGS =
ASM_FLAGS = -f elf64
OBJ = $(SRCS:srcs/%.c=srcs/.obj/%.o)
OBJ_ASM = $(SRCS_ASM:srcs/%.asm=srcs/.obj/%.o)
OBJ_DIR = srcs/.obj
INCS = includes

all: $(EXEC)

$(EXEC): $(OBJ) $(OBJ_ASM) 
	@$(CC) -o $@ $^
	@echo -n "\n"

$(OBJ_DIR)/%.o: srcs/%.c
	@mkdir -p srcs/.obj
	@$(CC) -o $@ -c $< $(CFLAGS) -I $(INCS)
	@echo -n .

$(OBJ_ASM): $(SRCS_ASM)
	@$(NASM) $(ASM_FLAGS) -o $@ $<
	@echo -n .

run: all test
	./packer test

test:
	@rm -rf test
	@$(CC) -z execstack -o test test.c

clean:
	@rm -rf $(EXEC) $(OBJ) $(OBJ_ASM) $(OBJ_DIR)

re: clean all
