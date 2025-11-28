#include "Operacoes.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

void exibirEstadoCripto()
{
    printf("\n=== Estado Atual da Criptoanálise ===\n");
    printf("\n=== Texto Criptografado ===\n");
    printf("%s\n", estadoAtual.textoCifrado); // Exibe os primeiros 200 chars por causa do tamanho definido por TAMANHO_MAX_TEXTO

    printf("\n === Chave ===\n");
    for (int i = 0; i < TAMANHO_ALFABETO; i++)
    {
        printf("%c", 'A' + i);
    }
    printf("\n");

    for (int i = 0; i < TAMANHO_ALFABETO; i++)
    {
        char charOriginal = estadoAtual.chaveDecifracao[i];

        if (charOriginal != '\0')
        {
            printf("%c", charOriginal); // Exibe a letra mapeada
        }
        else
        {
            printf(" "); // Exibe espaço vazio se não houver mapeamento ainda
        }
    }
    printf("\n");

    printf("\n=== Texto Parcialmente Decifrado ===\n");

    // Decifração parcial: atualiza estadoAtual.textoParcial
    int i;
    int len = strlen(estadoAtual.textoCifrado);

    for (i = 0; i < len && i < TAMANHO_MAX_TEXTO - 1; i++)
    {
        char charCifrado = estadoAtual.textoCifrado[i];

        if (isalpha(charCifrado))
        {
            int indice = charCifrado - 'A';
            char charOriginal = estadoAtual.chaveDecifracao[indice];

            if (charOriginal != '\0')
            {
                printf("\033[1;32m%c\033[0m", charOriginal);
            }
            else
            {
                printf("%c", charCifrado);
            }
        }
        else
        {
            printf("%c", charCifrado);
        }
    }
    printf("\n");
}

void alterarChave(char charCifrado, char charOriginal)
{
    charCifrado = toupper(charCifrado);
    charOriginal = toupper(charOriginal);

    if (charCifrado >= 'A' && charCifrado <= 'Z' &&
        charOriginal >= 'A' && charOriginal <= 'Z')
    {
        // Remove mapeamento antigo do caractere ORIGINAL, se existir
        for (int i = 0; i < TAMANHO_ALFABETO; i++)
        {
            if (estadoAtual.chaveDecifracao[i] == charOriginal)
            {
                estadoAtual.chaveDecifracao[i] = '\0';
                printf("Aviso: Mapeamento antigo %c -> %c removido.\n", 'A' + i, charOriginal);
            }
        }

        // Verifica se a letra cifrada já estava mapeada (sobrescreve)
        if (estadoAtual.chaveDecifracao[charCifrado - 'A'] != '\0')
        {
            printf("Aviso: O caractere cifrado '%c' já estava mapeado. Mapeamento sobrescrito.\n", charCifrado);
        }

        // Aplica o novo mapeamento (Cifrado -> Original)
        estadoAtual.chaveDecifracao[charCifrado - 'A'] = charOriginal;
        printf("Registrado: %c (cifrado) -> %c (original)\n", charCifrado, charOriginal);
    }
    else
    {
        printf("Erro: Caracteres inválidos. Use apenas letras de A-Z.\n");
    }
}

void frequenciaCaracter()
{

    int alfabeto[26] = {0};
    int totalLetras = 0;

    for (int i = 0; estadoAtual.textoCifrado[i] != '\0'; i++)
    {
        unsigned char c = (unsigned char)estadoAtual.textoCifrado[i];

        if (c >= 'A' && c <= 'Z')
        {
            int indice = c - 'A';
            alfabeto[indice]++;
            totalLetras++;
        }
    }

    Frequencia listaOcorrencia[26];
    for (int i = 0; i < 26; i++)
    {
        listaOcorrencia[i].letra = 'A' + i;
        listaOcorrencia[i].quantidade = alfabeto[i];

        if (totalLetras > 0)
        {
            listaOcorrencia[i].frequencia =
                ((double)listaOcorrencia[i].quantidade / totalLetras) * 100.0;
        }
        else
        {
            listaOcorrencia[i].frequencia = 0.0;
        }
    }

    for (int i = 0; i < 25; i++)
    {
        for (int j = 0; j < 25 - i; j++)
        {
            if (listaOcorrencia[j].quantidade < listaOcorrencia[j + 1].quantidade)
            {
                Frequencia temp = listaOcorrencia[j];
                listaOcorrencia[j] = listaOcorrencia[j + 1];
                listaOcorrencia[j + 1] = temp;
            }
        }
    }

    printf("\n=== Frequencia LOCAL do arquivo ===\n");
    printf("Letra | Qtde | Freq(%%)\n");
    for (int i = 0; i < 26; i++)
    {
        if (listaOcorrencia[i].quantidade > 0)
        {
            printf("%c | %d | %.2f%%\n",
                   listaOcorrencia[i].letra,
                   listaOcorrencia[i].quantidade,
                   listaOcorrencia[i].frequencia);
        }
    }

    int freqGlobal[26];
    int totalGlobal;

    carregarEEncriptarTodosTextos(estadoAtual.shiftCriptografia, freqGlobal, &totalGlobal);

    printf("\n=== Frequencia GLOBAL (12 arquivos juntos) ===\n");
    printf("Letra | Qtde | Freq(%%)\n");
    for (int i = 0; i < 26; i++)
    {
        if (freqGlobal[i] > 0)
        {
            printf("%c | %d | %.2f%%\n", 'A' + i, freqGlobal[i], (freqGlobal[i] * 100.0) / totalGlobal);
        }
    }

    printf("\n=== Comparacao LOCAL x GLOBAL ===\n");
    printf("Letra | Local(%%) | Global(%%) | Diferenca\n");

    for (int i = 0; i < 26; i++)
    {
        double local = (totalLetras > 0) ? (alfabeto[i] * 100.0 / totalLetras) : 0.0;
        double global = (totalGlobal > 0) ? (freqGlobal[i] * 100.0 / totalGlobal) : 0.0;

        printf("%c | %.2f | %.2f | %.2f\n",
               'A' + i, local, global, local - global);
    }

    double freqPortugues[26] = {
        14.63, 1.04, 3.88, 4.99, 12.57, 1.02,
        1.30, 1.28, 6.18, 0.40, 0.02, 2.78,
        4.74, 5.05, 10.73, 2.52, 1.20, 6.53,
        7.81, 4.34, 4.63, 1.67, 0.01, 0.21,
        0.01, 0.47}; // tabela da especificacao

    int ordemPortugues[26];
    for (int i = 0; i < 26; i++)
        ordemPortugues[i] = i;

    for (int i = 0; i < 25; i++)
    {
        for (int j = 0; j < 25 - i; j++)
        {
            if (freqPortugues[ordemPortugues[j]] < freqPortugues[ordemPortugues[j + 1]])
            {
                int tmp = ordemPortugues[j];
                ordemPortugues[j] = ordemPortugues[j + 1];
                ordemPortugues[j + 1] = tmp;
            }
        }
    }

    printf("\n=== CHUTE DE MAPEAMENTO SUGERIDO ===\n");
    printf("(com base em frequencia local x lingua portuguesa)\n\n");

    for (int i = 0; i < 26; i++)
    {
        char cifrada = listaOcorrencia[i].letra;
        char originalSugerido = 'A' + ordemPortugues[i];

        printf("%c (cifrado) -> %c (provavel original)\n",
               cifrada, originalSugerido);
    }

    char opcao;
    do
    {
        printf("\nDeseja aplicar algum mapeamento sugerido? (s/n): ");
        scanf(" %c", &opcao);

        if (opcao == 's' || opcao == 'S')
        {
            char cifrada, original;
            printf("Letra cifrada: ");
            scanf(" %c", &cifrada);

            printf("Mapear para: ");
            scanf(" %c", &original);

            alterarChave(cifrada, original);
        }

    } while (opcao == 's' || opcao == 'S');
}

void casamentoExato(char *padrao)
{
    // Algoritmo Shift-And
    int m = strlen(padrao);
    int n = strlen(estadoAtual.textoCifrado);

    if (m > 64)
    {
        printf("Erro: O padrão e muito longo para o algoritmo Shift-And (max 64 caracteres).\n");
        return;
    }

    unsigned long M[256]; // Tabela M

    for (int i = 0; i < 256; i++)
    {
        M[i] = 0;
    }
    for (int j = 0; j < m; j++)
    {
        unsigned char c = (unsigned char)padrao[j];
        M[c] |= (1UL << j);
    }

    unsigned long R = 0;
    int ocorrencias = 0;

    for (int i = 0; i < n; i++)
    {
        unsigned char c = (unsigned char)estadoAtual.textoParcial[i];

        R = ((R << 1) | 1UL) & M[c];

        if (R & (1UL << (m - 1)))
            ocorrencias++;
    }

    printf("\nOcorrencias: %d\n", ocorrencias);
}

void casamentoAproximado(char *padrao, int k)
{
    int m = strlen(padrao);                   // é o tamanho do padrão
    int n = strlen(estadoAtual.textoParcial); // é o tamanho do texto decifrado

    if (m > 64)
    {
        printf("Erro: O padrao e muito longo (max 64 caracteres).\n");
        return;
    }
    char *textoDecifradoTemp = (char *)malloc((n + 1) * sizeof(char));
    if (!textoDecifradoTemp)
        return;

    for (int i = 0; i < n; i++)
    {
        char c = estadoAtual.textoParcial[i];
        if (isalpha(c))
        {
            int indice = c - 'A';
            if (estadoAtual.chaveDecifracao[indice] != '\0')
            {
                textoDecifradoTemp[i] = estadoAtual.chaveDecifracao[indice];
            }
            else
            {
                textoDecifradoTemp[i] = c;
            }
        }
        else
        {
            textoDecifradoTemp[i] = c;
        }
    }
    textoDecifradoTemp[n] = '\0';

    unsigned long M[256];
    for (int i = 0; i < 256; i++)
        M[i] = 0;

    for (int j = 0; j < m; j++)
    {
        unsigned char c = (unsigned char)padrao[j];
        M[c] |= (1UL << j);
    }
    unsigned long R[k + 1];
    unsigned long R_antigo[k + 1];

    for (int j = 0; j <= k; j++)
        R[j] = 0;

    int ocorrencias = 0;

    printf("\n--- Ocorrencias Aproximadas (Erro max: %d) ---\n", k);
    for (int i = 0; i < n; i++)
    {
        unsigned char c = (unsigned char)textoDecifradoTemp[i];

        for (int j = 0; j <= k; j++)
            R_antigo[j] = R[j];

        R[0] = ((R_antigo[0] << 1) | 1UL) & M[c];

        for (int j = 1; j <= k; j++)
        {
            unsigned long match = ((R_antigo[j] << 1) | 1UL) & M[c];
            unsigned long substituicao = (R_antigo[j - 1] << 1) | 1UL;

            R[j] = match | substituicao;
        }

        if (R[k] & (1UL << (m - 1)))
        {
            ocorrencias++;

            int inicio = i - m + 1;
            if (inicio < 0)
                inicio = 0;
            printf("Posicao %d: ", inicio);

            for (int x = inicio; x <= i; x++)
            {
                if (textoDecifradoTemp[x] == padrao[x - inicio])
                {
                    printf("\033[1;32m%c\033[0m", textoDecifradoTemp[x]);
                }
                else
                {
                    printf("%c", textoDecifradoTemp[x]);
                }
            }
            printf("\n");
        }
    }

    printf("Total de ocorrencias encontradas: %d\n", ocorrencias);
    free(textoDecifradoTemp);
}

void gerarTextoDecifradoCompleto(char *saida)
{
    int len = strlen(estadoAtual.textoCifrado);

    for (int i = 0; i < len; i++)
    {
        char c = estadoAtual.textoCifrado[i];

        if (isalpha(c))
        {
            int indice = c - 'A';
            char decif = estadoAtual.chaveDecifracao[indice];

            if (decif != '\0')
            {
                saida[i] = decif;
            }
            else
            {
                saida[i] = c;
            }
        }
        else
        {
            saida[i] = c;
        }
    }
    saida[len] = '\0';
}



void exportarChaveETexto(const char *nomeArquivo)
{
    FILE *f = fopen(nomeArquivo, "w");
    if (!f)
    {
        printf("Erro: Não foi possível criar o arquivo '%s'.\n", nomeArquivo);
        return;
    }
 
   
    fprintf(f, "===== CHAVE DECRIPTOGRAFIA =====\n\n");
    fprintf(f, "Cifrado  ->  Original\n");
    fprintf(f, "--------------------------\n");

    for (int i = 0; i < 26; i++)
    {
        char original = estadoAtual.chaveDecifracao[i];
        if (original == '\0')
            fprintf(f, "%c -> (desconhecida)\n", 'A' + i);
        else
            fprintf(f, "%c ->  %c\n", 'A' + i, original);
    }

    fprintf(f, "\n\n===== TEXTO DECIFRADO =====\n\n");

    char bufferDecifrado[TAMANHO_MAX_TEXTO];
    gerarTextoDecifradoCompleto(bufferDecifrado);

    fprintf(f, "%s\n", bufferDecifrado);

    fclose(f);

    printf("Arquivo completo exportado com sucesso para '%s'.\n", nomeArquivo);
}