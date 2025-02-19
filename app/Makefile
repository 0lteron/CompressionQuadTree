SRC := ./src
INC := ./include
OBJ := ./obj
BIN := ./bin
PGM := ./PGM
QTC := ./QTC

CC 			 := clang
STD 		 := -std=c17
PFLAGS   := $(Iqtc) -I$(INC)
CFLAGS   := -Wall
LFLAGS   := -lm $(Lqtc)

EXEC    = $(BIN)/codec
ARCHIVE = L3.2024.ProgC-Maloum.Benesby

OBJ_FILES :=  $(OBJ)/main.o \
							$(OBJ)/parse_arg.o\
							$(OBJ)/verbose.o

all: $(EXEC)

$(EXEC): $(OBJ_FILES) | $(OBJ) $(PGM) $(QTC) $(BIN)
	$(CC) -o $@ $^ $(LFLAGS)

$(OBJ)/%.o: $(SRC)/%.c | $(OBJ)
	$(CC) $(STD) $(CFLAGS) $(PFLAGS) -c -o $@ $<

$(OBJ):
	mkdir -p $(OBJ)

$(BIN):
	mkdir -p $(BIN)

$(PGM):
	mkdir -p $(PGM)

$(QTC):
	mkdir -p $(QTC)

.PHONY: clean cleanall dist

clean:
	rm -rf $(OBJ)/

cleanall: clean
	rm -f $(EXEC)
	rm -rf $(QTC)/* $(PGM)/* $(BIN)