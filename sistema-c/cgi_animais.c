#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <cjson/cJSON.h>
#include "petcare.h"

int main(void) {

//acessa as pastas do arquivo JSON
    if (chdir("/home/usuario/Área de trabalho/PetCare/sistema-c") !=0) {
        printf("Status: 500 Internal Server Error\r\n");
        printf("Content-Type: application/json; charset=utf-8\r\n\r\n");
        printf("{\"sucesso\":false,\"mensagem\":\"Nao foi possivel acessar os arquivos.\"}");
        return 1;

    }

    carregarAnimais();

    cJSON *resposta = cJSON_CreateObject();
    cJSON *dados = cJSON_CreateArray();

    if (resposta == NULL || dados == NULL) {
        cJSON_Delete(resposta);
        cJSON_Delete(dados);

        printf("Status: 500 Internal Server Error\r\n");
        printf("Content-Type: application/json; charset=utf-8\r\n\r\n");
        printf("{\"sucesso\":false,\"mensagem\":\"Erro ao criar resposta.\"}");
        return 1;
    }

    cJSON_AddBoolToObject(resposta, "sucesso", 1);
    cJSON_AddStringToObject(resposta, "mensagem", "Animais listados com sucesso.");

    for (int i = 0; i < totalAnimais; i++) {

        cJSON *animal = cJSON_CreateObject();
        cJSON *responsavel = cJSON_CreateObject();
        cJSON *contatos = cJSON_CreateArray();

    if (animal == NULL || responsavel == NULL || contatos == NULL) {
        cJSON_Delete(animal);
        cJSON_Delete(responsavel);
        cJSON_Delete(contatos);
        continue;
    }

    cJSON_AddNumberToObject(animal, "id", animais[i].id);
    cJSON_AddStringToObject(animal, "nome", animais[i].nome);
    cJSON_AddStringToObject(animal, "especie", animais[i].especie);
    cJSON_AddStringToObject(animal, "raca", animais[i].raca);
    cJSON_AddNumberToObject(animal, "sexo", animais[i].sexo);
    cJSON_AddNumberToObject(animal, "idadeAnos", animais[i].idadeAnos);
    cJSON_AddNumberToObject(animal, "idadeMeses", animais[i].idadeMeses);

    //Dados do responsável
    cJSON_AddStringToObject(responsavel, "nome", animais[i].responsavel.nome);
    cJSON_AddStringToObject(responsavel, "telefone", animais[i].responsavel.telefone);
    cJSON_AddStringToObject(responsavel, "rua", animais[i].responsavel.rua);
    cJSON_AddStringToObject(responsavel, "numero", animais[i].responsavel.numero);
    cJSON_AddStringToObject(responsavel, "complemento", animais[i].responsavel.complemento);
    cJSON_AddStringToObject(responsavel, "bairro", animais[i].responsavel.bairro);
    cJSON_AddStringToObject(responsavel, "cidade", animais[i].responsavel.cidade);

    cJSON_AddItemToObject(animal, "responsavel", responsavel);

    // CNTTS ADICIONAIS
    int limite = animais[i].totalContatos;

    if (limite > 2) {
        limite = 2;
    }

    for (int j = 0; j < limite; j++) {
        cJSON *contato = cJSON_CreateObject();

        if (contato != NULL) {
        cJSON_AddStringToObject(contato, "nome", animais[i].contatos[j].nome);
        cJSON_AddStringToObject(contato, "telefone", animais[i].contatos[j].telefone);
        cJSON_AddItemToArray(contatos, contato);
      }
    }
    cJSON_AddNumberToObject(animal, "totalContatos", limite);
    cJSON_AddItemToObject(animal, "contatos", contatos);
    cJSON_AddItemToArray(dados, animal);

}
cJSON_AddItemToObject(resposta, "dados", dados);

char *json = cJSON_PrintUnformatted(resposta);

printf("Content-Type: application/json; charset=utf-8\r\n\r\n");

    if (json != NULL) {
        printf("%s", json);
        free(json);
    }

    cJSON_Delete(resposta);

    return 0;
}