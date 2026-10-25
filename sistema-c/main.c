#include <stdio.h>

int main() {
    int opcao;
    printf("========== PETCARE ==========\n");
    printf("1 - Cadastrar animal\n");
    printf("2 - Listar animais\n");
    printf("3 - Pesquisar animais\n");
    printf("4 - Registrar atendimento\n");
    printf("5 - Registrar serviço\n");
    printf("6 - Consultar histórico\n");
    printf("7 - Relatório de atendimento\n");
    printf("8 - Faturamento\n");
    printf("0 - Sair\n");
    printf("Digite uma opção: ");
    scanf("%d", &opcao);
    printf("Você escolheu a opão: %d\n", opcao);

    return 0;
}
