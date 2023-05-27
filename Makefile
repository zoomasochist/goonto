LIBS=$(shell wx-config --cppflags --libs std) \
	 $(shell pkg-config --cflags --libs libvlc xtst x11 nlohmann_json)

.PHONY: clean

goonto: main.o pack.o config.o
	c++ $(LIBS) -o $@ $^

%.o: %.cpp
	c++ $(LIBS) -c -o $@ $<

clean:
	@rm goonto *.o