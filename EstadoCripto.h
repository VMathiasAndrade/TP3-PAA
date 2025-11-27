#ifndef ESTADO_CRIPTO_H
#define ESTADO_CRIPTO_H

#define TAMANHO_ALFABETO 26
#define TAMANHO_MAX_TEXTO 1024 * 10 // 10 KB


typedef struct {
    char alfabetoOriginal[TAMANHO_ALFABETO + 1]; // "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    char chaveDecifracao[TAMANHO_ALFABETO + 1]; 
    char textoCifrado[TAMANHO_MAX_TEXTO];
    char textoParcial[TAMANHO_MAX_TEXTO];
} EstadoCripto;

extern EstadoCripto estadoAtual;

void inicializarEstadoCripto(void);
int nomarlizarAcentuacao(FILE* f);
int carregarEEncriptarTexto(const char *nomeArquivoClaro, const char *nomeArquivoCifrado);
void casamentoExato(char* padrao);
void exportarChave(const char *nomeArquivoChave);

#endif