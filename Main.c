#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "EstadoCripto.h"

EstadoCripto estadoAtual;

void exibirMenu(void);
void lidarComOpcaoDoMenu(int opcao);

int main(int argc, char *argv[]) {

    inicializarEstadoCripto();

    printf("--- Trabalho Prático 3 - Criptoanálise ---\n");

    char nomeArquivoClaro[64];
    char nomeArquivoCifrado[66];
        
    printf("Informe o nome do arquivo de texto claro a ser lido (ex: Teste.txt): ");
    scanf("%63s", nomeArquivoClaro);

    printf("Informe o nome do arquivo para escrever o texto criptografado: ");
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
    printf("\n--- Opções ---\n");
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
        printf("\nOpção 1 selecionada. Texto Cifrado (Início): %.100s...\n", estadoAtual.textoCifrado);
        break;
    case 2:
        printf("Opção 2 selecionada. (Análise de Frequência - Não implementada)\n");
        break;
    case 3:
        printf("Opção 3 selecionada. (Casamento Exato)\n");
        
        char padrao[64];

        printf("Qual o padrao utilizado: ");
        scanf("%63s", padrao);
        
        casamentoExato(padrao);
        break;
    case 4:
        printf("Opção 4 selecionada. (Casamento Aproximado - Não implementado)\n\n");
        break;
    case 5:
        printf("Opção 5 selecionada. (Alterar Chave - Não implementada)\n");
        break;
    case 6:
        printf("Informe o nome do arquivo para salvar a chave (ex: chave_final.txt): ");
        scanf("%63s", nomeArquivoSaida);
        exportarChave(nomeArquivoSaida);
        printf("Encerrando o programa...\n");
        break;
    default:
        printf("Opção inválida. Tente novamente.\n");
    }
}