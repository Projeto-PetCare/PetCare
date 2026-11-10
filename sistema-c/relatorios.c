#include <stdio.h>
#include "petcare.h"
 
/* Procura o nome do animal pelo ID (usado nos dois relatórios) */
static const char *buscarNomeAnimal(int idAnimal) {
    for (int i = 0; i < totalAnimais; i++) {
        if (animais[i].id == idAnimal) {
            return animais[i].nome;
        }
    }
    return "Animal não encontrado";
}
 
/* OPÇÃO 7 - RELATÓRIO DE ATENDIMENTOS */
void relatorioAtendimentos(void) {
    printf("\n=== RELATÓRIO DE ATENDIMENTOS ===\n");
 
    if (totalAtendimentos == 0) {
        printf("Nenhum atendimento registrado até o momento.\n");
        return;
    }
 
    for (int i = 0; i < totalAtendimentos; i++) {
        printf("\nAtendimento %d\n", i + 1);
        printf("Animal: %s (ID %d)\n", buscarNomeAnimal(atendimentos[i].idAnimal),
               atendimentos[i].idAnimal);
        printf("Data: %s\n", atendimentos[i].data);
        printf("Descrição: %s\n", atendimentos[i].descricao);
    }
 
    printf("\n------------------------------\n");
    printf("Total de atendimentos: %d\n", totalAtendimentos);
}
 
/* OPÇÃO 8 - FATURAMENTO
 * Soma o valor de cada serviço realizado. Como cada serviço tem ID único e
 * é percorrido uma única vez, nenhum serviço é contado duas vezes. */
void faturamento(void) {
    double total = 0;   /* double: mais precisão ao somar muitos valores */
 
    printf("\n=== FATURAMENTO ===\n");
 
    if (totalServicos == 0) {
        printf("Nenhum serviço realizado registrado até o momento.\n");
        return;   /* garante que a divisão abaixo nunca seja por zero */
    }
 
    for (int i = 0; i < totalServicos; i++) {
        printf("\nServiço #%d\n", servicos[i].id);
        printf("Animal: %s (ID %d)\n", buscarNomeAnimal(servicos[i].idAnimal),
               servicos[i].idAnimal);
        printf("Data: %s\n", servicos[i].data);
        printf("Descrição: %s\n", servicos[i].descricao);
        printf("Valor: R$ %.2f\n", servicos[i].valor);
 
        total += servicos[i].valor;
    }
 
    printf("\n------------------------------\n");
    printf("Serviços contabilizados: %d\n", totalServicos);
    printf("Faturamento total: R$ %.2f\n", total);
    printf("Valor médio por serviço: R$ %.2f\n", total / totalServicos);
}