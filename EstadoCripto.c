#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "EstadoCripto.h"

// Tabela de frequência do Português 
typedef struct { 
    char letra;
    double frequencia;
    } FrequenciaPortugues;

FrequenciaPortugues freqPortugues[] = {
    {'A', 14.63}, {'E', 12.57}, {'O', 10.73}, {'S', 7.81}, {'R', 6.53}, {'I', 6.18}, 
    {'N', 5.05}, {'D', 4.99}, {'M', 4.74}, {'U', 4.63}, {'T', 4.34}, {'C', 3.88}, 
    {'L', 2.78}, {'P', 2.52}, {'V', 1.67}, {'G', 1.30}, {'H', 1.28}, {'Q', 1.20}, 
    {'B', 1.04}, {'F', 1.02}, {'Z', 0.47}, {'J', 0.40}, {'X', 0.21}, {'K', 0.02}, 
    {'W', 0.01}, {'Y', 0.01}
};
#define NUM_LETRAS_PORTUGUES 26

const char* ARQUIVOS_PROFECIAS[12] = {
    "texto_claro/Anaxa.txt", 
    "texto_claro/Cipher.txt",
    "texto_claro/Mydei.txt",
    "texto_claro/Cerydra.txt",
    "texto_claro/Aglaea.txt", 
    "texto_claro/Hyacine.txt",
    "texto_claro/Tribios.txt", 
    "texto_claro/Dang Heng - Terravox.txt", 
    "texto_claro/Hysilens.txt",
    "texto_claro/Phainon.txt",
    "texto_claro/Castorice.txt",
    "texto_claro/Cyrene.txt"
};

typedef struct { char letra;
    int contagem;
    double frequencia;
} Frequencia;

void inicializarEstadoCripto(void) {
    strcpy(estadoAtual.alfabetoOriginal, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    
    for (int i = 0; i < TAMANHO_ALFABETO; i++) {
        estadoAtual.chaveDecifracao[i] = '\0'; // '\0' indicando que a letra ainda não foi mapeada
    }
    estadoAtual.chaveDecifracao[TAMANHO_ALFABETO] = '\0';
    estadoAtual.textoCifrado[0] = '\0';
    estadoAtual.textoParcial[0] = '\0';
    estadoAtual.shiftCriptografia = 0; // Inicializando o shift
    
   srand(time(NULL));
}

int nomarlizarAcentuacao(FILE* f){
    int c = fgetc(f);
    if (c == EOF) return EOF;

    if (c == 0xC3) {
        int prox = fgetc(f);

        switch (prox) {
        case 0x80: case 0x81: case 0x82: case 0x83: // À, Á, Â, Ã
        case 0xA0: case 0xA1: case 0xA2: case 0xA3: // à, á, â, ã
            return 'A';
            
        case 0x89: case 0x8A: // É, Ê
        case 0xA9: case 0xAA: // é, ê
            return 'E';
            
        case 0x8D: // Í
        case 0xAD: // í
            return 'I';
        
        case 0x93: case 0x94: case 0x95: // Ó, Ô, Õ
        case 0xB3: case 0xB4: case 0xB5: // ó, ô, õ
            return 'O';
        
        case 0x9A: // Ú
        case 0xBA: // ú
            return 'U';
        
        case 0x87: // Ç
        case 0xA7: // ç
            return 'C';
        
        default:
            break;
        }
    }
    
    if (isalpha(c)) return toupper(c);

    return c;
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

    estadoAtual.shiftCriptografia = (rand() % (TAMANHO_ALFABETO - 1)) + 1; 
    printf("Texto cifrado usando Cifra de Deslocamento (shift = %d).\n", estadoAtual.shiftCriptografia);
    
    int c;
    int indice = 0;
    
    while ((c = nomarlizarAcentuacao(fClaro)) != EOF && indice < TAMANHO_MAX_TEXTO - 1) {
        char caractereCifrado = c;
        if (isalpha(c)) {
            caractereCifrado = aplicarDeslocamento(toupper(c), estadoAtual.shiftCriptografia);
        }
        
        estadoAtual.textoCifrado[indice++] = caractereCifrado;
        fputc(caractereCifrado, fCifrado);
    }
    
    estadoAtual.textoCifrado[indice] = '\0';
    strcpy(estadoAtual.textoParcial, estadoAtual.textoCifrado);
    
    printf("Texto cifrado salvo em '%s'.\n", nomeArquivoCifrado);
    
    fclose(fClaro);
    fclose(fCifrado);
    return 1;
}

void exibirEstadoCripto() {
    printf("\n=== Estado Atual da Criptoanálise ===\n");
    printf("\n=== Texto Criptografado ===\n");
    printf(estadoAtual.textoCifrado); // Exibe os primeiros 200 chars por causa do tamanho definido por TAMANHO_MAX_TEXTO

    printf("\n=== Chave (Cifrado -> Original - Mapeamentos Conhecidos) ===\n");
    printf("Chave: ");
    int mapeamentosEncontrados = 0;
    for (int i = 0; i < TAMANHO_ALFABETO; i++) {
        char charCifrado = 'A' + i;
        char charOriginal = estadoAtual.chaveDecifracao[i];
        
        if (charOriginal != '\0') {
            printf("%c -> %c | ", charCifrado, charOriginal);
            mapeamentosEncontrados = 1;
        }
    }
    if (!mapeamentosEncontrados) {
        printf("Vazia.\n");
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
                // Letra mapeada: usa a letra original (decifrada)
                estadoAtual.textoParcial[i] = charOriginal;
            } else {
                // Letra não mapeada: usa a letra cifrada original
                estadoAtual.textoParcial[i] = charCifrado;
            }
        } else {
            // Copia pontuação e espaço
            estadoAtual.textoParcial[i] = charCifrado;
        }
    }
    estadoAtual.textoParcial[i] = '\0';
    
    printf( estadoAtual.textoParcial); 
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

/* int compararFrequencias(const void *a, const void *b) {
    Frequencia *fa = (Frequencia *)a;
    Frequencia *fb = (Frequencia *)b;
    
    // Ordena por frequência decrescente
    if (fb->frequencia > fa->frequencia) return 1;
    if (fb->frequencia < fa->frequencia) return -1;
    return fa->letra - fb->letra;
}

void gerarAnaliseEChute(const char *rotulo, const char *textoParaAnalise) {
    int contagens[TAMANHO_ALFABETO] = {0};
    int totalLetras = 0;
    
    // Contagem de frequência
    for (int i = 0; textoParaAnalise[i] != '\0'; i++) {
        char c = toupper(textoParaAnalise[i]);
        if (c >= 'A' && c <= 'Z') {
            contagens[c - 'A']++;
            totalLetras++;
        }
    }
    
    if (totalLetras == 0) {
        printf("Não há letras para análise no texto %s.\n", rotulo);
        return;
    }

    Frequencia listaFrequencias[TAMANHO_ALFABETO];
    int numLetrasEncontradas = 0;
    
    for (int i = 0; i < TAMANHO_ALFABETO; i++) {
        if (contagens[i] > 0) {
            listaFrequencias[numLetrasEncontradas].letra = 'A' + i;
            listaFrequencias[numLetrasEncontradas].contagem = contagens[i];
            listaFrequencias[numLetrasEncontradas].frequencia = ((double)contagens[i] / totalLetras) * 100.0;
            numLetrasEncontradas++;
        }
    }
    
    qsort(listaFrequencias, numLetrasEncontradas, sizeof(Frequencia), compararFrequencias);

    printf("\n--- Tabela de Frequência do Texto Cifrado (%s) ---\n", rotulo);
    printf("| Letra Cifrada | Contagem | Frequência Relativa |\n");
    printf("|---------------|----------|---------------------|\n");
    for (int i = 0; i < numLetrasEncontradas; i++) {
        printf("| %13c | %8d | %19.2f%% |\n", 
               listaFrequencias[i].letra, 
               listaFrequencias[i].contagem, 
               listaFrequencias[i].frequencia);
    }
    printf("--------------------------------------------------\n");
    
    // Tentativa de Chute
    printf("\n--- Tentativa de Chute de Chave (Comparação com Português) ---\n");
    printf("Decisão do Grupo: Mapear a i-ésima letra mais frequente do cifrado para a i-ésima mais frequente em Português.\n");
    printf("| Letra Cifrada | Freq. Cifrada | Chute Original | Freq. Português |\n");
    printf("|---------------|---------------|----------------|------------------|\n");

    printf("\nChave Sugerida (Cifrado -> Original):\n");
    for (int i = 0; i < numLetrasEncontradas && i < NUM_LETRAS_PORTUGUES; i++) {
        char charCifrado = listaFrequencias[i].letra;
        char charOriginal = freqPortugues[i].letra;
        
        printf("| %13c | %13.2f%% | %14c | %16.2f%% |\n",
               charCifrado, 
               listaFrequencias[i].frequencia, 
               charOriginal, 
               freqPortugues[i].frequencia);
        
        printf("%c -> %c | ", charCifrado, charOriginal);
    }
    printf("\n");
    
    printf("\n*O chute acima deve ser aplicado manualmente via Opção 5 se considerado confiável.*\n");
}

void analisarEChutarFrequencia(void) {
    // Verifica se o shift foi armazenado (CRÍTICO)
    if (estadoAtual.shiftCriptografia == 0) {
        printf("Erro: O shift de criptografia inicial não foi armazenado ou é 0. Reinicie o programa.\n");
        return;
    }
    
    // --- 1. Análise do Arquivo Individual ---
    printf("\n=========================================================\n");
    printf("= 1. ANÁLISE DE FREQUÊNCIA: ARQUIVO INDIVIDUAL (ANAXA) =\n");
    printf("=========================================================\n");
    gerarAnaliseEChute("Anaxa.txt (Individual)", estadoAtual.textoCifrado);

    // --- 2. Análise dos 12 Arquivos Combinados ---
    char *textoCombinado = (char *)malloc(TAMANHO_MAX_TEXTO * NUM_TEXTOS_PROFECIAS + 1);
    if (textoCombinado == NULL) {
        printf("Erro de alocação de memória para o texto combinado.\n");
        return;
    }

    int tamanhoTextoCombinado = 0;
    
    for (int j = 0; j < NUM_TEXTOS_PROFECIAS; j++) {
        FILE *fClaro = fopen(ARQUIVOS_PROFECIAS[j], "r");
        if (!fClaro) {
            printf("Aviso: Não foi possível abrir o arquivo %s para combinação. Pulando.\n", ARQUIVOS_PROFECIAS[j]);
            continue;
        }

        int c;
        // Lê, normaliza e cifra com o shift guardado
        while ((c = nomarlizarAcentuacao(fClaro)) != EOF) {
            if (tamanhoTextoCombinado >= TAMANHO_MAX_TEXTO * NUM_TEXTOS_PROFECIAS - 1) {
                break; // Limite de buffer
            }
            
            char caractereCifrado = c;
            if (c >= 'A' && c <= 'Z') {
                // Aplica a mesma Cifra de Deslocamento armazenada
                caractereCifrado = aplicarDeslocamento(c, estadoAtual.shiftCriptografia);
            }
            
            textoCombinado[tamanhoTextoCombinado++] = caractereCifrado;
        }
        fclose(fClaro);
    }
    
    textoCombinado[tamanhoTextoCombinado] = '\0';

    printf("\n=======================================================\n");
    printf("= 2. ANÁLISE DE FREQUÊNCIA: 12 ARQUIVOS COMBINADOS (=) =\n");
    printf("=======================================================\n");
    gerarAnaliseEChute("Todos os 12 Textos Cifrados", textoCombinado);
    
    free(textoCombinado);

    printf("\n**Comentário (Para o Relatório)**:\n");
    printf("A análise de frequência do texto combinado (12 arquivos) usa uma amostra maior e mais representativa\n");
    printf("da língua portuguesa do que o arquivo individual, o que torna o 'chute' combinatório geralmente mais preciso.\n");
    printf("Isso ajuda a confirmar os mapeamentos para as letras de alta frequência (A, E, O, etc.).\n");
} */
// esse comentado esta 100 gemini nao mexi em nada

void casamentoExato(char* padrao) {
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