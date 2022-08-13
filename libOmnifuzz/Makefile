CC=clang
CXX=clang++
LINK_OMNIFUZZ=-L. -lOmnifuzz -Wl,-rpath,.

all: libomni
main: main.cc
	$(CXX) -o main.exe main.cc $(LINK_OMNIFUZZ)
libomni:
	$(CXX) -fPIC -o libOmnifuzz.o -c libOmnifuzz.cc
	$(CC) -fPIC -shared -o libOmnifuzz.so libOmnifuzz.o

clean:
	rm $(wildcard *.o) $(wildcard *.so)
