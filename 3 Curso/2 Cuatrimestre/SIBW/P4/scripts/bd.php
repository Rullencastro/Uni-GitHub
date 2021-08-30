<?php

  $mysqli = new mysqli("mysql", "raul", "aprobar", "SIBW");
  if ($mysqli->connect_errno) {
    echo ("Fallo al conectar: " . $mysqli->connect_error);
  }

  function getEvento($idEv) {
  
    global $mysqli;
    
    $stmt = $mysqli->prepare("SELECT id,titulo, subtitulo, fecha, texto, enlace,textoEnlace, twitter, facebook  FROM eventos WHERE id= ?");
    $stmt->bind_param("i",$idEv);
    $stmt->execute();
    $res = $stmt->get_result();
    
    $evento = array('id' => 'XXX','titulo' => 'XXX','subtitulo' => 'XXX','fecha'=> 'XXX','texto'=> 'XXX','enlace'=> 'XXX','textoEnlace'=> 'XXX','twitter'=> 'XXX','facebook'=> 'XXX');
    
    if ($res->num_rows > 0) {
      $row = $res->fetch_assoc();
      
      $evento = array('id' => $row['id'] ,'titulo' => $row['titulo'],'subtitulo' => $row['subtitulo'],'fecha'=> $row['fecha'],'texto'=> $row['texto'],'enlace'=> $row['enlace'],'textoEnlace'=>$row['textoEnlace'] ,'twitter'=> $row['twitter'],'facebook'=> $row['facebook']);
    }
    
    return $evento;
  }

  function getImagenes($idEv){
    global $mysqli; 

    $stmt = $mysqli->prepare("SELECT id,img, texto,id_ref FROM imagenes WHERE id_evento= ?");
    $stmt->bind_param("i",$idEv);
    $stmt->execute();
    $res = $stmt->get_result();
    
    $imagenes = array();

    if ($res->num_rows > 0) {
      $imagenes = array();
      foreach ($res as $row) {
        $imagenes[] = array('id' => $row['id'],'img' => $row['img'],'texto' => $row['texto'],'id_ref' => $row['id_ref']);
      } 
    }
    return $imagenes;
  }

  function getImagenPortada($idEv){
    global $mysqli; 

    $stmt = $mysqli->prepare("SELECT id,img, texto,id_ref FROM imagenes WHERE id_ref = ?");
    $stmt->bind_param("i",$idEv);
    $stmt->execute();
    $res = $stmt->get_result();
  
    if ($res->num_rows > 0) {
      $row = $res->fetch_assoc();
      $imagen = array('id' => $row['id'],'img' => $row['img'],'texto' => $row['texto'],'id_ref' => $row['id_ref']);
      
      return $imagen;
    }
  }

  function borrarImg($idImg){
    global $mysqli;

    $stmt2 = $mysqli->prepare("SELECT img FROM imagenes WHERE id=?");
    $stmt2->bind_param("i",$idImg);
    $stmt2->execute();
    $res2 = $stmt2->get_result();
    if($res2->num_rows > 0){
      $row = $res2->fetch_assoc();
        unlink($row['img']);
    }
    
    $stmt = $mysqli->prepare("DELETE FROM imagenes WHERE id=? LIMIT 1");
    $stmt->bind_param("i",$idImg);
    $stmt->execute();
    
    return true;

  }

  function getComentarios($idEv){
    global $mysqli; 

    $stmt = $mysqli->prepare("SELECT id,nombre,fecha,hora,comentario,editado FROM comentarios WHERE id_evento= ?");
    $stmt->bind_param("i",$idEv);
    $stmt->execute();
    $res = $stmt->get_result();

    $comentarios = array();
    
    if ($res->num_rows > 0) {
      $comentarios = array();
      foreach ($res as $row) {
        $comentarios[] = array('id' => $row['id'],'nombre' => $row['nombre'],'fecha' => $row['fecha'],'hora' => $row['hora'],'comentario' => $row['comentario'],'editado' => $row['editado']);
      } 
    }
    return $comentarios;
  }

  function getComentario($idComentario){
    global $mysqli; 

    $stmt = $mysqli->prepare("SELECT nombre,fecha,hora,comentario,editado FROM comentarios WHERE id= ?");
    $stmt->bind_param("i",$idComentario);
    $stmt->execute();
    $res = $stmt->get_result();

    if ($res->num_rows > 0) {
      $row = $res->fetch_assoc();

      $comentario = array('nombre' => $row['nombre'],'fecha' => $row['fecha'],'hora' => $row['hora'],'comentario' => $row['comentario'],'editado' => $row['editado']);
      return $comentario;
    }
  }

  function getComentariosAll(){
    global $mysqli; 

    $stmt = $mysqli->prepare("SELECT id,nombre,fecha,hora,comentario,editado FROM comentarios");
    $stmt->execute();
    $res = $stmt->get_result();

    $comentarios = array();
    
    if ($res->num_rows > 0) {
      $comentarios = array();
      foreach ($res as $row) {
        $comentarios[] = array('id' => $row['id'],'nombre' => $row['nombre'],'fecha' => $row['fecha'],'hora' => $row['hora'],'comentario' => $row['comentario'],'editado' => $row['editado']);
      } 
    }
    return $comentarios;
  }
  
  function guardarComentario($nombre,$fecha,$hora,$comentario,$idEv){
    global $mysqli;
    
    $stmt = $mysqli->prepare("INSERT INTO comentarios(nombre,fecha,hora,comentario,id_evento,editado) VALUES (?,?,?,?,?,false)");
    $stmt->bind_param("ssssi",$nombre,$fecha,$hora,$comentario,$idEv);
    $stmt->execute();
    
    return true;
  }

  function editarComentario($idCom,$comentario){
    global $mysqli;
    
    $stmt = $mysqli->prepare("UPDATE comentarios SET comentario=? ,editado=true WHERE id=?");
    $stmt->bind_param("si",$comentario,$idCom);
    $stmt->execute();
    
    return true;
  }

  function borrarComentario($idCom){
    global $mysqli;
    
    $stmt = $mysqli->prepare("DELETE FROM comentarios WHERE id=? LIMIT 1");
    $stmt->bind_param("i",$idCom);
    $stmt->execute();
    
    return true;
  }
  
  function getNoticias(){
    global $mysqli; 

    $res = $mysqli->query("SELECT descripcion FROM noticias");
    
    $noticias = array('XXX');
    
    if ($res->num_rows > 0) {
      $noticias = array();
      foreach ($res as $row) {
        $noticias[] = $row['descripcion'];
      } 
    }
    return $noticias;
  } 
  
  function getCensuradas(){
    global $mysqli; 

    $res = $mysqli->query("SELECT palabra FROM palabrasCensuradas");
    
    $censuradas = array('XXX');
    
    if ($res->num_rows > 0) {
      $censuradas = array();
      foreach ($res as $row) {
        $censuradas[] = $row['palabra'];
      } 
    }
    return $censuradas;
  }

  // Devuelve true si existe un usuario con esa contraseña
  function checkLogin($nick, $pass){
    global $mysqli;
    
    $stmt = $mysqli->prepare("SELECT pass FROM usuarios WHERE nick= ?");
    $stmt->bind_param("s",$nick);
    $stmt->execute();
    $usuario = $stmt->get_result();
    
    if ($usuario->num_rows > 0){
      $row = $usuario->fetch_assoc();
      if (password_verify($pass, $row['pass'] )) {
          return true;
      }
      return false;
    }
    
    return false;
  }

  function registrar($nick,$nombre,$pass,$email){
    global $mysqli;

    $stmt0 = $mysqli->prepare("SELECT nombre FROM usuarios WHERE nick= ?");
    $stmt0->bind_param("s",$nick);
    $stmt0->execute();
    $compr = $stmt0->get_result();

   if ($compr->num_rows === 0){
      $stmt = $mysqli->prepare("INSERT INTO usuarios(nick,nombre,pass,email,tipo) VALUES (?,?,?,?,0)");
      $stmt->bind_param("ssss",$nick,$nombre,password_hash($pass,PASSWORD_DEFAULT),$email);
      $stmt->execute();
      return true;
    }else{
      return false;
    }
 }

 function modificarDatos($oldNick,$nick,$nombre,$pass,$email){
  global $mysqli;

  $stmt0 = $mysqli->prepare("SELECT nombre FROM usuarios WHERE nick= ?");
  $stmt0->bind_param("s",$nick);
  $stmt0->execute();
  $compr = $stmt0->get_result();

 if ($compr->num_rows === 0){
    $stmt = $mysqli->prepare("UPDATE usuarios SET nick= ?, nombre= ?, pass= ?, email= ? WHERE nick= ?");
    $stmt->bind_param("sssss",$nick,$nombre,password_hash($pass,PASSWORD_DEFAULT),$email,$oldNick);
    $stmt->execute();
    return true;
  }else{
    return false;
  }
}

function borrarDatos($nick) {
  global $mysqli;
  
  $stmt = $mysqli->prepare("DELETE FROM usuarios WHERE nick=? LIMIT 1");
  $stmt->bind_param("s",$nick);
  $stmt->execute();
  
}


  
  // Devuelve la información de un usuario a partir de su nick 
  function getUser($nick) {
    global $mysqli;
    
    $stmt = $mysqli->prepare("SELECT * FROM usuarios WHERE nick= ?");
    $stmt->bind_param("s",$nick);
    $stmt->execute();
    $res = $stmt->get_result();

    if($res->num_rows > 0){
      $row = $res->fetch_assoc();

      $usuario = array('nick' => $row['nick'],'nombre' => $row['nombre'],'pass' => $row['pass'],'email' => $row['email'],'tipo' => $row['tipo']);
      return $usuario;
    }

    return 0;
  }

  function getUsers() {
    global $mysqli;
    
    $stmt = $mysqli->prepare("SELECT * FROM usuarios");
    $stmt->execute();
    $res = $stmt->get_result();

    if($res->num_rows > 0){
      $usuarios = array();
      foreach ($res as $row) {
        $usuarios[] = array('nick' => $row['nick'],'nombre' => $row['nombre'],'pass' => $row['pass'],'email' => $row['email'],'tipo' => $row['tipo']);
      } 
      return $usuarios;
    }

    return 0;
  }

  function addEvento($titulo,$subtitulo,$fecha,$texto,$enlace,$textoEnlace,$twitter,$facebook){
    global $mysqli;
    
    $stmt = $mysqli->prepare("INSERT INTO eventos(titulo,subtitulo,fecha,texto,enlace,textoEnlace,twitter,facebook) VALUES (?,?,?,?,?,?,?,?)");
    $stmt->bind_param("ssssssss",$titulo,$subtitulo,$fecha,$texto,$enlace,$textoEnlace,$twitter,$facebook);
    $stmt->execute();
    $res = $stmt->get_result();
  }

  function addImagen($img,$texto,$id_evento,$id_ref){
    global $mysqli;
    
    $stmt = $mysqli->prepare("INSERT INTO imagenes(img,texto,id_evento,id_ref) VALUES (?,?,?,?)");
    $stmt->bind_param("ssii",$img,$texto,$id_evento,$id_ref);
    $stmt->execute();
    $res = $stmt->get_result();
  }

  function obtenerMaxIdEv(){
    global $mysqli;

    $stmt = $mysqli->prepare("SELECT MAX(id) AS id FROM eventos");
    $stmt->execute();
    $res = $stmt->get_result();

    if($res->num_rows > 0){
      $row = $res->fetch_assoc();
      return $row['id'];
    }
  }

  function borrarEvento($idEv){
    global $mysqli;
  
    $stmt = $mysqli->prepare("DELETE FROM eventos WHERE id=? LIMIT 1");
    $stmt->bind_param("i",$idEv);
    $stmt->execute();

    $stmt0 = $mysqli->prepare("SELECT img FROM imagenes WHERE id_evento=?");
    $stmt0->bind_param("i",$idEv);
    $stmt0->execute();
    $res = $stmt0->get_result();
    if($res->num_rows > 0){
      foreach ($res as $row) {
        unlink($row['img']);
      } 
    }

    $stmt1 = $mysqli->prepare("DELETE FROM imagenes WHERE id_evento=? LIMIT 4");
    $stmt1->bind_param("i",$idEv);
    $stmt1->execute();

    $stmt2 = $mysqli->prepare("SELECT img FROM imagenes WHERE id_ref=?");
    $stmt2->bind_param("i",$idEv);
    $stmt2->execute();
    $res2 = $stmt2->get_result();
    if($res2->num_rows > 0){
      $row = $res2->fetch_assoc();
        unlink($row['img']);
    }

    $stmt3 = $mysqli->prepare("DELETE FROM imagenes WHERE id_ref=? LIMIT 1");
    $stmt3->bind_param("i",$idEv);
    $stmt3->execute();
  }

  function editEvento($idEv,$titulo,$subtitulo,$fecha,$texto,$enlace,$textoEnlace,$twitter,$facebook){
    global $mysqli;
    
    $stmt = $mysqli->prepare("UPDATE eventos SET titulo=?, subtitulo=?, fecha=?, texto=?, enlace=?, textoEnlace=?, twitter=?, facebook=? WHERE id=?");
    $stmt->bind_param("ssssssssi",$titulo,$subtitulo,$fecha,$texto,$enlace,$textoEnlace,$twitter,$facebook,$idEv);
    $stmt->execute();
    $res = $stmt->get_result();

  }

  function editImgPortada($img,$texto,$idRef){
    global $mysqli;

    $stmt2 = $mysqli->prepare("SELECT img FROM imagenes WHERE id_ref=?");
    $stmt2->bind_param("i",$idRef);
    $stmt2->execute();
    $res2 = $stmt2->get_result();
    if($res2->num_rows > 0){
      $row = $res2->fetch_assoc();
        unlink($row['img']);
    }
    
    $stmt = $mysqli->prepare("UPDATE imagenes SET img=?, texto=? WHERE id_ref=?");
    $stmt->bind_param("ssi",$img,$texto,$idRef);
    $stmt->execute();
    
  }

  function editImg($img,$texto,$idImg){
    global $mysqli;

    $stmt2 = $mysqli->prepare("SELECT img FROM imagenes WHERE id=?");
    $stmt2->bind_param("i",$idImg);
    $stmt2->execute();
    $res2 = $stmt2->get_result();
    if($res2->num_rows > 0){
      $row = $res2->fetch_assoc();
        unlink($row['img']);
    }
    
    $stmt = $mysqli->prepare("UPDATE imagenes SET img=?, texto=? WHERE id=?");
    $stmt->bind_param("ssi",$img,$texto,$idImg);
    return $stmt->execute();
    
  }

  function cambiarPermisos($permisos,$nick){
    global $mysqli;
    
    $stmt = $mysqli->prepare("UPDATE usuarios SET tipo=? WHERE nick=?");
    $stmt->bind_param("is",$permisos,$nick);
    $stmt->execute();
    
  }
  
  function addEtiqueta($etiq,$idEv){
    global $mysqli;
    
    $stmt = $mysqli->prepare("INSERT INTO etiquetas(etiq,id_evento) VALUES (?,?)");
    $stmt->bind_param("si",$etiq,$idEv);
    $stmt->execute();
    $res = $stmt->get_result();
  }

  function getEtiquetas($idEv){
    global $mysqli;
    
    $stmt = $mysqli->prepare("SELECT id,etiq FROM etiquetas WHERE id_evento= ?");
    $stmt->bind_param("i",$idEv);
    $stmt->execute();
    $res = $stmt->get_result();
    
    $etiquetas = array();
    if ($res->num_rows > 0) {
      foreach ($res as $row) {
        $etiquetas[] = array('id' => $row['id'],'etiq' => $row['etiq']);
      } 
    }
    return $etiquetas;
  }

  function editEtiqueta($id,$etiq){
    global $mysqli;
    
    $stmt = $mysqli->prepare("UPDATE etiquetas SET etiq=? WHERE id=?");
    $stmt->bind_param("si",$etiq,$id);
    $stmt->execute();
  }

  function borrarEtiqueta($id){
    global $mysqli;
    
    $stmt = $mysqli->prepare("DELETE FROM etiquetas WHERE id=? LIMIT 1");
    $stmt->bind_param("i",$id);
    $stmt->execute();
  }

  function comprobarSuperusuarios(){
    global $mysqli;
    
    $stmt = $mysqli->prepare("SELECT * FROM usuarios WHERE tipo=3");
    $stmt->execute();
    $res = $stmt->get_result();
    return $res->num_rows;
  }

?>