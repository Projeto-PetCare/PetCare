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

/* ---------- NOVO: atendimentos (movido de atendimentos.c) ---------- */
#define MAX_ATENDIMENTOS 100

struct Atendimento
{
    int idAnimal;
    char data[15];
    char descricao[150];
};

/* ---------- NOVO: serviços realizados ---------- */
#define MAX_SERVICOS 100

struct Servico
{
    int id;            /* identificador único: evita contar o mesmo serviço duas vezes */
    int idAnimal;      /* animal que recebeu o serviço */
    char data[15];     /* DD/MM/AAAA */
    char descricao[100];
    float valor;       /* valor cobrado, em reais */
};

#define MAX_ANIMAIS 100

/* Variáveis compartilhadas (definidas em animais.c) */
extern struct Animal animais[MAX_ANIMAIS];
extern int totalAnimais;
extern int proximoId;

/* NOVO: definidas em atendimentos.c */
extern struct Atendimento atendimentos[MAX_ATENDIMENTOS];
extern int totalAtendimentos;

/* NOVO: definidas em servicos.c */
extern struct Servico servicos[MAX_SERVICOS];
extern int totalServicos;
extern int proximoIdServico;

/* animais.c */
void cadastrarAnimal(void);
void listarAnimais(void);
void pesquisarAnimais(void);

/* atendimentos.c */
void registrarAtendimento(void);
void consultarHistorico(void);

/* servicos.c */
void registrarServico(void);

/* relatorios.c */
void relatorioAtendimentos(void);
void faturamento(void);

#endif

