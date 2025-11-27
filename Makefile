# Makefile para Trabalho Prático 3 - Criptoanálise (Windows/MinGW)

# ====================================================================
# Variáveis de Configuração
# ====================================================================

# Compilador C (use 'gcc' se estiver no ambiente MinGW ou Cygwin)
CC = gcc

# Flags de compilação:
# -Wall -Wextra: Avisos de compilação (boas práticas)
# -std=c99: Usar o padrão C99
# -g: Incluir informações de debug (útil para depuração)
CFLAGS = -Wall -Wextra -std=c99 -g

# Arquivos fonte
SRCS = Main.c EstadoCripto.c

# Arquivos objeto gerados
OBJS = $(SRCS:.c=.o)

# Nome do executável (para Windows)
TARGET = tp3_criptoanalise.exe

# ====================================================================
# Regras de Build
# ====================================================================

# Regra padrão: compilar tudo
all: $(TARGET)

# Regra de linkagem: Cria o executável a partir dos arquivos objeto
$(TARGET): $(OBJS)
	@echo "Ligando: $^ para $@..."
	$(CC) $(OBJS) -o $(TARGET)

# Regra de compilação: Cria os arquivos objeto (.o) a partir dos fontes (.c)
# Esta regra usa $< (primeiro pré-requisito) e $@ (nome do alvo)
.c.o:
	@echo "Compilando: $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# ====================================================================
# Regras de Limpeza
# ====================================================================

# Regra de limpeza: Remove arquivos gerados (.o e .exe)
clean:
	@echo "Removendo arquivos temporários (.o e .exe)..."
	-del /Q $(OBJS) $(TARGET)

# Regra para evitar conflitos de nomes (não é um arquivo)
.PHONY: all clean