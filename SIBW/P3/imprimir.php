<?php
    require_once "/usr/local/lib/php/vendor/autoload.php";
    include("bd.php");

    $loader = new \Twig\Loader\FilesystemLoader('templates');
    $twig = new \Twig\Environment($loader);

    if(isset($_GET['ev'])){
        $idEv = (int)$_GET['ev'];
    }else{
        $idEv = -1;
    }

    $evento = getEvento($idEv);
    $imgs = getImagenes($idEv);

    echo $twig->render('eventos-imprimir.html', ['evento' => $evento,'imgs' => $imgs]);

?>