CPP = main.cpp \
			gakview.cpp \
			gakview_window.cpp \
			gakview_image.cpp \
			gakview_filemanager.cpp

LIB = -lSDL2 \
			-lboost_filesystem \
			-lboost_system

FLAGS = -Wall \
				-Wextra \
				-std=c++14 \

clang++ : clean
	clang++ $(FLAGS) $(CPP) -g -o gakview -I. $(LIB)

g++ : clean
	g++ $(FLAGS) $(CPP) -g -o gakview -I. $(LIB)

clean :
	rm -f gakview
