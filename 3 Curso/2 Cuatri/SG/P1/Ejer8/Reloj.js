
import * as THREE from '../libs/three.module.js'

class Reloj extends THREE.Object3D {
  constructor(gui,titleGui) {
    super();
    
    // Se crea la parte de la interfaz que corresponde a la caja
    // Se crea primero porque otros métodos usan las variables que se definen para la interfaz
    this.createGUI(gui,titleGui);
    
    // Un Mesh se compone de geometría y material

    var model = new THREE.SphereBufferGeometry(5,10,10);
    // Como material se crea uno a partir de un color
    var material= new THREE.MeshBasicMaterial({color: 0x228B22});
    var mrojo= new THREE.MeshBasicMaterial({color: 0x8B0000});
    // Ya podemos construir el Mesh
    var esfera = new THREE.Mesh (model, material);
    this.esferas = new THREE.Object3D();

    var x = 50; var z = 0;

    esfera.position.set(x,0,z);
    this.esferas.add(esfera);

    this.alfa = (2*Math.PI)/12;

    for(var i = 0;i < 12 ; i++){
      var ang = this.alfa*i;
      var nueva_esfera = esfera.clone();
      var nx = x * Math.cos(ang) + z * Math.sin(ang);
      var nz = -x * Math.sin(ang) + z * Math.cos(ang);
      nueva_esfera.position.set(nx,0,nz);
      this.esferas.add(nueva_esfera);
    }
    this.minutero = new THREE.Mesh (model, mrojo);
    this.x = 40; this.z = 0;
    this.minutero.position.set(this.x, 0 ,this.z);
    
    
    this.add(this.minutero);
    this.add(this.esferas);
    this.scale.set(0.1,0.1,0.1);
    
    // Las geometrías se crean centradas en el origen.
    // Como queremos que el sistema de referencia esté en la base,
    // subimos el Mesh de la caja la mitad de su altura
  }
  
  createGUI (gui,titleGui) {
    

    // Controles para el tamaño, la orientación y la posición de la caja
    this.guiControls = new function () {
      this.velocidad = 0;
      
      // Un botón para dejarlo todo en su posición inicial
      // Cuando se pulse se ejecutará esta función.
      this.reset = function () {
        this.velocidad = 0;
      }
    } 
    
    // Se crea una sección para los controles de la caja
    var folder = gui.addFolder (titleGui);
    // Estas lineas son las que añaden los componentes de la interfaz
    // Las tres cifras indican un valor mínimo, un máximo y el incremento
    // El método   listen()   permite que si se cambia el valor de la variable en código, el deslizador de la interfaz se actualice
    
    

    folder.add (this.guiControls, 'velocidad', -12.0, 12.0, 0.1).name ('Velocidad : ').listen();
    folder.add (this.guiControls, 'reset').name ('[ Reset ]');

  }
  
  update () {
    var fast = (this.alfa * this.guiControls.velocidad) / 60; //Entre 60 fotogramas por segundo

		var x = this.x * Math.cos(fast) + this.z * Math.sin(fast);
		var z = -this.x * Math.sin(fast) + this.z * Math.cos(fast);

		this.x = x;
		this.z = z;
		this.minutero.position.set(x, 0, -z);
    
  }
}

export { Reloj };
