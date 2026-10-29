#include <stdio.h>

#define MAX_ANIMAIS 100

struct Contato 
{
    char nome[100];
    char telefone[20];
};

struct Responsavel
{
    char nome[100];
    char telefone[20];
    char rua[100];
    char numero[10];
    char complemento[50];
    char bairro[50];
    char cidade[50];
};

struct Animal
{
   int id;
   char nome[50];
   char especie[30];
   char raca[50];
   int sexo;
   int idadeAnos;
   int idadeMeses; 
   struct Responsavel responsavel;
   struct Contato contatos[2];
   int totalContatos;
   
   
};

struct Animal animais[MAX_ANIMAIS];
int totalAnimais = 0;
int proximoId = 1;


int main() {
    // Guarda a opção escolhida pelo usuário
    // Começa com -1 para que o menu seja exibido pela primeira vez
    int opcao = -1;

    // Repete o menu enquanto(WHILE) a opção escolhida for diferente(!=) de 0 (SAIR)
    while (opcao != 0) {

        //Exibe o menu principal do sistema.
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

        // Solicita ao usuário que escolha uma opção do menu.
        printf("Digite uma opção: ");
        scanf("%d", &opcao);

        // Mostra o número da opção escolhida.
        printf("Você escolheu a opção: %d\n", opcao);

        // Verifica se o usuário escolheu a opção 1.
        if (opcao == 1){
            printf("Você escolheu cadastrar animal!\n");
        }
        // A partir daqui vai realizar a verificação de outras opções que o usuário pode escolher além da 1.
        else if (opcao == 2) {
            printf("Você escolheu listar animais!\n");
        }
        else if (opcao == 3) {
            printf("Você escolheu pesquisar animais!\n");
        }
        else if (opcao == 4) {
            printf("Você escolheu registrar atendimento!\n");
        }
        else if (opcao == 5) {
            printf("Você escolheu registrar serviço!\n");
        }
        else if (opcao == 6) {
            printf("Você escolheu consultar histórico!\n");
        }
        else if (opcao == 7) {
            printf("Você escolheu relatório de atendimento!\n");
        }
        else if (opcao == 8) {
            printf("Você escolheu faturamento!\n");
        }
        // Encerra o sistema.
        else if (opcao == 0) {
            printf("Encerrando o sistema Petcare!\n");
        }
        // Executa esta mensagem quando a escolha da opção não ta no menu
        else {
            printf("Opção inválida! Tente novamente.\n");
        }
    }// FIM do WHILE: volta ao menu ou encerra a repetição
    
    return 0; // Indica que o programa terminou normalmente.
}
