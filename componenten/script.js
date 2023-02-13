
const loginForm = document.getElementById("login-form");
const loginButton = document.getElementById("login-form-submit");
const loginErrorMsg = document.getElementById("login-error-msg");



loginButton.addEventListener("click", (e) => {
    e.preventDefault();
    const username = loginForm.username.value;
    const password = loginForm.password.value;

    if (username === "Watervoeding" && password === "WSysteem") {
        location.reload()
        console.log("admin show");
        window.location.replace("https://plantenvoedingsysteem.netlify.app/");

    } else {
        loginErrorMsg.style.opacity = 1;
        setTimeout(function() {
            location.reload()
        }, 500);
        
    }
})