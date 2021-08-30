import * as THREE from '../libs/three.module.js';
import * as TWEEN from '../libs/tween.esm.js';
import { Puño } from "./Puño.js"

class ParedObstaculo extends THREE.Object3D{
    constructor(){
        super();

        var rojo = new THREE.MeshPhongMaterial({color: 0xFF0000});
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

        //--------------Pared-------------------------
        this.pared = new THREE.Mesh(new THREE.BoxBufferGeometry (2,7,25), rojo);
        this.pared.position.x = 1;
        this.add(this.pared);

        this.add(this.puño1);
        this.add(this.puño2);
        this.add(this.puño3);
        this.add(this.puño4);
        this.add(this.puño5);

        this.puño1.anim0.start();
        this.puño4.anim1.start();
        this.puño2.anim2.start();
        this.puño5.anim2.start();
        this.puño3.anim0.start();
    }

    update () {
        TWEEN.update();
    }

  }

  export { ParedObstaculo };