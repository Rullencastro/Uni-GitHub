
import * as THREE from '../libs/three.module.js'

class Barrido extends THREE.Object3D {
  constructor(gui,titleGui) {
    super();
    
    // Se crea la parte de la interfaz que corresponde a la caja
    // Se crea primero porque otros métodos usan las variables que se definen para la interfaz
    this.createGUI(gui,titleGui);
    
    // Un Mesh se compone de geometría y material
    this.puntos = [];

    var shape = new THREE.Shape();

    shape.moveTo(0,0);
    shape.quadraticCurveTo (-1.5,0.5,-2,2);
    shape.quadraticCurveTo (-1,4,0,2);
    shape.quadraticCurveTo (1,4,2,2);
    shape.quadraticCurveTo (1.5,0.5,0,0);
    

    var pts = [];
    for(var i = 0;i < 50 ; i++){
      pts.push(new THREE.Vector3(Math.cos(i*Math.PI/180)*10,i,10*Math.sin(i*Math.PI/180)));
    }

    var path = new THREE.CatmullRomCurve3(pts);

    var options= {depth : 20 , steps : 50 , curveSegments : 10,bevelThickness : 1 , bevelSize : 1 , bevelSegments : 10,extrudePath : path } ;

    var model = new THREE.ExtrudeBufferGeometry(shape,options);

    // Como material se crea uno a partir de un color
    var material= new THREE.MeshNormalMaterial({color: 0xCF0000});
    
    // Ya podemos construir el Mesh
    this.barr = new THREE.Mesh (model, material);
    // Y añadirlo como hijo del Object3D (el this)
    this.add (this.barr);
    this.scale.set(0.1,0.1,0.1);
    
    // Las geometrías se crean centradas en el origen.
    // Como queremos que el sistema de referencia esté en la base,
    // subimos el Mesh de la caja la mitad de su altura
  }
  
  createGUI (gui,titleGui) {

    var that = this;
  
    

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

export { Barrido };
