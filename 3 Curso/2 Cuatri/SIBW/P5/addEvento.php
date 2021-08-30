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

    $errores = array();
    function verificarImg($img){
        global $errores;
        $file_name = $_FILES[$img]['name'];
        $file_size = $_FILES[$img]['size'];
        $file_tmp = $_FILES[$img]['tmp_name'];
        $file_type = $_FILES[$img]['type'];
        $file_ext = strtolower(end(explode('.',$_FILES[$img]['name'])));

        $extensiones = array("jpeg","jpg","png");

        if(in_array($file_ext,$extensiones) === false){
            $errores[] = "Extensión no permitida, solo se permite JPEG, JPG y PNG";
        }

        if($file_size > 2097152){
            $errores[] = "El fichero es demasiado grande";
        }

        if(empty($errores)){
            move_uploaded_file($file_tmp,"imgs/" . $file_name);
            return true;
        }

        if(sizeof($errores) > 0){
            return false;
        }
    }


    if ($_SERVER['REQUEST_METHOD'] === 'POST') {
        if(isset($_POST['addEventButton'])){
            if(!empty($_POST['titulo'])&&!empty($_POST['subtitulo'])&&!empty($_POST['fecha'])&&!empty($_POST['textoEvento'])&&!empty($_POST['enlace'])&&!empty($_POST['textoEnlace'])&&!empty($_POST['twitter'])&&!empty($_POST['facebook']) && !empty($_FILES['img0']['name']) && !empty($_POST['text0'])){
                $titulo = $_POST['titulo'];
                $subtitulo = $_POST['subtitulo'];
                $fecha = $_POST['fecha'];
                $textoEvento = $_POST['textoEvento'];
                $enlace = $_POST['enlace'];
                $textoEnlace = $_POST['textoEnlace'];
                $twitter = $_POST['twitter'];
                $facebook = $_POST['facebook'];

                if(verificarImg("img0")){
                    addEvento($titulo,$subtitulo,$fecha,$textoEvento,$enlace,$textoEnlace,$twitter,$facebook);
                    $idEvNuevo = obtenerMaxIdEv();
                    addImagen("imgs/" . $_FILES['img0']['name'],$_POST['text0'],0,$idEvNuevo);

                    for ($i = 0; $i < 3; $i++){
                        $et = "etiq" . $i;
                        if(!empty($_POST[$et])){
                            addEtiqueta($_POST[$et],$idEvNuevo);
                        }
                    }

                    for ($i = 1; $i < 4; $i++) {
                        $im = 'img' . $i;
                        $tx = 'text' . $i;
                        if(!empty($_FILES[$im]['name'])){
                            if(verificarImg($im)){
                                addImagen("imgs/" . $_FILES[$im]['name'],$_POST[$tx],$idEvNuevo,NULL);
                            }
                            else{
                                $errores[] = "Error con la imagen opcional número " . $i;
                            }
                        }
                    }
                }
                else{
                    $errores[] = "Error con la imagen de Portada";
                }

            }
            else{
                $errores[] = "Debes rellenar todos los campos, incluido el de la foto de la portada y su texto";
            }
        }

    }

    echo $twig->render('addEvento.html', ['errores' => $errores]);
?>