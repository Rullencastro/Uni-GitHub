
// Clases de la biblioteca

import * as THREE from '../libs/three.module.js'
import * as TWEEN from '../libs/tween.esm.js'
import { TrackballControls } from '../libs/TrackballControls.js'
import { WipeIn } from './WipeIn.js'

const listener = new THREE.AudioListener();
const sound = new THREE.Audio(listener);
const muerte = new THREE.Audio(listener);
const audioLoader = new THREE.AudioLoader();

class MyScene extends THREE.Scene {
  
  constructor (myCanvas) { 
    super();
    
    this.renderer = this.createRenderer(myCanvas);
    
    this.createLights ();
    
    this.createCamera ();

    this.cargarMusicaFondo();
    
    //this.axis = new THREE.AxesHelper (5);
    //this.add (this.axis);
    
    this.model = new WipeIn();
    this.add (this.model);
    
    this.alturaSuelo = 2.3;

    var that = this;
    var origen = { y: this.alturaSuelo};
    var destino = {y: this.alturaSuelo+5};
    this.salto = new TWEEN.Tween(origen).to(destino,400).easing(TWEEN.Easing.Quadratic.InOut)
      .onUpdate(function(){
        that.model.personaje.position.y = origen.y;
      }).repeat(1).yoyo(true);


    this.px = 0;
    this.py = this.alturaSuelo;
    this.pz = 0;
    this.checkX = 0;
    this.checkY = this.alturaSuelo;
    this.checkZ = 0;
    this.w = false;
    this.a = false;
    this.d = false;
    this.s = false;
    this.space = false;
    this.orientacion="NORTE";
    this.AnimAndar = false;
    this.empezar = true;
    this.terminar = false;
    this.vida = 3;
    this.colorPersonaje = 0;
    this.volumen = 0.05;
    this.mute = false;
  }
  
  createCamera () {
    this.camera = new THREE.PerspectiveCamera(45, window.innerWidth / window.innerHeight, 0.1, 1000);
    // También se indica dónde se coloca
    this.camera.position.set (0, 10, -15);
    // Y hacia dónde mira
    var look = new THREE.Vector3 (this.px,this.py,this.pz);
    this.camera.lookAt(look);
    this.add (this.camera);
    
    // Para el control de cámara usamos una clase que ya tiene implementado los movimientos de órbita
    this.cameraControl = new TrackballControls (this.camera, this.renderer.domElement);
    
    // Se configuran las velocidades de los movimientos
    this.cameraControl.rotateSpeed = 5;
    this.cameraControl.zoomSpeed = -2;
    this.cameraControl.panSpeed = 0.5;
    // Debe orbitar con respecto al punto de mira de la cámara
    this.cameraControl.target = look;
  }

  cargarMusicaFondo(){
    audioLoader.load('sonido/musicaFondo.mp3', function( buffer ) {
      sound.setBuffer( buffer );
      sound.setLoop( true );
      sound.setVolume( 0.05 );
      sound.play();
    });
  }

  
  modificarMusica(opcion){
    if(opcion == 0 && !this.mute)
      this.volumen += 0.05;
    else if (opcion == 1 && this.volumen >= 0 && !this.mute)
      this.volumen -= 0.05;
      if(this.volumen < 0)
        this.volumen = 0;
    if(opcion == 3){
      this.mute = false;
      document.getElementById("activar").style.display = "none";
      document.getElementById("desactivar").style.display = "block";
    }    
    if(opcion != 2 && !this.mute){  
      sound.setVolume(this.volumen);
    }
    else{
      this.mute = true;
      document.getElementById("activar").style.display = "block";
      document.getElementById("desactivar").style.display = "none";
      sound.setVolume(0);
    }
  }


  sonidoMuerte(){
    audioLoader.load('sonido/muerte.mp3', function( buffer ) {
      muerte.setBuffer( buffer );
      muerte.setLoop( false );
      muerte.setVolume( 0.5 );
      muerte.play();
    });
  }
  
  
  createLights () {
  
    var ambientLight = new THREE.AmbientLight(0xccddee, 0.35);
    this.add (ambientLight);
    
    this.spotLight = new THREE.SpotLight( 0xffffff, 0.5 );
    this.spotLight.position.set( 30, 60, 120 );
    this.spotLight.angle = 180;

    this.spotLight.castShadow = true;
    this.spotLight.shadow.mapSize.width = 512;
    this.spotLight.shadow.mapSize.height = 512;
    this.spotLight.shadow.camera.near = 0.5;
    this.spotLight.shadow.camera.far = 500;

    this.add (this.spotLight);
  }
  
  createRenderer (myCanvas) {
    // Se recibe el lienzo sobre el que se van a hacer los renderizados. Un div definido en el html.
    
    // Se instancia un Renderer   WebGL
    var renderer = new THREE.WebGLRenderer();
    
    // Se establece un color de fondo en las imágenes que genera el render
    renderer.setClearColor(new THREE.Color(0xEEEEEE), 1.0);
    
    // Se establece el tamaño, se aprovecha la totalidad de la ventana del navegador
    renderer.setSize(window.innerWidth, window.innerHeight);

    renderer.shadowMap.enabled = true;
    renderer.shadowMap.type = THREE.PCFSoftShadowMap;
    // La visualización se muestra en el lienzo recibido
    $(myCanvas).append(renderer.domElement);
    
    return renderer;  
  }
  
  getCamera () {
    return this.camera;
  }
  
  setCameraAspect (ratio) {
    // Cada vez que el usuario modifica el tamaño de la ventana desde el gestor de ventanas de
    // su sistema operativo hay que actualizar el ratio de aspecto de la cámara
    this.camera.aspect = ratio;
    // Y si se cambia ese dato hay que actualizar la matriz de proyección de la cámara
    this.camera.updateProjectionMatrix();
  }
    
  onWindowResize () {
    // Este método es llamado cada vez que el usuario modifica el tamapo de la ventana de la aplicación
    // Hay que actualizar el ratio de aspecto de la cámara
    this.setCameraAspect (window.innerWidth / window.innerHeight);
    
    // Y también el tamaño del renderizador
    this.renderer.setSize (window.innerWidth, window.innerHeight);
  }

  empezarPartida(){
    this.empezar = false;
    this.terminar = false;
    this.vida = 3;
    document.getElementById("init").style.display = "none";
    document.getElementById("vida1").style.display = "block";
    document.getElementById("vida2").style.display = "block";
    document.getElementById("vida3").style.display = "block";
  }

  volverAJugar(){
    this.terminar = false;
    this.vida = 3;
    document.getElementById("gameover").style.display = "none";
    document.getElementById("win").style.display = "none";
    document.getElementById("vida1").style.display = "block";
    document.getElementById("vida2").style.display = "block";
    document.getElementById("vida3").style.display = "block";
  }

  volverMenu(){
    this.vida = 3;
    document.getElementById("gameover").style.display = "none";
    document.getElementById("win").style.display = "none";
    document.getElementById("help").style.display = "none";
    document.getElementById("init").style.display = "block";
  }

  ayuda(){
    document.getElementById("init").style.display = "none";
    document.getElementById("help").style.display = "block";
  }

  
  cambiarColorPersonaje(){
    this.colorPersonaje++;
    this.colorPersonaje = this.colorPersonaje % 4;
    if(this.colorPersonaje == 0){
      document.getElementById("c3").style.display = "none";
      document.getElementById("c0").style.display = "block";
    }else if(this.colorPersonaje == 1){
      document.getElementById("c0").style.display = "none";
      document.getElementById("c1").style.display = "block";
    }else if(this.colorPersonaje == 2){
      document.getElementById("c1").style.display = "none";
      document.getElementById("c2").style.display = "block";
    }else if(this.colorPersonaje == 3){
      document.getElementById("c2").style.display = "none";
      document.getElementById("c3").style.display = "block";
    }
    this.model.personaje.cambiarColor(this.colorPersonaje);
  }

  update () {

    if(this.vida == 2){
      document.getElementById("vida3").style.display = "none";
    }

    if(this.vida == 1){
      document.getElementById("vida2").style.display = "none";
    }

    if(this.vida == 0){
      document.getElementById("vida1").style.display = "none";
    }

    if(this.vida <= 0){
        this.terminar = true;
        this.px = 0;
        this.py = this.alturaSuelo;
        this.pz = 0;
        this.checkX = 0;
        this.checkY = this.alturaSuelo;
        this.checkZ = 0;
        this.model.personaje.position.set(0,this.alturaSuelo,0);

      document.getElementById("gameover").style.display = "block";
    }

    if(this.pz >= this.model.recorrido.posCheck()){
      this.checkX = 0;
      this.checkY = this.alturaSuelo;
      this.checkZ = this.model.recorrido.posCheck()+2.5;
    }

    if(this.pz >= this.model.recorrido.posMeta()){
      this.terminar = true;
      this.px = 0;
      this.py = this.alturaSuelo;
      this.pz = 0;
      this.checkX = 0;
      this.checkY = this.alturaSuelo;
      this.checkZ = 0;
      this.model.personaje.position.set(0,this.alturaSuelo,0);
      document.getElementById("win").style.display = "block";
      document.getElementById("vida3").style.display = "none";
      document.getElementById("vida2").style.display = "none";
      document.getElementById("vida1").style.display = "none";
    }

    if(this.model.bordesMapaLaterales() || this.model.bordesMapaVerticales()){
      if(this.w){
        this.w = false;
        if(this.pz > 52.5)
        this.pz -= 1;
      }
      if(this.s){
        this.s = false;
        if(this.pz < -2.5)
        this.pz += 1;
      }
      if(this.d){
        this.d = false;
        if(this.px < -10)
        this.px += 1;
      }
      if(this.a){
        this.a = false;
        if(this.px > 10)
          this.px -= 1;
      }
    }
    // Le decimos al renderizador "visualiza la escena que te indico usando la cámara que te estoy pasando"
    this.renderer.render (this, this.getCamera());

    if(this.model.colisiones(this.checkX,this.checkY,this.checkZ)){
      this.sonidoMuerte();
      this.vida = this.vida - 1;
      this.px = this.checkX;
      this.py = this.checkY;
      this.pz = this.checkZ;
    }
    
    this.animacionPersonaje();
    
    this.spotLight.intensity = 1;
    
    //this.axis.visible = true;
    
    var look = new THREE.Vector3 (this.px,this.py,this.pz+5);
    this.cameraControl.target = look;
    this.camera.position.set(this.px,this.py+10,this.pz+(-15));
    this.cameraControl.update();
    
    this.model.update();
    
    requestAnimationFrame(() => this.update())
  }

  onKeyDown(event){
    var tecla = event.which || event.keyCode;
    if((tecla == 119 || tecla == 87)  && !this.model.bordesMapaVerticales()){ //ADELANTE
      if(!this.empezar && !this.terminar)
        this.w = true;
    }
    if((tecla == 97 || tecla == 65) && !this.model.bordesMapaLaterales()){ //IZQUIERDA
      if(!this.empezar && !this.terminar)
        this.a = true;
    }
    if((tecla == 115 || tecla == 83)  && !this.model.bordesMapaVerticales()){ //ATRAS
      if(!this.empezar && !this.terminar)
        this.s = true;
    }
    if((tecla == 100 || tecla == 68) && !this.model.bordesMapaLaterales()){ //DERECHA
      if(!this.empezar && !this.terminar)
        this.d = true;
    }
    if(tecla == 32 && this.model.personaje.position.y == this.alturaSuelo){ //SALTO
      if(!this.empezar && !this.terminar)
        this.space = true;
 
    }
    
  }

  onKeyUp(event){
    var tecla = event.which || event.keyCode;
    if(tecla == 119 || tecla == 87){ //ADELANTE
      this.w = false;
    }
    if(tecla == 97 || tecla == 65){ //IZQUIERDA
      this.a = false;
    }
    if(tecla == 115 || tecla == 83){ //ATRAS
      this.s = false;
    }
    if(tecla == 100 || tecla == 68){ //DERECHA
      this.d = false;
    }
    if(tecla == 32){ //SALTO
      this.space = false;
    }
  }

  onMouseUp(){
    var that = this;
    
    $("#empezar").unbind('click')
    $("#empezar").click(function(){
      that.empezarPartida();
    });

    $("#volverJugar").unbind('click')
    $("#volverJugar").click(function(){
      that.volverAJugar();
    });

    $("#volverJugar1").unbind('click')
    $("#volverJugar1").click(function(){
      that.volverAJugar();
    });

    $("#volverMenu").unbind('click')
    $("#volverMenu").click(function(){
      that.volverMenu();
    });

    $("#volverMenu1").unbind('click')
    $("#volverMenu1").click(function(){
      that.volverMenu();
    });

    $("#volverMenu2").unbind('click')
    $("#volverMenu2").click(function(){
      that.volverMenu();
    });

    $("#ayuda").unbind('click')
    $("#ayuda").click(function(){
      that.ayuda();
    });
    
  
    $("#cambiar").unbind('click')
    $("#cambiar").click(function(){
      that.cambiarColorPersonaje();
    });

    $("#subirVolumen").unbind('click')
    $("#subirVolumen").click(function(){
      that.modificarMusica(0);
    });

    $("#bajarVolumen").unbind('click')
    $("#bajarVolumen").click(function(){
      that.modificarMusica(1);
    });
    
    $("#desactivar").unbind('click')
    $("#desactivar").click(function(){
      that.modificarMusica(2);
    });

    $("#activar").unbind('click')
    $("#activar").click(function(){
      that.modificarMusica(3);
    });    
  }


  animacionPersonaje(){
    var velocidad = 0.2;

   if(this.w){ //ADELANTE
      var that= this;
      var origen = {x: this.px, y: this.py, z: this.pz};
      var destino = {x: this.px, y: this.py, z: this.pz+velocidad};
      var animacion1 = new TWEEN.Tween(origen).to(destino,100).easing(TWEEN.Easing.Linear.None)
      .onUpdate(function(){
        that.model.personaje.position.z = origen.z;
      });
      if(this.orientacion != "NORTE"){
        var origen1;
        var destino1 = {r: 0};
        if(this.orientacion == "ESTE"){
          origen1 = {r: -Math.PI/2};
        }else if(this.orientacion == "OESTE"){
          origen1 = {r: Math.PI/2};
        }else if(this.orientacion == "SUR"){
          origen1 = {r: Math.PI};
        }
        this.orientacion = "NORTE";
        var animacion2 = new TWEEN.Tween(origen1).to(destino1,100).easing(TWEEN.Easing.Linear.None)
        .onUpdate(function(){
        that.model.personaje.rotation.y = origen1.r;
        });
        animacion2.start();
      }
      
      if(!this.AnimAndar){  
        this.model.personaje.andar.start();
        that.AnimAndar = true;
        setTimeout(function(){that.AnimAndar = false},600);
      }
      animacion1.start();
      this.pz += velocidad;
    }
    if(this.a){ //IZQUIERDA
      var that= this;
      var origen = {x: this.px, y: this.py, z: this.pz};
      var destino = {x: this.px+velocidad, y: this.py, z: this.pz};
      var animacion1 = new TWEEN.Tween(origen).to(destino,100).easing(TWEEN.Easing.Linear.None)
      .onUpdate(function(){
        that.model.personaje.position.x = origen.x;
      });
      if(this.orientacion != "OESTE"){
        var origen1;
        var destino1 = {r: Math.PI/2};
        if(this.orientacion == "ESTE"){
          origen1 = {r: -Math.PI/2};
        }else if(this.orientacion == "NORTE"){
          origen1 = {r: 0};
        }else if(this.orientacion == "SUR"){
          origen1 = {r: Math.PI};
        }
        this.orientacion = "OESTE";
        var animacion2 = new TWEEN.Tween(origen1).to(destino1,100).easing(TWEEN.Easing.Linear.None)
        .onUpdate(function(){
        that.model.personaje.rotation.y = origen1.r;
        });
        animacion2.start();
      }
      if(!this.AnimAndar){  
        this.model.personaje.andar.start();
        that.AnimAndar = true;
        setTimeout(function(){that.AnimAndar = false},600);
      }
      animacion1.start();
      this.px += velocidad;
    }
    if(this.s){ //ATRAS
      var that= this;
      var origen = {x: this.px, y: this.py, z: this.pz};
      var destino = {x: this.px, y: this.py, z: this.pz-velocidad};
      var animacion1 = new TWEEN.Tween(origen).to(destino,100).easing(TWEEN.Easing.Linear.None)
      .onUpdate(function(){
        that.model.personaje.position.z = origen.z;
      });
      if(this.orientacion != "SUR"){
        var origen1;
        var destino1 = {r: Math.PI};
        if(this.orientacion == "OESTE"){
          origen1 = {r: Math.PI/2};
        }else if(this.orientacion == "NORTE"){
          origen1 = {r: 0};
        }else if(this.orientacion == "ESTE"){
          origen1 = {r: -Math.PI/2};
          destino1 = {r: -Math.PI};
        }
        this.orientacion = "SUR";
        var animacion2 = new TWEEN.Tween(origen1).to(destino1,100).easing(TWEEN.Easing.Linear.None)
        .onUpdate(function(){
          that.model.personaje.rotation.y = origen1.r;
        });
        animacion2.start();
      }
      if(!this.AnimAndar){  
        this.model.personaje.andar.start();
        that.AnimAndar = true;
        setTimeout(function(){that.AnimAndar = false},600);
      }
      animacion1.start();
      this.pz -= velocidad;
    }
    if(this.d){ //DERECHA
      var that= this;
      var origen = {x: this.px, y: this.py, z: this.pz};
      var destino = {x: this.px-velocidad, y: this.py, z: this.pz};
      var animacion1 = new TWEEN.Tween(origen).to(destino,100).easing(TWEEN.Easing.Linear.None)
      .onUpdate(function(){
        that.model.personaje.position.x = origen.x;
      });
      if(this.orientacion != "ESTE"){
        var origen1;
        var destino1 = {r: -Math.PI/2};
        if(this.orientacion == "OESTE"){
          origen1 = {r: Math.PI/2};
        }else if(this.orientacion == "NORTE"){
          origen1 = {r: 0};
        }else if(this.orientacion == "SUR"){
          origen1 = {r: -Math.PI};
        }
        this.orientacion = "ESTE";
        var animacion2 = new TWEEN.Tween(origen1).to(destino1,100).easing(TWEEN.Easing.Linear.None)
        .onUpdate(function(){
          that.model.personaje.rotation.y = origen1.r;
        });
        animacion2.start();
      }
      if(!this.AnimAndar){  
        this.model.personaje.andar.start();
        that.AnimAndar = true;
        setTimeout(function(){that.AnimAndar = false},600);
      }
      animacion1.start();
      this.px -= velocidad;
    }
    if(this.space){
      this.salto.start();
    }

  }
}


/// La función   main
$(function () {
  
  var scene = new MyScene("#WebGL-output");

  
  const bgTexture = new THREE.TextureLoader().load('texturas/cielo.jpg');
  scene.background = bgTexture;

  // Se añaden los listener de la aplicación. En este caso, el que va a comprobar cuándo se modifica el tamaño de la ventana de la aplicación.
  window.addEventListener ("resize", () => scene.onWindowResize());
  window.addEventListener("keydown", (event) => scene.onKeyDown(event));
  window.addEventListener("keyup", (event) => scene.onKeyUp(event));
  window.addEventListener("mouseup", () => scene.onMouseUp());
  
  // Que no se nos olvide, la primera visualización.
  scene.update();
});
