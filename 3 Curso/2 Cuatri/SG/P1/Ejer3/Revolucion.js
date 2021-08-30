
import * as THREE from '../libs/three.module.js'

class Revolucion extends THREE.Object3D {
  constructor(gui,titleGui) {
    super();
    
    // Se crea la parte de la interfaz que corresponde a la caja
    // Se crea primero porque otros métodos usan las variables que se definen para la interfaz
    this.createGUI(gui,titleGui);
    
    // Un Mesh se compone de geometría y material
    this.puntos = [];

    this.puntos.push(new THREE.Vector3(1.0, -1.4, 0.0));
    this.puntos.push(new THREE.Vector3(1.0, -1.1, 0.0));
    this.puntos.push(new THREE.Vector3(0.5, -0.7, 0.0));
    this.puntos.push(new THREE.Vector3(0.4, -0.4, 0.0));
    this.puntos.push(new THREE.Vector3(0.4, 0.5, 0.0));
    this.puntos.push(new THREE.Vector3(0.5, 0.6, 0.0));
    this.puntos.push(new THREE.Vector3(0.3, 0.6, 0.0));
    this.puntos.push(new THREE.Vector3(0.5, 0.8, 0.0));
    this.puntos.push(new THREE.Vector3(0.55, 1.0, 0.0));
    this.puntos.push(new THREE.Vector3(0.5, 1.2, 0.0));
    this.puntos.push(new THREE.Vector3(0.3, 1.4, 0.0));

    var model = new THREE.LatheBufferGeometry(this.puntos,this.guiControls.instancias,0,this.guiControls.ang);

    //var model = new THREE.SphereBufferGeometry(5);
    //var model2 = new THREE.CylinderBufferGeometry(5,5,10);

    // Como material se crea uno a partir de un color
    var material= new THREE.MeshNormalMaterial({color: 0xCF0000});
    
    // Ya podemos construir el Mesh
    this.rev = new THREE.Mesh (model, material);
    //this.rev2 = new THREE.Mesh (model2, material);
    
    //this.rev2.position.y = -5;
    
    // Y añadirlo como hijo del Object3D (el this)
    this.add (this.rev);
    //this.add (this.rev2);
    //this.position.y = 0.25;
    this.scale.set(0.1,0.1,0.1);
    
    // Las geometrías se crean centradas en el origen.
    // Como queremos que el sistema de referencia esté en la base,
    // subimos el Mesh de la caja la mitad de su altura
  }
  
  createGUI (gui,titleGui) {

    var that = this;
  
    

    // Controles para el tamaño, la orientación y la posición de la caja
    this.guiControls = new function () {
      this.instancias = 20;
      this.ang = Math.PI;
      
      
      // Un botón para dejarlo todo en su posición inicial
      // Cuando se pulse se ejecutará esta función.
      this.reset = function () {
        this.instancias = 20;
        this.ang = Math.PI;
        that.rev.geometry = new THREE.LatheBufferGeometry(that.puntos,that.guiControls.instancias,0,that.guiControls.ang);
      }
    } 
    
    // Se crea una sección para los controles de la caja
    var folder = gui.addFolder (titleGui);
    // Estas lineas son las que añaden los componentes de la interfaz
    // Las tres cifras indican un valor mínimo, un máximo y el incremento
    // El método   listen()   permite que si se cambia el valor de la variable en código, el deslizador de la interfaz se actualice
    
    

    folder.add (this.guiControls, 'instancias', 2.0, 50.0, 1.0).name ('Instancias : ').onChange(function(value){
      that.rev.geometry = new THREE.LatheBufferGeometry(that.puntos,value,0,that.guiControls.ang);
    });

    folder.add (this.guiControls, 'ang', 0.1, 2*Math.PI, 0.1).name ('Angulo : ').onChange(function(value){
      that.rev.geometry = new THREE.LatheBufferGeometry(that.puntos,that.guiControls.instancias,0,value);
    });
    
    folder.add (this.guiControls, 'reset').name ('[ Reset ]');

    
    
  }
  
  update () {
    
  }
}

export { Revolucion };
