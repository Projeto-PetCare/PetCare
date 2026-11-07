#include <stdio.h>
#include <string.h>
#include "petcare.h"

#define MAX_ATENDIMENTOS 100

struct Atendimento {
    int idAnimal;
    char data[15];
    char descricao[150];
};

struct Atendimento atendimentos[MAX_ATENDIMENTOS];
int totalAtendimentos = 0;

// FUNÇÃO DE REGISTRAR ATENDIMENTO
void registrarAtendimento(void) {

    if (totalAtendimentos >= MAX_ATENDIMENTOS) {
        printf("Limite de atendimentos atingido!\n");
        return;
    }

    int id;
    int encontrado = 0;

    printf("\n=== REGISTRAR ATENDIMENTO ===\n");
    printf("Digite o ID do animal: ");
    scanf("%d", &id);

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

    atendimentos[totalAtendimentos].idAnimal = id;

    printf("Data do atendimento (DD/MM/AAAA): ");
    scanf(" %14[^\n]", atendimentos[totalAtendimentos].data);

    printf("Descrição do atendimento: ");
    scanf(" %149[^\n]", atendimentos[totalAtendimentos].descricao);

    totalAtendimentos++;

    printf("\nAtendimento registrado com sucesso!\n");
}

// FUNÇÃO DE CONSULTAR HISTÓRICO
void consultarHistorico(void) {

    int id;
    int encontrado = 0;

    printf("\n=== HISTÓRICO DE ATENDIMENTOS ===\n");
    printf("Digite o ID do animal: ");
    scanf("%d", &id);

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