#ifndef ESTADO_CRIPTO_H
#define ESTADO_CRIPTO_H

#define TAMANHO_ALFABETO 26
#define TAMANHO_MAX_TEXTO 1024 * 10 // 10 KB


typedef struct {
    char alfabetoOriginal[TAMANHO_ALFABETO + 1]; // "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    char textoCifrado[TAMANHO_MAX_TEXTO];
    char chaveDecifracao[TAMANHO_ALFABETO + 1]; 
} EstadoCripto;

extern EstadoCripto estadoAtual;

void inicializarEstadoCripto(void);
int carregarEEncriptarTexto(const char *nomeArquivoClaro, const char *nomeArquivoCifrado);
void exportarChave(const char *nomeArquivoChave);

#endif