"use strict";

// ELEMENTOS DO HTML
const campoCep = document.getElementById("resp-cep");
const botaoCep = document.getElementById("buscar-cep");
const mensagemCep = document.getElementById("mensagem-cep");

const campoRua = document.getElementById("resp-rua");
const campoBairro = document.getElementById("resp-bairro");
const campoCidade = document.getElementById("resp-cidade");
const campoNumero = document.getElementById("resp-numero");

// FUNÇÃO DE CONSULTA
async function buscarCep() {

    // Remove hífen, espaços e outros caracteres
    const cep = campoCep.value.replace(/\D/g, "");

    mensagemCep.classList.remove("erro");

    // Validação: CEP deve conter 8 dígitos
    if (cep.length !== 8) {
        mensagemCep.textContent = "Informe um CEP com 8 números.";
        mensagemCep.classList.add("erro");
        return;
    }

    // Indica que a consulta começou
    mensagemCep.textContent = "Consultando CEP...";
    botaoCep.disabled = true;

    // Limpa endereço anterior para não misturar dados
    campoRua.value = "";
    campoBairro.value = "";
    campoCidade.value = "";

    try {

        const resposta = await fetch(
            `https://viacep.com.br/ws/${cep}/json/`
        );

        if (!resposta.ok) {
            throw new Error("Não foi possível consultar o CEP.");
        }

        const endereco = await resposta.json();

        // ViaCEP informa erro quando não encontra o CEP
        if (endereco.erro) {
            throw new Error("CEP não encontrado.");
        }

        // Preenche os campos disponíveis
        campoCep.value = endereco.cep || cep;
        campoRua.value = endereco.logradouro || "";
        campoBairro.value = endereco.bairro || "";
        campoCidade.value = endereco.localidade || "";

        mensagemCep.textContent =
            "Endereço localizado. Confira os dados e informe o número.";

        // Se a rua não vier preenchida, funcionário digita
        if (!campoRua.value) {
            campoRua.focus();
        } else {
            campoNumero.focus();
        }

    } catch (erro) {

        mensagemCep.textContent = erro.message;
        mensagemCep.classList.add("erro");

    } finally {

        // Reativa o botão mesmo quando ocorrer erro
        botaoCep.disabled = false;

    }
}

// CONSULTAR AO CLICAR NO BOTÃO
botaoCep.addEventListener("click", buscarCep);

// CONSULTAR QUANDO O USUÁRIO SAIR DO CAMPO
campoCep.addEventListener("blur", function () {

    if (campoCep.value.trim()) {
        buscarCep();
    }

});