CXXFLAGS = -Wall
LDLIBS=-lpcap 

all: airodump

main.o: main.cpp

mac.o: mac.cpp

airodump: main.o mac.o
	$(LINK.cc) $^ $(LOADLIBES) $(LDLIBS) -o $@


clean:
	rm -f airodump *.o