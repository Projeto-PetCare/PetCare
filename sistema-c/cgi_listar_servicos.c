
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <cjson/cJSON.h>
#include "petcare.h"

int main(void) {

    chdir("/home/usuario/Área de trabalho/PetCare/sistema-c");

    carregarServicos();

    cJSON *resposta = cJSON_CreateObject();
    cJSON *lista = cJSON_CreateArray();

    if (resposta == NULL || lista == NULL) {
        cJSON_Delete(resposta);
        cJSON_Delete(lista);

        printf("Status: 500 Internal Server Error\r\n");
        printf("Content-Type: application/json\r\n\r\n");
        printf("{\"sucesso\":false,\"mensagem\":\"Erro ao criar resposta JSON.\"}");
        return 1;
    }

    for (int i = 0; i < totalServicos; i++) {

        cJSON *servico = cJSON_CreateObject();

        if (servico == NULL) {
            cJSON_Delete(resposta);
            cJSON_Delete(lista);

            printf("Status: 500 Internal Server Error\r\n");
            printf("Content-Type: application/json\r\n\r\n");
            printf("{\"sucesso\":false,\"mensagem\":\"Erro ao listar servicos.\"}");
            return 1;
        }

        cJSON_AddNumberToObject(servico, "id", servicos[i].id);
        cJSON_AddNumberToObject(servico, "idAnimal", servicos[i].idAnimal);
        cJSON_AddStringToObject(servico, "data", servicos[i].data);
        cJSON_AddStringToObject(servico, "descricao", servicos[i].descricao);
        cJSON_AddNumberToObject(servico, "valor", servicos[i].valor);

        cJSON_AddItemToArray(lista, servico);
    }

    cJSON_AddBoolToObject(resposta, "sucesso", 1);
    cJSON_AddStringToObject(resposta, "mensagem", "Servicos listados com sucesso.");
    cJSON_AddItemToObject(resposta, "dados", lista);

    char *json = cJSON_Print(resposta);

    printf("Content-Type: application/json\r\n\r\n");

    if (json != NULL) {
        printf("%s", json);
        free(json);
    }

    cJSON_Delete(resposta);

    return 0;
}
