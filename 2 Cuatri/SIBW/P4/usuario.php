<?php
    require_once "/usr/local/lib/php/vendor/autoload.php";
    include("scripts/bd.php");

    $loader = new \Twig\Loader\FilesystemLoader('templates');
    $twig = new \Twig\Environment($loader);

    $errores = array();
    
    if ($_SERVER['REQUEST_METHOD'] === 'POST') {

        if(isset($_POST['inicioButton'])){
            if(!empty($_POST['user']) && !empty($_POST['pass'])){
                $nick = $_POST['user'];
                $pass = $_POST['pass'];
            
                if (checkLogin($nick, $pass)) {
                    session_start();
                    $_SESSION['nick'] = $nick; 
                    header("Location: index");
                    exit();
                }
                else{
                    header("Location: ");
                    $errores[] = "Error, usuario o contraseña incorrectos";
                }
            }
            else{
                $errores[] = "Debes rellenar todos los campos para iniciar sesión";
            }
        }

        else if(isset($_POST['registroButton'])){
            if(!empty($_POST['username']) && !empty($_POST['nombre']) && !empty($_POST['password']) && !empty($_POST['email'])){
                if(preg_match("/^([\w_\.-]+)@([\w-]+)\.([a-z\.]{2,6})$/",$_POST['email'])){
                    if(registrar($_POST['username'],$_POST['nombre'],$_POST['password'],$_POST['email'])){
                        session_start();
                        $_SESSION['nick'] = $_POST['username']; 
                        header("Location: index");
                        exit();
                    }
                    else{
                        $errores[] = "El nickname utilizado como nombre de usuario ya existe, use otro por favor"; 
                    }
                }
                else{
                    header("Location: ");
                    $errores[] = "El formato del email no es correcto, revíselo por favor";
                }      
            }
            else{
                header("Location: ");
                $errores[] = "Debes rellenar todos los campos para poder registrarte";
            }
        }
    }

    echo $twig->render('usuario.html', ['errores' => $errores]);
?>