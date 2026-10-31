#include <stdio.h>
#include <string.h>
#include "petcare.h"

struct Animal animais[MAX_ANIMAIS];
int totalAnimais = 0;
int proximoId = 1;

// FUNÇÃO DE CADASTRAR
void cadastrarAnimal(){
    if (totalAnimais >= MAX_ANIMAIS) {
        printf("Limite de animais atingido!\n");
        return;
    }
    struct Animal novoAnimal = {0};
    novoAnimal.id = proximoId++;
    printf("\n=== CADASTRO DE ANIMAL ===\n");
    printf("ID do animal: %03d\n", novoAnimal.id);
    printf("Nome do animal: ");
    scanf(" %49[^\n]", novoAnimal.nome);
    printf("\nEscolha a espécie:\n");
    printf("1 - Cachorro\n");
    printf("2 - Gato\n");
    printf("3 - Ave\n");
    printf("4 - Réptil\n");
    printf("5 - Roedor\n");
    printf("6 - Outra espécie\n");
    printf("Digite uma opção: ");
    int opcaoEspecie;
    scanf("%d", &opcaoEspecie);
    if (opcaoEspecie == 1) {
        strcpy(novoAnimal.especie, "Cachorro");
        printf("Espécie escolhida: Cachorro\n");
    }
    else if (opcaoEspecie == 2) {
        printf("Espécie escolhida: Gato\n");
    }
    else if (opcaoEspecie == 3) {
        printf("Espécie escolhida: Ave\n");
    }
    else if (opcaoEspecie == 4) {
        printf("Espécie escolhida: Réptil\n");
    }
    else if (opcaoEspecie == 5) {
        printf("Espécie escolhida: Roedor\n");
    }
    else if (opcaoEspecie == 6) {
        printf("Espécie escolhida: Outra espécie\n");
    }
    else {
        printf("Opção inválida!\n");
    }
}