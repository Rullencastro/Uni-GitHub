<?php
    require_once "/usr/local/lib/php/vendor/autoload.php";
    include("scripts/bd.php");

    $loader = new \Twig\Loader\FilesystemLoader('templates');
    $twig = new \Twig\Environment($loader);

    $noticias = getNoticias();
    $imgs = getImagenes(0);

    /*function startsWith($string,$query){
        return substr($string,0,strlen($query)) === $query;
    }

    $uri = $_SERVER['REQUEST_URI'];
    
    if (startsWith($uri,"/eventos")){
        include("scripts/eventos.php");
    }else{
        echo $twig->render('portada.html', ['noticias' => $noticias]);
    }*/

    echo $twig->render('portada.html', ['noticias' => $noticias,'imgs' => $imgs]);
?>