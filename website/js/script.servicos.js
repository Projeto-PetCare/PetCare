"use strict";

// Endpoints existentes no projeto.
// Funcionam quando o servidor CGI estiver configurado.
// Use a mesma origem do site ou um proxy para /cgi-bin.
const API_ANIMAIS = "/cgi-bin/animais";
const API_CADASTRO = "/cgi-bin/cadastrar_animal";

const formAnimal = document.getElementById("form-animal");
const corpoTabela = document.getElementById("corpo-tabela-animais");
const pesquisa = document.getElementById("pesquisa-animal");

const mensagemCadastro =
    document.getElementById("mensagem-cadastro");

const mensagemListagem =
    document.getElementById("mensagem-listagem");

let animaisCarregados = [];

// 1. NAVEGAÇÃO ENTRE TELAS

document.querySelectorAll(".painel-botao").forEach(botao => {

    botao.addEventListener("click", () => {

        document.querySelectorAll(".painel-tela")
            .forEach(tela => {
                tela.hidden = true;
            });

        document.querySelectorAll(".painel-botao")
            .forEach(item => {
                item.classList.remove("ativo");
            });

        const tela = document.getElementById(botao.dataset.tela);

        if (!tela) return;

        tela.hidden = false;
        botao.classList.add("ativo");

        if (tela.id === "tela-listagem") {
            carregarAnimais();
        }
    });

});

// 2. FUNÇÃO PARA LER CAMPOS

function valor(id) {
    return document.getElementById(id).value.trim();
}

// 3. MONTAR OS DADOS DO ANIMAL

function montarAnimal() {

    const contatos = [];

    for (let i = 1; i <= 2; i++) {

        const nome = valor(`contato${i}-nome`);
        const telefone = valor(`contato${i}-telefone`);

        // Um contato deve ter nome e telefone.
        if (nome || telefone) {

            if (!nome || !telefone) {
                throw new Error(
                    `Preencha nome e telefone do contato ${i}.`
                );
            }

            contatos.push({ nome, telefone });
        }
    }

    return {
        nome: valor("animal-nome"),
        especie: valor("animal-especie"),
        raca: valor("animal-raca") || "Não informada",
        sexo: Number(valor("animal-sexo")),
        idadeAnos: Number(valor("animal-anos")),
        idadeMeses: Number(valor("animal-meses")),

        responsavel: {
            nome: valor("resp-nome"),
            telefone: valor("resp-telefone"),
            rua: valor("resp-rua"),
            numero: valor("resp-numero"),
            complemento: valor("resp-complemento"),
            bairro: valor("resp-bairro"),
            cidade: valor("resp-cidade")
        },

        totalContatos: contatos.length,
        contatos: contatos
    };
}

// 4. ENVIAR O CADASTRO

formAnimal.addEventListener("submit", async (event) => {

    event.preventDefault();

    mensagemCadastro.textContent = "Processando cadastro...";

    try {
        const animal = montarAnimal();

        const resposta = await fetch(API_CADASTRO, {
            method: "POST",
            headers: {
                "Content-Type": "application/json"
            },
            body: JSON.stringify(animal)
        });

        const resultado = await resposta.json();

        if (!resposta.ok || resultado.sucesso !== true) {
            throw new Error(
                resultado.mensagem || "Falha no cadastro."
            );
        }

        // Atenção: o CGI atual confirma apenas o
        // recebimento do JSON, não a gravação.
        mensagemCadastro.textContent =
    resultado.mensagem || "Animal cadastrado com sucesso.";

formAnimal.reset();

        // Só limpar o formulário após o backend
        // confirmar a gravação de verdade.
        // formAnimal.reset();
    } catch (erro) {
        mensagemCadastro.textContent =
            `Erro: ${erro.message}`;
    }

});

// 5. CONSULTAR ANIMAIS

async function carregarAnimais() {

    mensagemListagem.textContent = "Carregando animais...";

    try {
        const resposta = await fetch(API_ANIMAIS);

        const resultado = await resposta.json();

        if (!resposta.ok || resultado.sucesso !== true) {
            throw new Error(
                resultado.mensagem || "Erro ao listar animais."
            );
        }

        if (!Array.isArray(resultado.dados)) {
            throw new Error("Formato de listagem inválido.");
        }

        animaisCarregados = resultado.dados;

        filtrarAnimais();

    } catch (erro) {

        animaisCarregados = [];
        corpoTabela.replaceChildren();

        mensagemListagem.textContent =
            `Erro ao carregar: ${erro.message}`;
    }
}

// 6. MONTAR A TABELA

function mostrarAnimais(animais) {

    corpoTabela.replaceChildren();

    for (const animal of animais) {

        const linha = document.createElement("tr");

        //Dados do responsável
        const responsavel = animal.responsavel
            ?`${animal.responsavel.nome} - ${animal.responsavel.telefone}`
            :"Não cadastrado";

        //Contatos adicionais

        //Primeiro contato

        const contato1 = animal.contatos?.[0]
            ?`${animal.contatos[0].nome} - ${animal.contatos[0].telefone}`
            : "Não cadastrado";
            
        //Segundo contato
        const contato2 = animal.contatos?.[1]
            ? `${animal.contatos[1].nome} - ${animal.contatos[1].telefone}`
            : "Não cadastrado";

        const campos = [
            animal.id,
            animal.nome,
            animal.especie,
            animal.raca,
            Number(animal.sexo) === 1 ? "Macho" : "Fêmea",
            `${animal.idadeAnos}a ${animal.idadeMeses}m`,
            responsavel,
            contato1,
            contato2
            
        ];

        for (const campo of campos) {
            const celula = document.createElement("td");
            celula.textContent = String(campo ?? "");
            linha.appendChild(celula);
        }

        corpoTabela.appendChild(linha);
    }

    mensagemListagem.textContent =
        `${animais.length} animal(is) encontrado(s).`;
}

// 7. PESQUISA POR NOME OU ID

function filtrarAnimais() {

    const termo = pesquisa.value.trim().toLowerCase();

    const filtrados = animaisCarregados.filter(animal => {

        const nome = String(animal.nome).toLowerCase();
        const id = String(animal.id);

        return nome.includes(termo) || id === termo;
    });

    mostrarAnimais(filtrados);
}

pesquisa.addEventListener("input", filtrarAnimais);

document.getElementById("atualizar-animais")
    .addEventListener("click", carregarAnimais);
