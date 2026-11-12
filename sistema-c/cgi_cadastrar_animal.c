#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <cjson/cJSON.h>
#include "petcare.h"

int main(void) {

    char *tamanhoTexto = getenv ("CONTENT_LENGTH");

    if (tamanhoTexto == NULL) {
        printf("Status: 400 Bad Request\r\n");
        printf("Content-Type: application/json; charset=utf-8\r\n\r\n");
        printf("{\"sucesso\":false,\"mensagem\":\"Dados nao recebidos.\"}");
        return 1;
    }
    int tamanho = atoi(tamanhoTexto);

    if (tamanho <= 0 || tamanho > 10000) {
        printf("Status: 400 Bad Request\r\n");
        printf("Content-Type: application/json; charset=utf-8\r\n\r\n");
        printf("{\"sucesso\":false,\"mensagem\":\"Tamanho dos dados invalido.\"}");
        return 1;
    }
    char *dados = malloc(tamanho + 1);

    if (dados == NULL) {
        printf("Status: 500 Internal Server Error\r\n");
        printf("Content-Type: application/json; charset=utf-8\r\n\r\n");
        printf("{\"sucesso\":false,\"mensagem\":\"Erro de memoria.\"}");
        return 1;
    }

    size_t lidos = fread(dados, 1, tamanho,stdin);

    if (lidos != (size_t)tamanho) {
        free(dados);

        printf("Status: 400 Bad Request\r\n");
        printf("Content-Type: application/json; charset=utf-8\r\n\r\n");
        printf("{\"sucesso\":false,\"mensagem\":\"Falha ao receber os dados.\"}");
        return 1;
    }
    dados[tamanho] = '\0';

    cJSON *jsonRecebido = cJSON_Parse(dados);

    if (jsonRecebido == NULL) {
        free(dados);

        printf("Status: 400 Bad Request\r\n");
        printf("Content-Type: application/json; charset=utf-8\r\n\r\n");
        printf("{\"sucesso\":false,\"mensagem\":\"JSON invalido.\"}");

        return 1;
    }

    if (!cJSON_IsObject(jsonRecebido)) {
        cJSON_Delete(jsonRecebido);
        free(dados);

        printf("Status: 400 Bad Request\r\n");
        printf("Content-Type: application/json; charset=utf-8\r\n\r\n");
        printf("{\"sucesso\":false,\"mensagem\":\"Formato de dados invalido.\"}");

        return 1;
    }
    cJSON *nome = cJSON_GetObjectItem(jsonRecebido, "nome");

    if (!cJSON_IsString(nome) || nome->valuestring[0] == '\0') {
        cJSON_Delete(jsonRecebido);
        free(dados);

        printf("Status: 400 Bad Request\r\n");
        printf("Content-Type: application/json; charset=utf-8\r\n\r\n");
        printf("{\"sucesso\":false,\"mensagem\":\"Nome do animal obrigatorio.\"}");

        return 1;
    }

    printf("Content-Type: application/json; charset=utf-8\r\n\r\n");
    printf("{\"sucesso\":true,\"mensagem\":\"JSON recebido e interpretado com sucesso.\"}");

    cJSON_Delete(jsonRecebido);
    free(dados);

    return 0;
}