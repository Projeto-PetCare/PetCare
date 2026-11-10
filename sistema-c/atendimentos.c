#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include "petcare.h"

/* Definições reais das variáveis compartilhadas de atendimentos
 * (a struct Atendimento e o MAX_ATENDIMENTOS ficam no petcare.h) */
struct Atendimento atendimentos[MAX_ATENDIMENTOS];
int totalAtendimentos = 0;

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

// FUNÇÃO DE REGISTRAR ATENDIMENTO
void registrarAtendimento(void) {
    struct Atendimento novo;
    int id;

    printf("\n=== REGISTRAR ATENDIMENTO ===\n");

    if (totalAtendimentos >= MAX_ATENDIMENTOS) {
        printf("Limite de atendimentos atingido!\n");
        return;
    }

    if (!lerInteiro("Digite o ID do animal: ", &id) || id <= 0) {
        printf("ID inválido!\n");
        return;
    }

    if (buscarAnimal(id) == -1) {
        printf("Animal não encontrado!\n");
        return;
    }

    novo.idAnimal = id;

    // Data (repete até ser válida)
    while (1) {
        if (!lerLinha("Data do atendimento (DD/MM/AAAA): ",
                      novo.data, sizeof novo.data)) {
            return;
        }

        if (dataValida(novo.data)) break;
        printf("Data inválida! Use o formato DD/MM/AAAA com uma data que exista.\n");
    }

    // Descrição (repete até não ser vazia)
    while (1) {
        if (!lerLinha("Descrição do atendimento: ",
                      novo.descricao, sizeof novo.descricao)) {
            return;
        }

        if (!textoVazio(novo.descricao)) break;
        printf("A descrição não pode ficar vazia!\n");
    }

    // Só grava depois que tudo foi preenchido com sucesso
    atendimentos[totalAtendimentos] = novo;
    totalAtendimentos++;
    salvarAtendimentos();
    
    printf("\nAtendimento registrado com sucesso!\n");
}

// FUNÇÃO DE CONSULTAR HISTÓRICO
void consultarHistorico(void) {
    int id;

    printf("\n=== HISTÓRICO DE ATENDIMENTOS ===\n");

    if (!lerInteiro("Digite o ID do animal: ", &id) || id <= 0) {
        printf("ID inválido!\n");
        return;
    }

    int pos = buscarAnimal(id);
    if (pos == -1) {
        printf("Animal não encontrado!\n");
        return;
    }

    printf("\nAnimal: %s\n", animais[pos].nome);

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