#include <stdio.h>
#include "petcare.h"

#ifdef _WIN32
#include <windows.h>
#endif

// Lê um inteiro com segurança, sem travar em loop se o usuário digitar letras
int lerInteiro(void) {
    int valor;
    int c;

    while (scanf("%d", &valor) != 1) {
        while ((c = getchar()) != '\n' && c != EOF);  // descarta a linha inválida
        if (c == EOF) return 0;                       // evita loop infinito se a entrada fechar
        printf("Entrada inválida! Digite um número: ");
    }

    while ((c = getchar()) != '\n' && c != EOF);      // descarta o '\n' que sobrou

    return valor;
}

int main() {
    carregarAnimais();
    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);
    #endif

    int opcao = -1;

    while (opcao != 0) {
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
        opcao = lerInteiro();   // <-- era scanf("%d", &opcao);

        printf("Você escolheu a opção: %d\n", opcao);

                if (opcao == 1)      cadastrarAnimal();
        else if (opcao == 2) listarAnimais();
        else if (opcao == 3) pesquisarAnimais();
        else if (opcao == 4) registrarAtendimento();
        else if (opcao == 5) registrarServico();
        else if (opcao == 6) consultarHistorico();
        else if (opcao == 7) relatorioAtendimentos();
        else if (opcao == 8) faturamento();
        else if (opcao == 0) printf("Encerrando o sistema Petcare!\n");
        else                 printf("Opção inválida! Tente novamente.\n");
    }

    return 0;
}