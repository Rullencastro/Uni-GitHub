import * as THREE from '../libs/three.module.js';
import * as TWEEN from '../libs/tween.esm.js';
import { ThreeBSP } from '../libs/ThreeBSP.js';

class Puño extends THREE.Object3D{
    constructor(){
        super();

        var verde = new THREE.MeshPhongMaterial({color: 0x00FF00});
        var rojo = new THREE.MeshPhongMaterial({color: 0xFF0000});
        this.punio = new THREE.Object3D();

        //--------------Brazo-------------------------
        this.palo = new THREE.Mesh(new THREE.BoxGeometry(1, 2, 1), rojo);
        //this.pelota = new THREE.Mesh(new THREE.SphereGeometry (2,20,20), textpunio);
        this.pelota = this.crearPuño();

        this.palo.rotation.z = Math.PI/2;

        this.palo.position.x = 1;
        this.pelota.position.x = 4;

        this.aux = new THREE.Object3D();
        this.aux.scale.x = 2;
        this.aux.add(this.palo);

        this.punio.add(this.aux);
        this.punio.add(this.pelota);

        this.add(this.punio);
        //--------------Pared-------------------------
        

        var that = this;
        var ori = {p:  4};
        var dest = {p: 20};
        
        this.anim0 = new TWEEN.Tween(ori).to(dest,1000).easing(TWEEN.Easing.Quadratic.InOut)
        .onUpdate(function(){
        that.aux.scale.x = ori.p /2;
        that.pelota.position.set(ori.p, 0.0, 0.0);
      }).yoyo(true).repeat(Infinity);
        
        this.anim1 = new TWEEN.Tween(ori).to(dest,2000).easing(TWEEN.Easing.Quadratic.InOut)
        .onUpdate(function(){
        that.aux.scale.x = ori.p / 2;
        that.pelota.position.set(ori.p, 0.0, 0.0);
      }).yoyo(true).repeat(Infinity);

      
      this.anim2 = new TWEEN.Tween(ori).to(dest,3000).easing(TWEEN.Easing.Quadratic.InOut)
      .onUpdate(function(){
      that.aux.scale.x = ori.p/2;
      that.pelota.position.set(ori.p, 0.0, 0.0);
    }).yoyo(true).repeat(Infinity);
       
    
  }

  crearPuño(){
    var base = new THREE.SphereGeometry (2,30,30);

    var hueco = new THREE.CylinderGeometry(1.5,1.5,4,20,20);
    hueco.rotateX(Math.PI/2);
    hueco.translate(-1,-1,0);

    var corteSup = new THREE.BoxGeometry(4,1,4);
    corteSup.translate(0,2,0);

    var p = new ThreeBSP(base).subtract(new ThreeBSP(hueco));
    p = p.subtract(new ThreeBSP(corteSup));
    
    var geometry = p.toGeometry();
    var model = new THREE.BufferGeometry().fromGeometry(geometry);
    
    var textpunio = new THREE.MeshPhongMaterial({color: 0xFF00FF,side:THREE.DoubleSide});
    var punio = new THREE.Mesh (model, textpunio);
    
    var dedo = new THREE.Mesh( new THREE.TorusGeometry(2,0.5,20,20,Math.PI/2),textpunio);
    dedo.rotation.x = Math.PI/2;
    dedo.rotation.z = Math.PI/2;
    dedo.position.set(0.5,-0.1,0.5);

    var pulgar = new THREE.Mesh( new THREE.SphereGeometry(0.5,20,20,Math.PI/2),textpunio);
    pulgar.position.set(0.5,-0.1,2.5);


    var pundef = new THREE.Object3D();
    pundef.add(punio);
    pundef.add(dedo);
    pundef.add(pulgar);
    
    
    return pundef;
  }
  
  getPelotaPuño(){
    return this.pelota;
  }

    getPaloPuño(){
      return this.aux;
    }

    update () {
      TWEEN.update();
    }

  }

  export { Puño };