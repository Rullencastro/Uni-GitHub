

#include "miniwin.h"
#include <ctime>
#include <iostream>
#include "particula.h"
#include "definiciones.h"
#include "conjuntoparticulas.h"
#include "simulador.h"

using namespace miniwin;
using namespace std;

void pintaNube(const ConjuntoParticulas & miConjunto) {
    int N = miConjunto.GetNroParticulas();
    Particula p;
    for (int i = 0; i < N; i++) {
        p = miConjunto.ObtieneParticula(i);
        color(p.getColor());
        circulo_lleno(p.getX(), p.getY(), RADIO);
    }

}

int main() {
    srand(time(0));

    int ancho = 800;
    int alto = 600;

    int opcion;

    cout << "\n  Opcion a probar: ";
    cin >> opcion;

    switch (opcion) {

        case 1:
        {
            cout << "\n** Prueba sobrecarga de operadores **" << endl;
            //vredimensiona(100, 100);
            ConjuntoParticulas a1, b1(4);
            cout << b1 << endl;

            Particula x;
            a1.AgregaParticula(x);
            x.Mover(ancho, alto);
            a1.AgregaParticula(x);
            cout << a1 << endl;

            b1.Mover(ancho, alto);
            a1 = a1 + b1;
            cout << a1 << endl;

            ConjuntoParticulas cc = a1 + b1;
            ConjuntoParticulas xx(cc);
            cout << xx << endl;
            cout << endl;
            vcierra();

            break;
        }

        case 2:
        {
            cout << "\n** Prueba Conjunto Particulas ** " << endl;
            int NRO = 20;
            vredimensiona(ancho, alto);
            ConjuntoParticulas rojas(NRO);
            ConjuntoParticulas verdes(NRO);
            ConjuntoParticulas blancas;
            Particula p;


            for (int i = 0; i < NRO; i++) {
                p = rojas.ObtieneParticula(i);
                p.setColor(ROJO);
                rojas.ReemplazaParticula(i, p);

                p = verdes.ObtieneParticula(i);
                p.setColor(VERDE);
                verdes.ReemplazaParticula(i, p);
            }

            ConjuntoParticulas aux;
            for (int i = 0; i < 200; i++) {
                rojas.Mover(ancho, alto);
                verdes.Mover(ancho, alto);
                blancas.Mover(ancho, alto);
                aux = rojas + verdes + blancas;

                borra();
                pintaNube(aux);

                if (i % 10 == 0) {
                    Particula p;
                    p.setColor(BLANCO);
                    blancas.AgregaParticula(p);
                    rojas.BorraParticula(rand() % rojas.GetNroParticulas());
                    verdes.BorraParticula(rand() % verdes.GetNroParticulas());

                }

                refresca();
                espera(35);
            }
            vcierra();

            break;
        }

        case 3:
        {
            cout << "\n**************  Probando Simulador ****************\n";
            ConjuntoParticulas base(30);
            ConjuntoParticulas otro(10);
            ConjuntoParticulas aux;
            int contador = 0;

            Simulador game(base, otro, ancho, alto);

            while (contador < 500) {
                game.Step();
                game.Pintar(30);
                contador++;
            }
            break;
        }
        
        case 4:
        {
            cout << "\n************** Probando gestión dinámica de memoria **********\n";
            ConjuntoParticulas movil(10);
            ConjuntoParticulas fijo(5);
            int t=tecla();
            int contador = 0;
            
            Simulador mySim(movil, fijo,ancho,alto);
            while ((t != ESCAPE)&&(contador <1000)){
            mySim.Step();
            mySim.anadirMoviles(t);
            mySim.borrarMoviles(t);
            mySim.anadirFijas(t);
            mySim.borrarFijas(t);
            mySim.Pintar(25);
            t=tecla();
            contador++;
            }
            break;
        }

    };

    return 0;
}
