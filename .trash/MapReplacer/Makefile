STD_11_FLAG = -std=c++11 -D_GLIBCXX_USE_CXX11_ABI=0
CXXFLAGS = $(STD_11_FLAG)

.PHONY: clean

mapreplacer: mapreplacer.o
	g++ $(CXXFLAGS) mapreplacer.o -o mapreplacer -lsfml-graphics

clean:
	find . -name '*.o' -delete

%.o: %.cc
	g++ $(CXXFLAGS) -c $< -o $@