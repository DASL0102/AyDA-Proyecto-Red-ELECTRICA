# Compilador y flags
CXX         = g++  -std=c++17
WARN        = -Wall -Wextra -Wcast-align -Wno-sign-compare -Wno-write-strings -Wno-parentheses -Wfloat-equal -pedantic
FLAGS       = -DDEBUG -g -O0 $(WARN)

# Directorios
SRC_DIR     = src
INC_DIR     = include
BUILD_DIR   = build
DSG         = DeSiGNAR

# Archivos fuente
SRCS        = $(wildcard $(SRC_DIR)/*.cpp)
OBJS        = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))
MAIN        = main.cpp
#MAIN        = test.cpp

TARGET      = programa

# Bibliotecas
INCLUDES    = -I$(INC_DIR) -I$(DSG)/include
LIBS        = -L$(DSG)/lib -lDesignar -lpthread
SFML_LIBS   = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

# Reglas
all: $(BUILD_DIR) $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(FLAGS) $(INCLUDES) -c $< -o $@

$(TARGET): $(MAIN) $(OBJS)
	$(CXX) $(FLAGS) $(INCLUDES) $^ -o $@ $(LIBS) $(SFML_LIBS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

.PHONY: all clean run
