
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <cjson/cJSON.h>
#include "petcare.h"

int main(void) {

    chdir("/home/usuario/Área de trabalho/PetCare/sistema-c");

    carregarAtendimentos();

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

    for (int i = 0; i < totalAtendimentos; i++) {

        cJSON *atendimento = cJSON_CreateObject();

        if (atendimento == NULL) {
            cJSON_Delete(resposta);
            cJSON_Delete(lista);

            printf("Status: 500 Internal Server Error\r\n");
            printf("Content-Type: application/json\r\n\r\n");
            printf("{\"sucesso\":false,\"mensagem\":\"Erro ao gerar relatorio.\"}");
            return 1;
        }

        cJSON_AddNumberToObject(atendimento, "idAnimal",
                                atendimentos[i].idAnimal);

        cJSON_AddStringToObject(atendimento, "data",
                                atendimentos[i].data);

        cJSON_AddStringToObject(atendimento, "descricao",
                                atendimentos[i].descricao);

        cJSON_AddItemToArray(lista, atendimento);
    }

    cJSON_AddBoolToObject(resposta, "sucesso", 1);
    cJSON_AddStringToObject(resposta, "mensagem",
                            "Relatorio de atendimentos gerado com sucesso.");
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
