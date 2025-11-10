# Compilateur et flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinc
LDFLAGS = 

# Répertoires
SRC_DIR = src
INC_DIR = inc
OBJ_DIR = obj
BIN_DIR = .

# Nom de l'exécutable
TARGET = $(BIN_DIR)/chipasm

# Fichiers sources et objets
SRCS = $(wildcard $(SRC_DIR)/*.c) main.c
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(filter $(SRC_DIR)/%,$(SRCS))) $(OBJ_DIR)/main.o

# Headers (pour les dépendances)
HEADERS = $(wildcard $(INC_DIR)/*.h)

# Règle par défaut
all: $(OBJ_DIR) $(TARGET)

# Création du répertoire obj s'il n'existe pas
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Compilation de l'exécutable
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Compilation des fichiers .c du dossier src
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Compilation de main.c
$(OBJ_DIR)/main.o: main.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyage
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

# Nettoyage complet et recompilation
rebuild: clean all

# Pour exécuter après compilation
run: all
	./$(TARGET)

# Débogage : afficher les variables
debug:
	@echo "SRCS: $(SRCS)"
	@echo "OBJS: $(OBJS)"
	@echo "HEADERS: $(HEADERS)"

.PHONY: all clean rebuild run debug