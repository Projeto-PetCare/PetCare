#include <stdio.h>
#include <string.h>
#include "petcare.h"

/* Definições reais das variáveis compartilhadas de serviços */
struct Servico servicos[MAX_SERVICOS];
int totalServicos = 0;
int proximoIdServico = 1;

/* Descarta o que sobrou na linha digitada (evita problemas entre scanf) */
static void limparBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
    }
}

/* Confere se a data está no formato DD/MM/AAAA com valores possíveis */
static int dataValida(const char *data) {
    if (strlen(data) != 10) return 0;

    for (int i = 0; i < 10; i++) {
        if (i == 2 || i == 5) {
            if (data[i] != '/') return 0;
        } else if (data[i] < '0' || data[i] > '9') {
            return 0;
        }
    }

    int dia = (data[0] - '0') * 10 + (data[1] - '0');
    int mes = (data[3] - '0') * 10 + (data[4] - '0');

    if (mes < 1 || mes > 12) return 0;
    if (dia < 1 || dia > 31) return 0;
    return 1;
}

/*
 * REGISTRAR SERVIÇO
 * Cada registro representa um serviço EFETIVAMENTE REALIZADO para um animal,
 * com valor cobrado. Esses registros alimentam o faturamento (opção 8).
 */
void registrarServico(void) {
    struct Servico novo;
    int id;
    int indice = -1;
    int r;
    int c;
    float valor;

    printf("\n=== REGISTRAR SERVIÇO ===\n");

    if (totalServicos >= MAX_SERVICOS) {
        printf("Limite de serviços atingido!\n");
        return;
    }

    /* 1) Animal que recebeu o serviço */
    printf("Digite o ID do animal: ");
    if (scanf("%d", &id) != 1) {
        limparBuffer();
        printf("ID inválido!\n");
        return;
    }
    limparBuffer();

    for (int i = 0; i < totalAnimais; i++) {
        if (animais[i].id == id) {
            indice = i;
            break;
        }
    }

    if (indice == -1) {
        printf("Animal não encontrado!\n");
        return;
    }

    printf("Animal: %s\n", animais[indice].nome);
    novo.idAnimal = id;

    /* 2) Data (repete até ser válida) */
    while (1) {
        printf("Data do serviço (DD/MM/AAAA): ");
        if (scanf(" %14[^\n]", novo.data) != 1) {
            limparBuffer();
            return;
        }
        limparBuffer();

        if (dataValida(novo.data)) break;
        printf("Data inválida! Use o formato DD/MM/AAAA.\n");
    }

    /* 3) Descrição */
    printf("Descrição do serviço: ");
    if (scanf(" %99[^\n]", novo.descricao) != 1) {
        limparBuffer();
        return;
    }
    limparBuffer();

    /* 4) Valor (repete até ser um número maior que zero, com ponto decimal) */
    while (1) {
        printf("Valor do serviço (R$, use ponto: 80.50): ");
        r = scanf("%f", &valor);
        if (r == EOF) return;

        c = getchar(); /* caractere logo após o número */
        if (r == 1 && valor > 0 && (c == '\n' || c == EOF)) break;

        if (c != '\n' && c != EOF) limparBuffer();
        printf("Valor inválido! Digite um número maior que zero.\n");
    }
    novo.valor = valor;

    /* 5) Guarda o registro com ID único */
    novo.id = proximoIdServico;
    servicos[totalServicos] = novo;
    totalServicos++;
    proximoIdServico++;

    printf("\nServiço registrado com sucesso! (ID %d | R$ %.2f)\n", novo.id, novo.valor);
}
