#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <ctype.h>
#include <cjson/cJSON.h>
#include "petcare.h"

void decodificarURL(char *texto)
{
    char *origem = texto;
    char *destino = texto;

    while (*origem) {
        if (*origem == '+') {
            *destino++ = ' ';
            origem++;
        }
        else if (*origem == '%' &&
                 isxdigit((unsigned char)origem[1]) &&
                 isxdigit((unsigned char)origem[2])) {

            char hexadecimal[3] = {origem[1], origem[2], '\0'};
            *destino++ = (char)strtol(hexadecimal, NULL, 16);
            origem += 3;
        }
        else {
            *destino++ = *origem++;
        }
    }

    *destino = '\0';
}

int main(void)
{
    if (chdir("/home/usuario/Área de trabalho/PetCare/sistema-c") != 0) {
        printf("Status: 500 Internal Server Error\r\n");
        printf("Content-Type: application/json; charset=utf-8\r\n\r\n");
        printf("{\"sucesso\":false,\"mensagem\":\"Erro ao acessar os arquivos.\"}");
        return 1;
    }

    if (!carregarAnimais()) {
        printf("Status: 500 Internal Server Error\r\n");
        printf("Content-Type: application/json; charset=utf-8\r\n\r\n");
        printf("{\"sucesso\":false,\"mensagem\":\"Erro ao carregar os animais.\"}");
        return 1;
    }

    char consulta[256] = "";
    char tipo[20] = "";
    char valor[200] = "";

    char *queryString = getenv("QUERY_STRING");

    if (queryString != NULL) {
        snprintf(consulta, sizeof(consulta), "%s", queryString);
    }

    char *token = strtok(consulta, "&");

    while (token != NULL) {
        char *igual = strchr(token, '=');

        if (igual != NULL) {
            *igual = '\0';

            char *chave = token;
            char *conteudo = igual + 1;

            decodificarURL(chave);
            decodificarURL(conteudo);

            if (strcmp(chave, "id") == 0) {
                strcpy(tipo, "id");
                snprintf(valor, sizeof(valor), "%s", conteudo);
                break;
            }

            if (strcmp(chave, "nome") == 0) {
                strcpy(tipo, "nome");
                snprintf(valor, sizeof(valor), "%s", conteudo);
                break;
            }
        }

        token = strtok(NULL, "&");
    }

    if (tipo[0] == '\0' || valor[0] == '\0') {
        printf("Status: 400 Bad Request\r\n");
        printf("Content-Type: application/json; charset=utf-8\r\n\r\n");
        printf("{\"sucesso\":false,\"mensagem\":\"Informe o ID ou o nome do animal.\"}");
        return 0;
    }

    int indiceEncontrado = -1;

    if (strcmp(tipo, "id") == 0) {
        char *fim;
        long id = strtol(valor, &fim, 10);

        if (*fim != '\0' || id <= 0) {
            printf("Status: 400 Bad Request\r\n");
            printf("Content-Type: application/json; charset=utf-8\r\n\r\n");
            printf("{\"sucesso\":false,\"mensagem\":\"ID invalido.\"}");
            return 0;
        }

        for (int i = 0; i < totalAnimais; i++) {
            if (animais[i].id == id) {
                indiceEncontrado = i;
                break;
            }
        }
    }
    else {
        for (int i = 0; i < totalAnimais; i++) {
            if (strcasecmp(animais[i].nome, valor) == 0) {
                indiceEncontrado = i;
                break;
            }
        }
    }

    if (indiceEncontrado == -1) {
        printf("Status: 404 Not Found\r\n");
        printf("Content-Type: application/json; charset=utf-8\r\n\r\n");
        printf("{\"sucesso\":false,\"mensagem\":\"Animal nao encontrado.\"}");
        return 0;
    }

    struct Animal *a = &animais[indiceEncontrado];

    cJSON *resposta = cJSON_CreateObject();
    cJSON *animal = cJSON_CreateObject();
    cJSON *responsavel = cJSON_CreateObject();
    cJSON *contatos = cJSON_CreateArray();

    if (resposta == NULL || animal == NULL ||
        responsavel == NULL || contatos == NULL) {

        cJSON_Delete(resposta);
        cJSON_Delete(animal);
        cJSON_Delete(responsavel);
        cJSON_Delete(contatos);

        printf("Status: 500 Internal Server Error\r\n");
        printf("Content-Type: application/json; charset=utf-8\r\n\r\n");
        printf("{\"sucesso\":false,\"mensagem\":\"Erro ao criar resposta.\"}");
        return 1;
    }

    cJSON_AddBoolToObject(resposta, "sucesso", 1);
    cJSON_AddStringToObject(resposta, "mensagem", "Animal encontrado.");

    cJSON_AddNumberToObject(animal, "id", a->id);
    cJSON_AddStringToObject(animal, "nome", a->nome);
    cJSON_AddStringToObject(animal, "especie", a->especie);
    cJSON_AddStringToObject(animal, "raca", a->raca);
    cJSON_AddNumberToObject(animal, "sexo", a->sexo);
    cJSON_AddNumberToObject(animal, "idadeAnos", a->idadeAnos);
    cJSON_AddNumberToObject(animal, "idadeMeses", a->idadeMeses);

    cJSON_AddStringToObject(responsavel, "nome", a->responsavel.nome);
    cJSON_AddStringToObject(responsavel, "telefone", a->responsavel.telefone);
    cJSON_AddStringToObject(responsavel, "rua", a->responsavel.rua);
    cJSON_AddStringToObject(responsavel, "numero", a->responsavel.numero);
    cJSON_AddStringToObject(responsavel, "complemento", a->responsavel.complemento);
    cJSON_AddStringToObject(responsavel, "bairro", a->responsavel.bairro);
    cJSON_AddStringToObject(responsavel, "cidade", a->responsavel.cidade);

    cJSON_AddItemToObject(animal, "responsavel", responsavel);

    int limite = a->totalContatos;

    if (limite > 2) {
        limite = 2;
    }

    for (int i = 0; i < limite; i++) {
        cJSON *contato = cJSON_CreateObject();

        if (contato != NULL) {
            cJSON_AddStringToObject(contato, "nome", a->contatos[i].nome);
            cJSON_AddStringToObject(contato, "telefone", a->contatos[i].telefone);
            cJSON_AddItemToArray(contatos, contato);
        }
    }

    cJSON_AddNumberToObject(animal, "totalContatos", limite);
    cJSON_AddItemToObject(animal, "contatos", contatos);
    cJSON_AddItemToObject(resposta, "dados", animal);

    char *json = cJSON_PrintUnformatted(resposta);

    printf("Status: 200 OK\r\n");
    printf("Content-Type: application/json; charset=utf-8\r\n\r\n");

    if (json != NULL) {
        printf("%s", json);
        free(json);
    }

    cJSON_Delete(resposta);

    return 0;
}