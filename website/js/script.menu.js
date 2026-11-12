const botaoMenu = document.getElementById("mobile-menu");

const listaMenu = document.getElementById("nav-list");

if (botaoMenu && listaMenu) {

    function fecharMenu() {

        listaMenu.classList.remove("active");
        botaoMenu.classList.remove("active");

        botaoMenu.setAttribute("aria-expanded", "false");
        botaoMenu.setAttribute("aria-label", "Abrir menu");

    }

    botaoMenu.addEventListener("click", function () {

        const aberto = listaMenu.classList.toggle("active");

        botaoMenu.classList.toggle("active", aberto);

        botaoMenu.setAttribute(
            "aria-expanded",
            String(aberto)
        );

        botaoMenu.setAttribute(
            "aria-label",
            aberto ? "Fechar menu" : "Abrir menu"
        );

    });

    // FECHAR AO PRESSIONAR ESC
    document.addEventListener("keydown", function (event) {

        if (event.key === "Escape") {
            fecharMenu();
        }

    });

    // FECHAR AO RETORNAR PARA O LAYOUT DESKTOP
    window.addEventListener("resize", function () {

        if (window.innerWidth > 768) {
            fecharMenu();
        }

    });

}