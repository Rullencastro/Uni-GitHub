<?php

  $mysqli = new mysqli("mysql", "raul", "aprobar", "SIBW");
  if ($mysqli->connect_errno) {
    echo ("Fallo al conectar: " . $mysqli->connect_error);
  }

  function getEvento($idEv) {
  
    global $mysqli; 
    
    $res = $mysqli->query("SELECT titulo, subtitulo, fecha, texto, enlace,textoEnlace, twitter, facebook  FROM eventos WHERE id=" . $idEv);
    
    $evento = array('titulo' => 'XXX','subtitulo' => 'XXX','fecha'=> 'XXX','texto'=> 'XXX','enlace'=> 'XXX','textoEnlace'=> 'XXX','twitter'=> 'XXX','facebook'=> 'XXX');
    
    if ($res->num_rows > 0) {
      $row = $res->fetch_assoc();
      
      $evento = array('titulo' => $row['titulo'],'subtitulo' => $row['subtitulo'],'fecha'=> $row['fecha'],'texto'=> $row['texto'],'enlace'=> $row['enlace'],'textoEnlace'=>$row['textoEnlace'] ,'twitter'=> $row['twitter'],'facebook'=> $row['facebook']);
    }
    
    return $evento;
  }

  function getImagenes($idEv){
    global $mysqli; 

    $res = $mysqli->query("SELECT img, texto FROM imagenes WHERE id_evento=" . $idEv);
    
    $imagenes = array(array( 'img' => 'XXX','texto' => 'XXX'));
    
    if ($res->num_rows > 0) {
      $imagenes = array();
      foreach ($mysqli->query("SELECT img, texto FROM imagenes WHERE id_evento=" . $idEv) as $row) {
        $imagenes[] = array('img' => $row['img'],'texto' => $row['texto']);
      } 
    }
    return $imagenes;
  }

  function getComentarios($idEv){
    global $mysqli; 

    $res = $mysqli->query("SELECT img, texto FROM imagenes WHERE id_evento=" . $idEv);
    
    $comentarios = array(array( 'nombre' => 'XXX','fecha' => 'XXX','hora' => 'XXX','comentario' => 'XXX'));
    
    if ($res->num_rows > 0) {
      $comentarios = array();
      foreach ($mysqli->query("SELECT nombre,fecha,hora,comentario FROM comentarios WHERE id_evento=" . $idEv) as $row) {
        $comentarios[] = array('nombre' => $row['nombre'],'fecha' => $row['fecha'],'hora' => $row['hora'],'comentario' => $row['comentario']);
      } 
    }
    return $comentarios;
  } 
  
  function getNoticias(){
    global $mysqli; 

    $res = $mysqli->query("SELECT descripcion FROM noticias");
    
    $noticias = array('XXX');
    
    if ($res->num_rows > 0) {
      $noticias = array();
      foreach ($mysqli->query("SELECT descripcion FROM noticias") as $row) {
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
      foreach ($mysqli->query("SELECT palabra FROM palabrasCensuradas") as $row) {
        $censuradas[] = $row['palabra'];
      } 
    }
    return $censuradas;
  }  

?>