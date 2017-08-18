
file_name=main
target_file=$(file_name)
cpp_compiler=g++
cpp_version=-std=c++11
optimization_level=-O3
other_flag=-pthread
#include_path=
#link_path=

$(file_name) : $(file_name).cpp ./src/corpus.o ./src/gibbs_lda.o ./src/Timer.o
	$(cpp_compiler) $(cpp_version) $(optimization_level) \
	./src/corpus.o \
	./src/gibbs_lda.o \
	./src/Timer.o \
	$(file_name).cpp \
	-o $(file_name)

./src/corpus.o : ./src/corpus.cpp ./src/corpus.hpp
	$(cpp_compiler) $(cpp_version) $(optimization_level) \
	-c ./src/corpus.cpp \
	-o ./src/corpus.o

./src/gibbs_lda.o : ./src/gibbs_lda.cpp ./src/gibbs_lda.hpp
	$(cpp_compiler) $(cpp_version) $(optimization_level) \
	-c ./src/gibbs_lda.cpp \
	-o ./src/gibbs_lda.o

./src/Timer.o : ./src/Timer.hpp ./src/Timer.cpp
	$(cpp_compiler) $(cpp_version) $(optimization_level) \
	-c ./src/Timer.cpp \
	-o ./src/Timer.o

clean:
	rm ./src/*.o
