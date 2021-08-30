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

    $usuarios = getUsers();
    $errores = array();
    
    if ($_SERVER['REQUEST_METHOD'] === 'POST') {
        if(isset($_POST['normalButton'])){
            $aux = getUser($_SESSION['userSel']);
            //$numSupers = comprobarSuperusuarios();
            if($aux['tipo'] == 3 && comprobarSuperusuarios() == 1){
                $errores[] = "El sistema debe tener al menos un Superusuario";
            }
            else{
                cambiarPermisos(0,$_SESSION['userSel']);
                if($user['nick'] == $aux['nick']){
                    header("Location: /index");
                }
                else{
                    header("Location: ");
                }
            }
        }
        if(isset($_POST['moderButton'])){
            $aux = getUser($_SESSION['userSel']);
            if($aux['tipo'] == 3 && comprobarSuperusuarios() == 1){
                $errores[] = "El sistema debe tener al menos un Superusuario";
            }
            else{
                cambiarPermisos(1,$_SESSION['userSel']);
                if($user['nick'] == $aux['nick']){
                    header("Location: /index");
                }
                else{
                    header("Location: ");
                }
            }
        }
        if(isset($_POST['gestButton'])){
            $aux = getUser($_SESSION['userSel']);
            if($aux['tipo'] == 3 && comprobarSuperusuarios() == 1){
                $errores[] = "El sistema debe tener al menos un Superusuario";
            }
            else{
                cambiarPermisos(2,$_SESSION['userSel']);
                if($user['nick'] == $aux['nick']){
                    header("Location: /index");
                }
                else{
                    header("Location: ");
                }
            }
        }
        if(isset($_POST['superButton'])){
            cambiarPermisos(3,$_SESSION['userSel']);
            header("Location: ");
        }

        $userSel = getUser($_SESSION['userSel']);
        $twig->addGlobal('userSel',$userSel);
        
        
        for ($i = 0; $i < count($usuarios); $i++){
            $butname = "selectButton" . $i;
            if(isset($_POST[$butname])){
                $_SESSION['userSel'] = $usuarios[$i]['nick']; 
                $userSel = getUser($usuarios[$i]['nick']);
                $twig->addGlobal('userSel',$userSel);
                header("Location: ");
            }
        }      
    } 
    
    $usuarios = getUsers();
    $twig->addGlobal('usuarios',$usuarios);
    
    echo $twig->render('permisos.html', ['errores' => $errores]);
    ?>