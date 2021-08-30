
import * as THREE from '../libs/three.module.js'
import * as TWEEN from '../libs/tween.esm.js'
import { Personaje } from './Personaje.js'
import { Pendulo } from './Pendulo.js'
import { Recorrido } from './Recorrido.js'
import { ParedObstaculo } from './ParedObstaculo.js'
import { Pinchos } from './Pinchos.js'

class WipeIn extends THREE.Object3D {
  constructor() {
    super();
    
    this.personaje = new Personaje();
    this.pendulo1 = new Pendulo();
    this.pendulo2 = new Pendulo();
    this.pendulo3 = new Pendulo();
    this.pendulo4 = new Pendulo();
    this.pendulo5 = new Pendulo();
    this.recorrido = new Recorrido();
    this.pared1 = new ParedObstaculo();
    this.pared2 = new ParedObstaculo();
    this.pinchos1 = new Pinchos();
    this.pinchos2 = new Pinchos();
    this.pinchos3 = new Pinchos();
    this.pinchos4 = new Pinchos();

    this.pendulo1.position.z = 10;
    this.pendulo1.position.y = 2;
    this.pendulo1.oscilar.start();

    this.pendulo2.position.z = 20;
    this.pendulo2.position.y = 2;
    this.pendulo2.rotation.y = Math.PI;
    this.pendulo2.oscilar.start();

    this.pendulo3.position.x = -6;
    this.pendulo3.position.z = 15;
    this.pendulo3.position.y = 2;
    this.pendulo3.rotation.y = Math.PI/2;
    this.pendulo3.oscilar.start();

    this.pendulo4.position.x = 6;
    this.pendulo4.position.z = 15;
    this.pendulo4.position.y = 2;
    this.pendulo4.rotation.y = -Math.PI/2;
    this.pendulo4.oscilar.start();

    this.pendulo5.position.z = 15;
    this.pendulo5.position.y = 2;
    this.pendulo5.rotation.y = Math.PI/4;
    this.pendulo5.oscilar1.start();
   

    this.pared1.position.z = 70;
    this.pared1.position.y = 3.5;
    this.pared1.rotation.y =  Math.PI;
    this.pared1.position.x = 12;
    this.pared1.setOrientacion(1);

    this.pared2.position.z = 100;
    this.pared2.position.y = 3.5;
    this.pared2.position.x = -12;
    this.pared2.setOrientacion(0);

    this.pinchos1.position.z = 25;
    this.pinchos2.position.z = 35;
    this.pinchos3.position.z = 45;
    this.pinchos4.position.z = 90;

    this.recorrido.position.y = -0.5;
    this.personaje.position.y = 2.3;

    this.obstaculos = [];
    this.obstaculos.push(this.pendulo1);
    this.obstaculos.push(this.pendulo2);
    this.obstaculos.push(this.pendulo3);
    this.obstaculos.push(this.pendulo4);
    this.obstaculos.push(this.pendulo5);
    this.obstaculos.push(this.pared1);
    this.obstaculos.push(this.pared2);
    this.obstaculos.push(this.pinchos1);
    this.obstaculos.push(this.pinchos2);
    this.obstaculos.push(this.pinchos3);
    this.obstaculos.push(this.pinchos4);

    this.recorrido.traverseVisible(function(unNodo){
      unNodo.receiveShadow = true;
    });

    this.personaje.traverseVisible(function(unNodo){
      unNodo.castShadow = true;
    });
    this.obstaculos.forEach(function(obs){
      obs.traverseVisible(function(unNodo){
        unNodo.castShadow = true;
      });
    })


    this.add(this.personaje);
    this.obstaculos.forEach(obstaculo => this.add(obstaculo));
    this.add(this.recorrido);
    
  }

  colisiones(a,b,c){
    var posObstaculo = new THREE.Vector3();
    var posPersonaje = new THREE.Vector3();
    var that = this;
    var choca = false;
    
    this.personaje.getCuerpo().getWorldPosition(posPersonaje);

    this.obstaculos.forEach(function(obstaculo){
        if(obstaculo.getTipo() == 0){
          obstaculo.getBola().getWorldPosition(posObstaculo);
          if(posObstaculo.distanceTo(posPersonaje) <= 3 ){
            that.personaje.position.set(a,b,c);
            choca = true;
        }
      }else if(obstaculo.getTipo() == 1){
        obstaculo.punios.forEach(function(punio){
          punio.getPelotaPuño().getWorldPosition(posObstaculo);
          if(posObstaculo.distanceTo(posPersonaje) <= 3 ){
            that.personaje.position.set(a,b,c);
            choca = true;
          }
          punio.getPaloPuño().getWorldPosition(posObstaculo);
          if(that.personaje.position.x > posObstaculo.x + punio.getEscalado().x  && obstaculo.getOrientacion() == 0)
            posObstaculo.setX(posObstaculo.x + punio.getPaloPuño().scale.x);
          else if(that.personaje.position.x < posObstaculo.x - punio.getEscalado().x  && obstaculo.getOrientacion() == 1)
            posObstaculo.setX(posObstaculo.x - punio.getPaloPuño().scale.x);
          else
            posObstaculo.setX(that.personaje.position.x);

          if(posObstaculo.distanceTo(posPersonaje) <= 1.5 ){
            that.personaje.position.set(a,b,c);
            choca = true;
          }

        })
      }else if(obstaculo.getTipo() == 2){
        obstaculo.getWorldPosition(posObstaculo);
        posObstaculo.setX(that.personaje.position.x);
        posObstaculo.setY(3);
        if(posObstaculo.distanceTo(posPersonaje) <= 1 ){
          that.personaje.position.set(a,b,c);
          choca = true;
        }
      }
    });

    return choca;
  }

  bordesMapaLaterales(){
    var posPersonaje = new THREE.Vector3();
    this.personaje.getCuerpo().getWorldPosition(posPersonaje);

    if(posPersonaje.x <= -10){
      this.personaje.position.x = this.personaje.position.x + 1;
      return true;
    }

    if(posPersonaje.x >= 10){
      this.personaje.position.x = this.personaje.position.x - 1;
      return true;
    }

    return false;
  }

  bordesMapaVerticales(){
    var posPersonaje = new THREE.Vector3();
    this.personaje.getCuerpo().getWorldPosition(posPersonaje);

    if(posPersonaje.z <= -2.5){
      this.personaje.position.z = this.personaje.position.z + 1;
      return true;
    }

    if(posPersonaje.z >= this.recorrido.longitudRecorrido()){
      this.personaje.position.z = this.personaje.position.z - 1;
      return true;
    }

    return false;
  }
  
  cambiarColorPersonaje(color){
    this.personaje.cambiarColor(color);
  }
  
  update () {
    TWEEN.update();
  }
}

export { WipeIn };
