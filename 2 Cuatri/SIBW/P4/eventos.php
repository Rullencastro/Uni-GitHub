<?php
    require_once "/usr/local/lib/php/vendor/autoload.php";
    
    include("scripts/bd.php");
    
    $loader = new \Twig\Loader\FilesystemLoader('templates');
    $twig = new \Twig\Environment($loader);

    date_default_timezone_set("Europe/Madrid");
    
    if(isset($_GET['ev'])){
        $idEv = (int)$_GET['ev'];
    }else{
        $idEv = -1;
    }
    
    $censuradas = getCensuradas();
    
    $evento = getEvento($idEv);
    $imgs = getImagenes($idEv);
    $comentarios = getComentarios($idEv);
    $noticias = getNoticias();
    $etiquetas = getEtiquetas($idEv);
    
    session_start();
    
    if(isset($_SESSION['nick'])){
        $user = getUser($_SESSION['nick']);
        $twig->addGlobal('user',$user);
    }
    if ($_SERVER['REQUEST_METHOD'] === 'POST') {
        if(isset($_POST['comentarButton'])){
            if(!empty($_POST['comText']) ){
                $fecha =  date('j-n-Y');
                $hora = date('H:i');
                if(guardarComentario($user['nombre'],$fecha,$hora,$_POST['comText'],$_POST['ev'])){
                    header("Location: /eventos/" . $_POST['ev']);
                }
                else{
                    $errores[] = "Ha habido un error a la hora de guardar su comentario"; 
                }   
            }
        }
        if(isset($_POST['borrarEvButton'])){
            borrarEvento($idEv);
            header("Location: /index");
            exit();
        }
        if(isset($_POST['editarEvButton'])){
            header("Location: /editEvento/" . $idEv);
            exit();
        }
    }

    echo $twig->render('eventos.html', ['evento' => $evento,'imgs' => $imgs,'coment' => $comentarios,'noticias' => $noticias,'etiquetas' => $etiquetas]);
?>

<script type="text/javascript">
    var censuradas=<?php echo json_encode($censuradas);?>;
</script>