#ifndef OPERACOES_H
#define OPERACOES_H
#define COR_VERDE "\033[1;32m"
#define COR_RESET "\033[0m"
#include "EstadoCripto.h"

typedef struct {
    char letra;
    int quantidade;
    double frequencia;
}Frequencia;

void exibirEstadoCripto(); // Opção 1
void frequenciaCaracter(); // Opção 2
void casamentoExato(char* padrao); //opcão 3
void exportarChave(const char *nomeArquivoChave); //opcao 6

#endif