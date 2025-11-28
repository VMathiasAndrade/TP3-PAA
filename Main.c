#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "src/Operacoes.h"

EstadoCripto estadoAtual;

void exibirMenu(void);
void lidarComOpcaoDoMenu(int opcao);

int main(int argc, char *argv[]) {

    inicializarEstadoCripto();

    printf("--- Trabalho Pratico 3 - Criptoanalise ---\n");

    char nomeArquivoClaro[64];
    char nomeArquivoCifrado[66];
        
    printf("Informe o nome do arquivo de texto claro a ser lido (ex: texto_claro/Anaxa.txt): ");
    scanf("%63s", nomeArquivoClaro);
    
    printf("Informe o nome do arquivo para escrever o texto criptografado (ex: texto_cripto/Anaxa.txt): ");
    scanf("%65s", nomeArquivoCifrado);


    if (!carregarEEncriptarTexto(nomeArquivoClaro, nomeArquivoCifrado))
    {
        return 1;
    }

    int escolha;
    do
    {
        exibirMenu();
        if (scanf("%d", &escolha) != 1)
        {
            while (getchar() != '\n');
            escolha = 0;
        }
        lidarComOpcaoDoMenu(escolha);
    } while (escolha != 6);

    return 0;
}

void exibirMenu(void)
{
    printf("\n--- Opcoes ---\n");
    printf("1. Apresentar o estado atual da criptoanálise\n");
    printf("2. Fazer um chute baseado análise de frequência\n");
    printf("3. Realizar casamento exato de caracteres\n");
    printf("4. Realizar casamento aproximado de caracteres\n");
    printf("5. Alterar chave de criptografia (Ex: A->S)\n");
    printf("6. Exportar resultado e encerrar o programa\n");
    printf("Escolha uma opção: ");
}

void lidarComOpcaoDoMenu(int opcao)
{
    char nomeArquivoSaida[64];

    switch (opcao)
    {
    case 1:
        exibirEstadoCripto();
        break;
    case 2:
        frequenciaCaracter();
        break;
    case 3:
    {
        char padrao[64];

        printf("Qual o padrao utilizado: ");
        scanf("%63s", padrao);
        
        casamentoExato(padrao);
    }
    break;
    case 4:
    {
        char padrao1[64];
        int tolerancia;
        
        printf("Digite um padrao: ");
        scanf("%s", padrao1);
        
        printf("Digite a tolerancia de erros: ");
        scanf("%d", &tolerancia);
        for(int i=0; padrao1[i]; i++) 
            padrao1[i] = toupper(padrao1[i]);

        casamentoAproximado(padrao1, tolerancia);
    }
    break;
    case 5:
    {
        char letraCif, letraDecif;

        printf("digite a letra a substituir: ");
        scanf(" %c", &letraCif);
        
        printf("digite a letra decifrada: ");
        scanf(" %c", &letraDecif);
        
        alterarChave(letraCif, letraDecif);
    }
    break;

    case 6:
        printf("Informe o nome do arquivo para salvar a chave (ex: chave_final.txt): ");
        scanf("%63s", nomeArquivoSaida);
        exportarChaveETexto(nomeArquivoSaida);
        printf("Encerrando o programa...\n");
        break;
    default:
        printf("Opcao invalida. Tente novamente.\n");
    }
}