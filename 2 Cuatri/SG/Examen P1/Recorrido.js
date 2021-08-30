
import * as THREE from '../libs/three.module.js'
import * as TWEEN from '../libs/tween.esm.js'
import { ThreeBSP } from '../libs/ThreeBSP.js'

class Recorrido extends THREE.Object3D {
  constructor(gui,titleGui) {
    super();
    
    // Se crea la parte de la interfaz que corresponde a la caja
    // Se crea primero porque otros métodos usan las variables que se definen para la interfaz
    this.createGUI(gui,titleGui);
    
    
    // Un Mesh se compone de geometría y material
    this.semiesf = new THREE.SphereGeometry(1,20,20,0,Math.PI);
    this.semiesf.rotateX(90*Math.PI/180);
    this.esf = new THREE.SphereGeometry(1,20,20,0,Math.PI);
    this.esf.rotateX(270*Math.PI/180);
    
    
    var model2 = new THREE.CylinderGeometry(0.2,0.2,2,20,20);
    model2.rotateX(90*Math.PI/180);
    model2.translate(0.5,0.5,0);
    

    // Como material se crea uno a partir de un color
    
    var bsp = new ThreeBSP(this.esf);
    var bsp2 = new ThreeBSP(model2);
    var p = bsp.subtract(bsp2);

    
    
    var geometry = p.toGeometry();
    var model = new THREE.BufferGeometry().fromGeometry(geometry) ;
    
    var material= new THREE.MeshPhongMaterial({color: 0xE5BE01});
    material.side = THREE.DoubleSide;
    // Ya podemos construir el Mesh
    this.comecocos = new THREE.Mesh (model, material);
  
    this.sem = new THREE.Mesh (this.semiesf, material);
    this.comecocos.rotateY(90*Math.PI/180);
    this.sem.rotateY(270*Math.PI/180);
    //this.add(this.sem);

    this.camino = this.createSpline();

    this.come = new THREE.Object3D;
    this.come.add(this.sem);
    this.come.add(this.comecocos);

    var that=this;
    
    var ori = {z:0}; 
    var dest = {z:Math.PI/2};
    var ori3 = {p:0}; 
    var dest3 = {p:Math.PI/2};
    var animBoca=new TWEEN.Tween(ori).to(dest,1000)   
     .easing(TWEEN.Easing.Linear.None)
     .onUpdate(function(){
       that.sem.rotation.z = ori.z;
     }).repeat(Infinity).yoyo(true).start();

     /*var animBoca2=new TWEEN.Tween(ori3).to(dest3,3000)   
     .easing(TWEEN.Easing.Linear.None)
     .onUpdate(function(){
       that.sem.rotateZ(-ori3.p);
     })*/


    var ori1 = {p:0.6}; var dest1 = {p:0};
    var bucle1 = new TWEEN.Tween(ori1).to(dest1, 6000).easing(TWEEN.Easing.Quadratic.InOut).onUpdate(()=>{
      var pos = this.spline.getPointAt(ori1.p);
      that.come.position.copy(pos);
      var tangente = this.spline.getTangentAt(ori1.p);
      pos.add(tangente);

      this.come.lookAt(pos);
  });

  var ori2 = {p:1}; var dest2 = {p:0.6};
    var bucle2 = new TWEEN.Tween(ori2).to(dest2, 4000).easing(TWEEN.Easing.Quadratic.InOut).onUpdate(()=>{
      var pos = this.spline.getPointAt(ori2.p);
      that.come.position.copy(pos);
      var tangente = that.spline.getTangentAt(ori2.p);
      pos.add(tangente);

      this.come.lookAt(pos);
      
  });
  
    bucle1.chain(bucle2);
    bucle2.chain(bucle1);
   // animBoca.chain(animBoca2);
    /*animBoca2.chain(animBoca);
    animBoca.start();*/
    bucle1.start();
    

    this.add(this.camino);

    
    this.add(this.come);
  }

  createSpline(){

    var visibleSpline;
    var mrojo= new THREE.LineBasicMaterial({color: 0x8B0000});
    this.puntos = [];
    this.puntos.push(new THREE.Vector3(0.0, 0.0, 1.0));
    this.puntos.push(new THREE.Vector3(3.0, 0.0, 0.0));
    this.puntos.push(new THREE.Vector3(3.0, 0.0, -6.0));
    this.puntos.push(new THREE.Vector3(-3.0, 0.0, -6.0));
    this.puntos.push(new THREE.Vector3(-3.0, 0.0, 0.0));
    this.puntos.push(new THREE.Vector3(-3.0, 0.0, 6.0));
    this.puntos.push(new THREE.Vector3(0.0, 0.0, 6.0));
    this.puntos.push(new THREE.Vector3(0.0, 0.0, 1.0));
    
    
    this.spline = new THREE.CatmullRomCurve3(this.puntos);

    var geometryLine = new THREE.Geometry();
    geometryLine.vertices = this.spline.getPoints(100);
    var visibleSpline = new THREE.Line(geometryLine,mrojo);

    return visibleSpline
  }


  
  createGUI (gui,titleGui) {
    

    // Controles para el tamaño, la orientación y la posición de la caja
    
    
    // Se crea una sección para los controles de la caja
    var folder = gui.addFolder (titleGui);
    // Estas lineas son las que añaden los componentes de la interfaz
    // Las tres cifras indican un valor mínimo, un máximo y el incremento
    // El método   listen()   permite que si se cambia el valor de la variable en código, el deslizador de la interfaz se actualice
    //folder.add (this.guiControls, 'reset').name ('[ Reset ]');

  }
  
  update () {
  
    this.boca += 0.1;
    this.esf.geometry = new THREE.SphereGeometry(1,20,20,0,this.boca);
    TWEEN.update();
  }
}

export { Recorrido };
