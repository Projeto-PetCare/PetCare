const formulario = document.getElementById("form-contato");

formulario.addEventListener("submit", function(event) {

    event.preventDefault();

    alert(
        "Formulário validado! " +
        "O envio estará disponível após " +
        "a integração com o backend."
    );

});