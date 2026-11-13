
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <cjson/cJSON.h>
#include "petcare.h"

int main(void) {

    chdir("/home/usuario/Área de trabalho/PetCare/sistema-c");

    char *consulta = getenv("QUERY_STRING");

    if (consulta == NULL || strncmp(consulta, "id=", 3) != 0) {
        printf("Status: 400 Bad Request\r\n");
        printf("Content-Type: application/json\r\n\r\n");
        printf("{\"sucesso\":false,\"mensagem\":\"Informe o ID do animal.\"}");
        return 1;
    }

    char *fim;
    long idAnimal = strtol(consulta + 3, &fim, 10);

    if (idAnimal <= 0 || *fim != '\0') {
        printf("Status: 400 Bad Request\r\n");
        printf("Content-Type: application/json\r\n\r\n");
        printf("{\"sucesso\":false,\"mensagem\":\"ID do animal invalido.\"}");
        return 1;
    }

    if (!carregarAnimais()) {
        printf("Status: 500 Internal Server Error\r\n");
        printf("Content-Type: application/json\r\n\r\n");
        printf("{\"sucesso\":false,\"mensagem\":\"Erro ao carregar animais.\"}");
        return 1;
    }

    carregarAtendimentos();

    int animalEncontrado = 0;

    for (int i = 0; i < totalAnimais; i++) {
        if (animais[i].id == idAnimal) {
            animalEncontrado = 1;
            break;
        }
    }

    if (!animalEncontrado) {
        printf("Status: 404 Not Found\r\n");
        printf("Content-Type: application/json\r\n\r\n");
        printf("{\"sucesso\":false,\"mensagem\":\"Animal nao encontrado.\"}");
        return 1;
    }

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

        if (atendimentos[i].idAnimal == idAnimal) {

            cJSON *atendimento = cJSON_CreateObject();

            if (atendimento == NULL) {
                cJSON_Delete(resposta);
                cJSON_Delete(lista);

                printf("Status: 500 Internal Server Error\r\n");
                printf("Content-Type: application/json\r\n\r\n");
                printf("{\"sucesso\":false,\"mensagem\":\"Erro ao consultar historico.\"}");
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
    }

    cJSON_AddBoolToObject(resposta, "sucesso", 1);
    cJSON_AddStringToObject(resposta, "mensagem",
                            "Historico consultado com sucesso.");
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
