#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "EstadoCripto.h"

void inicializarEstadoCripto(void) {
    strcpy(estadoAtual.alfabetoOriginal, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    
    for (int i = 0; i < TAMANHO_ALFABETO; i++) {
        estadoAtual.chaveDecifracao[i] = '\0'; // '\0' indicando que a letra ainda não foi mapeada
    }
    estadoAtual.chaveDecifracao[TAMANHO_ALFABETO] = '\0';
    estadoAtual.textoCifrado[0] = '\0';
    
   srand(time(NULL));
}

char aplicarDeslocamento(char caractereClaro, int deslocamento) {
    if (isalpha(caractereClaro)) {
        char caractereCifrado = ((caractereClaro - 'A') + deslocamento) % TAMANHO_ALFABETO;
        return caractereCifrado + 'A';
    }
    return caractereClaro;
}

int carregarEEncriptarTexto(const char *nomeArquivoClaro, const char *nomeArquivoCifrado) {
    FILE *fClaro = fopen(nomeArquivoClaro, "r");
    if (!fClaro) {
        printf("Erro: Não foi possível abrir o arquivo de texto claro '%s'.\n", nomeArquivoClaro);
        return 0;
    }
    
    FILE *fCifrado = fopen(nomeArquivoCifrado, "w");
    if (!fCifrado) {
        printf("Erro: Não foi possível criar o arquivo de texto cifrado '%s'.\n", nomeArquivoCifrado);
        fclose(fClaro);
        return 0;
    }

    int deslocamento = (rand() % (TAMANHO_ALFABETO - 1)) + 1; 
    printf("Texto cifrado usando Cifra de Deslocamento (shift = %d).\n", deslocamento);
    
    int c;
    int indice = 0;
    
    while ((c = fgetc(fClaro)) != EOF && indice < TAMANHO_MAX_TEXTO - 1) {
        char caractereCifrado = c;
        if (isalpha(c)) {
            caractereCifrado = aplicarDeslocamento(toupper(c), deslocamento);
        }
        
        estadoAtual.textoCifrado[indice++] = caractereCifrado;
        fputc(caractereCifrado, fCifrado);
    }
    
    estadoAtual.textoCifrado[indice] = '\0';
    
    printf("Texto cifrado salvo em '%s'.\n", nomeArquivoCifrado);
    
    fclose(fClaro);
    fclose(fCifrado);
    return 1;
}

void exportarChave(const char *nomeArquivoChave) {
    FILE *fChave = fopen(nomeArquivoChave, "w");
    if (!fChave) {
        printf("Erro: Não foi possível salvar a chave em '%s'.\n", nomeArquivoChave);
        return;
    }

    fprintf(fChave, "--- CHAVE FINAL DE CRIPTOANÁLISE ---\n");
    fprintf(fChave, "Cifrado -> Original\n");

    for (int i = 0; i < TAMANHO_ALFABETO; i++) {
        char mapeadoPara = estadoAtual.chaveDecifracao[i];
        char charCifrado = 'A' + i;

        if (mapeadoPara != '\0') {
            fprintf(fChave, "%c -> %c\n", charCifrado, mapeadoPara);
        } else {
            fprintf(fChave, "%c -> DESCONHECIDA\n", charCifrado);
        }
    }

    printf("Chave de criptografia exportada com sucesso para '%s'.\n", nomeArquivoChave);
    fclose(fChave);
}