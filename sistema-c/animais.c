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
    novoAnimal.totalContatos = 0;
    novoAnimal.id = proximoId++;
    printf("\n=== CADASTRO DE ANIMAL ===\n");
    printf("ID do animal: %03d\n", novoAnimal.id);
    printf("Nome do animal: ");
    getchar();
    fgets(novoAnimal.nome, sizeof(novoAnimal.nome), stdin);

    novoAnimal.nome[strcspn(novoAnimal.nome, "\n")] = '\0';
    
    if (strlen(novoAnimal.nome) == 0) {
        printf("O nome do animal não pode ficar vazio!\n");
        return;
    }

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
    }
    else if (opcaoEspecie == 2) {
        strcpy(novoAnimal.especie, "Gato");
    }
    else if (opcaoEspecie == 3) {
        strcpy(novoAnimal.especie, "Ave");
    }
    else if (opcaoEspecie == 4) {
        strcpy(novoAnimal.especie, "Réptil");
    }
    else if (opcaoEspecie == 5) {
        strcpy(novoAnimal.especie, "Roedor");
    }
    else if (opcaoEspecie == 6) {
        strcpy(novoAnimal.especie,"Outra espécie");
    }
    else {
        printf("Opção inválida!\n");
        return;
    }
    printf("Espécie cadastrada: %s\n", novoAnimal.especie);

    printf("Raça do animal: (ou Enter para não informar): ");
    getchar();
    fgets(novoAnimal.raca, sizeof(novoAnimal.raca), stdin);

    novoAnimal.raca[strcspn(novoAnimal.raca, "\n")] = '\0';
    if (strlen(novoAnimal.raca) == 0) {
        strcpy(novoAnimal.raca, "Não informada");
    }
    printf("\nSexo do animal:\n");
    printf("1 - Macho\n");
    printf("2 - Fêmea\n");

    int caractere;

    while (1) {
        printf("Digite uma opção: ");

        if (scanf("%d", &novoAnimal.sexo) != 1) {
            printf("Entrada inválida! Digite apenas 1 ou 2. \n");

            while ((caractere = getchar()) != '\n' && caractere != EOF) {

            }
            
            continue;
        }
        if (novoAnimal.sexo == 1 || novoAnimal.sexo == 2) {
            break;
        }
        printf("Opção inválida! Escolha 1 ou 2. \n");

        while ((caractere = getchar()) != '\n' && caractere != EOF) {
            
        }
    }
    
    printf("\nIdade do animal:\n");

    printf("Anos: ");
    scanf("%d", &novoAnimal.idadeAnos);

    printf("Meses: ");
    scanf("%d", &novoAnimal.idadeMeses);

    if (novoAnimal.idadeAnos < 0 || novoAnimal.idadeMeses < 0 || novoAnimal.idadeMeses > 11) {
        printf("Idade inválida!\n");
        return;
    }
    printf("\n=== DADOS DO RESPONSÁVEL ===\n");
    printf("Nome do responsável: ");
    scanf(" %99[^\n]", novoAnimal.responsavel.nome);
    printf("Telefone do responsável: ");
    scanf(" %19[^\n]", novoAnimal.responsavel.telefone);


    printf("\n=== ENDEREÇO DO RESPONSÁVEL ===\n");

    printf("Rua: ");
    scanf(" %99[^\n]", novoAnimal.responsavel.rua);

    printf("Número: ");
    scanf(" %9[^\n]", novoAnimal.responsavel.numero);

    printf("Complemento: ");
    scanf(" %49[^\n]", novoAnimal.responsavel.complemento);

    printf("Bairro: ");
    scanf(" %49[^\n]", novoAnimal.responsavel.bairro);

    printf("Cidade: ");
    scanf(" %49[^\n]", novoAnimal.responsavel.cidade);

    int opcaoContato;
    printf("\nDeseja adicionar contatos adicionais?\n");
    printf("1 - Sim\n");
    printf("2 - Não\n");
    printf("Digite uma opção: ");
    scanf("%d", &opcaoContato);
    if (opcaoContato == 1) {
        printf("\n=== CONTATO ADICIONAL ===\n");
        printf("Nome: ");
        scanf(" %99[^\n]", novoAnimal.contatos[0].nome);

        printf("Telefone: ");
        scanf(" %19[^\n]", novoAnimal.contatos[0].telefone);

        novoAnimal.totalContatos++;

        int segundoContato;

        printf("\nDeseja adicionar um segundo contato?\n");
        printf("1 - Sim\n");
        printf("2 - Não\n");
        printf("Digite uma opção: ");
        scanf("%d", &segundoContato);

        if (segundoContato == 1) {
            printf("\n=== SEGUNDO CONTATO ===\n");
            printf("Nome: ");
            scanf(" %99[^\n]", novoAnimal.contatos[1].nome);

            printf("Telefone: ");
            scanf(" %19[^\n]", novoAnimal.contatos[1].telefone);

            novoAnimal.totalContatos++;
        }

    }
    else if (opcaoContato != 2) {
        printf("Opção inválida!\n");
        return;
    }

    animais[totalAnimais] = novoAnimal;
    totalAnimais++;
}
void listarAnimais(void){
    if (totalAnimais == 0){
        printf("Nenhum animal cadastrado!\n");
        return;
    }
    printf("\n=== ANIMAIS CADASTRADOS ===\n");
    for (int i = 0; i < totalAnimais; i++) {
        printf("\nID: %03d\n", animais[i].id);
        printf("Nome: %s\n", animais[i].nome);

        printf("Responsável: %s\n", animais[i].responsavel.nome);
        printf("Telefone: %s\n", animais[i].responsavel.telefone);
        printf("Rua: %s\n", animais[i].responsavel.rua);
        printf("Número: %s\n", animais[i].responsavel.numero);
        printf("Complemento: %s\n", animais[i].responsavel.complemento);
        printf("Bairro: %s\n", animais[i].responsavel.bairro);
        printf("Cidade: %s\n", animais[i].responsavel.cidade);

        for (int j = 0; j < animais[i].totalContatos; j++) {
            printf("\nContato adicional %d:\n", j + 1);
            printf("Nome: %s\n", animais[i].contatos[j].nome);
            printf("Telefone: %s\n", animais[i].contatos[j].telefone);
        }
        printf("Espécie: %s\n", animais[i].especie);
        printf("Raça: %s\n", animais[i].raca);
        if (animais[i].sexo == 1) {
            printf("Sexo: Macho\n");
        }
        else {
            printf("Sexo: Fêmea\n");
        }
        printf("Idade: %d ano(s) e %d mes(es)\n", animais[i].idadeAnos, animais[i].idadeMeses);
    }
}