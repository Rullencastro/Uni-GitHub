import * as THREE from '../libs/three.module.js';
import * as TWEEN from '../libs/tween.esm.js';
import { Puño } from "./Puño.js"

class ParedObstaculo extends THREE.Object3D{
    constructor(){
        super();

        this.orientacion = 0; //0 de derecha a izquierda , 1 de izquierda a derecha

        var textpared = new THREE.MeshPhongMaterial({map: new THREE.TextureLoader().load('texturas/palo.jpg')});

        this.puño1 = new Puño();
        this.puño2 = new Puño();
        this.puño3 = new Puño();
        this.puño4 = new Puño();
        this.puño5 = new Puño();

        this.puño1.position.z = 10;
        this.puño2.position.z = 5;
        this.puño2.position.y = -1;
        this.puño3.position.y = 1;
        this.puño4.position.z = -5;
        this.puño4.position.y = -2;
        this.puño5.position.z = -10;
        this.puño5.position.y = 2;

        this.puño1.anim4.start();
        this.puño4.anim1.start();
        this.puño2.anim3.start();
        this.puño5.anim2.start();
        this.puño3.anim0.start();

        //--------------Pared-------------------------
        this.pared = new THREE.Mesh(new THREE.BoxBufferGeometry (2,7,25), textpared);
        this.pared.position.x = 1;
        this.add(this.pared);

        this.punios = []
        this.punios.push(this.puño1)
        this.punios.push(this.puño2)
        this.punios.push(this.puño3)
        this.punios.push(this.puño4)
        this.punios.push(this.puño5)

        this.punios.forEach(punio => this.add(punio));

    }

    getTipo(){  
        return 1;
    }

    setOrientacion(valor){
        this.orientacion = valor;
        if(valor == 0)
            this.punios.forEach(punio => punio.getPelotaPuño().scale.z = -1);
    }

    getOrientacion(){
        return this.orientacion;    
    }

    update () {
        TWEEN.update();
    }

  }

  export { ParedObstaculo };