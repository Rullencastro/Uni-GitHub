import * as THREE from '../libs/three.module.js';

class Recorrido extends THREE.Object3D{
    constructor(){
        super();
        

        var verde = new THREE.MeshPhongMaterial({color: 0x00FF00});
        var rojo = new THREE.MeshPhongMaterial({color: 0xFF0000});

        var texture = new THREE.TextureLoader().load("/Wipe-In/texturas/suelo.jpg");
        var suelo = new THREE.MeshPhongMaterial ({map: texture});

        var mat = new THREE.MeshPhongMaterial({map: new THREE.TextureLoader().load('texturas/meta4.jpg')});
        var chk = new THREE.MeshPhongMaterial({map: new THREE.TextureLoader().load('texturas/check.jpg')});

      //-----------------Recorrido-------------------------
      this.circuito = new THREE.Object3D();

      this.largoMSC = 5;
      var largoR1 = 50;
      var largoR2 = 60;

      var salida = new THREE.Mesh(new THREE.BoxGeometry(20, 1, this.largoMSC), mat);
      var recorrido1 = new THREE.Mesh(new THREE.BoxGeometry(20, 1, largoR1), suelo);
      this.check1 = new THREE.Mesh(new THREE.BoxGeometry(20, 1, this.largoMSC), chk);
      this.check1.rotation.y = Math.PI;
      var recorrido2 = new THREE.Mesh(new THREE.BoxGeometry(20, 1, largoR2), suelo);
      this.meta = new THREE.Mesh(new THREE.BoxGeometry(20, 1, this.largoMSC), mat);

      
      recorrido1.position.z = salida.position.z + this.largoMSC/2 + largoR1/2;
      this.check1.position.z = recorrido1.position.z + this.largoMSC/2 + largoR1/2;
      recorrido2.position.z = this.check1.position.z + this.largoMSC/2 + largoR2/2;
      this.meta.position.z = recorrido2.position.z + largoR2/2 + this.largoMSC/2;
      
      
      this.circuito.add(salida);
      this.circuito.add(recorrido1);
      this.circuito.add(recorrido2);
      this.circuito.add(this.check1);
      this.circuito.add(this.meta);
      this.add(this.circuito);
    }

    longitudRecorrido(){
      return this.meta.position.z + this.largoMSC/2;
    }

    posMeta(){
      return this.meta.position.z - this.largoMSC/2;
    }

    posCheck(){
      return this.check1.position.z - this.largoMSC/2;
    }

    update () {
    }
  
  }

  export { Recorrido };