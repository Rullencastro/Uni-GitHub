$(document).ready(function(){
   busqueda.onkeyup= hacerPeticionAjax;
});

function hacerPeticionAjax(){
    palabra = $("#busqueda").val();

    if(palabra != ""){
        $.ajax({
            data: {palabra},
            url: '/ajax.php',
            type: 'get',
            success: function(respuesta){
                console.log(respuesta);
                procesarRespuestaAjax(respuesta,palabra);
            }
        }); 
    }else
        $("#resultadosBusqueda").empty();
}

function procesarRespuestaAjax(respuesta,palabra){
    res = "";
    

    if(respuesta.length > 0)
        res+="<p id=busq>Resultados de la búsqueda</p>";
    else if(respuesta.length <= 0)
        res+="<p id=busq>Resultados de la búsqueda</p><p>No se ha encontrado ninguna coincidencia</p>";

    for(i = 0;i < respuesta.length;i++){
        res+="<a class=results href=/eventos/"+respuesta[i].id+"/"+palabra+">"+respuesta[i].titulo+"</a><br>"; 
    }
    
    $("#resultadosBusqueda").html(res);

}