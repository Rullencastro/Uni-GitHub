<?php
    include("scripts/bd.php");
    session_start();

    if(isset($_GET['idCom'])){
        $idCom = (int)$_GET['idCom'];
    }else{
        $idCom = -1;
    }

    if(isset($_GET['ev'])){
        $idEv = (int)$_GET['ev'];
    }else{
        $idEv = -1;
    }

    borrarComentario($idCom);

    if($idEv === -1)
        header("Location:  comentarios/0");
    else
        header("Location:  eventos/" . $idEv);

    exit();
?>