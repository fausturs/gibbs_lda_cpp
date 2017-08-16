
file_name=main
target_file=$(file_name)
cpp_compiler=g++
cpp_version=-std=c++14
optimization_level=-O3
other_flag=-pthread
#include_path=
#link_path=

$(file_name) : $(file_name).cpp ./src/corpus.o
	$(cpp_compiler) $(cpp_version) $(optimization_level) \
	./src/corpus.o \
	$(file_name).cpp \
	-o $(file_name)


./src/corpus.o : ./src/corpus.cpp
	$(cpp_compiler) $(cpp_version) $(optimization_level) \
	-c ./src/corpus.cpp \
	-o ./src/corpus.o

clean:
	rm ./src/*.o
