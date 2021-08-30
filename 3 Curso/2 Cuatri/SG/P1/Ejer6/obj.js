
import * as THREE from '../libs/three.module.js'
import { OBJLoader } from '../libs/OBJLoader.js'
import { MTLLoader } from '../libs/MTLLoader.js'


class Obj extends THREE.Object3D {
  constructor(gui,titleGui) {
    super();
    
    // Se crea la parte de la interfaz que corresponde a la caja
    // Se crea primero porque otros métodos usan las variables que se definen para la interfaz
    this.createGUI(gui,titleGui);
    
    // Un Mesh se compone de geometría y material
    var that = this;
    var mtlLoader = new MTLLoader();
    var objLoader = new OBJLoader();
    mtlLoader.load('porsche911/911.mtl',function(materials){objLoader.setMaterials(materials);objLoader.load('porsche911/Porsche_911_GT2.obj',function (object){var modelo = object;that.add(modelo);},null,null)});

    this.position.x = 0;
    this.position.y = 0;
    this.position.z = 0;
    
    
    //var model = new THREE.BufferGeometry().fromGeometry(geometry) ;
    

    // Como material se crea uno a partir de un color
    //var material= new THREE.MeshNormalMaterial({color: 0xCF0000});
    
    // Ya podemos construir el Mesh
    //this.barr = new THREE.Mesh (model, material);
    // Y añadirlo como hijo del Object3D (el this)
    //this.add (this.barr);
    //this.scale.set(10,10,10);
    
    // Las geometrías se crean centradas en el origen.
    // Como queremos que el sistema de referencia esté en la base,
    // subimos el Mesh de la caja la mitad de su altura
  }
  
  createGUI (gui,titleGui) {

    //var that = this;
  
    

    // Controles para el tamaño, la orientación y la posición de la caja
    this.guiControls = new function () {
      //this.instancias = 20;
      //this.ang = Math.PI;
      
      
      // Un botón para dejarlo todo en su posición inicial
      // Cuando se pulse se ejecutará esta función.
      this.reset = function () {
        //this.instancias = 20;
        //this.ang = Math.PI;
        //that.rev.geometry = new THREE.LatheBufferGeometry(that.puntos,that.guiControls.instancias,0,that.guiControls.ang);
      }
    } 
    
    // Se crea una sección para los controles de la caja
    var folder = gui.addFolder (titleGui);
    // Estas lineas son las que añaden los componentes de la interfaz
    // Las tres cifras indican un valor mínimo, un máximo y el incremento
    // El método   listen()   permite que si se cambia el valor de la variable en código, el deslizador de la interfaz se actualice
    
    

    /*folder.add (this.guiControls, 'instancias', 2.0, 50.0, 1.0).name ('Instancias : ').onChange(function(value){
      that.rev.geometry = new THREE.LatheBufferGeometry(that.puntos,value,0,that.guiControls.ang);
    });

    folder.add (this.guiControls, 'ang', 0.1, 2*Math.PI, 0.1).name ('Angulo : ').onChange(function(value){
      that.rev.geometry = new THREE.LatheBufferGeometry(that.puntos,that.guiControls.instancias,0,value);
    });*/
    
    folder.add (this.guiControls, 'reset').name ('[ Reset ]');

    
    
  }
  
  update () {
    
  }
}
export { Obj };
