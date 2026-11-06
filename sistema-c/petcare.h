#ifndef PETCARE_H
#define PETCARE_H
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

#define MAX_ANIMAIS 100

extern struct Animal animais[MAX_ANIMAIS];
extern int totalAnimais;
extern int proximoId;

void cadastrarAnimal(void);
void listarAnimais(void);
void pesquisarAnimais(void);

#endif