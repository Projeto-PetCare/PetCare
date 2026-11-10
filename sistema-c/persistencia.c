#include <stdio.h>
#include <stdlib.h>
#include <cjson/cJSON.h>
#include "petcare.h"
void salvarAnimais(void) {
    cJSON *lista = cJSON_CreateArray();

    if (lista == NULL) {
        printf("Erro ao criar lista JSON!\n");
        return;
    }
    for (int i = 0; i < totalAnimais; i++) {
    cJSON *animal = cJSON_CreateObject();

    cJSON_AddNumberToObject(animal, "id", animais[i].id);
    cJSON_AddStringToObject(animal, "nome", animais[i].nome);
    cJSON_AddStringToObject(animal, "especie", animais[i].especie);
    cJSON_AddStringToObject(animal, "raca", animais[i].raca);

    cJSON_AddNumberToObject(animal, "sexo", animais[i].sexo);
    cJSON_AddNumberToObject(animal, "idadeAnos", animais[i].idadeAnos);
    cJSON_AddNumberToObject(animal, "idadeMeses", animais[i].idadeMeses);

    cJSON *responsavel = cJSON_CreateObject();

    cJSON_AddStringToObject(responsavel, "nome", animais[i].responsavel.nome);
    cJSON_AddStringToObject(responsavel, "telefone", animais[i].responsavel.telefone);
    cJSON_AddStringToObject(responsavel, "rua", animais[i].responsavel.rua);
    cJSON_AddStringToObject(responsavel, "numero", animais[i].responsavel.numero);
    cJSON_AddStringToObject(responsavel, "complemento", animais[i].responsavel.complemento);
    cJSON_AddStringToObject(responsavel, "bairro", animais[i].responsavel.bairro);
    cJSON_AddStringToObject(responsavel, "cidade", animais[i].responsavel.cidade);

    cJSON_AddItemToObject(animal, "responsavel", responsavel);
    // Salva a quantidade de contatos
    cJSON_AddNumberToObject(animal, "totalContatos", animais[i].totalContatos);

    // Cria a lista de contatos
    cJSON *contatos = cJSON_CreateArray();

    for (int j = 0; j < animais[i].totalContatos; j++) {
        cJSON *contato = cJSON_CreateObject();

        cJSON_AddStringToObject(contato, "nome", animais[i].contatos[j].nome);
        cJSON_AddStringToObject(contato, "telefone", animais[i].contatos[j].telefone);

        cJSON_AddItemToArray(contatos, contato);
    }

    cJSON_AddItemToObject(animal, "contatos", contatos);

    
    cJSON_AddItemToArray(lista, animal);
}
        char *json = cJSON_Print(lista);

    if (json == NULL) {
        printf("Erro ao converter os dados para JSON!\n");
        cJSON_Delete(lista);
        return;
    }

    FILE *arquivo = fopen("animais.json", "w");

    if (arquivo == NULL) {
        printf("Erro ao abrir animais.json!\n");
        free(json);
        cJSON_Delete(lista);
        return;
    }

    fprintf(arquivo, "%s\n", json);

    fclose(arquivo);
    free(json);
    cJSON_Delete(lista);
}

void carregarAnimais(void) {
    FILE *arquivo = fopen("animais.json", "r");
    if (arquivo == NULL) {
    printf("Arquivo animais.json ainda não existe.\n");
    return;
}

    fseek(arquivo, 0, SEEK_END);
    long tamanho = ftell(arquivo);
    rewind(arquivo);
    char *conteudo = malloc(tamanho + 1);

if (conteudo == NULL) {
    printf("Erro ao reservar memória!\n");
    fclose(arquivo);
    return;
}

    size_t lidos = fread(conteudo, 1, tamanho, arquivo);
    conteudo[lidos] = '\0';

    fclose(arquivo);
    cJSON *lista = cJSON_Parse(conteudo);

if (lista == NULL) {
    printf("Erro ao interpretar o JSON!\n");
    free(conteudo);
    return;
}
if (!cJSON_IsArray(lista)) {
    printf("O JSON não contém uma lista válida!\n");
    cJSON_Delete(lista);
    free(conteudo);
    return;
}
totalAnimais = 0;

cJSON *item = NULL;

cJSON_ArrayForEach(item, lista) {
    if (totalAnimais >= MAX_ANIMAIS) {
        break;
    }

    cJSON *id = cJSON_GetObjectItemCaseSensitive(item, "id");

    if (cJSON_IsNumber(id)) {
        animais[totalAnimais].id = id->valueint;
    }
    cJSON *nome = cJSON_GetObjectItemCaseSensitive(item, "nome");
    cJSON *especie = cJSON_GetObjectItemCaseSensitive(item, "especie");
    cJSON *raca = cJSON_GetObjectItemCaseSensitive(item, "raca");

    if (cJSON_IsString(nome)) {
        snprintf(animais[totalAnimais].nome,
                sizeof(animais[totalAnimais].nome), "%s", nome->valuestring);
    }

    if (cJSON_IsString(especie)) {
        snprintf(animais[totalAnimais].especie,
                sizeof(animais[totalAnimais].especie), "%s", especie->valuestring);
    }

    if (cJSON_IsString(raca)) {
        snprintf(animais[totalAnimais].raca,
                sizeof(animais[totalAnimais].raca), "%s", raca->valuestring);
    }
    cJSON *sexo = cJSON_GetObjectItemCaseSensitive(item, "sexo");
    cJSON *idadeAnos = cJSON_GetObjectItemCaseSensitive(item, "idadeAnos");
    cJSON *idadeMeses = cJSON_GetObjectItemCaseSensitive(item, "idadeMeses");

    if (cJSON_IsNumber(sexo)) {
        animais[totalAnimais].sexo = sexo->valueint;
    }

    if (cJSON_IsNumber(idadeAnos)) {
        animais[totalAnimais].idadeAnos = idadeAnos->valueint;
    }

    if (cJSON_IsNumber(idadeMeses)) {
        animais[totalAnimais].idadeMeses = idadeMeses->valueint;
    }
    cJSON *responsavel = cJSON_GetObjectItemCaseSensitive(item, "responsavel");

    if (cJSON_IsObject(responsavel)) {
    cJSON *nomeResp = cJSON_GetObjectItemCaseSensitive(responsavel, "nome");
    cJSON *telefone = cJSON_GetObjectItemCaseSensitive(responsavel, "telefone");
    cJSON *rua = cJSON_GetObjectItemCaseSensitive(responsavel, "rua");
    cJSON *numero = cJSON_GetObjectItemCaseSensitive(responsavel, "numero");
    cJSON *complemento = cJSON_GetObjectItemCaseSensitive(responsavel, "complemento");
    cJSON *bairro = cJSON_GetObjectItemCaseSensitive(responsavel, "bairro");
    cJSON *cidade = cJSON_GetObjectItemCaseSensitive(responsavel, "cidade");

    if (cJSON_IsString(nomeResp))
        snprintf(animais[totalAnimais].responsavel.nome, sizeof(animais[totalAnimais].responsavel.nome), "%s", nomeResp->valuestring);

    if (cJSON_IsString(telefone))
        snprintf(animais[totalAnimais].responsavel.telefone, sizeof(animais[totalAnimais].responsavel.telefone), "%s", telefone->valuestring);

    if (cJSON_IsString(rua))
        snprintf(animais[totalAnimais].responsavel.rua, sizeof(animais[totalAnimais].responsavel.rua), "%s", rua->valuestring);

    if (cJSON_IsString(numero))
        snprintf(animais[totalAnimais].responsavel.numero, sizeof(animais[totalAnimais].responsavel.numero), "%s", numero->valuestring);

    if (cJSON_IsString(complemento))
        snprintf(animais[totalAnimais].responsavel.complemento, sizeof(animais[totalAnimais].responsavel.complemento), "%s", complemento->valuestring);

    if (cJSON_IsString(bairro))
        snprintf(animais[totalAnimais].responsavel.bairro, sizeof(animais[totalAnimais].responsavel.bairro), "%s", bairro->valuestring);

    if (cJSON_IsString(cidade))
        snprintf(animais[totalAnimais].responsavel.cidade, sizeof(animais[totalAnimais].responsavel.cidade), "%s", cidade->valuestring);
}
    // Carrega os contatos adicionais
    animais[totalAnimais].totalContatos = 0;

    cJSON *contatos = cJSON_GetObjectItemCaseSensitive(item, "contatos");

    if (cJSON_IsArray(contatos)) {
        cJSON *contato = NULL;

        cJSON_ArrayForEach(contato, contatos) {
            int indice = animais[totalAnimais].totalContatos;

        // Limita a quantidade a 2 contatos
        if (indice >= 2) {
            break;
        }

        cJSON *nomeContato = cJSON_GetObjectItemCaseSensitive(contato, "nome");
        cJSON *telefoneContato = cJSON_GetObjectItemCaseSensitive(contato, "telefone");

        if (cJSON_IsString(nomeContato)) {
            snprintf(animais[totalAnimais].contatos[indice].nome,
                     sizeof(animais[totalAnimais].contatos[indice].nome),
                     "%s", nomeContato->valuestring);
        }

        if (cJSON_IsString(telefoneContato)) {
            snprintf(animais[totalAnimais].contatos[indice].telefone,
                     sizeof(animais[totalAnimais].contatos[indice].telefone),
                     "%s", telefoneContato->valuestring);
        }

        animais[totalAnimais].totalContatos++;
    }
}
    totalAnimais++;
}
    proximoId = 1;

    for (int i = 0; i < totalAnimais; i++) {
        if (animais[i].id >= proximoId) {
            proximoId = animais[i].id + 1;
        }
    }

    cJSON_Delete(lista);
    free(conteudo);
}