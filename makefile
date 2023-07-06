# Variables
CXX = g++
CXXFLAGS = -Wall --std=c++2a -Isrc/include

# Nombres de los archivos
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# Busca todos los archivos .cpp en SRC_DIR para compilarlos
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
# Genera los nombres de los archivos .o en OBJ_DIR
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))

# Nombre del ejecutable
TARGET = $(BIN_DIR)/DRP

# Reglas
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

test:
	./bin/DRP DRP-INSTANCIAS/SJC-DRP/test.txt 1000 100

# Limpia los archivos compilados y el ejecutable
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

.PHONY: clean
