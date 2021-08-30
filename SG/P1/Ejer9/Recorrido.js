
import * as THREE from '../libs/three.module.js'
import * as TWEEN from '../libs/tween.esm.js'

class Recorrido extends THREE.Object3D {
  constructor(gui,titleGui) {
    super();
    
    // Se crea la parte de la interfaz que corresponde a la caja
    // Se crea primero porque otros métodos usan las variables que se definen para la interfaz
    this.createGUI(gui,titleGui);
    
    // Un Mesh se compone de geometría y material

    var model = new THREE.ConeBufferGeometry(5,10,10);
    // Como material se crea uno a partir de un color
    var material= new THREE.MeshBasicMaterial({color: 0x228B22});
    var mrojo= new THREE.LineBasicMaterial({color: 0x8B0000});
    // Ya podemos construir el Mesh
    this.esfera = new THREE.Mesh (model, material);


    this.puntos = [];
    this.puntos.push(new THREE.Vector3(0.0, 0.0, 0.0));
    this.puntos.push(new THREE.Vector3(1.0, -1.1, 1.0));
    this.puntos.push(new THREE.Vector3(2.5, -0.7, 2.0));
    this.puntos.push(new THREE.Vector3(3, -0.4, 1.5));
    this.puntos.push(new THREE.Vector3(2.4, 0.5, 0.0));
    this.puntos.push(new THREE.Vector3(0.5, 0.6, -1.0));
    this.puntos.push(new THREE.Vector3(0.0, 0.0, 0.0));
    this.puntos.push(new THREE.Vector3(-0.5, 0.8, 0.5));
    this.puntos.push(new THREE.Vector3(-1.55, 1.0, 1.0));
    this.puntos.push(new THREE.Vector3(-2.5, 1.2, 0.0));
    this.puntos.push(new THREE.Vector3(-3.5, 1.2, -1.0));
    this.puntos.push(new THREE.Vector3(-1.5, -1.2, -1.3));
    this.puntos.push(new THREE.Vector3(0.0, 0.0, 0.0));

    this.spline = new THREE.CatmullRomCurve3(this.puntos);

    var geometryLine = new THREE.Geometry();
    geometryLine.vertices = this.spline.getPoints(100);
    var visibleSpline = new THREE.Line(geometryLine,mrojo);

    this.esfera.scale.set(0.1,0.1,0.1);
    this.esfera.rotateX(Math.PI);

    var ori1 = {p:0}; var dest1 = {p:0.48};
    var bucle1 = new TWEEN.Tween(ori1).to(dest1, 4000).easing(TWEEN.Easing.Quadratic.InOut).onUpdate(()=>{
      var pos = this.spline.getPointAt(ori1.p);
      this.esfera.position.copy(pos);
      var tangente = this.spline.getTangentAt(ori1.p);
      pos.add(tangente);

      this.esfera.lookAt(pos);
  });

  var ori2 = {p:0.48}; var dest2 = {p:1};
    var bucle2 = new TWEEN.Tween(ori2).to(dest2, 8000).easing(TWEEN.Easing.Quadratic.InOut).onUpdate(()=>{
      var pos = this.spline.getPointAt(ori2.p);
      this.esfera.position.copy(pos);
      var tangente = this.spline.getTangentAt(ori1.p);
      pos.add(tangente);

      this.esfera.lookAt(pos);
  });
  
    bucle1.chain(bucle2);
    bucle2.chain(bucle1);
    bucle1.start();

    this.add(this.esfera);
    this.add(visibleSpline);
    
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
    TWEEN.update();
  }
}

export { Recorrido };
