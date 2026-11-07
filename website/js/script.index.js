// Arquivo de funcionalidades do site PetCare

const menuToggle = document.getElementById("mobile-menu");

const navList = document.getElementById("nav-list");

menuToggle.addEventListener("click", () => {

    navList.classList.toggle("active");

});