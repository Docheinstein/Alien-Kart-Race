STD_11_FLAG = -std=c++11 -D_GLIBCXX_USE_CXX11_ABI=0
CXXFLAGS = $(STD_11_FLAG)

.PHONY: clean

minimapgenerator: minimapgenerator.o
	g++ $(CXXFLAGS) minimapgenerator.o -o minimapgenerator -lsfml-graphics

debug: minimapgenerator.o
	g++ -g $(CXXFLAGS) minimapgenerator.o -o minimapgenerator -lsfml-graphics

clean:
	find . -name '*.o' -delete

%.o: %.cc
	g++ $(CXXFLAGS) -c $< -o $@