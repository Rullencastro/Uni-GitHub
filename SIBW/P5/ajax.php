<?php
    require_once "/usr/local/lib/php/vendor/autoload.php";
    include("scripts/bd.php");
    
    header('Content-Type: application/json');

    $palabra = $_GET['palabra'];

    session_start();

    $datos = array();
    
    if(isset($_SESSION['nick'])){
        $user = getUser($_SESSION['nick']);
        if($user['tipo'] == 2 || $user['tipo'] == 3)
            $datos = busquedaGeneral($palabra);
        else
            $datos = busquedaPublicado($palabra);
    }else
        $datos = busquedaPublicado($palabra);
    
    echo(json_encode($datos));

?>