#include "Operacoes.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

void exibirEstadoCripto() {
    printf("\n=== Estado Atual da Criptoanálise ===\n");
    printf("\n=== Texto Criptografado ===\n");
    printf("%s\n", estadoAtual.textoCifrado); // Exibe os primeiros 200 chars por causa do tamanho definido por TAMANHO_MAX_TEXTO

    printf("\n === Chave ===\n");
    for (int i = 0; i < TAMANHO_ALFABETO; i++) {
        printf("%c", 'A' + i);
    }
    printf("\n");

    for (int i = 0; i < TAMANHO_ALFABETO; i++) {
        char charOriginal = estadoAtual.chaveDecifracao[i];
        
        if (charOriginal != '\0') {
            printf("%c", charOriginal); // Exibe a letra mapeada
        } else {
            printf(" "); // Exibe espaço vazio se não houver mapeamento ainda
        }
    }
    printf("\n");

    printf("\n=== Texto Parcialmente Decifrado ===\n");
    
    // Decifração parcial: atualiza estadoAtual.textoParcial
    int i;
    int len = strlen(estadoAtual.textoCifrado);
    
    for (i = 0; i < len && i < TAMANHO_MAX_TEXTO - 1; i++) {
        char charCifrado = estadoAtual.textoCifrado[i];
        
        if (isalpha(charCifrado)) {
            int indice = charCifrado - 'A';
            char charOriginal = estadoAtual.chaveDecifracao[indice];
            
            if (charOriginal != '\0') {
                printf("\033[1;32m%c\033[0m", charOriginal); 
            } else {
                printf("%c", charCifrado);
            }
        } else {
            printf("%c", charCifrado);
        }
    }
    printf("\n");
}

void alterarChave(char charCifrado, char charOriginal) {
    charCifrado = toupper(charCifrado);
    charOriginal = toupper(charOriginal);

    if (charCifrado >= 'A' && charCifrado <= 'Z' &&
        charOriginal >= 'A' && charOriginal <= 'Z') 
    {
        // Remove mapeamento antigo do caractere ORIGINAL, se existir
        for (int i = 0; i < TAMANHO_ALFABETO; i++) {
            if (estadoAtual.chaveDecifracao[i] == charOriginal) {
                estadoAtual.chaveDecifracao[i] = '\0'; 
                printf("Aviso: Mapeamento antigo %c -> %c removido.\n", 'A' + i, charOriginal);
            }
        }
        
        // Verifica se a letra cifrada já estava mapeada (sobrescreve)
        if (estadoAtual.chaveDecifracao[charCifrado - 'A'] != '\0') {
            printf("Aviso: O caractere cifrado '%c' já estava mapeado. Mapeamento sobrescrito.\n", charCifrado);
        }

        // Aplica o novo mapeamento (Cifrado -> Original)
        estadoAtual.chaveDecifracao[charCifrado - 'A'] = charOriginal;
        printf("Registrado: %c (cifrado) -> %c (original)\n", charCifrado, charOriginal);
    } else {
        printf("Erro: Caracteres inválidos. Use apenas letras de A-Z.\n");
    }
}

void frequenciaCaracter() {
    int alfabeto[26] = {0};
    int totalLetras = 0;

    for (int i = 0; estadoAtual.textoCifrado[i] != '\0'; i++) {
        unsigned char c = (unsigned char)estadoAtual.textoCifrado[i];

        if (c >= 'A' && c <= 'Z') {
            int indice = c - 'A';
            alfabeto[indice]++;
            totalLetras++;
        }
    }
    
    Frequencia listaOcorrencia[26];
    for (int i = 0; i < 26; i++) {
        listaOcorrencia[i].letra = 'A' + i;
        listaOcorrencia[i].quantidade = alfabeto[i];
        listaOcorrencia[i].frequencia = ((double)listaOcorrencia[i].quantidade / totalLetras) * 100.0;
    }

    for (int i = 0; i < 26 - 1; i++) {
        for (int j = 0; j < 26 - i - 1; j++) {
            if (listaOcorrencia[j].quantidade < listaOcorrencia[j + 1].quantidade) {
                Frequencia temp = listaOcorrencia[j];
                listaOcorrencia[j] = listaOcorrencia[j + 1];
                listaOcorrencia[j + 1] = temp;
            }
        }
    }
    
    printf("Letra  |  QTDE  |  Freq.\n");
    for (int i = 0; i < 26; i++) {
        if (listaOcorrencia[i].quantidade > 0) {
            printf("%c    |    %d    |    %.2f%%\n",
                listaOcorrencia[i].letra,
                listaOcorrencia[i].quantidade,
                listaOcorrencia[i].frequencia);
        }
    }
    
    char opcao;
    do {
        printf("\nDeseja dar um 'chute'? (Sim - s / Nao - n)");
        scanf(" %c", &opcao);

        if (opcao == 'S' || opcao == 's') {
            char cifrada, original;

            printf("Qual letra deseja trocar: ");
            scanf(" %c", &cifrada);
            printf("Trocar por qual: ");
            scanf(" %c", &original);

            cifrada = toupper(cifrada); 
            original = toupper(original);
            
            int indice = cifrada - 'A';

            if (estadoAtual.chaveDecifracao[indice] != '\0') {
                printf("AVISO: letra foi mapeada e ja houve uma suposicao!\n");
                break;
            }

            estadoAtual.chaveDecifracao[indice] = original;
        }
    } while (opcao == 'S' || opcao == 's');
}

void casamentoExato(char* padrao) {
    //Algoritmo Shift-And
    int m = strlen(padrao);
    int n = strlen(estadoAtual.textoParcial);

    if (m > 64) {
        printf("Erro: O padrão é muito longo para o algoritmo Shift-And (max 64 caracteres).\n");
        return;
    }
    
    unsigned long M[256]; //Tabela M

    for (int i = 0; i < 256; i++) {
        M[i] = 0;
    }
    for (int j = 0; j < m; j++) {
        unsigned char c = (unsigned char)padrao[j];
        M[c] |= (1UL << j);
    }
    
    unsigned long R = 0;
    int ocorrencias = 0;

    for (int i = 0; i < n; i++) {
        unsigned char c = (unsigned char)estadoAtual.textoParcial[i];

        R = ((R << 1) | 1UL) & M[c];
        
        if (R & (1UL << (m - 1))) ocorrencias++;
    }

    printf("\nOcorrencias: %d\n", ocorrencias);
}

void casamentoAproximado(char* padrao) {

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