#include <stdio.h>
#include <string.h>
#include "petcare.h"

/* Definições reais das variáveis compartilhadas de atendimentos
 * (a struct Atendimento e o MAX_ATENDIMENTOS ficam no petcare.h) */
struct Atendimento atendimentos[MAX_ATENDIMENTOS];
int totalAtendimentos = 0;

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

// FUNÇÃO DE REGISTRAR ATENDIMENTO
void registrarAtendimento(void) {
    struct Atendimento novo;
    int id;
    int encontrado = 0;

    printf("\n=== REGISTRAR ATENDIMENTO ===\n");

    if (totalAtendimentos >= MAX_ATENDIMENTOS) {
        printf("Limite de atendimentos atingido!\n");
        return;
    }

    printf("Digite o ID do animal: ");
    if (scanf("%d", &id) != 1) {
        limparBuffer();
        printf("ID inválido!\n");
        return;
    }
    limparBuffer();

    // Verifica se o animal está cadastrado
    for (int i = 0; i < totalAnimais; i++) {
        if (animais[i].id == id) {
            encontrado = 1;
            break;
        }
    }

    if (encontrado == 0) {
        printf("Animal não encontrado!\n");
        return;
    }

    novo.idAnimal = id;

    // Data (repete até ser válida)
    while (1) {
        printf("Data do atendimento (DD/MM/AAAA): ");
        if (scanf(" %14[^\n]", novo.data) != 1) {
            limparBuffer();
            return;
        }
        limparBuffer();

        if (dataValida(novo.data)) break;
        printf("Data inválida! Use o formato DD/MM/AAAA.\n");
    }

    printf("Descrição do atendimento: ");
    if (scanf(" %149[^\n]", novo.descricao) != 1) {
        limparBuffer();
        return;
    }
    limparBuffer();

    // Só grava depois que tudo foi preenchido com sucesso
    atendimentos[totalAtendimentos] = novo;
    totalAtendimentos++;

    printf("\nAtendimento registrado com sucesso!\n");
}

// FUNÇÃO DE CONSULTAR HISTÓRICO
void consultarHistorico(void) {

    int id;
    int encontrado = 0;

    printf("\n=== HISTÓRICO DE ATENDIMENTOS ===\n");
    printf("Digite o ID do animal: ");
    if (scanf("%d", &id) != 1) {
        limparBuffer();
        printf("ID inválido!\n");
        return;
    }
    limparBuffer();

    for (int i = 0; i < totalAnimais; i++) {
        if (animais[i].id == id) {
            printf("\nAnimal: %s\n", animais[i].nome);
            encontrado = 1;
            break;
        }
    }

    if (encontrado == 0) {
        printf("Animal não encontrado!\n");
        return;
    }

    int possuiAtendimento = 0;

    for (int i = 0; i < totalAtendimentos; i++) {

        if (atendimentos[i].idAnimal == id) {

            printf("\nData: %s\n", atendimentos[i].data);
            printf("Descrição: %s\n", atendimentos[i].descricao);

            possuiAtendimento = 1;
        }
    }

    if (possuiAtendimento == 0) {
        printf("Nenhum atendimento registrado para este animal.\n");
    }
}
