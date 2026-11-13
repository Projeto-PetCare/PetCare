
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <cjson/cJSON.h>
#include "petcare.h"

int main(void) {
    chdir("/home/usuario/Área de trabalho/PetCare/sistema-c");

    carregarServicos();

    double faturamentoTotal = 0;

    for (int i = 0; i < totalServicos; i++) {
        faturamentoTotal += servicos[i].valor;
    }

    cJSON *resposta = cJSON_CreateObject();
    cJSON *dados = cJSON_CreateObject();

    if (resposta == NULL || dados == NULL) {
        cJSON_Delete(resposta);
        cJSON_Delete(dados);

        printf("Status: 500 Internal Server Error\r\n");
        printf("Content-Type: application/json\r\n\r\n");
        printf("{\"sucesso\":false,\"mensagem\":\"Erro ao gerar relatorio.\"}");

        return 1;
    }

    cJSON_AddBoolToObject(resposta, "sucesso", 1);
    cJSON_AddStringToObject(resposta, "mensagem",
        "Relatorio de faturamento gerado com sucesso.");

    cJSON_AddNumberToObject(dados, "quantidadeServicos", totalServicos);
    cJSON_AddNumberToObject(dados, "faturamentoTotal", faturamentoTotal);

    cJSON_AddItemToObject(resposta, "dados", dados);

    char *json = cJSON_Print(resposta);

    printf("Content-Type: application/json\r\n\r\n");

    if (json != NULL) {
        printf("%s", json);
        free(json);
    }

    cJSON_Delete(resposta);

    return 0;
}
