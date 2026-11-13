
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <cjson/cJSON.h>
#include "petcare.h"

int main(void) {

    chdir("/home/usuario/Área de trabalho/PetCare/sistema-c");

    printf("Content-Type: application/json\r\n\r\n");

    char *tamanhoTexto = getenv("CONTENT_LENGTH");

    if (tamanhoTexto == NULL) {
        printf("{\"sucesso\":false,\"mensagem\":\"Tamanho da requisicao nao informado.\"}");
        return 1;
    }

    int tamanho = atoi(tamanhoTexto);

    if (tamanho <= 0 || tamanho > 10000) {
        printf("{\"sucesso\":false,\"mensagem\":\"Tamanho da requisicao invalido.\"}");
        return 1;
    }

    char *corpo = malloc(tamanho + 1);

    if (corpo == NULL) {
        printf("{\"sucesso\":false,\"mensagem\":\"Erro de memoria.\"}");
        return 1;
    }

    size_t lidos = fread(corpo, 1, tamanho, stdin);
    corpo[lidos] = '\0';

    if (lidos != (size_t)tamanho) {
        free(corpo);
        printf("{\"sucesso\":false,\"mensagem\":\"Erro ao receber os dados.\"}");
        return 1;
    }

    cJSON *dados = cJSON_Parse(corpo);
    free(corpo);

    if (dados == NULL) {
        printf("{\"sucesso\":false,\"mensagem\":\"JSON invalido.\"}");
        return 1;
    }

    cJSON *idAnimalJSON = cJSON_GetObjectItemCaseSensitive(dados, "idAnimal");
    cJSON *dataJSON = cJSON_GetObjectItemCaseSensitive(dados, "data");
    cJSON *descricaoJSON = cJSON_GetObjectItemCaseSensitive(dados, "descricao");

    if (!cJSON_IsNumber(idAnimalJSON) ||
        !cJSON_IsString(dataJSON) ||
        !cJSON_IsString(descricaoJSON)) {

        cJSON_Delete(dados);
        printf("{\"sucesso\":false,\"mensagem\":\"Dados obrigatorios invalidos.\"}");
        return 1;
    }

    int idAnimal = idAnimalJSON->valueint;
    const char *data = dataJSON->valuestring;
    const char *descricao = descricaoJSON->valuestring;

    if (idAnimal <= 0 ||
        strlen(data) != 10 ||
        strlen(descricao) == 0 ||
        strlen(descricao) >= sizeof(atendimentos[0].descricao)) {

        cJSON_Delete(dados);
        printf("{\"sucesso\":false,\"mensagem\":\"Valores informados invalidos.\"}");
        return 1;
    }

    if (!carregarAnimais()) {
        cJSON_Delete(dados);
        printf("{\"sucesso\":false,\"mensagem\":\"Erro ao carregar os animais.\"}");
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
        cJSON_Delete(dados);
        printf("{\"sucesso\":false,\"mensagem\":\"Animal nao encontrado.\"}");
        return 1;
    }

    if (totalAtendimentos >= MAX_ATENDIMENTOS) {
        cJSON_Delete(dados);
        printf("{\"sucesso\":false,\"mensagem\":\"Limite de atendimentos atingido.\"}");
        return 1;
    }

    struct Atendimento novo;

    novo.idAnimal = idAnimal;
    snprintf(novo.data, sizeof(novo.data), "%s", data);
    snprintf(novo.descricao, sizeof(novo.descricao), "%s", descricao);

    atendimentos[totalAtendimentos] = novo;
    totalAtendimentos++;

    salvarAtendimentos();

    cJSON_Delete(dados);

    printf("{\"sucesso\":true,\"mensagem\":\"Atendimento registrado com sucesso.\"}");

    return 0;
}
