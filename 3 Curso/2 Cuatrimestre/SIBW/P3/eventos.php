<?php
    require_once "/usr/local/lib/php/vendor/autoload.php";

    include("scripts/bd.php");
    
    $loader = new \Twig\Loader\FilesystemLoader('templates');
    $twig = new \Twig\Environment($loader);

    if(isset($_GET['ev'])){
        $idEv = (int)$_GET['ev'];
    }else{
        $idEv = -1;
    }

    $censuradas = array('puta','cabron','pene','tonto','coño','mierda','subnormal','gilipollas','imbecil','idiota');

    $evento = getEvento($idEv);
    $imgs = getImagenes($idEv);
    $comentarios = getComentarios($idEv);
    $noticias = getNoticias();

    echo $twig->render('eventos.html', ['evento' => $evento,'imgs' => $imgs,'coment' => $comentarios,'noticias' => $noticias]);
?>

<script type="text/javascript">
    var censuradas=<?php echo json_encode($censuradas);?>;
</script>