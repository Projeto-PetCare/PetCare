# PetCare — Contrato de Integração

## 1. Objetivo
Este documento define os padrões de comunicação entre o frontend e o backend do sistema PetCare.

O frontend será desenvolvido utilizando HTML, CSS e JavaScript.

O backend será desenvolvido exclusivamente em linguagem C, com armazenamento de informações em arquivos JSON.

O objetivo é garantir que as duas partes trabalhem de forma integrada, utilizando os mesmos padrões para enviar, receber e organizar as informações do sistema.


## 2. Padrão de organização dos dados

As informações trocadas entre o frontend e o backend serão organizadas no formato JSON.

Os campos utilizados deverão seguir os nomes definidos nas estruturas do backend, principalmente na estrutura `struct Animal`, evitando diferenças na identificação das informações.

O cadastro de um animal deverá contemplar:

- Identificação: id gerado pelo backend;
- Dados do animal: nome, especie, raca, sexo, idadeAnos e idadeMeses;
- Dados do responsável: nome, telefone, rua, numero, complemento, bairro e cidade;
- Contatos adicionais: lista com até dois objetos, cada um contendo nome e telefone.
- Quantidade de contatos: o backend deverá calcular a quantidade de contatos recebidos e preencher o campo `totalContatos` internamente.

        ### 2.1. Regras de validação do cadastro de animais

        O cadastro deverá respeitar as seguintes regras:

        - Os campos nome, espécie, sexo, idade, dados do responsável, rua, número, bairro e cidade são obrigatórios.
        - A raça e o complemento são opcionais.
        - O animal poderá possuir até dois contatos adicionais.
        - O sexo deverá utilizar os valores 1 (Macho) ou 2 (Fêmea).
        - A espécie deverá seguir as opções disponíveis no sistema.
        - A idade em anos não poderá ser negativa.
        - A idade em meses deverá estar entre 0 e 11.
        - Os telefones deverão conter DDD e 10 ou 11 dígitos.

        O backend deverá validar as informações recebidas antes de armazenar o cadastro.

        ### 2.2. Exemplo de envio do cadastro

        Ao cadastrar um animal, o frontend deverá enviar as informações no formato JSON, seguindo a estrutura abaixo:

        {
        "nome": "Thor",
        "especie": "Cachorro",
        "raca": "Labrador",
        "sexo": 1,
        "idadeAnos": 3,
        "idadeMeses": 0,
        "responsavel": {
            "nome": "João",
            "telefone": "11999999999",
            "rua": "Rua das Flores",
            "numero": "120",
            "complemento": "Casa 2",
            "bairro": "Centro",
            "cidade": "São Paulo"
        },
        "contatos": [
            {
            "nome": "Maria",
            "telefone": "11888888888"
            }
        ]
        }

        O identificador do animal (`id`) será gerado pelo backend. A quantidade de contatos (`totalContatos`) também será calculada internamente pelo backend, com base nos contatos recebidos.


## 3. Módulo de atendimentos

O módulo de atendimentos será responsável pelo registro e pela consulta dos atendimentos realizados nos animais cadastrados.

        ### 3.1. Informações identificadas

        Com base na estrutura atual do arquivo atendimentos.c, foram identificados os seguintes campos:

        - idAnimal: identificador do animal relacionado ao atendimento.
        - data: data em que o atendimento foi realizado.
        - descricao: descrição do atendimento.

        ### 3.2. Operações previstas

        - Registrar um atendimento, vinculando-o a um animal já cadastrado.
        - Consultar o histórico de atendimentos de um animal.

        ### 3.3. Pendências

        - Definir se o atendimento terá um identificador próprio.
        - Definir se serão necessários outros campos.
        - Implementar o armazenamento dos atendimentos em arquivos JSON.
        - Adaptar as funções para permitir a comunicação com o frontend.
        - Confirmar a estrutura definitiva com os responsáveis pelo módulo.


## 4. Rotas da API

        ### 4.1. Cadastro de animais

        **Objetivo:** permitir que o frontend envie os dados de um novo animal para o backend.

        - Método HTTP: POST
        - Rota: `/animais`
        - Formato dos dados enviados: JSON
        - Responsabilidade do backend: receber os dados, validar as informações e salvar o cadastro no arquivo JSON.
        - Resposta esperada: informar se o cadastro foi realizado com sucesso ou se ocorreu algum erro.
        **Exemplo de resposta em caso de sucesso:**
        {
        "sucesso": true,
        "mensagem": "Animal cadastrado com sucesso.",
        "dados": {
            "id": 1
        }
        }
        **Exemplo de resposta em caso de erro:**
        ```json
        {
        "sucesso": false,
        "mensagem": "O campo nome do animal é obrigatório."
        }
        ```
        ### 4.2. Listagem de animais

        **Objetivo:** permitir que o frontend solicite ao backend a lista de animais cadastrados.

        - Método HTTP: GET
        - Rota: `/animais`
        - Dados enviados: nenhum dado obrigatório.
        - Responsabilidade do backend: consultar os registros armazenados no arquivo JSON e retornar a lista de animais.
        - Resposta esperada: retornar os animais cadastrados ou uma lista vazia caso ainda não existam registros.

        **Exemplo de resposta:**
        {
        "sucesso": true,
        "mensagem": "Animais listados com sucesso.",
        "dados": [
            {
            "id": 1,
            "nome": "Thor",
            "especie": "Cachorro"
            },
            {
            "id": 2,
            "nome": "Mimi",
            "especie": "Gato"
            }
        ]
        }

        ### 4.3. Pesquisa de animal por ID

        **Objetivo:** permitir que o frontend solicite os dados de um animal específico.

        - Método HTTP: GET
        - Rota: `/animais/{id}`
        - Dados enviados: identificador (ID) do animal na própria rota.
        - Responsabilidade do backend: localizar o animal correspondente ao ID informado no arquivo JSON.
        - Resposta esperada: retornar os dados do animal encontrado ou informar que o animal não foi localizado.
        #### Pesquisa por nome

        Além da pesquisa por ID, o sistema deverá permitir localizar animais pelo nome.

        - Método HTTP: GET
        - Rota: `/animais?nome={nome}`
        - Responsabilidade do backend: localizar os animais cujo nome corresponda ao termo informado.
        - Resposta esperada: retornar os animais encontrados ou informar que nenhum resultado foi localizado.


        **Exemplo de resposta:**

        {
        "sucesso": true,
        "mensagem": "Pesquisa realizada com sucesso.",
        "dados": [
            {
            "id": 1,
            "nome": "Thor",
            "especie": "Cachorro"
            }
        ]
        }
        **Exemplo quando nenhum animal for encontrado:**

        {
        "sucesso": true,
        "mensagem": "Nenhum animal encontrado.",
        "dados": []
        }



        **Exemplo de resposta em caso de sucesso:**

       {
        "sucesso": true,
        "mensagem": "Animal encontrado.",
        "dados": {
            "id": 1,
            "nome": "Thor",
            "especie": "Cachorro",
            "raca": "Labrador",
            "sexo": 1,
            "idadeAnos": 3,
            "idadeMeses": 0
        }
        }

        **Exemplo de resposta em caso de erro:**

        {
        "sucesso": false,
        "mensagem": "Animal não encontrado.",
        "dados": null
        }

        ### 4.4. Registro de atendimento

        **Objetivo:** permitir que o frontend envie os dados de um novo atendimento para o backend.

        - Método HTTP: POST
        - Rota: `/atendimentos`
        - Formato dos dados enviados: JSON.
        - Informações previstas: ID do animal, data, descrição e ID do serviço, quando houver.
        - Responsabilidade do backend: verificar se o animal existe, validar as informações e registrar o atendimento no arquivo JSON.
        - Resposta esperada: informar se o atendimento foi registrado ou se ocorreu algum erro.


        ### 4.5. Consulta do histórico de atendimentos

        **Objetivo:** permitir que o frontend consulte o histórico de atendimentos de um animal específico.

        - Método HTTP: GET
        - Rota: `/animais/{id}/atendimentos`
        - Dados enviados: ID do animal na própria rota.
        - Responsabilidade do backend: localizar os atendimentos vinculados ao animal informado no arquivo JSON.
        - Resposta esperada: retornar a lista de atendimentos encontrados ou uma lista vazia caso o animal ainda não possua atendimentos.


        ### 4.6. Cadastro de serviços

        **Objetivo:** permitir que o frontend envie os dados de um novo serviço para o backend.

        - Método HTTP: POST
        - Rota: `/servicos`
        - Formato dos dados enviados: JSON.
        - Informações previstas: nome e preço.
        - Responsabilidade do backend: validar as informações, gerar um ID para o serviço e armazená-lo no arquivo JSON.
        - Resposta esperada: informar se o serviço foi cadastrado com sucesso ou se ocorreu algum erro.

        ### 4.7. Listagem de serviços

        **Objetivo:** permitir que o frontend consulte todos os serviços cadastrados no PetCare.

        - Método HTTP: GET
        - Rota: `/servicos`
        - Dados enviados: nenhum dado obrigatório.
        - Responsabilidade do backend: consultar os serviços armazenados no arquivo JSON e retornar a lista.
        - Resposta esperada: retornar os serviços cadastrados ou uma lista vazia caso ainda não existam registros.

        ### 4.8. Relatório de atendimentos

        **Objetivo:** permitir que o frontend solicite um relatório com os atendimentos registrados no PetCare.

        - Método HTTP: GET
        - Rota: `/relatorios/atendimentos`
        - Dados enviados: nenhum dado obrigatório.
        - Responsabilidade do backend: consultar os atendimentos registrados e organizar as informações para o relatório.
        - Resposta esperada: retornar os dados dos atendimentos para exibição no frontend.

        ### 4.9. Consulta de faturamento

        **Objetivo:** permitir que o frontend consulte o faturamento gerado pelos atendimentos registrados no PetCare.

        - Método HTTP: GET
        - Rota: `/relatorios/faturamento`
        - Dados enviados: nenhum dado obrigatório.
        - Responsabilidade do backend: consultar os atendimentos vinculados a serviços e calcular o valor total com base nos preços cadastrados.
        - Resposta esperada: retornar o total do faturamento e as informações necessárias para sua exibição no frontend.


## 5. Padrão de respostas HTTP

O backend deverá utilizar códigos de status HTTP para informar ao frontend o resultado de cada solicitação.

Os principais códigos utilizados serão:

- 200 (OK): solicitação realizada com sucesso.
- 201 (Created): cadastro realizado com sucesso.
- 400 (Bad Request): informações enviadas incorretas ou inválidas.
- 404 (Not Found): registro solicitado não encontrado.
- 500 (Internal Server Error): erro interno durante o processamento da solicitação.

O frontend deverá utilizar essas respostas para identificar o resultado da operação e apresentar mensagens adequadas ao usuário.

        ### 5.1. Estrutura padrão das respostas

        Todas as respostas do backend deverão utilizar o formato JSON.
        As respostas deverão seguir um padrão com os seguintes campos:

        - sucesso: indica se a operação foi realizada corretamente.
        - mensagem: apresenta uma descrição do resultado, quando necessário.
        - dados: contém as informações retornadas pelo backend, quando houver.

        O campo `dados` poderá conter um objeto ou uma lista, dependendo da operação realizada.

        As informações específicas de cada módulo deverão ficar organizadas dentro do campo `dados`, mantendo um padrão de resposta entre todas as operações.

        ### 5.2. Envio de informações pelo frontend

        O frontend utilizará JavaScript para enviar solicitações HTTP ao backend.

        Nas operações que enviarem informações, os dados deverão ser organizados no formato JSON.

        As solicitações deverão utilizar o cabeçalho `Content-Type: application/json`, indicando que o conteúdo enviado está no formato JSON.

        O backend deverá receber essas informações, interpretar os dados, realizar as validações necessárias e devolver uma resposta em JSON.

        Nas operações de consulta, o frontend deverá receber e interpretar os dados retornados pelo backend para apresentá-los na interface do sistema.


## 6. Armazenamento das informações

O backend será responsável por armazenar e recuperar os dados do PetCare utilizando arquivos JSON, sem a utilização de banco de dados.

Inicialmente, os dados serão organizados em arquivos separados por módulo:

- `animais.json`: armazenará os cadastros dos animais.
- `atendimentos.json`: armazenará os atendimentos realizados.
- `servicos.json`: armazenará os serviços cadastrados.

O backend deverá consultar os arquivos para recuperar as informações e atualizá-los sempre que novos registros forem realizados ou informações existentes forem alteradas.

Os arquivos deverão preservar os dados cadastrados mesmo após o encerramento do programa.