# Chess engine - bouwen met: make   |   starten met: make run   |   opkuisen: make clean
CXX      = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -O2
SRC      = $(wildcard src/*.cpp)
TARGET   = engine

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: run clean
