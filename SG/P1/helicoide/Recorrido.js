
import * as THREE from '../libs/three.module.js'
import * as TWEEN from '../libs/tween.esm.js'

class Recorrido extends THREE.Object3D {
  constructor(gui,titleGui) {
    super();
    
    // Se crea la parte de la interfaz que corresponde a la caja
    // Se crea primero porque otros métodos usan las variables que se definen para la interfaz
    this.createGUI(gui,titleGui);
    
    // Un Mesh se compone de geometría y material
    this.radio = 1;
    this.altura = 3;

    var model = new THREE.SphereBufferGeometry(this.radio,20,20);
    var model2 = new THREE.CylinderBufferGeometry(this.radio,this.radio,this.altura,20,20);
    var model3 = new THREE.ConeBufferGeometry(this.radio,this.altura,20);
    // Como material se crea uno a partir de un color
    var material= new THREE.MeshBasicMaterial({color: 0x228B22});
    var transparente= new THREE.MeshNormalMaterial({opacity: 0.35,transparent: true});
    // Ya podemos construir el Mesh
    this.esfera = new THREE.Mesh (model, material);
    this.cilindro = new THREE.Mesh(model2,transparente);
    this.cono = new THREE.Mesh(model3,transparente);
    this.sfere = new THREE.Mesh(model,transparente);

    this.esfera.scale.set(0.1,0.1,0.1);
    
    var origenTangente= {x: 0, z:0};
    var origenAltura= {y:-this.altura/2};
    var oriAltEsfera = {y:-this.radio};
 
    var destinoTangente = {x:2*Math.PI, z:2*Math.PI};
    var destinoAltura = {y:this.altura/2};
    var destAltEsfera = {y:this.radio};
 
    var that=this;
 
    var movimiento=new TWEEN.Tween(origenTangente).to(destinoTangente,2000).easing(TWEEN.Easing.Linear.None)
    .onUpdate(function(){
       //Para esfera
       let nuevoRadio = Math.sqrt(Math.pow(that.radio,2) - Math.pow(oriAltEsfera.y,2));
       that.esfera.position.x= Math.cos(origenTangente.x) * nuevoRadio * that.guiControls.escalado;
       that.esfera.position.z= Math.sin(origenTangente.x) * nuevoRadio * that.guiControls.escalado;

       //Para el cono
       //let nuevoRadio = that.radio*(that.altura-(origenAltura.y + that.altura/2))/ that.altura;
       //that.esfera.position.x= Math.cos(origenTangente.x) * nuevoRadio * that.guiControls.escalado;
       //that.esfera.position.z= Math.sin(origenTangente.x) * nuevoRadio * that.guiControls.escalado;

       //Para el cilindro
       //that.esfera.position.x= Math.cos(origenTangente.x) * that.radio * that.guiControls.escalado;
       //that.esfera.position.z= Math.sin(origenTangente.x) * that.radio * that.guiControls.escalado;
     }).repeat(Infinity).start();

     //Para la altura en cilindro y cono
     /*var movimientoAltura=new TWEEN.Tween(origenAltura)
     .to(destinoAltura,5000)   //Modificar el tiempo para hacerla saltarina
     .easing(TWEEN.Easing.Linear.None)
     .onUpdate(function(){
       that.esfera.position.y=origenAltura.y;
     }).repeat(Infinity).yoyo(true).repeatDelay(500).start();*/

     var movimientoAltura=new TWEEN.Tween(oriAltEsfera)
     .to(destAltEsfera,5000)   //Modificar el tiempo para hacerla saltarina
     .easing(TWEEN.Easing.Linear.None)
     .onUpdate(function(){
       that.esfera.position.y=oriAltEsfera.y;
     }).repeat(Infinity).yoyo(true).repeatDelay(500).start();
   

    this.add(this.esfera);
    this.add(this.sfere);
    //this.add(this.cilindro);
    //this.add(this.cono);
  }
  
  createGUI (gui,titleGui) {
    

    // Controles para el tamaño, la orientación y la posición de la caja
    this.guiControls = new function () {
      this.escalado = 1;
      
      // Un botón para dejarlo todo en su posición inicial
      // Cuando se pulse se ejecutará esta función.
      this.reset = function () {
        this.escalado = 1;
      }
    } 
    
    // Se crea una sección para los controles de la caja
    var folder = gui.addFolder (titleGui);
    // Estas lineas son las que añaden los componentes de la interfaz
    // Las tres cifras indican un valor mínimo, un máximo y el incremento
    // El método   listen()   permite que si se cambia el valor de la variable en código, el deslizador de la interfaz se actualice
    folder.add (this.guiControls, 'escalado', 0.5, 3.0, 0.1).name ('Radio cilindro : ').listen();
    folder.add (this.guiControls, 'reset').name ('[ Reset ]');

  }
  
  update () {
    //this.cilindro.scale.set(this.guiControls.escalado,1,this.guiControls.escalado);
    //this.cono.scale.set(this.guiControls.escalado,1,this.guiControls.escalado); 
    this.sfere.scale.set(this.guiControls.escalado,1,this.guiControls.escalado); 
    TWEEN.update();
  }
}

export { Recorrido };
