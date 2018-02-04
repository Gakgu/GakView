CPP = src/main.cpp \
			src/application.cpp \
			src/window.cpp \
			src/image.cpp

LIB = -lSDL2 \
			-lSDL2_image \
			-lboost_filesystem \
			-lboost_system

INCLUDE_PATH = -Iinclude

FLAGS = -Wall \
				-Wextra \
				-std=c++14 \

clang++ : clean
	clang++ $(FLAGS) $(CPP) -g -o gakview $(INCLUDE_PATH) $(LIB)

g++ : clean
	g++ $(FLAGS) $(CPP) -g -o gakview $(INCLUDE_PATH) $(LIB)

clean :
	rm -f gakview
