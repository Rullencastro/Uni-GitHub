<?php
    require_once "/usr/local/lib/php/vendor/autoload.php";
    include("scripts/bd.php");

    $loader = new \Twig\Loader\FilesystemLoader('templates');
    $twig = new \Twig\Environment($loader);
    
    session_start();
    
    if(isset($_SESSION['nick'])){
        $user = getUser($_SESSION['nick']);
        $twig->addGlobal('user',$user);
    }

    if(isset($_GET['idCom'])){
        $idCom = (int)$_GET['idCom'];
    }else{
        $idCom = -1;
    }

    $com = getComentario($idCom);
    $comentarios = getComentariosAll();

    if ($_SERVER['REQUEST_METHOD'] === 'POST') {
        if(isset($_POST['editarButton'])){
            if(!empty($_POST['editText']) ){
                if(editarComentario($idCom,$_POST['editText'])){
                    header("Location: /comentarios/" . $idCom);
                }
                else{
                    $errores[] = "Ha habido un error a la hora de editar su comentario"; 
                }   
            }
        }
    }


    echo $twig->render('comentarios.html', ['com' => $com,'comentarios' => $comentarios,'errores' => $errores]);
?>