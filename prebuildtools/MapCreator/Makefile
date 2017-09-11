STD_11_FLAG = -std=c++11 -D_GLIBCXX_USE_CXX11_ABI=0
CXXFLAGS = $(STD_11_FLAG)

.PHONY: clean

mapcreator: mapcreator.o
	g++ $(CXXFLAGS) mapcreator.o -o mapcreator -lsfml-graphics

clean:
	find . -name '*.o' -delete

%.o: %.cc
	g++ $(CXXFLAGS) -c $< -o $@