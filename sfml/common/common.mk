CC = g++ -fdiagnostics-color=always
CFLAGS = -std=c++11 -Wall -Wextra
OBJS = $(TARGET).o
COMMON = ../common/useful.o
IDIRS = -I ..
LIBS = -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-system

$(TARGET): $(OBJS) $(DEPS) $(COMMON)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $< $(IDIRS)

clean:
	rm -f *.o
distclean: clean
	rm -f $(TARGET)

.PHONY: clean
