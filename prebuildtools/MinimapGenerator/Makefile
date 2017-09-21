STD_11_FLAG = -std=c++11 -D_GLIBCXX_USE_CXX11_ABI=0
CXXFLAGS = $(STD_11_FLAG)
PRJ_NAME = minimapgenerator

.PHONY: clean

$(PRJ_NAME): $(PRJ_NAME).o
	g++ $(CXXFLAGS) $(PRJ_NAME).o -o $(PRJ_NAME) -lsfml-graphics

debug: $(PRJ_NAME).o
	g++ -g $(CXXFLAGS) $(PRJ_NAME).o -o $(PRJ_NAME) -lsfml-graphics

clean:
	find . -name '*.o' -delete

%.o: %.cc
	g++ $(CXXFLAGS) -c $< -o $@