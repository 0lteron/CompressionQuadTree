SRC := ./src
INC := ./include
OBJ := ./obj
LIB := ./lib

CC 			 := clang
STD			 := -std=c17
PFLAGS   := -I$(INC)
CFLAGS   := -Wall -fPIC
LFLAGS   := -shared

ARCHIVE = libqtc

LIBNAME := qtc

OBJ_FILES :=  $(OBJ)/decoder.o \
              $(OBJ)/coder.o \
              $(OBJ)/quadtree.o \
              $(OBJ)/pgm_io.o \
              $(OBJ)/segmentation.o \
              $(OBJ)/file_naming.o \
              $(OBJ)/verbose.o \
              $(OBJ)/qtc.o \

all: $(LIBNAME)

$(LIBNAME): $(OBJ_FILES) | $(LIB)
	$(CC) $(LFLAGS) -o $(LIB)/lib$@.so $^

$(OBJ)/%.o: $(SRC)/%.c | $(OBJ)
	$(CC) $(STD) $(PFLAGS) $(CFLAGS) -c -o $@ $<

$(OBJ):
	mkdir -p $(OBJ)

$(LIB):
	mkdir -p $(LIB)

# for installation as a standard library
install: $(LIBNAME)
	cp $(LIB)/lib$(LIBNAME).so /usr/lib/
	cp $(INC)/qtc.h /usr/include/

uninstall:
	rm /usr/lib/lib$(LIBNAME).so
	rm /usr/include/qtc.h

.PHONY: clean cleanall

clean:
	rm -rf $(OBJ)/

cleanall: clean
	rm -rf $(LIB)/

arch:
	tar -czf $(ARCHIVE).tar.gz makelib include/ src/ install.sh uninstall.sh
