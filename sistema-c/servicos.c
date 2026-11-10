#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include "petcare.h"

/* Definições reais das variáveis compartilhadas de serviços */
struct Servico servicos[MAX_SERVICOS];
int totalServicos = 0;
int proximoIdServico = 1;

#define VALOR_MAXIMO 1000000.0f

/* Descarta o que sobrou na linha digitada */
static void limparBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
    }
}

/* Lê uma linha inteira com segurança. Retorna 0 se a entrada acabou (EOF). */
static int lerLinha(const char *msg, char *dest, int tam) {
    printf("%s", msg);
    if (fgets(dest, tam, stdin) == NULL) return 0;

    size_t len = strlen(dest);
    if (len > 0 && dest[len - 1] == '\n') {
        dest[len - 1] = '\0';
    } else {
        limparBuffer(); /* linha maior que o buffer: descarta o excesso */
    }
    return 1;
}

/* Lê um inteiro. Retorna 1 se for um número válido (sem letras sobrando). */
static int lerInteiro(const char *msg, int *valor) {
    char buf[32];
    char *fim;

    if (!lerLinha(msg, buf, sizeof buf)) return 0;

    long n = strtol(buf, &fim, 10);
    if (fim == buf) return 0;                       /* nenhum dígito */
    while (*fim == ' ' || *fim == '\t') fim++;      /* tolera espaços no fim */
    if (*fim != '\0') return 0;                     /* sobrou lixo (ex: 12abc) */
    if (n < INT_MIN || n > INT_MAX) return 0;       /* número gigante */

    *valor = (int) n;
    return 1;
}

/* Converte texto em valor monetário. Aceita "80.50" e "80,50". */
static int converterValor(char *texto, float *valor) {
    char *fim;

    for (int i = 0; texto[i] != '\0'; i++) {
        if (texto[i] == ',') texto[i] = '.';
    }

    float v = strtof(texto, &fim);
    if (fim == texto) return 0;                     /* nenhum número */
    while (*fim == ' ' || *fim == '\t') fim++;
    if (*fim != '\0') return 0;                     /* sobrou lixo (ex: 80abc) */

    /* Rejeita zero, negativo, absurdo, e também "inf" e "nan" */
    if (!(v > 0 && v <= VALOR_MAXIMO)) return 0;

    *valor = v;
    return 1;
}

/* Retorna 1 se o texto for vazio ou só tiver espaços */
static int textoVazio(const char *s) {
    for (int i = 0; s[i] != '\0'; i++) {
        if (s[i] != ' ' && s[i] != '\t') return 0;
    }
    return 1;
}

static int anoBissexto(int ano) {
    return (ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0);
}

/* Confere se a data está no formato DD/MM/AAAA e se ela realmente existe */
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
    int ano = (data[6] - '0') * 1000 + (data[7] - '0') * 100
            + (data[8] - '0') * 10   + (data[9] - '0');

    if (ano < 1900 || ano > 2100) return 0;
    if (mes < 1 || mes > 12) return 0;

    int diasNoMes[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (mes == 2 && anoBissexto(ano)) diasNoMes[1] = 29;

    return dia >= 1 && dia <= diasNoMes[mes - 1];
}

/* Retorna o índice do animal com esse ID, ou -1 se não existir */
static int buscarAnimal(int id) {
    for (int i = 0; i < totalAnimais; i++) {
        if (animais[i].id == id) return i;
    }
    return -1;
}

/*
 * REGISTRAR SERVIÇO
 * Cada registro representa um serviço EFETIVAMENTE REALIZADO para um animal,
 * com valor cobrado. Esses registros alimentam o faturamento (opção 8).
 */
void registrarServico(void) {
    struct Servico novo;
    int id;
    int indice;
    float valor;
    char buf[32];

    printf("\n=== REGISTRAR SERVIÇO ===\n");

    if (totalServicos >= MAX_SERVICOS) {
        printf("Limite de serviços atingido!\n");
        return;
    }

    /* 1) Animal que recebeu o serviço */
    if (!lerInteiro("Digite o ID do animal: ", &id) || id <= 0) {
        printf("ID inválido!\n");
        return;
    }

    indice = buscarAnimal(id);
    if (indice == -1) {
        printf("Animal não encontrado!\n");
        return;
    }

    printf("Animal: %s\n", animais[indice].nome);
    novo.idAnimal = id;

    /* 2) Data (repete até ser válida) */
    while (1) {
        if (!lerLinha("Data do serviço (DD/MM/AAAA): ",
                      novo.data, sizeof novo.data)) {
            return;
        }

        if (dataValida(novo.data)) break;
        printf("Data inválida! Use o formato DD/MM/AAAA com uma data que exista.\n");
    }

    /* 3) Descrição (repete até não ser vazia) */
    while (1) {
        if (!lerLinha("Descrição do serviço: ",
                      novo.descricao, sizeof novo.descricao)) {
            return;
        }

        if (!textoVazio(novo.descricao)) break;
        printf("A descrição não pode ficar vazia!\n");
    }

    /* 4) Valor (repete até ser um número maior que zero) */
    while (1) {
        if (!lerLinha("Valor do serviço (R$, ex: 80.50): ", buf, sizeof buf)) {
            return;
        }

        if (converterValor(buf, &valor)) break;
        printf("Valor inválido! Digite um número maior que zero (máx. R$ 1000000).\n");
    }
    novo.valor = valor;

    /* 5) Guarda o registro com ID único */
    novo.id = proximoIdServico;
    servicos[totalServicos] = novo;
    totalServicos++;
    proximoIdServico++;

    salvarServicos();

    printf("\nServiço registrado com sucesso! (ID %d | R$ %.2f)\n", novo.id, novo.valor);
}