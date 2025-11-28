# Makefile para Trabalho Prático 3 - Criptoanálise (Windows/MinGW)

# ====================================================================
# Variáveis de Configuração
# ====================================================================

# Compilador C
CC = gcc

# Diretório onde estão os códigos fonte (.c e .h)
SRC_DIR = src

# Flags de compilação:
# -I$(SRC_DIR): Diz ao compilador para procurar os .h dentro da pasta src
CFLAGS = -Wall -Wextra -std=c99 -g -I$(SRC_DIR)

# Arquivos fonte (Agora apontando explicitamente para a pasta src)
# Nota: Adicionei Operacoes.c que estava faltando
SRCS = $(SRC_DIR)/Main.c $(SRC_DIR)/EstadoCripto.c $(SRC_DIR)/Operacoes.c

# Arquivos objeto gerados (Mantemos eles na raiz para simplificar)
# Esta linha converte "src/Arquivo.c" em "Arquivo.o"
OBJS = Main.o EstadoCripto.o Operacoes.o

# Nome do executável
TARGET = tp3_criptoanalise.exe

# ====================================================================
# Regras de Build
# ====================================================================

# Regra padrão
all: $(TARGET)

# Regra de linkagem
$(TARGET): $(OBJS)
	@echo "Ligando: $^ para $@..."
	$(CC) $(OBJS) -o $(TARGET)

# Regra de compilação genérica
# Diz: "Para criar um .o na raiz, pegue o .c correspondente na pasta src"
%.o: $(SRC_DIR)/%.c
	@echo "Compilando: $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# ====================================================================
# Regras de Limpeza
# ====================================================================

clean:
	@echo "Removendo arquivos temporarios..."
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean