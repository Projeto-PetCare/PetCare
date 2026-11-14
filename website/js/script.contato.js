"use strict";

const formulario = document.getElementById("form-contato");

formulario.addEventListener("submit", function (event) {
    event.preventDefault();

    if (!formulario.reportValidity()) return;

    const nome = document.getElementById("nome").value.trim();
    const email = document.getElementById("email").value.trim();
    const assuntoSelect = document.getElementById("assunto");
    const assunto = assuntoSelect.options[assuntoSelect.selectedIndex].text.trim();
    const mensagem = document.getElementById("mensagem").value.trim();

    if (!nome || !email || !assuntoSelect.value || !mensagem) {
        alert("Preencha todos os campos obrigatórios.");
        return;
    }

    const titulo = `PetCare - ${assunto}`;
    const corpo = [
        `Nome: ${nome}`,
        `E-mail para resposta: ${email}`,
        `Assunto: ${assunto}`,
        "",
        "Mensagem:",
        mensagem
    ].join("\n");

    const destino = `mailto:suporte.petcare@gmail.com?subject=${encodeURIComponent(titulo)}&body=${encodeURIComponent(corpo)}`;

    // O navegador solicita a abertura do aplicativo de e-mail configurado.
    // O usuário deve confirmar o envio nesse aplicativo.
    window.location.href = destino;

    const aviso = document.getElementById("aviso-contato");
    if (aviso) {
        aviso.textContent = "Seu aplicativo de e-mail deverá abrir com a mensagem pronta. Confira os dados e clique em Enviar no aplicativo. Se nada abrir, configure um aplicativo de e-mail no navegador.";
    }
});
