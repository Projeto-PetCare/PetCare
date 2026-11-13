#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <cjson/cJSON.h>
#include "petcare.h"

int main(void) {


    if (chdir("/home/usuario/Área de trabalho/PetCare/sistema-c") != 0) {
        printf("Status: 500 Internal Server Error\r\n");
        printf("Content-Type: application/json; charset=utf-8\r\n\r\n");
        printf("{\"sucesso\":false,\"mensagem\":\"Erro ao acessar a pasta do sistema.\"}");
        return 1;
    }

    if (carregarAnimais() == 0) {
        printf("Status: 500 Internal Server Error\r\n");
        printf("Content-Type: application/json; charset=utf-8\r\n\r\n");
        printf("{\"sucesso\":false,\"mensagem\":\"Erro ao carregar os animais cadastrados.\"}");
        return 1;
    }

    char *tamanhoTexto = getenv("CONTENT_LENGTH");


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
    cJSON *especie = cJSON_GetObjectItem(jsonRecebido, "especie");
    cJSON *raca = cJSON_GetObjectItem(jsonRecebido, "raca");
    cJSON *sexo = cJSON_GetObjectItem(jsonRecebido, "sexo");
    cJSON *idadeAnos = cJSON_GetObjectItem(jsonRecebido, "idadeAnos");
    cJSON *idadeMeses = cJSON_GetObjectItem(jsonRecebido, "idadeMeses");

    if (!cJSON_IsString(nome) || nome->valuestring[0] == '\0' ||
        !cJSON_IsString(especie) || especie->valuestring[0] == '\0' ||
        !cJSON_IsNumber(sexo) ||
        !cJSON_IsNumber(idadeAnos) ||
        !cJSON_IsNumber(idadeMeses) ||
        (sexo->valueint != 1 && sexo->valueint != 2) ||
        idadeAnos->valueint < 0 ||
        idadeMeses->valueint < 0 ||
        idadeMeses->valueint > 11) {

    cJSON_Delete(jsonRecebido);
    free(dados);

    printf("Status: 400 Bad Request\r\n");
    printf("Content-Type: application/json; charset=utf-8\r\n\r\n");
    printf("{\"sucesso\":false,\"mensagem\":\"Dados principais do animal invalidos ou incompletos.\"}");

    return 1;
    }
    cJSON *responsavel = cJSON_GetObjectItem(jsonRecebido, "responsavel");

    if (!cJSON_IsObject(responsavel)) {
        cJSON_Delete(jsonRecebido);
        free(dados);

    printf("Status: 400 Bad Request\r\n");
    printf("Content-Type: application/json; charset=utf-8\r\n\r\n");
    printf("{\"sucesso\":false,\"mensagem\":\"Dados do responsavel invalidos ou ausentes.\"}");

    return 1;
    }

    cJSON *nomeResponsavel = cJSON_GetObjectItem(responsavel, "nome");
    cJSON *telefone = cJSON_GetObjectItem(responsavel, "telefone");
    cJSON *rua = cJSON_GetObjectItem(responsavel, "rua");
    cJSON *numero = cJSON_GetObjectItem(responsavel, "numero");
    cJSON *bairro = cJSON_GetObjectItem(responsavel, "bairro");
    cJSON *cidade = cJSON_GetObjectItem(responsavel, "cidade");

    if (!cJSON_IsString(nomeResponsavel) || nomeResponsavel->valuestring[0] == '\0' ||
        !cJSON_IsString(telefone) || telefone->valuestring[0] == '\0' ||
        !cJSON_IsString(rua) || rua->valuestring[0] == '\0' ||
        !cJSON_IsString(numero) || numero->valuestring[0] == '\0' ||
        !cJSON_IsString(bairro) || bairro->valuestring[0] == '\0' ||
        !cJSON_IsString(cidade) || cidade->valuestring[0] == '\0') {

        cJSON_Delete(jsonRecebido);
        free(dados);

        printf("Status: 400 Bad Request\r\n");
        printf("Content-Type: application/json; charset=utf-8\r\n\r\n");
        printf("{\"sucesso\":false,\"mensagem\":\"Dados obrigatorios do responsavel invalidos ou incompletos.\"}");

        return 1;
    }

    int tamanhoTelefone = strlen(telefone->valuestring);
    int telefoneValido = 1;

    if (tamanhoTelefone != 10 && tamanhoTelefone != 11) {
        telefoneValido = 0;
    }

    for (int i = 0; i < tamanhoTelefone; i++) {
        if (telefone->valuestring[i] < '0' ||
            telefone->valuestring[i] > '9') {
            telefoneValido = 0;
            break;
        }
    }

    if (!telefoneValido) {
        cJSON_Delete(jsonRecebido);
        free(dados);

        printf("Status: 400 Bad Request\r\n");
        printf("Content-Type: application/json; charset=utf-8\r\n\r\n");
        printf("{\"sucesso\":false,\"mensagem\":\"Telefone do responsavel invalido. Informe DDD e numero.\"}");

        return 1;
    }

    struct Animal novoAnimal = {0};

    novoAnimal.id = proximoId;

    if (strlen(nome->valuestring) >= sizeof(novoAnimal.nome) ||
     strlen(especie->valuestring) >= sizeof(novoAnimal.especie) ||
     (cJSON_IsString(raca) &&
     strlen(raca->valuestring) >= sizeof(novoAnimal.raca))) {

        cJSON_Delete(jsonRecebido);
        free(dados);

        printf("Status: 400 Bad Request\r\n");
        printf("Content-Type: application/json; charset=utf-8\r\n\r\n");
        printf("{\"sucesso\":false,\"mensagem\":\"Nome, especie ou raca excede o tamanho permitido.\"}");

    return 1;
    }

    strcpy(novoAnimal.nome, nome->valuestring);
    strcpy(novoAnimal.especie, especie->valuestring);

    if (cJSON_IsString(raca)) {
        strcpy(novoAnimal.raca, raca->valuestring);
    }

    novoAnimal.sexo = sexo->valueint;
    novoAnimal.idadeAnos = idadeAnos->valueint;
    novoAnimal.idadeMeses = idadeMeses->valueint;

    cJSON *complemento = cJSON_GetObjectItem(responsavel, "complemento");

    if (strlen(nomeResponsavel->valuestring) >= sizeof(novoAnimal.responsavel.nome) ||
        strlen(telefone->valuestring) >= sizeof(novoAnimal.responsavel.telefone) ||
        strlen(rua->valuestring) >= sizeof(novoAnimal.responsavel.rua) ||
        strlen(numero->valuestring) >= sizeof(novoAnimal.responsavel.numero) ||
        strlen(bairro->valuestring) >= sizeof(novoAnimal.responsavel.bairro) ||
        strlen(cidade->valuestring) >= sizeof(novoAnimal.responsavel.cidade) ||
        (cJSON_IsString(complemento) &&
        strlen(complemento->valuestring) >= sizeof(novoAnimal.responsavel.complemento))) {

        cJSON_Delete(jsonRecebido);
        free(dados);

        printf("Status: 400 Bad Request\r\n");
        printf("Content-Type: application/json; charset=utf-8\r\n\r\n");
        printf("{\"sucesso\":false,\"mensagem\":\"Dados do responsavel excedem o tamanho permitido.\"}");

        return 1;
    }

    strcpy(novoAnimal.responsavel.nome, nomeResponsavel->valuestring);
    strcpy(novoAnimal.responsavel.telefone, telefone->valuestring);
    strcpy(novoAnimal.responsavel.rua, rua->valuestring);
    strcpy(novoAnimal.responsavel.numero, numero->valuestring);
    strcpy(novoAnimal.responsavel.bairro, bairro->valuestring);
    strcpy(novoAnimal.responsavel.cidade, cidade->valuestring);

    if (cJSON_IsString(complemento)) {
        strcpy(novoAnimal.responsavel.complemento, complemento->valuestring);
    }

        if (totalAnimais >= MAX_ANIMAIS) {
        cJSON_Delete(jsonRecebido);
        free(dados);

        printf("Status: 400 Bad Request\r\n");
        printf("Content-Type: application/json; charset=utf-8\r\n\r\n");
        printf("{\"sucesso\":false,\"mensagem\":\"Limite de animais cadastrados atingido.\"}");

        return 1;
    }

    animais[totalAnimais] = novoAnimal;
    totalAnimais++;
    proximoId++;

    if (salvarAnimais() == 0) {
        totalAnimais--;
        proximoId--;

        cJSON_Delete(jsonRecebido);
        free(dados);

        printf("Status: 500 Internal Server Error\r\n");
        printf("Content-Type: application/json; charset=utf-8\r\n\r\n");
        printf("{\"sucesso\":false,\"mensagem\":\"Erro ao salvar o animal.\"}");

        return 1;
    }

    printf("Status: 201 Created\r\n");
    printf("Content-Type: application/json; charset=utf-8\r\n\r\n");
    printf("{\"sucesso\":true,\"mensagem\":\"Animal cadastrado com sucesso.\"}");
    cJSON_Delete(jsonRecebido);
    free(dados);

    return 0;
}