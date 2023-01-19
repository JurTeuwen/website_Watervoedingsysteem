// fetch("/componenten/alert.html") // Haal de in te laden component op
//   .then(response => {   // Vorm deze om naar tekst.
//     return response.text()
//   })
//   .then(data => {       // Zet de tekst in een Custom Tag.
//     document.querySelector("alert").innerHTML = data;
//   });


$(".alert-div").load("/componenten/alert.html")

setTimeout(function() {
    var leeg = true;
    if (leeg) {
        $(".detectie").show();
    }  
    console.log( $(".detectie"))
}, 1000);


