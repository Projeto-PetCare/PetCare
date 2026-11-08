#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>
#include "petcare.h"
 
struct Animal animais[MAX_ANIMAIS];
int totalAnimais = 0;
int proximoId = 1;
 
/* ------------------------------------------------------------------
 * FUNÇÕES AUXILIARES DE LEITURA
 * Tudo é lido linha a linha com fgets, então o buffer do teclado
 * nunca fica com sobras (nada de getchar() solto antes do fgets).
 * Todas retornam 0 se a entrada acabar (EOF), evitando loop infinito.
 * ------------------------------------------------------------------ */
 
/* Lê uma linha, remove o '\n' e descarta o que passar do tamanho */
static int lerLinha(char *dest, size_t tam) {
    if (fgets(dest, (int)tam, stdin) == NULL) return 0;
 
    size_t len = strcspn(dest, "\n");
    if (dest[len] == '\n') {
        dest[len] = '\0';
    } else {
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {
        }
    }
    return 1;
}
 
/* 1 se o texto for vazio ou só tiver espaços/tabs */
static int textoVazio(const char *s) {
    return s[strspn(s, " \t")] == '\0';
}
 
/* Pede um inteiro entre min e max; repete até ser válido */
static int lerInteiroFaixa(const char *prompt, int min, int max,
                           const char *erro, int *saida) {
    char linha[32];
    char *fim;
 
    while (1) {
        printf("%s", prompt);
        if (!lerLinha(linha, sizeof(linha))) return 0;
 
        long v = strtol(linha, &fim, 10);
        if (fim != linha && textoVazio(fim) && v >= min && v <= max) {
            *saida = (int)v;
            return 1;
        }
        printf("%s\n", erro);
    }
}
 
/* Campo de texto obrigatório: repete até não ficar vazio */
static int lerObrigatorio(const char *prompt, const char *erro,
                          char *dest, size_t tam) {
    while (1) {
        printf("%s", prompt);
        if (!lerLinha(dest, tam)) return 0;
        if (!textoVazio(dest)) return 1;
        printf("%s\n", erro);
    }
}
 
/* Campo de texto opcional: usa o valor padrão se ficar vazio */
static int lerOpcional(const char *prompt, const char *padrao,
                       char *dest, size_t tam) {
    printf("%s", prompt);
    if (!lerLinha(dest, tam)) return 0;
    if (textoVazio(dest)) {
        snprintf(dest, tam, "%s", padrao);
    }
    return 1;
}
 
/* Aceita dígitos e ( ) - espaço; exige 10 ou 11 dígitos (DDD + número) */
static int telefoneValido(const char *tel) {
    int digitos = 0;
 
    for (int i = 0; tel[i] != '\0'; i++) {
        unsigned char c = (unsigned char)tel[i];
        if (isdigit(c)) {
            digitos++;
        } else if (c != '(' && c != ')' && c != '-' && c != ' ') {
            return 0;
        }
    }
    return digitos == 10 || digitos == 11;
}
 
static int lerTelefone(const char *prompt, char *dest, size_t tam) {
    while (1) {
        printf("%s", prompt);
        if (!lerLinha(dest, tam)) return 0;
        if (telefoneValido(dest)) return 1;
        printf("Telefone inválido! Informe o DDD e um número com 10 ou 11 dígitos "
               "(pode usar ( ), - e espaço).\n");
    }
}
 
/* Lê nome + telefone de um contato adicional */
static int lerContato(char *nome, size_t tamNome, char *tel, size_t tamTel) {
    if (!lerObrigatorio("Nome: ", "O nome do contato não pode ficar vazio!",
                        nome, tamNome)) return 0;
    return lerTelefone("Telefone: ", tel, tamTel);
}
 
/* ------------------------------------------------------------------
 * FUNÇÃO DE CADASTRAR
 * ------------------------------------------------------------------ */
void cadastrarAnimal(void) {
    if (totalAnimais >= MAX_ANIMAIS) {
        printf("Limite de animais atingido!\n");
        return;
    }
 
    struct Animal novoAnimal = {0};
    int opcao;
 
    novoAnimal.totalContatos = 0;
    novoAnimal.id = proximoId;
 
    printf("\n=== CADASTRO DE ANIMAL ===\n");
    printf("ID do animal: %03d\n", novoAnimal.id);
 
    if (!lerObrigatorio("Nome do animal: ",
                        "O nome do animal não pode ficar vazio!",
                        novoAnimal.nome, sizeof(novoAnimal.nome))) return;
 
    /* Espécie */
    const char *especies[] = {
        "Cachorro", "Gato", "Ave", "Réptil", "Roedor", "Outra espécie"
    };
 
    printf("\nEscolha a espécie:\n");
    for (int i = 0; i < 6; i++) {
        printf("%d - %s\n", i + 1, especies[i]);
    }
 
    if (!lerInteiroFaixa("Digite uma opção: ", 1, 6,
                         "Opção inválida! Escolha um número de 1 a 6.",
                         &opcao)) return;
 
    snprintf(novoAnimal.especie, sizeof(novoAnimal.especie), "%s", especies[opcao - 1]);
    printf("Espécie cadastrada: %s\n", novoAnimal.especie);
 
    /* Raça (opcional) */
    if (!lerOpcional("Raça do animal (ou Enter para não informar): ",
                     "Não informada",
                     novoAnimal.raca, sizeof(novoAnimal.raca))) return;
 
    /* Sexo */
    printf("\nSexo do animal:\n");
    printf("1 - Macho\n");
    printf("2 - Fêmea\n");
 
    if (!lerInteiroFaixa("Digite uma opção: ", 1, 2,
                         "Opção inválida! Escolha 1 ou 2.",
                         &opcao)) return;
    novoAnimal.sexo = opcao;
 
    /* Idade */
    printf("\nIdade do animal:\n");
 
    if (!lerInteiroFaixa("Anos: ", 0, 200,
                         "Idade inválida! Digite um número entre 0 e 200.",
                         &novoAnimal.idadeAnos)) return;
 
    if (!lerInteiroFaixa("Meses: ", 0, 11,
                         "Digite uma quantidade de meses entre 0 e 11.",
                         &novoAnimal.idadeMeses)) return;
 
    /* Responsável */
    printf("\n=== DADOS DO RESPONSÁVEL ===\n");
 
    if (!lerObrigatorio("Nome do responsável: ",
                        "O nome do responsável não pode ficar vazio!",
                        novoAnimal.responsavel.nome,
                        sizeof(novoAnimal.responsavel.nome))) return;
 
    if (!lerTelefone("Telefone do responsável: ",
                     novoAnimal.responsavel.telefone,
                     sizeof(novoAnimal.responsavel.telefone))) return;
 
    /* Endereço do responsável */
    printf("\n=== ENDEREÇO DO RESPONSÁVEL ===\n");
 
    if (!lerObrigatorio("Rua: ", "A rua não pode ficar vazia!",
                        novoAnimal.responsavel.rua,
                        sizeof(novoAnimal.responsavel.rua))) return;
 
    if (!lerObrigatorio("Número: ", "O número não pode ficar vazio!",
                        novoAnimal.responsavel.numero,
                        sizeof(novoAnimal.responsavel.numero))) return;
 
    if (!lerOpcional("Complemento (ou Enter para não informar): ",
                     "Não informado",
                     novoAnimal.responsavel.complemento,
                     sizeof(novoAnimal.responsavel.complemento))) return;
 
    if (!lerObrigatorio("Bairro: ", "O bairro não pode ficar vazio!",
                        novoAnimal.responsavel.bairro,
                        sizeof(novoAnimal.responsavel.bairro))) return;
 
    if (!lerObrigatorio("Cidade: ", "A cidade não pode ficar vazia!",
                        novoAnimal.responsavel.cidade,
                        sizeof(novoAnimal.responsavel.cidade))) return;
 
    /* Contatos adicionais (até 2) */
    printf("\nDeseja adicionar contatos adicionais?\n");
    printf("1 - Sim\n");
    printf("2 - Não\n");
    if (!lerInteiroFaixa("Digite uma opção: ", 1, 2,
                         "Opção inválida! Escolha 1 ou 2.",
                         &opcao)) return;
 
    if (opcao == 1) {
        printf("\n=== CONTATO ADICIONAL ===\n");
        if (!lerContato(novoAnimal.contatos[0].nome,
                        sizeof(novoAnimal.contatos[0].nome),
                        novoAnimal.contatos[0].telefone,
                        sizeof(novoAnimal.contatos[0].telefone))) return;
        novoAnimal.totalContatos++;
 
        printf("\nDeseja adicionar um segundo contato?\n");
        printf("1 - Sim\n");
        printf("2 - Não\n");
        if (!lerInteiroFaixa("Digite uma opção: ", 1, 2,
                             "Opção inválida! Escolha 1 ou 2.",
                             &opcao)) return;
 
        if (opcao == 1) {
            printf("\n=== SEGUNDO CONTATO ===\n");
            if (!lerContato(novoAnimal.contatos[1].nome,
                            sizeof(novoAnimal.contatos[1].nome),
                            novoAnimal.contatos[1].telefone,
                            sizeof(novoAnimal.contatos[1].telefone))) return;
            novoAnimal.totalContatos++;
        }
    }
 
    /* Só grava depois que tudo foi preenchido com sucesso */
    animais[totalAnimais] = novoAnimal;
    totalAnimais++;
    proximoId++;
 
    printf("\nAnimal cadastrado com sucesso!\n");
}
 
/* ------------------------------------------------------------------
 * FUNÇÃO DE LISTAR
 * ------------------------------------------------------------------ */
void listarAnimais(void) {
    if (totalAnimais == 0) {
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
        printf("Sexo: %s\n", animais[i].sexo == 1 ? "Macho" : "Fêmea");
        printf("Idade: %d %s e %d %s\n",
               animais[i].idadeAnos,
               animais[i].idadeAnos == 1 ? "ano" : "anos",
               animais[i].idadeMeses,
               animais[i].idadeMeses == 1 ? "mês" : "meses");
    }
}
 
/* ------------------------------------------------------------------
 * FUNÇÃO DE PESQUISAR
 * ------------------------------------------------------------------ */
static void mostrarResumoAnimal(int i) {
    printf("\n=== ANIMAL ENCONTRADO ===\n");
    printf("ID: %03d\n", animais[i].id);
    printf("Nome: %s\n", animais[i].nome);
    printf("Espécie: %s\n", animais[i].especie);
    printf("Raça: %s\n", animais[i].raca);
}
 
void pesquisarAnimais(void) {
    int opcao;
    int id;
    int encontrado = 0;
    char nomeBusca[50];
 
    printf("\n=== PESQUISAR ANIMAIS ===\n");
    printf("1 - Pesquisar por ID\n");
    printf("2 - Pesquisar por nome\n");
    printf("0 - Voltar ao menu principal\n");
 
    if (!lerInteiroFaixa("Escolha uma opção: ", 0, 2,
                         "Opção inválida! Escolha 0, 1 ou 2.",
                         &opcao)) return;
 
    if (opcao == 0) {
        printf("\nVoltando ao menu principal...\n");
        return;
    }
 
    if (opcao == 1) {
        if (!lerInteiroFaixa("Digite o ID do animal: ", 1, INT_MAX,
                             "ID inválido! Digite um número positivo.",
                             &id)) return;
 
        for (int i = 0; i < totalAnimais; i++) {
            if (animais[i].id == id) {
                mostrarResumoAnimal(i);
                encontrado = 1;
                break;
            }
        }
    } else {
        printf("Digite o nome do animal: ");
        if (!lerLinha(nomeBusca, sizeof(nomeBusca))) return;
 
        if (textoVazio(nomeBusca)) {
            printf("Digite um nome para pesquisar!\n");
            return;
        }
 
        for (int i = 0; i < totalAnimais; i++) {
            if (strstr(animais[i].nome, nomeBusca) != NULL) {
                mostrarResumoAnimal(i);
                encontrado = 1;
            }
        }
    }
 
    if (!encontrado) {
        printf("\nAnimal não encontrado!\n");
    }
}
 