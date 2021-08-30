<?php
    require_once "/usr/local/lib/php/vendor/autoload.php";
    include("scripts/bd.php");

    $loader = new \Twig\Loader\FilesystemLoader('templates');
    $twig = new \Twig\Environment($loader);

    $errores = array();

    session_start();

    if(isset($_SESSION['nick'])){
        $user = getUser($_SESSION['nick']);
        $twig->addGlobal('user',$user);
    }

    if ($_SERVER['REQUEST_METHOD'] === 'POST') {
        if(isset($_POST['modificarButton'])){
            if(!empty($_POST['userM']) && !empty($_POST['nombreM']) && !empty($_POST['passwordM']) && !empty($_POST['emailM'])){
                if(preg_match("/^([\w_\.-]+)@([\w-]+)\.([a-z\.]{2,6})$/",$_POST['emailM'])){
                    if(modificarDatos($user['nick'],$_POST['userM'],$_POST['nombreM'],$_POST['passwordM'],$_POST['emailM'])){
                        session_start();
                        $_SESSION['nick'] = $_POST['userM']; 
                        header("Location: index.php");
                        exit();
                    }
                    else{
                        $errores[] = "El nickname utilizado como nombre de usuario ya existe, use otro por favor"; 
                    }
                }
                else{
                    //header("Location: ");
                    $errores[] = "El formato del email no es correcto, revíselo por favor";
                } 
            }
            else{
                $errores[] = "Debes rellenar todos los campos para modificar los datos";
            }
        }

        else if(isset($_POST['eliminarButton'])){
            if(!empty($_POST['delete'])){
                if($_POST['delete'] === "BORRAR"){
                    borrarDatos($user['nick']);
                    header("Location: index.php");
                }
                else{
                    //header("Location: ");
                    $errores[] = "En el cuadro de texto se debe escribir 'BORRAR' para confirmar la eliminación de la cuenta";
                }      
            }
            else{
                header("Location: ");
                $errores[] = "Debes rellenar el campo con la palabra 'BORRAR' para confirmar la eliminación de la cuenta";
            }
        }
    }

    

    echo $twig->render('cuenta.html', ['errores' => $errores]);
?>