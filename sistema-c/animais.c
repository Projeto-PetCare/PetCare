#include <stdio.h>
#include <string.h>
#include "petcare.h"
#include <ctype.h>

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
    novoAnimal.id = proximoId;
    printf("\n=== CADASTRO DE ANIMAL ===\n");
    printf("ID do animal: %03d\n", novoAnimal.id);
    printf("Nome do animal: ");
    getchar();
    fgets(novoAnimal.nome, sizeof(novoAnimal.nome), stdin);

    novoAnimal.nome[strcspn(novoAnimal.nome, "\n")] = '\0';

    if (strlen(novoAnimal.nome) == 0 ||
    strspn(novoAnimal.nome, " \t") == strlen(novoAnimal.nome)) {
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
    int opcaoEspecie;
    int caractere;
    while (1) {
        printf("Digite uma opção: ");

        if (scanf("%d", &opcaoEspecie) != 1) {
            printf("Entrada inválida! Digite apenas um número de 1 a 6. \n");

            while ((caractere = getchar()) != '\n' && caractere != EOF) {

            }
            continue;
        }
        if (opcaoEspecie >= 1 && opcaoEspecie <= 6) {
            break;
        }
        printf("Opção inválida! Escolha uma opção de 1 a 6.\n");

        while ((caractere = getchar()) != '\n' && caractere != EOF) {

        }
    }
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
        strcpy(novoAnimal.especie, "Outra espécie");
    }
    printf("Espécie cadastrada: %s\n", novoAnimal.especie);

    printf("Raça do animal: (ou Enter para não informar): ");
    getchar();
    fgets(novoAnimal.raca, sizeof(novoAnimal.raca), stdin);

    novoAnimal.raca[strcspn(novoAnimal.raca, "\n")] = '\0';
    if (strlen(novoAnimal.raca) == 0 ||
    strspn(novoAnimal.raca, " \t") == strlen(novoAnimal.raca)) {
    strcpy(novoAnimal.raca, "Não informada");
    }
    printf("\nSexo do animal:\n");
    printf("1 - Macho\n");
    printf("2 - Fêmea\n");

    
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
    while (1) {
        if (scanf("%d", &novoAnimal.idadeAnos) != 1) {
            printf("Idade inválida! digite apenas números.\n");

            while ((caractere = getchar()) != '\n' && caractere != EOF) {
            }
            continue;
        }
        if (novoAnimal.idadeAnos >= 0) {
            break;
        }
        printf("A idade não pode ser negativa!\n");
        
    }
    printf("Meses: ");

    while (1) {
        if (scanf("%d", &novoAnimal.idadeMeses) != 1) {
            printf("Entrada inválida! Digite apenas números.\n");

            while ((caractere = getchar()) != '\n' && caractere != EOF) {  
            }
            continue;
        }
        if (novoAnimal.idadeMeses >= 0 && novoAnimal.idadeMeses <= 11) {
            break;
        }
        printf("Digite uma quantidade de meses entre 0 e 11. \n");
    }

    if (novoAnimal.idadeAnos < 0 || novoAnimal.idadeMeses < 0 || novoAnimal.idadeMeses > 11) {
        printf("Idade inválida!\n");
        return;
    }
    printf("\n=== DADOS DO RESPONSÁVEL ===\n");
    printf("Nome do responsável: ");
    getchar();
    fgets(novoAnimal.responsavel.nome, sizeof(novoAnimal.responsavel.nome), stdin);

    novoAnimal.responsavel.nome[strcspn(novoAnimal.responsavel.nome, "\n")] = '\0';

    if (strlen(novoAnimal.responsavel.nome) == 0 ||
    strspn(novoAnimal.responsavel.nome, " \t") == strlen(novoAnimal.responsavel.nome)) {
    printf("O nome do responsável não pode ficar vazio!\n");
    return;
}


    printf("Telefone do responsável: ");
    fgets(novoAnimal.responsavel.telefone, sizeof(novoAnimal.responsavel.telefone), stdin);

    novoAnimal.responsavel.telefone[strcspn(novoAnimal.responsavel.telefone, "\n")] = '\0';
    if (strlen(novoAnimal.responsavel.telefone) == 0) {
        printf("O telefone do responsável não pode ficar vazio!\n");
        return;
    }
    int telefoneValido = 1;
    int quantidadeDigitos = 0;

    for (int i = 0; novoAnimal.responsavel.telefone[i] != '\0'; i++) {
        char c = novoAnimal.responsavel.telefone[i];
        if (isdigit(c)) {
            quantidadeDigitos++;
        }

        if (!isdigit(c) && c != '(' && c != ')' && c != '-' && c != ' ') {
            telefoneValido = 0;
            break;
        }
    }

    if (telefoneValido == 0) {
        printf("Telefone inválido! Digite apenas números e caracteres de formatação.\n");
        return;
    }
    if (quantidadeDigitos != 10 && quantidadeDigitos != 11) {
        printf("telefone inválido! Informe o DDD e um número com 10 ou 11 digitos.\n");
        return;
    }


    //ENDEREÇO RESPONSÁVEL
    printf("\n=== ENDEREÇO DO RESPONSÁVEL ===\n");

    // RUA
    while (1) {
        printf("Rua: ");
        fgets(novoAnimal.responsavel.rua, sizeof(novoAnimal.responsavel.rua), stdin);
        novoAnimal.responsavel.rua[strcspn(novoAnimal.responsavel.rua, "\n")] = '\0';
         if (strlen(novoAnimal.responsavel.rua) > 0 &&
            strspn(novoAnimal.responsavel.rua, " \t") !=
            strlen(novoAnimal.responsavel.rua)) {
                break;
            }
        printf("A rua não pode ficar vazia!\n");
    }

    // NÚMERO - OBRIGATÓRIO
      while (1) {
        printf("Número: ");
        fgets(novoAnimal.responsavel.numero, sizeof(novoAnimal.responsavel.numero), stdin);
        novoAnimal.responsavel.numero[strcspn(novoAnimal.responsavel.numero, "\n")] = '\0';
        if (strlen(novoAnimal.responsavel.numero) > 0 &&
            strspn(novoAnimal.responsavel.numero, " \t") !=
            strlen(novoAnimal.responsavel.numero)) {
                break;
            }
        printf("O número não pode ficar vazio!\n");
      }
      // COMPLEMENTO - OPCIONAL
       printf("Complemento: ");
       fgets(novoAnimal.responsavel.complemento, sizeof(novoAnimal.responsavel.complemento), stdin);
        novoAnimal.responsavel.complemento[strcspn(novoAnimal.responsavel.complemento, "\n")] = '\0';
        if (strlen(novoAnimal.responsavel.complemento) == 0 ||
            strspn(novoAnimal.responsavel.complemento, " \t") == strlen(novoAnimal.responsavel.complemento)) {
            strcpy(novoAnimal.responsavel.complemento, "Não informado");
        }
        // BAIRRO - OBRIGATÓRIO
        while (1) {
            printf("Bairro: ");
            fgets(novoAnimal.responsavel.bairro, sizeof(novoAnimal.responsavel.bairro), stdin);
            novoAnimal.responsavel.bairro[strcspn(novoAnimal.responsavel.bairro, "\n")] = '\0';
            if (strlen(novoAnimal.responsavel.bairro) > 0 &&
            strspn(novoAnimal.responsavel.bairro, " \t") !=
            strlen(novoAnimal.responsavel.bairro))  {
                break;
            }
             printf("O bairro não pode ficar vazio!\n");
        }
        // CIDADE - OBRIGATÓRIO
        while (1) {
            printf("Cidade: ");
            fgets(novoAnimal.responsavel.cidade, sizeof(novoAnimal.responsavel.cidade), stdin);
            novoAnimal.responsavel.cidade[strcspn(novoAnimal.responsavel.cidade, "\n")] = '\0';
             if (strlen(novoAnimal.responsavel.cidade) > 0 &&
            strspn(novoAnimal.responsavel.cidade, " \t") !=
            strlen(novoAnimal.responsavel.cidade)) {
                break;
            }
            printf("A cidade não pode ficar vazia!\n");
        }
    int opcaoContato;

    while (1) {
        printf("\nDeseja adicionar contatos adicionais?\n");
        printf("1 - Sim\n");
        printf("2 - Não\n");
        printf("Digite uma opção: ");

    if (scanf("%d", &opcaoContato) != 1) {
        printf("Entrada inválida! Digite apenas 1 ou 2.\n");
        while ((caractere = getchar()) != '\n' && caractere != EOF) {
        }
        continue;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   
    }

    if (opcaoContato == 1 || opcaoContato == 2) {
        break;

    }
    
    printf("Opção inválida! Escolha 1 ou 2.\n");

    }

    // PRIMEIRO CONTATO ADICIONAL
    if (opcaoContato == 1) {
        printf("\n=== CONTATO ADICIONAL ===\n");
        printf("Nome: ");
        getchar();
        fgets(novoAnimal.contatos[0].nome, sizeof(novoAnimal.contatos[0].nome), stdin);
        novoAnimal.contatos[0].nome[strcspn(novoAnimal.contatos[0].nome, "\n")] = '\0';

        if (strlen(novoAnimal.contatos[0].nome) == 0 ||
            strspn(novoAnimal.contatos[0].nome, " \t") == strlen(novoAnimal.contatos[0].nome)) {
            printf("O nome do contato não pode ficar vazio!\n");
            return;
        }

        // TELEFONE 1 CONTATO
        printf("Telefone: ");
        fgets(novoAnimal.contatos[0].telefone, sizeof(novoAnimal.contatos[0].telefone), stdin);
        novoAnimal.contatos[0].telefone[strcspn(novoAnimal.contatos[0].telefone, "\n")] = '\0';

        if (strlen(novoAnimal.contatos[0].telefone) == 0) {
            printf("O telefone do contato não pode ficar vazio!\n");
            return;
        }

        int telefoneContatoValido = 1;
        int digitosContato = 0;

        for (int i = 0; novoAnimal.contatos[0].telefone[i] != '\0'; i++) {
            char c = novoAnimal.contatos[0].telefone[i];

            if (isdigit(c)) {
                digitosContato++;
            }

            if (!isdigit(c) && c != '(' && c != ')' && c != '-' && c != ' ') {
                telefoneContatoValido = 0;
                break;
            }
        }
        if (telefoneContatoValido == 0) {
            printf("Telefone inválido! Digite apenas números e caracteres de formatação.\n");
            return;
        }
        if (digitosContato != 10 && digitosContato != 11) {
             printf("Telefone inválido! Informe o DDD e um número com 10 ou 11 dígitos.\n");
             return;
        }
        novoAnimal.totalContatos++;


        // SEGUNDO CONTATO ADICIONAL
        int segundoContato;

        while (1) {
            printf("\nDeseja adicionar um segundo contato?\n");
            printf("1 - Sim\n");
            printf("2 - Não\n");
            printf("Digite uma opção: ");

        if  (scanf("%d", &segundoContato) != 1) {
            printf("Entrada inválida! Digite apenas 1 ou 2.\n");

            while ((caractere = getchar()) != '\n' && caractere != EOF) {
            }

            continue;

        }

        if (segundoContato == 1 || segundoContato == 2) {
            break;

        }

        printf("Opção inválida! Escolha 1 ou 2.\n");
    }
        if (segundoContato == 1) {
            printf("\n=== SEGUNDO CONTATO ===\n");
            printf("Nome: ");
            getchar();
            fgets(novoAnimal.contatos[1].nome, sizeof(novoAnimal.contatos[1].nome), stdin);
            novoAnimal.contatos[1].nome[strcspn(novoAnimal.contatos[1].nome, "\n")] = '\0';
            if (strlen(novoAnimal.contatos[1].nome) == 0 ||
                strspn(novoAnimal.contatos[1].nome, " \t") == strlen(novoAnimal.contatos[1].nome)) {
                printf("O nome do contato não pode ficar vazio!\n");
                return;
            }
            // TELEFONE 2 CONTATO
            printf("Telefone: ");
            fgets(novoAnimal.contatos[1].telefone, sizeof(novoAnimal.contatos[1].telefone), stdin);
            novoAnimal.contatos[1].telefone[strcspn(novoAnimal.contatos[1].telefone, "\n")] = '\0';
            if (strlen(novoAnimal.contatos[1].telefone) == 0) {
                printf("O telefone do contato não pode ficar vazio!\n");
                return;
            }
            int telefoneSegundoValido = 1;
            int digitosSegundoContato = 0;
            for (int i = 0; novoAnimal.contatos[1].telefone[i] != '\0'; i++) {
                 char c = novoAnimal.contatos[1].telefone[i];

                 if (isdigit(c)) {
                    digitosSegundoContato++;
                 }
                   if (!isdigit(c) && c != '(' && c != ')' && c != '-' && c != ' ') {
                    telefoneSegundoValido = 0;
                    break;
                   }
            }
            if (telefoneSegundoValido == 0) {
                printf("Telefone inválido! Digite apenas números e caracteres de formatação.\n");
                return;
            }
            if (digitosSegundoContato != 10 && digitosSegundoContato != 11) {
                printf("Telefone inválido! Informe o DDD e um número com 10 ou 11 dígitos.\n");
                return;
            }
            novoAnimal.totalContatos++;
        }
    }
    animais[totalAnimais] = novoAnimal;
    totalAnimais++;
    proximoId++;
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
        printf("Idade: %d %s e %d %s\n",
            animais[i].idadeAnos,
            animais[i].idadeAnos == 1 ? "ano" : "anos",
            animais[i].idadeMeses,
            animais[i].idadeMeses == 1 ? "mês" : "meses");
    }
}