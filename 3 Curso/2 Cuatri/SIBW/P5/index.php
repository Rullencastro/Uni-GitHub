<?php
    require_once "/usr/local/lib/php/vendor/autoload.php";
    include("scripts/bd.php");

    $loader = new \Twig\Loader\FilesystemLoader('templates');
    $twig = new \Twig\Environment($loader);

    $noticias = getNoticias();
    $imgs = getImagenes(0);
    
    session_start();
    
    if(isset($_SESSION['nick'])){
        $user = getUser($_SESSION['nick']);
        if($user['tipo'] === 2 or $user['tipo'] === 3){
            $imgs = getImagenesSinFiltro(0);
        }
        $twig->addGlobal('user',$user);
    }
    


    echo $twig->render('portada.html', ['noticias' => $noticias,'imgs' => $imgs]);
?>