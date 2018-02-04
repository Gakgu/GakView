test : clean
	g++ \
		../src/window.cpp \
		../src/image.cpp \
		../src/application.cpp \
		test_window.cpp \
		test_image.cpp \
		test_application.cpp \
		-g -W -Wall \
		-o test \
		-I../include \
		`pkg-config --cflags --libs gtest sdl2` \
		-lSDL2_image \
		-lboost_system \
		-lboost_filesystem

clean :
	rm -rf test
