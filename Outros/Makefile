# My third makefile

# Name of the project
PROJ_NAME=main

# .cpp files
C_SOURCE=$(wildcard ./source/*.cpp)

# .h files
H_SOURCE=$(wildcard ./source/*.h)

# Object files
OBJ=$(subst .cpp,.o,$(subst source,objects,$(C_SOURCE)))

# Compiler and linker
CC=g++

# Flags for compiler
CC_FLAGS=-c         \
         -W         \
         -Wall
#         -pedantic	\
#		 -ansi

# Command used at clean target
RM = rm -rf

#
# Compilation and linking
#
all: objFolder $(PROJ_NAME)

$(PROJ_NAME): $(OBJ)
	@ echo 'Building binary using g++ linker: $@'
	$(CC) $^ -o $@
	@ echo.
	@ echo 'Finished building binary: $@'

./objects/%.o: ./source/%.cpp ./source/%.h
	@ echo 'Building target using g++ compiler: $<'
	$(CC) $< $(CC_FLAGS) -o $@
	@ echo.

./objects/main.o: ./source/main.cpp $(H_SOURCE)
	@ echo 'Building target using g++ compiler: $<'
	$(CC) $< $(CC_FLAGS) -o $@
	@ echo.

objFolder:
	@ mkdir -p objects

clean:
	@ $(RM) ./objects/*.o $(PROJ_NAME).exe *~
	@ rmdir objects

.PHONY: all clean