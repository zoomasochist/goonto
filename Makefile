LIBS=$(shell wx-config --cppflags --libs) \
	 $(shell pkg-config --cflags nlohmann_json)

goonto: main.o pack.o config.o
	c++ $(LIBS) -o $@ $^

%.o: %.cpp
	c++ $(LIBS) -c -o $@ $<