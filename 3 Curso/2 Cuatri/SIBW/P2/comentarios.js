
//Funcion para validar el formulario

function validateForm() {
    var caja = document.getElementById("coms");

    var nombre = document.getElementById("fname").value;
    var email = document.getElementById("ft").value;
    var coment = document.getElementById("fcom").value;

    //Con este if compruebo que los campos esten rellenos
    if(nombre == "" || email =="" || coment==""){
        alert("Debes rellenar todos los campos");
        return false;
    }

    //Con este if, compruebo que el campo del email, tenga un email
    if (!(/^([\w_\.-]+)@([\w-]+)\.([a-z\.]{2,6})$/.test(email))){
        alert("El formato del email no es correcto");
        return false;
    }

    var fecha = new Date();
    fecha.setMonth(fecha.getMonth()+1); 
    var z = '<p class="nf">'+nombre+' comentó el '+ fecha.getDate()+'-'+fecha.getMonth()+'-'+fecha.getFullYear()+' a las '+fecha.getHours()+':'+fecha.getMinutes()+'</p>'+
    '<p class="txtcom">'+coment+'</p>';
    caja.insertAdjacentHTML('beforeend', z);

    document.getElementById("coms").scrollTop =document.getElementById("coms").scrollHeight;
    
    document.getElementById("fcom").value = "";
}

//Funcion para filtrar palabras censuradas de la caja de comentarios
function filtroPalabras(){
    var censuradas = ['puta','cabron','pene','tonto','coño','mierda','subnormal','gilipollas','polla','follar'];
    var comentario = document.getElementById("fcom").value;
    var arrayCom = comentario.split(" ");
    var cambio = false;
    
    for (var i=0; i < arrayCom.length; i++) {
        for(var j=0; j < censuradas.length; j++){
            if(arrayCom[i].toLowerCase() == censuradas[j]){
                arrayCom[i] = arrayCom[i].replace(/\w/gi,"*");
                cambio = true;
            }
        }
    }
    if(cambio){
        comentario = "";
        comentario = comentario+arrayCom[0];
        for (var i=1; i < arrayCom.length; i++) {
            comentario = comentario+" "+arrayCom[i];
        }
        document.getElementById("fcom").value = comentario;
    }

}

//Funcion para hacer la seccion de comentarios desplegable
function mostrarComentarios(){
    var mostrar = document.getElementById("comentarios").style.display 
    if(mostrar=="" || mostrar=="none"){
        document.getElementsByClassName("comentarios")[0].style.display ='inline-block';
        document.getElementById("mostrar").value = "Quitar comentarios";
    }
    else{
        document.getElementsByClassName("comentarios")[0].style.display ='none';
        document.getElementById("mostrar").value = "Mostrar comentarios";
    }
}
