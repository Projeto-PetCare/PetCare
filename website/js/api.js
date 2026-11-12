const API_URL = "http://localhost/cgi-bin/animais";

async function listarAnimais() {
    const lista = document.getElementById("lista-animais");

    lista.textContent = "Carregando animais...";

    try {
        const resposta = await fetch(API_URL);

        if (!resposta.ok) {
            throw new Error("Erro na comunicação com o servidor.");
        }

        const resultado = await resposta.json();

        if (!resultado.sucesso) {
            throw new Error(resultado.mensagem);
        }

        lista.innerHTML = "";

        resultado.dados.forEach(animal => {
            const item = document.createElement("li");

            item.textContent = `${animal.nome} - ${animal.especie}`;
            lista.appendChild(item);
        });

    } catch (erro) {
        lista.textContent = "Não foi possível carregar os animais.";
        console.error("Erro:", erro);
    }
}