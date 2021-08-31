#include "../Comportamientos_Jugador/jugador.hpp"
#include "motorlib/util.h"

#include <iostream>
#include <cmath>
#include <set>
#include <stack>
#include <queue>

//Función para el nivel 2 que cambia la variable de mapaResultado en función
//de lo que detectan los sensores de terreno, es decir, funcion para
//pintar el mapa.
void Pintar(vector<vector<unsigned char>> & m, Sensores &s){
    m[s.posF][s.posC] = s.terreno[0];
    switch(s.sentido){
    case 0:
        m[s.posF - 1][s.posC - 1] = s.terreno[1];
        m[s.posF - 1][s.posC] = s.terreno[2];
        m[s.posF - 1][s.posC + 1] = s.terreno[3];
        m[s.posF - 2][s.posC - 2] = s.terreno[4];
        m[s.posF - 2][s.posC - 1] = s.terreno[5];
        m[s.posF - 2][s.posC] = s.terreno[6];
        m[s.posF - 2][s.posC + 1] = s.terreno[7];
        m[s.posF - 2][s.posC + 2] = s.terreno[8];
        m[s.posF - 3][s.posC - 3] = s.terreno[9];
        m[s.posF - 3][s.posC - 2] = s.terreno[10];
        m[s.posF - 3][s.posC - 1] = s.terreno[11];
        m[s.posF - 3][s.posC] = s.terreno[12];
        m[s.posF - 3][s.posC + 1] = s.terreno[13];
        m[s.posF - 3][s.posC + 2] = s.terreno[14];
        m[s.posF - 3][s.posC + 3] = s.terreno[15];
        break;
    case 1:
        m[s.posF - 1][s.posC + 1] = s.terreno[1];
        m[s.posF][s.posC + 1] = s.terreno[2];
        m[s.posF + 1][s.posC + 1] = s.terreno[3];
        m[s.posF - 2][s.posC + 2] = s.terreno[4];
        m[s.posF - 1][s.posC + 2] = s.terreno[5];
        m[s.posF][s.posC + 2] = s.terreno[6];
        m[s.posF + 1][s.posC + 2] = s.terreno[7];
        m[s.posF + 2][s.posC + 2] = s.terreno[8];
        m[s.posF - 3][s.posC + 3] = s.terreno[9];
        m[s.posF - 2][s.posC + 3] = s.terreno[10];
        m[s.posF - 1][s.posC + 3] = s.terreno[11];
        m[s.posF][s.posC + 3] = s.terreno[12];
        m[s.posF + 1][s.posC + 3] = s.terreno[13];
        m[s.posF + 2][s.posC + 3] = s.terreno[14];
        m[s.posF + 3][s.posC + 3] = s.terreno[15];
        break;
    case 2:
        m[s.posF + 1][s.posC + 1] = s.terreno[1];
        m[s.posF + 1][s.posC] = s.terreno[2];
        m[s.posF + 1][s.posC - 1] = s.terreno[3];
        m[s.posF + 2][s.posC + 2] = s.terreno[4];
        m[s.posF + 2][s.posC + 1] = s.terreno[5];
        m[s.posF + 2][s.posC] = s.terreno[6];
        m[s.posF + 2][s.posC - 1] = s.terreno[7];
        m[s.posF + 2][s.posC - 2] = s.terreno[8];
        m[s.posF + 3][s.posC + 3] = s.terreno[9];
        m[s.posF + 3][s.posC + 2] = s.terreno[10];
        m[s.posF + 3][s.posC + 1] = s.terreno[11];
        m[s.posF + 3][s.posC] = s.terreno[12];
        m[s.posF + 3][s.posC - 1] = s.terreno[13];
        m[s.posF + 3][s.posC - 2] = s.terreno[14];
        m[s.posF + 3][s.posC - 3] = s.terreno[15];
        break;
    case 3:
        m[s.posF + 1][s.posC - 1] = s.terreno[1];
        m[s.posF][s.posC - 1] = s.terreno[2];
        m[s.posF - 1][s.posC - 1] = s.terreno[3];
        m[s.posF + 2][s.posC - 2] = s.terreno[4];
        m[s.posF + 1][s.posC - 2] = s.terreno[5];
        m[s.posF][s.posC - 2] = s.terreno[6];
        m[s.posF - 1][s.posC - 2] = s.terreno[7];
        m[s.posF - 2][s.posC - 2] = s.terreno[8];
        m[s.posF + 3][s.posC - 3] = s.terreno[9];
        m[s.posF + 2][s.posC - 3] = s.terreno[10];
        m[s.posF + 1][s.posC - 3] = s.terreno[11];
        m[s.posF][s.posC - 3] = s.terreno[12];
        m[s.posF - 1][s.posC - 3] = s.terreno[13];
        m[s.posF - 2][s.posC - 3] = s.terreno[14];
        m[s.posF - 3][s.posC - 3] = s.terreno[15];
        break;
    }
}
// Este es el método principal que debe contener los 4 Comportamientos_Jugador
// que se piden en la práctica. Tiene como entrada la información de los
// sensores y devuelve la acción a realizar.
Action ComportamientoJugador::think(Sensores sensores) {
	Action accion = actIDLE;
    Pintar(mapaResultado,sensores);

    actual.fila = sensores.posF;												//actualizamos el estado actual del personaje
    actual.columna = sensores.posC;
    actual.orientacion = sensores.sentido;
    if(!irRec){																	//si no tiene que ir a la casilla de recarga
        destino.fila = sensores.destinoF;										//actualiza el destino con el contenido 
        destino.columna = sensores.destinoC;									//lo que marquen los sensores de destino
    }

    if(mapaResultado[sensores.posF][sensores.posC] == 'K' )						//si paso por la casilla de las zapatillas o el bikini
        actual.bikini = true;													//actualizo el estado poniendo a true 	
    if(mapaResultado[sensores.posF][sensores.posC] == 'D' )						//las variables correspondientes
        actual.zapatillas = true;

    if(sensores.terreno[0] == 'X' and contRecarga < mapaResultado.size()){		//si estoy en la casilla de recarga el contador de recarga 
        irRec = false;															//es menor que el tamaño del mapa, pongo que ya no voy a recargar
        hayplan = false;														//que recalcule plan , y aumente el contador, devolviendo
        contRecarga++;															//un actIDLE, haciendo que itere en la casilla de recarga
        return actIDLE;
    }

    if(sensores.terreno[0] == 'X' and contPaso < 10 and hayplan){				//en caso de lo anterior, pero ademas hay un plan activo, es decir
        contPaso++;																//estoy pasando por encima de la casilla porque el camino pasa por 
        return actIDLE;															//ahi, recargo 100 de bateria, aumentando el contador de pasar
    }

    if(sensores.terreno[0] != 'X' ){											//una vez estando en otra casilla, es decir despues de haber pasado
        contPaso=0;																//por la casilla de recarga, que restablezca el contador de paso
    }

    if(sensores.destinoC != copiaDestino.columna or sensores.destinoF != copiaDestino.fila){  	//cuando he llegado al objetivo y los sensores de destino marcan otro lugar
        copiaDestino.fila = sensores.destinoF;													//igualo la copia del destino a los sensores de destino y replanifico
        copiaDestino.columna = sensores.destinoC;								
        hayplan = false;
        if(sensores.bateria < 250 and recargaEnc and sensores.vida >200){						//en caso de que la bateria sea menor de 250 ,haya encontrado la casilla de
            destino.fila = filRec;																//recarga y el tiempo que le queda(vida) sea mayor de 200
            destino.columna = colRec;															//cambiamos el destino con el cual se replanifica a las coordenadas de
            hayplan = false;																	//la casilla de recarga, ponemos a que estamos yendo a recargar ,con el 
            irRec = true;																		//contador de recarga a 0, y replanificamos para ir a la casilla de recarga
            contRecarga = 0;
        }
    }

    unsigned int sentidoSiguientePaso = siguientePaso(sensores);

    if(plan.size()>0 and sensores.terreno[2]== 'B' and !actual.zapatillas and sensores.sentido == sentidoSiguientePaso)		//si me encuentro un arbol,agua,muro o un precipicio
        hayplan = false;																									//y la siguiente casilla a la que voy es esa, recalculo
    if(plan.size()>0 and sensores.terreno[2]== 'A' and !actual.bikini and sensores.sentido == sentidoSiguientePaso)			//el plan, y en el caso del arbol y el agua,
        hayplan = false;																									//lo hago en caso de no tener zapatillas y bikini
    if(plan.size()>0 and sensores.terreno[12]== 'M' and sensores.sentido == sentidoSiguientePaso)							//respectivamente
        hayplan = false;
    if(sensores.terreno[2]== 'P' and sensores.sentido == sentidoSiguientePaso)
        hayplan = false;

    if(!recargaEnc){													//aqui, si no he encontrado la casilla de recarga, recorro
        for(int i=0;i<15;i++){											//los sensores de terreno , en caso de verla pongo que la 
            if(sensores.terreno[i] == 'X')								//he encontrado
                recargaEnc = true;
        }
        if(recargaEnc){													//si la he encontrado, busco en todo el mapa donde se encuentra
            bool salir = false;											//y guardo sus coordenadas
            for(int i=0;i< mapaResultado.size() and !salir ;i++){
                for(int j=0;j<mapaResultado.size() and !salir;j++){
                    if(mapaResultado[i][j] == 'X'){
                        filRec = i;
                        colRec = j;
                        salir = true;
                    }
                }
            }
        }
    }

	if(!hayplan){																//aqui es donde recalcula, es decir llama al metodo de busqueda
        hayplan = pathFinding(sensores.nivel , actual, destino , plan);
        cout << "Bateria: " << sensores.bateria<<endl;
        cout << "Tiempo: " << sensores.vida << endl;
	}

    Action sigAccion;
    if(hayplan and plan.size()>0 and !sensores.colision){									//si hay plan y no estoy colisionando con nada, cojo la siguiente
        if(sensores.superficie[2] == 'a' and sentidoSiguientePaso != sensores.sentido){		//accion y la borro del plan, en caso de tener delante un aldeano
            sigAccion = plan.front();														//y que la casilla siguiente del camino sea donde esta el aldeano
            plan.erase(plan.begin());														//me quedo quieto sin consumir pasos del plan
        }
        else if(sensores.superficie[2] == 'a' and sentidoSiguientePaso == sensores.sentido)
            sigAccion = actIDLE;

        else{
            sigAccion = plan.front();
            plan.erase(plan.begin());
        }
    }
    else if(sensores.colision)								//en caso de colisionar con algo, replanifico
        hayplan = false;


  return sigAccion;
}

//Funcion con la que obtengo el sentido del siguiente paso
//con la que puedo saber a que casilla voy en el siguiente paso
unsigned int ComportamientoJugador::siguientePaso(Sensores s)
{
    unsigned int paso;

    if(plan.front()==actFORWARD and s.sentido == 0)
        paso = 0;
    else if(plan.front()==actFORWARD and s.sentido == 1)
        paso = 1;
    else if(plan.front()==actFORWARD and s.sentido == 2)
        paso = 2;
    else if(plan.front()==actFORWARD and s.sentido == 3)
        paso = 3;
    else if(plan.front()==actTURN_L and s.sentido == 0)
        paso = 3;
    else if(plan.front()==actTURN_L and s.sentido == 1)
        paso = 0;
    else if(plan.front()==actTURN_L and s.sentido == 2)
        paso = 1;
    else if(plan.front()==actTURN_L and s.sentido == 3)
        paso = 2;
    else if(plan.front()==actTURN_R and s.sentido == 0)
        paso = 1;
    else if(plan.front()==actTURN_R and s.sentido == 1)
        paso = 2;
    else if(plan.front()==actTURN_R and s.sentido == 2)
        paso = 3;
    else if(plan.front()==actTURN_R and s.sentido == 3)
        paso = 0;

    return paso;
}



// Llama al algoritmo de busqueda que se usará en cada comportamiento del agente
// Level representa el comportamiento en el que fue iniciado el agente.
bool ComportamientoJugador::pathFinding (int level, const estado &origen, const estado &destino, list<Action> &plan){
	switch (level){
		case 1: cout << "Busqueda en profundad\n";
			      return pathFinding_Profundidad(origen,destino,plan);
						break;
		case 2: cout << "Busqueda en Anchura\n";
			      return pathFinding_Anchura(origen,destino,plan);
						break;
		case 3: cout << "Busqueda Costo Uniforme\n";
                    return pathFinding_CosteUniforme(origen,destino,plan);
						break;
		case 4: cout << "Busqueda para el reto\n";
					return pathFinding_CosteUniforme(origen,destino,plan);
						break;
	}
	cout << "Comportamiento sin implementar\n";
	return false;
}


//---------------------- Implementación de la busqueda en profundidad ---------------------------

// Dado el código en carácter de una casilla del mapa dice si se puede
// pasar por ella sin riegos de morir o chocar.
bool EsObstaculo(unsigned char casilla){
	if (casilla=='P' or casilla=='M')
		return true;
	else
	  return false;
}


// Comprueba si la casilla que hay delante es un obstaculo. Si es un
// obstaculo devuelve true. Si no es un obstaculo, devuelve false y
// modifica st con la posición de la casilla del avance.
bool ComportamientoJugador::HayObstaculoDelante(estado &st){
	int fil=st.fila, col=st.columna;

  // calculo cual es la casilla de delante del agente
	switch (st.orientacion) {
		case 0: fil--; break;
		case 1: col++; break;
		case 2: fil++; break;
		case 3: col--; break;
	}

	// Compruebo que no me salgo fuera del rango del mapa
	if (fil<0 or fil>=mapaResultado.size()) return true;
	if (col<0 or col>=mapaResultado[0].size()) return true;

	// Miro si en esa casilla hay un obstaculo infranqueable
	if (!EsObstaculo(mapaResultado[fil][col])){
		// No hay obstaculo, actualizo el parámetro st poniendo la casilla de delante.
    st.fila = fil;
		st.columna = col;
		return false;
	}
	else{
	  return true;
	}
}



//Estructura de nodo que se usa para la búsqueda en profundidad y en anchura
struct nodo{
	estado st;
	list<Action> secuencia;
};

//Functor que compara los estados en la lista de cerrados tanto en 
//anchura como en profundidad
struct ComparaEstados{
	bool operator()(const estado &a, const estado &n) const{
		if ((a.fila > n.fila) or (a.fila == n.fila and a.columna > n.columna) or
	      (a.fila == n.fila and a.columna == n.columna and a.orientacion > n.orientacion))
			return true;
		else
			return false;
	}
};


// Implementación de la búsqueda en profundidad.
// Entran los puntos origen y destino y devuelve la
// secuencia de acciones en plan, una lista de acciones.
bool ComportamientoJugador::pathFinding_Profundidad(const estado &origen, const estado &destino, list<Action> &plan) {
	//Borro la lista
	cout << "Calculando plan\n";
	plan.clear();
	set<estado,ComparaEstados> generados; // Lista de Cerrados
	stack<nodo> pila;											// Lista de Abiertos

  nodo current;
	current.st = origen;
	current.secuencia.empty();

	pila.push(current);

  while (!pila.empty() and (current.st.fila!=destino.fila or current.st.columna != destino.columna)){

		pila.pop();
		generados.insert(current.st);

		// Generar descendiente de girar a la derecha
		nodo hijoTurnR = current;
		hijoTurnR.st.orientacion = (hijoTurnR.st.orientacion+1)%4;
		if (generados.find(hijoTurnR.st) == generados.end()){
			hijoTurnR.secuencia.push_back(actTURN_R);
			pila.push(hijoTurnR);

		}

		// Generar descendiente de girar a la izquierda
		nodo hijoTurnL = current;
		hijoTurnL.st.orientacion = (hijoTurnL.st.orientacion+3)%4;
		if (generados.find(hijoTurnL.st) == generados.end()){
			hijoTurnL.secuencia.push_back(actTURN_L);
			pila.push(hijoTurnL);
		}

		// Generar descendiente de avanzar
		nodo hijoForward = current;
		if (!HayObstaculoDelante(hijoForward.st)){
			if (generados.find(hijoForward.st) == generados.end()){
				hijoForward.secuencia.push_back(actFORWARD);
				pila.push(hijoForward);
			}
		}

		// Tomo el siguiente valor de la pila
		if (!pila.empty()){
			current = pila.top();
		}
	}

  cout << "Terminada la busqueda\n";

	if (current.st.fila == destino.fila and current.st.columna == destino.columna){
		cout << "Cargando el plan\n";
		plan = current.secuencia;
		cout << "Longitud del plan: " << plan.size() << endl;
		PintaPlan(plan);
		// ver el plan en el mapa
		VisualizaPlan(origen, plan);
		return true;
	}
	else {
		cout << "No encontrado plan\n";
	}


	return false;
}

// Implementación de la búsqueda en anchura.
// Entran los puntos origen y destino y devuelve la
// secuencia de acciones en plan, una lista de acciones.
bool ComportamientoJugador::pathFinding_Anchura(const estado &origen, const estado &destino, list<Action> &plan){
    cout << "Calculando plan\n";
	plan.clear();
	set<estado,ComparaEstados> generados;
	queue<nodo> cola;

    nodo current;
	current.st = origen;
	current.secuencia.empty();

	cola.push(current);

	while(!cola.empty() and (current.st.fila!=destino.fila or current.st.columna != destino.columna)){
        cola.pop();
		generados.insert(current.st);

		// Generar descendiente de girar a la derecha
		nodo hijoTurnR = current;
		hijoTurnR.st.orientacion = (hijoTurnR.st.orientacion+1)%4;
		if (generados.find(hijoTurnR.st) == generados.end()){
			hijoTurnR.secuencia.push_back(actTURN_R);
			cola.push(hijoTurnR);

		}

		// Generar descendiente de girar a la izquierda
		nodo hijoTurnL = current;
		hijoTurnL.st.orientacion = (hijoTurnL.st.orientacion+3)%4;
		if (generados.find(hijoTurnL.st) == generados.end()){
			hijoTurnL.secuencia.push_back(actTURN_L);
			cola.push(hijoTurnL);
		}

		// Generar descendiente de avanzar
		nodo hijoForward = current;
		if (!HayObstaculoDelante(hijoForward.st)){
			if (generados.find(hijoForward.st) == generados.end()){
				hijoForward.secuencia.push_back(actFORWARD);
				cola.push(hijoForward);
			}
		}

		// Tomo el siguiente valor de la pila
		if (!cola.empty()){
			current = cola.front();
		}
	}

  cout << "Terminada la busqueda\n";

	if (current.st.fila == destino.fila and current.st.columna == destino.columna){
		cout << "Cargando el plan\n";
		plan = current.secuencia;
		cout << "Longitud del plan: " << plan.size() << endl;
		PintaPlan(plan);
		// ver el plan en el mapa
		VisualizaPlan(origen, plan);
		return true;
	}
	else {
		cout << "No encontrado plan\n";
	}

	return false;
}

//Estructura para los nodos para el método de búsqueda por coste uniforme
struct nodoCoste{
	estado st;
	list<Action> secuencia;
	int coste;
};

//Functor para la lista de abiertos que compara los nodos segun su coste
//en el metodo de busqueda por coste uniforme
struct ComparaNodosCoste{
	bool operator()(const nodoCoste &a, const nodoCoste &n) const{
		if(a.coste > n.coste)
			return true;
		else
			return false;
	}
};

//Functor que compara la lista de cerrados en el metodo de busqueda por 
//coste uniforme, donde se añade a la comparacion, el bikini y las zapatillas
struct ComparaNodos{
	bool operator()(const nodoCoste &a, const nodoCoste &n) const{
		if ((a.st.fila > n.st.fila) or (a.st.fila == n.st.fila and a.st.columna > n.st.columna) or
	      (a.st.fila == n.st.fila and a.st.columna == n.st.columna and a.st.orientacion > n.st.orientacion) or
          (a.st.fila == n.st.fila and a.st.columna == n.st.columna and a.st.orientacion == n.st.orientacion and a.st.bikini > n.st.bikini) or
          (a.st.fila == n.st.fila and a.st.columna == n.st.columna and a.st.orientacion == n.st.orientacion and a.st.bikini == n.st.bikini and a.st.zapatillas > n.st.zapatillas))
			return true;
		else
			return false;
	}
};


//Funcion que devuelve el coste del movimiento a un nodo 
//dependiendo del tipo del casilla al que vaya y del equipamiento que se tenga
int ComportamientoJugador::costeMovimiento(const estado &a){
    char casilla = mapaResultado[a.fila][a.columna];

    if(casilla == 'A' && a.bikini == false)
        return 100;
    else if(casilla == 'A' && a.bikini == true)
        return 10;
    else if(casilla == 'B' && a.zapatillas == false)
        return 50;
    else if(casilla == 'B' && a.zapatillas == true)
        return 5;
    else if(casilla == 'T')
        return 2;
    else if(casilla == 'P')
        return 150;
    else if(casilla == 'X')
        return 0;
    else
        return 1;
}


// Implementación de la búsqueda por costo uniforme.
// Entran los puntos origen y destino y devuelve la
// secuencia de acciones en plan, una lista de acciones.
bool ComportamientoJugador::pathFinding_CosteUniforme(const estado &origen, const estado &destino, list<Action> &plan){
    cout << "Calculando plan\n";
	plan.clear();
	set<nodoCoste,ComparaNodos> generados;
	priority_queue<nodoCoste,vector<nodoCoste>,ComparaNodosCoste> cola;

    nodoCoste current;
	current.st = origen;
	current.secuencia.empty();
    current.coste = 0;

	cola.push(current);

	while(!cola.empty() and (current.st.fila!=destino.fila or current.st.columna != destino.columna)){
        cola.pop();
        if(mapaResultado[current.st.fila][current.st.columna] == 'K'){
            current.st.bikini = true;
        }

        if(mapaResultado[current.st.fila][current.st.columna] == 'D'){
            current.st.zapatillas = true;
        }

		generados.insert(current);

		// Generar descendiente de girar a la derecha
		nodoCoste hijoTurnR = current;
		hijoTurnR.st.orientacion = (hijoTurnR.st.orientacion+1)%4;
		if (generados.find(hijoTurnR) == generados.end()){
			hijoTurnR.secuencia.push_back(actTURN_R);
			hijoTurnR.coste += costeMovimiento(current.st);
			cola.push(hijoTurnR);

		}

		// Generar descendiente de girar a la izquierda
		nodoCoste hijoTurnL = current;
		hijoTurnL.st.orientacion = (hijoTurnL.st.orientacion+3)%4;
		if (generados.find(hijoTurnL) == generados.end()){
			hijoTurnL.secuencia.push_back(actTURN_L);
			hijoTurnL.coste += costeMovimiento(current.st);
			cola.push(hijoTurnL);
		}

		// Generar descendiente de avanzar
		nodoCoste hijoForward = current;
		if (!HayObstaculoDelante(hijoForward.st)){
			if (generados.find(hijoForward) == generados.end()){
				hijoForward.secuencia.push_back(actFORWARD);
				hijoForward.coste += costeMovimiento(current.st);
				cola.push(hijoForward);
			}
		}

		// Tomo el siguiente valor de la pila
		if (!cola.empty()){
			current = cola.top();
		}
	}

  cout << "Terminada la busqueda\n";

	if (current.st.fila == destino.fila and current.st.columna == destino.columna){
		cout << "Cargando el plan\n";
		plan = current.secuencia;
		cout << "Longitud del plan: " << plan.size() << endl;
		PintaPlan(plan);
		// ver el plan en el mapa
		VisualizaPlan(origen, plan);
		return true;
	}
	else {
		cout << "No encontrado plan\n";
	}

	return false;
}



// Sacar por la términal la secuencia del plan obtenido
void ComportamientoJugador::PintaPlan(list<Action> plan) {
	auto it = plan.begin();
	while (it!=plan.end()){
		if (*it == actFORWARD){
			cout << "A ";
		}
		else if (*it == actTURN_R){
			cout << "D ";
		}
		else if (*it == actTURN_L){
			cout << "I ";
		}
		else {
			cout << "- ";
		}
		it++;
	}
	cout << endl;
}



void AnularMatriz(vector<vector<unsigned char> > &m){
	for (int i=0; i<m[0].size(); i++){
		for (int j=0; j<m.size(); j++){
			m[i][j]=0;
		}
	}
}


// Pinta sobre el mapa del juego el plan obtenido
void ComportamientoJugador::VisualizaPlan(const estado &st, const list<Action> &plan){
  AnularMatriz(mapaConPlan);
	estado cst = st;

	auto it = plan.begin();
	while (it!=plan.end()){
		if (*it == actFORWARD){
			switch (cst.orientacion) {
				case 0: cst.fila--; break;
				case 1: cst.columna++; break;
				case 2: cst.fila++; break;
				case 3: cst.columna--; break;
			}
			mapaConPlan[cst.fila][cst.columna]=1;
		}
		else if (*it == actTURN_R){
			cst.orientacion = (cst.orientacion+1)%4;
		}
		else {
			cst.orientacion = (cst.orientacion+3)%4;
		}
		it++;
	}
}



int ComportamientoJugador::interact(Action accion, int valor){
  return false;
}
