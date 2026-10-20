RAYLIB_SRC := thirdparty/raylib-6.2/src
RAYLIB_LIB := $(RAYLIB_SRC)/libraylib.a

CXXFLAGS := -std=c++23 -Wall -Wextra -ggdb -Ithirdparty -I$(RAYLIB_SRC)
LDLIBS := -framework Cocoa -framework IOKit -framework OpenGL -framework CoreVideo -framework QuartzCore

main: main.cpp $(RAYLIB_LIB)
	$(CXX) $(CXXFLAGS) -o $@ main.cpp $(RAYLIB_LIB) $(LDLIBS)

$(RAYLIB_LIB):
	$(MAKE) -C $(RAYLIB_SRC) PLATFORM=PLATFORM_DESKTOP

clean:
	rm -rf main

disctclean: clean
	$(MAKE) -C $(RAYLIB_SRC) clean

.PHONY: clean disctclean

