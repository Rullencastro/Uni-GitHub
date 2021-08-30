
import * as THREE from '../libs/three.module.js'
import { ThreeBSP } from '../libs/ThreeBSP.js'


class Pendulos extends THREE.Object3D {
  constructor(gui) {
    super();
    
    // Se crea la parte de la interfaz que corresponde a la caja
    // Se crea primero porque otros métodos usan las variables que se definen para la interfaz
    this.createGUI(gui);
    
    // Un Mesh se compone de geometría y material
  
    // Como material se crea uno a partir de un color
    var m0= new THREE.MeshPhongMaterial({color: 0x8B0000});
    var m1= new THREE.MeshPhongMaterial({color: 0x228B22});
    var m2= new THREE.MeshPhongMaterial({color: 0x252850});

    this.c0 =  new THREE.Mesh (new THREE.BoxGeometry(4,5,4),m0);
    this.c1 = new THREE.Mesh (new THREE.BoxGeometry(4,4,4),m1);
    this.c2 = new THREE.Mesh (new THREE.BoxGeometry(4,4,4),m1);

    this.c3 = new THREE.Mesh (new THREE.BoxGeometry(3,10,2),m2);
    this.c4 = new THREE.Mesh (new THREE.BoxGeometry(1,1,3),m0);

    this.c0.position.set(0,-4.5,0);
    this.c1.position.set(0,0,0);
    this.c2.position.set(0,-9,0);

    this.spendulo = this.createSegundoPendulo();
    

    // Ya podemos construir el Mesh
    this.pendulos = new THREE.Object3D();

    this.pendulos.add(this.c0);
    this.pendulos.add(this.c1);
    this.pendulos.add(this.c2);
    this.pendulos.add(this.spendulo);
    // Y añadirlo como hijo del Object3D (el this)
    this.add (this.pendulos);
    this.scale.set(0.1,0.1,0.1);
    
    // Las geometrías se crean centradas en el origen.
    // Como queremos que el sistema de referencia esté en la base,
    // subimos el Mesh de la caja la mitad de su altura
  }

  createSegundoPendulo(){
    var m0= new THREE.MeshPhongMaterial({color: 0x8B0000});
    var m2= new THREE.MeshPhongMaterial({color: 0x252850});
    var c3 = new THREE.Mesh (new THREE.BoxGeometry(3,10,2),m2);
    var rojo = new THREE.Mesh (new THREE.BoxGeometry(1,1,3),m0);

    c3.position.set(0,-4,0);
    

    var F =  new THREE.Object3D();
    //F.rotateZ(45*Math.PI/180);
    F.position.y = -3;
    F.position.z = 3;
    F.add(rojo);
    F.add(c3);

    return F;
  }
  
  createGUI (gui) {

    var that = this;
  
    

    // Controles para el tamaño, la orientación y la posición de la caja
    this.guiControls = new function () {
      this.rotacion = 0.0;
      this.rotacion2 = 0.0;
      this.size = 5.0;
      this.size2 = 10.0;
      
      // Un botón para dejarlo todo en su posición inicial
      // Cuando se pulse se ejecutará esta función.
      this.reset = function () {
        this.rotacion = 0.0;
        this.size = 5.0;
      }
    } 
    
    // Se crea una sección para los controles de la caja
    var folder = gui.addFolder ("Controles Primer Pendulo");
    var folder2 = gui.addFolder ("Controles Segundo Pendulo");
    // Estas lineas son las que añaden los componentes de la interfaz
    // Las tres cifras indican un valor mínimo, un máximo y el incremento
    // El método   listen()   permite que si se cambia el valor de la variable en código, el deslizador de la interfaz se actualice
    

    folder.add (this.guiControls, 'size', 5, 10, 0.1).name ('Longitud : ').onChange(function(value){
      that.c0.scale.y = value/5;
      that.c0.position.y= -(value/2)-2;
      that.c2.position.y = -value-4;  
    });

    /*folder.add (this.guiControls, 'ang', 0.1, 2*Math.PI, 0.1).name ('Angulo : ').onChange(function(value){
      that.rev.geometry = new THREE.LatheBufferGeometry(that.puntos,that.guiControls.instancias,0,value);
    });*/
    folder.add (this.guiControls, 'rotacion', -0.8, 0.8, 0.1).name ('Rotacion : ');
    
    folder2.add (this.guiControls, 'rotacion2', -0.8, 0.8, 0.1).name ('Rotacion : ');
     
    
    folder2.add (this.guiControls, 'size2', 10, 20, 0.1).name ('Longitud : ').onChange(function(value){
      that.spendulo.scale.y = value/10;
      that.spendulo.position.y = -(value/10)-2;
    });

    folder.add (this.guiControls, 'reset').name ('[ Reset ]');
 
    
  }
  
  update () {
    this.rotation.z = this.guiControls.rotacion;
    this.spendulo.rotation.z = this.guiControls.rotacion2;
  }
}
export { Pendulos };
