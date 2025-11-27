#ifndef ESTADO_CRIPTO_H
#define ESTADO_CRIPTO_H
#define TAMANHO_ALFABETO 26
#define TAMANHO_MAX_TEXTO 1024 * 10 // 10 KB
#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char alfabetoOriginal[TAMANHO_ALFABETO + 1]; // "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    char chaveDecifracao[TAMANHO_ALFABETO + 1]; 
    char textoCifrado[TAMANHO_MAX_TEXTO];
    char textoParcial[TAMANHO_MAX_TEXTO];
    int shiftCriptografia;
} EstadoCripto;

extern EstadoCripto estadoAtual;

void inicializarEstadoCripto(void);
int nomarlizarAcentuacao(FILE* f);
int carregarEEncriptarTexto(const char *nomeArquivoClaro, const char *nomeArquivoCifrado);

#endif