#pragma once
#pragma once
// Librerias para la funcion del codigo
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <windows.h>
#include <stdlib.h>
#include <cstdlib>
#include <time.h>
#include <string>
#include <random>
#include <functional>

// librerias de allegro
#include <allegro5/allegro.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

const int maxFrame = 1;
int frameactual = 0;
int contadorDeFrames = 0;
int Delay = 25;
int alturaDeFrame = 1080;
int AnchoDeFrame = 1920;
#pragma warning(disable:4996); // quitar problemas con archivos
#define FPS 60.0

using namespace std;

typedef struct enemigo {
	int x;
	int y;
	float velocidady;
	int size;
	int vida;
	int fueraPantalla; //0 == no, 1 == si, 2 == no existe 
	ALLEGRO_BITMAP* imagen;
	bool direccion; // false == abajo, true arriba

	void probabilidad_aparicion(int x1, int y1) {
		// Crear un generador de números aleatorios
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> distribucion(1, 3);
		// Generar un número aleatorio entre 1 y 20
		int numeroAleatorio = distribucion(gen);
		// Ejecutar el bloque de código si el número es 1
		if (numeroAleatorio == 1) {
			aparicion_enemigo();
		}
	}
	void aparicion_enemigo() {
		x = 86 + rand() % (799);
		y = 475 + rand() % (775 - 475 - 48 + 1);
		fueraPantalla = 0;
	}
	void pintar_enemigo(int ResolucionY) {
		if (fueraPantalla == 0 && y < ResolucionY) {
			al_draw_scaled_bitmap(imagen, 0, 0, al_get_bitmap_width(imagen), al_get_bitmap_height(imagen), x, y, size, size, 0);
		}
	}

}enemigo;

typedef struct bola {
	float x; // Posicion en x
	float y; // Posicion en y
	float velocidadx;
	float velocidady;
	float bouncerx; // Posicion del rebote x
	float bouncery; // Posicion del rebote y 
	int size;
	int enPantalla; //si = 0, no = 1, no existe = 2
	ALLEGRO_BITMAP* imagen;
} bola;

typedef struct plataforma {
	int x;
	int y;
	float velocidad;
	int sizeX;
	int sizeY;
	ALLEGRO_BITMAP* imagen;

}plataforma;

typedef struct powerUp {
	int x;
	int y;
	int sizeX = 20;
	int sizeY = 10;
	ALLEGRO_BITMAP* imagen;
	int tomado; //0 == no, 1 == si, 2 == no existe 
	float velocidad;
	int tipo; //1, 2 o 3

	void probabilidadPowerUp(int x1, int y1) {
		// Crear un generador de números aleatorios
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> distribucion(1, 3);
		// Generar un número aleatorio entre 1 y 10
		int numeroAleatorio = distribucion(gen);
		// Ejecutar el bloque de código si el número es 1
		if (numeroAleatorio == 1) {
			datosPowerUp(x1, y1);
		}
	}
	void datosPowerUp(int LadrilloX, int LadrilloY) {
		x = LadrilloX;
		y = LadrilloY;
		tomado = 0;
	}

	void pintarPowerUp(int ResolucionY) {
		if (tomado == 0 && y < ResolucionY) {
			al_draw_scaled_bitmap(imagen, 0, 0, al_get_bitmap_width(imagen), al_get_bitmap_height(imagen), x, y, sizeX, sizeY, 0);
		}
	}

	void accionPowerUp(bola& bola1, bola& bola2, bola& bola3, ALLEGRO_TIMER* powerup_timer, ALLEGRO_EVENT_QUEUE* cola_eventos, int& vida) {
		//pastilla azul agrega 2 bolas extra
		if (tipo == 1) {
			if (bola1.enPantalla == 0) {
				bola2.enPantalla = 0;
				bola2.bouncerx = 250; //se coloca en el borde izquierdo
				bola2.bouncery = 250;
				bola3.enPantalla = 0;
				bola3.bouncerx = 500; //se coloca en el borde derecho
				bola3.bouncery = 300;
			}
			else if (bola2.enPantalla == 0) {
				bola1.enPantalla = 0;
				bola1.bouncerx = 250; //se coloca en el borde izquierdo
				bola1.bouncery = 250;
				bola3.enPantalla = 0;
				bola3.bouncerx = 500; //se coloca en el borde derecho
				bola3.bouncery = 300;
			}
			else if (bola3.enPantalla == 0) {
				bola1.enPantalla = 0;
				bola1.bouncerx = 250; //se coloca en el borde izquierdo
				bola1.bouncery = 250;
				bola2.enPantalla = 0;
				bola2.bouncerx = 500; //se coloca en el borde derecho
				bola2.bouncery = 300;
			}
		}

		//pastilla verde hace mas lenta la pelota
		else if (tipo == 2) {
			al_start_timer(powerup_timer);
			//baja la velocidad dependiendo de la direccion a la que se dirige la bola
			if (bola1.velocidadx < 0)
				bola1.velocidadx = -5.0;
			else
				bola1.velocidadx = 5.0;

			if (bola1.velocidady > 0)
				bola1.velocidady = 5.0;
			else
				bola1.velocidady = -5.0;

			//existe bola2
			if (bola2.enPantalla == 0) {
				if (bola2.velocidadx < 0)
					bola2.velocidadx = -5.0;
				else
					bola2.velocidadx = 5.0;

				if (bola2.velocidady > 0)
					bola2.velocidady = 5.0;
				else
					bola2.velocidady = -5.0;
			}

			//existe bola3
			if (bola3.enPantalla == 0) {
				if (bola3.velocidadx < 0)
					bola3.velocidadx = -5.0;
				else
					bola3.velocidadx = 5.0;

				if (bola3.velocidady > 0)
					bola3.velocidady = 5.0;
				else
					bola3.velocidady = -5.0;
			}
		}

		//pastilla roja agrega una vida extra
		else if (tipo == 3) {
			vida += 1;
		}
		tomado = 2;
	}
}powerUp;

typedef struct ladrillo {
	int x;
	int y;
	int sizeX;
	int sizeY;
	ALLEGRO_BITMAP* imagen;
	int vida;
	//este metodo se encarga de pintar el ladrillo y crear la lógica de la colisión hacia este
	void pintarLadrillo(bola& bola1, bola& bola2, bola& bola3, int& puntaje, powerUp& powerUp, enemigo& enemigo) {
		if (vida > 0) {
			al_draw_scaled_bitmap(imagen, 0, 0, al_get_bitmap_width(imagen), al_get_bitmap_height(imagen), x, y, sizeX, sizeY, 0);
		}

		// Verificar colisión con el ladrillo
		//bouncer + 35 porque 35 es el tamaño de la pelota
		if (bola1.bouncerx + 35 >= x && bola1.bouncerx <= x + sizeX &&
			bola1.bouncery + 35 >= y && bola1.bouncery <= y + sizeY && vida > 0) {

			// Determinar el lado de la colisión
			if (bola1.bouncerx + 35 - bola1.velocidadx <= x || bola1.bouncerx - bola1.velocidadx >= x + sizeX) {
				bola1.velocidadx = -bola1.velocidadx; // Rebote horizontal
				vida -= 1;
				//verificar si vida es 0 para sacar powerUp
				if (vida == 0 && bola1.enPantalla == 0 && bola2.enPantalla == 2 && bola3.enPantalla == 2) {
					//agregar probabilidad de sacar powerUp
					powerUp.probabilidadPowerUp(x, y);
				}

				if (vida == 0) {
					enemigo.probabilidad_aparicion(x, y);
				}
			}
			if (bola1.bouncery + 35 - bola1.velocidady <= y || bola1.bouncery - bola1.velocidady >= y + sizeY) {
				bola1.velocidady = -bola1.velocidady; // Rebote vertical
				vida -= 1;
				//verificar si vida es 0 para sacar powerUp
				if (vida == 0 && bola2.enPantalla == 2 && bola3.enPantalla == 2) {
					powerUp.probabilidadPowerUp(x, y);
				}

				if (vida == 0) {
					enemigo.probabilidad_aparicion(x, y);
				}
			}

			puntaje += 10; // Incrementar puntaje
		}

		if (bola2.enPantalla == 0) {

			if (bola2.bouncerx + 35 >= x && bola2.bouncerx <= x + sizeX &&
				bola2.bouncery + 35 >= y && bola2.bouncery <= y + sizeY && vida > 0) {

				// Determinar el lado de la colisión
				if (bola2.bouncerx + 35 - bola2.velocidadx <= x || bola2.bouncerx - bola2.velocidadx >= x + sizeX) {
					bola2.velocidadx = -bola2.velocidadx; // Rebote horizontal
					vida -= 1;
					//verificar si vida es 0 para sacar powerUp
					if (vida == 0 && bola2.enPantalla == 2 && bola3.enPantalla == 2) {
						//agregar probabilidad de sacar powerUp
						powerUp.datosPowerUp(x, y);
					}

					if (vida == 0) {
						enemigo.probabilidad_aparicion(x, y);
					}
				}
				if (bola2.bouncery + 35 - bola2.velocidady <= y || bola2.bouncery - bola2.velocidady >= y + sizeY) {
					bola2.velocidady = -bola2.velocidady; // Rebote vertical
					vida -= 1;
					//verificar si vida es 0 para sacar powerUp
					if (vida == 0 && bola2.enPantalla == 2 && bola3.enPantalla == 2) {
						powerUp.datosPowerUp(x, y);
					}

					if (vida == 0) {
						enemigo.probabilidad_aparicion(x, y);
					}
				}

				puntaje += 10; // Incrementar puntaje
			}
		}

		if (bola3.enPantalla == 0) {

			if (bola3.bouncerx + 35 >= x && bola3.bouncerx <= x + sizeX &&
				bola3.bouncery + 35 >= y && bola3.bouncery <= y + sizeY && vida > 0) {

				// Determinar el lado de la colisión
				if (bola3.bouncerx + 35 - bola3.velocidadx <= x || bola3.bouncerx - bola3.velocidadx >= x + sizeX) {
					bola3.velocidadx = -bola3.velocidadx; // Rebote horizontal
					vida -= 1;
					//verificar si vida es 0 para sacar powerUp
					if (vida == 0 && bola2.enPantalla == 2 && bola3.enPantalla == 2) {
						//agregar probabilidad de sacar powerUp
						powerUp.datosPowerUp(x, y);
					}
					if (vida == 0) {
						enemigo.probabilidad_aparicion(x, y);
					}

				}
				if (bola3.bouncery + 35 - bola3.velocidady <= y || bola3.bouncery - bola3.velocidady >= y + sizeY) {
					bola3.velocidady = -bola3.velocidady; // Rebote vertical
					vida -= 1;
					//verificar si vida es 0 para sacar powerUp
					if (vida == 0 && bola2.enPantalla == 2 && bola3.enPantalla == 2) {
						powerUp.datosPowerUp(x, y);
					}
					if (vida == 0) {
						enemigo.probabilidad_aparicion(x, y);
					}
				}

				puntaje += 10; // Incrementar puntaje
			}
		}
	}
	ladrillo* Siguiente;
}ladrillo;

typedef struct Estadisticas {
	int contadorLadrillos;
	int vidasGastadas;
	int bolasRebotadas;
	int enemigosDestruidos;
	int puntosTotales;
	int bolasPerdidas;
};

//-----------------------------------------------------FUNCIONES---------------------------------------------------


void pintarbola(bola bola) {
	if (bola.enPantalla == 0)
		al_draw_scaled_bitmap(bola.imagen, 0, 0, al_get_bitmap_width(bola.imagen), al_get_bitmap_height(bola.imagen), bola.bouncerx, bola.bouncery, bola.size, bola.size, 0);
}

int obtenerNumeroAleatorio() {
	// Generador de números aleatorios
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distrib(1, 3);

	// Retorna un número aleatorio entre 1 y 3
	return distrib(gen);
}

bool enemigoArribaAbajo() {
	// Crear un generador de números aleatorios
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distribucion(1, 10);
	// Generar un número aleatorio entre 1 y 20
	int numeroAleatorio = distribucion(gen);
	// Ejecutar el bloque de código si el número es 1
	if (numeroAleatorio == 1) {
		return false;
	}
	else
		return true;
}

//funcion que revisa si se puede seguir la ejecucion al perder una vida
void revisar_bolas_activas(bola& bola1, bola& bola2, bola& bola3, plataforma& plata, int vidas) {
	if ((bola1.enPantalla == 2 && bola2.enPantalla == 2 && bola3.enPantalla == 2) && vidas != 0){
		bola1.enPantalla = 0;
		bola1.bouncery = plata.y - bola1.size;
		bola1.bouncerx = plata.x;
	}
}

// si las bolas se salen del rango se borran
void borrar_bolas(bola& bola1, bola& bola2, bola& bola3, int ResolucionY, plataforma plata, int vidas, int&bolas_perdidas) {
	if (bola1.bouncery < 160 || bola1.bouncery > ResolucionY - 25) {
		bola1.enPantalla = 2;
		bolas_perdidas += 1;
		revisar_bolas_activas(bola1, bola2, bola3, plata, vidas);
	}
	if (bola2.bouncery < 160 || bola2.bouncery > ResolucionY - 25) {
		bola2.enPantalla = 2;
		bolas_perdidas += 1;
		revisar_bolas_activas(bola1, bola2, bola3, plata, vidas);
	}
	if (bola3.bouncery < 160 || bola3.bouncery > ResolucionY - 25) {
		bola3.enPantalla = 2;
		bolas_perdidas += 1;
		revisar_bolas_activas(bola1, bola2, bola3, plata, vidas);
	}
}

//funcion para quitar la vida si solo hay una bola
void vidas_bolas(bola& bola1, bola& bola2, bola& bola3, int ResolucionY, int& vidas, int&vidas_perdidas) {
	if ((bola1.enPantalla == 0 && bola2.enPantalla == 2 && bola3.enPantalla == 2) && (bola1.bouncery < 160 || bola1.bouncery > ResolucionY - 25)) {
		vidas -= 1;
		vidas_perdidas += 1;
	}
	if ((bola1.enPantalla == 2 && bola2.enPantalla == 0 && bola3.enPantalla == 2) && (bola2.bouncery < 160 || bola2.bouncery > ResolucionY - 25)) {
		vidas -= 1;
		vidas_perdidas += 1;
	}
	if ((bola1.enPantalla == 2 && bola2.enPantalla == 2 && bola3.enPantalla == 0) && (bola3.bouncery < 160 || bola3.bouncery > ResolucionY - 25)) {
		vidas -= 1;
		vidas_perdidas += 1;
	}
}

//-----------------------------------------------------------------------------------------------------------------
//                                              EXCLUSIVAS A UN JUGADOR
//funcion que revisa si se puede seguir la ejecucion al perder una vida
void revisar_bolas_activas1(bola& bola1, bola& bola2, bola& bola3, plataforma& plata, int vidas) {
	if ((bola1.enPantalla == 2 && bola2.enPantalla == 2 && bola3.enPantalla == 2) && vidas != 0){
		bola1.enPantalla = 0;
		bola1.bouncery = plata.y - bola1.size;
		bola1.bouncerx = plata.x;
	}
}

// si las bolas se salen del rango se borran
void borrar_bolas1(bola& bola1, bola& bola2, bola& bola3, int ResolucionY, plataforma plata, int vidas, int& bolas_perdidas) {
	if (bola1.bouncery > ResolucionY - 25 ) {
		bola1.enPantalla = 2;
		bolas_perdidas += 1;
		revisar_bolas_activas1(bola1, bola2, bola3, plata, vidas);
	}
	if (bola2.bouncery > ResolucionY - 25) {
		bola2.enPantalla = 2;
		bolas_perdidas += 1;
		revisar_bolas_activas1(bola1, bola2, bola3, plata, vidas);
	}
	if (bola3.bouncery > ResolucionY - 25) {
		bola3.enPantalla = 2;
		bolas_perdidas += 1;
		revisar_bolas_activas1(bola1, bola2, bola3, plata, vidas);
	}
}

//funcion para quitar la vida si solo hay una bola
void vidas_bolas1(bola& bola1, bola& bola2, bola& bola3, int ResolucionY, int& vidas, int& vidas_perdidas) {
	if ((bola1.enPantalla == 0 && bola2.enPantalla == 2 && bola3.enPantalla == 2) && (bola1.bouncery > ResolucionY - 25)) {
		vidas -= 1;
		vidas_perdidas += 1;
	}
	if ((bola1.enPantalla == 2 && bola2.enPantalla == 0 && bola3.enPantalla == 2) && (bola2.bouncery > ResolucionY - 25)) {
		vidas -= 1;
		vidas_perdidas += 1;
	}
	if ((bola1.enPantalla == 2 && bola2.enPantalla == 2 && bola3.enPantalla == 0) && (bola3.bouncery > ResolucionY - 25)) {
		vidas -= 1;
		vidas_perdidas += 1;
	}
}

//-----------------------------------------------------------------------------------------------------------------
//colison plataforma enemigo
void colision_plataforma_enemigo(plataforma& plata, enemigo& enemigo, bool arriba, int& vidas, int&choque_enemigo) {
	//plataforma de arriba
	if (arriba) {
		if ((enemigo.y >= plata.y && enemigo.y <= plata.y + plata.sizeY)
			&& (enemigo.x + enemigo.size >= plata.x && enemigo.x <= plata.x + plata.sizeX)) {
			enemigo.fueraPantalla = 1;
			enemigo.y = NULL;
			enemigo.x = NULL;
			vidas -= 1;
			choque_enemigo += 1;
		}
	}

	//plataforma de abajo
	else {
		if ((enemigo.y + enemigo.size >= plata.y && enemigo.y <= plata.y + plata.sizeY)
			&& (enemigo.x + enemigo.size >= plata.x && enemigo.x <= plata.x + plata.sizeX)) {
			enemigo.fueraPantalla = 1;
			enemigo.y = NULL;
			enemigo.x = NULL;
			vidas -= 1;
			choque_enemigo += 1;
		}
	}

}

void colision_bola_enemigo(enemigo& enemigo, int bouncerx, int bouncery, float& velocidadx, float& velocidady, int bolaSize, int &puntaje, int&bolas_rebotadas, int&choque_enemigo) {
	// Verifica si hay colisión con la plataforma
	if (bouncerx + bolaSize >= enemigo.x && bouncerx <= enemigo.x + enemigo.size &&
		bouncery + bolaSize >= enemigo.y && bouncery <= enemigo.y + enemigo.size) {

		// Rebote horizontal si hay colisión en los lados izquierdo o derecho de la plataforma
		if ((bouncerx + bolaSize - velocidadx <= enemigo.x) || (bouncerx - velocidadx >= enemigo.x + enemigo.size)) {
			velocidadx = -velocidadx;
		}

		// Rebote vertical si hay colisión en la parte superior o inferior de la plataforma
		if ((bouncery + bolaSize - velocidady <= enemigo.y) || (bouncery - velocidady >= enemigo.y + enemigo.size)) {
			velocidady = -velocidady;
		}
		enemigo.fueraPantalla = 1;
		enemigo.y = NULL;
		enemigo.x = NULL;
		puntaje += 100;
		bolas_rebotadas += 1;
		choque_enemigo += 1;
	}
}

void colision_plataforma_bola(plataforma plata, int bouncerx, int bouncery, float& velocidadx, float& velocidady, int bolaSize, int& bolas_rebotadas) {
	// Verifica si hay colisión con la plataforma
	if (bouncerx + bolaSize >= plata.x && bouncerx <= plata.x + plata.sizeX &&
		bouncery + bolaSize >= plata.y && bouncery <= plata.y + plata.sizeY) {

		// Rebote horizontal si hay colisión en los lados izquierdo o derecho de la plataforma
		if ((bouncerx + bolaSize - velocidadx <= plata.x) || (bouncerx - velocidadx >= plata.x + plata.sizeX)) {
			velocidadx = -velocidadx;
		}

		// Rebote vertical si hay colisión en la parte superior o inferior de la plataforma
		if ((bouncery + bolaSize - velocidady <= plata.y) || (bouncery - velocidady >= plata.y + plata.sizeY)) {
			velocidady = -velocidady;
		}
		bolas_rebotadas += 1;
	}
}



void colision_plataforma_powerup(plataforma plata, powerUp& powerUp) {
	if ((powerUp.y >= plata.y && powerUp.y <= plata.y + plata.sizeY)
		&& (powerUp.x + powerUp.sizeX >= plata.x && powerUp.x <= plata.x + plata.sizeX)) {
		powerUp.tomado = 1;
		powerUp.y = NULL;
		powerUp.x = NULL;
	}
}

void destruir_ladrillos(ladrillo*& Lista) {
	ladrillo* Aux = Lista; // Se posiciona Aux al primer elemento de lista
	while (Aux != NULL) { // Mientras Aux no sea NULL
		Lista = Lista->Siguiente; // Se asigna el siguiente elemento a Lista
		delete Aux; // Se borra el elemento actual
		Aux = Lista; // Aux se le asigna al siguiente elemento de la lista
	}
}

ladrillo* CrearLadrillo(int x, int y, int sizeX, int sizeY, int vida, ALLEGRO_BITMAP* imagen) {
	ladrillo* Pieza = new ladrillo; // Se crea un nuevo ladrillo
	Pieza->x = x;
	Pieza->y = y;
	Pieza->sizeX = sizeX;
	Pieza->sizeY = sizeY;
	Pieza->vida = vida;
	Pieza->imagen = imagen;
	Pieza->Siguiente = NULL; // Inicializa el siguiente ladrillo como NULL
	return Pieza; // Se retorna el puntero al nuevo ladrillo
}

void AgregarInicioladrillos(ladrillo*& Lista, ladrillo* Nuevo) {
	Nuevo->Siguiente = Lista; // El nuevo ladrillo apunta al inicio de la lista
	Lista = Nuevo; // La lista ahora apunta al nuevo ladrillo
}

bool revisar_ladrillos(ladrillo* Lista) {
	ladrillo* Aux = Lista; 
	while (Aux != NULL) { 
		if (Aux->vida == 1) 
			return true; 
		Aux = Aux->Siguiente; 
	}
	return false; 
}

int contar_ladrillos(ladrillo* Lista) {
	int destruidos = 0;
	ladrillo* Aux = Lista;
	while (Aux != NULL) {
		if (Aux->vida == 0)
			destruidos += 1;
		Aux = Aux->Siguiente;
	}
	return destruidos;
}

void inicializarEstadisticas(Estadisticas& estadisticas) {
	estadisticas.contadorLadrillos = 0;
	estadisticas.vidasGastadas = 0;
	estadisticas.bolasRebotadas = 0;
	estadisticas.enemigosDestruidos = 0;
	estadisticas.puntosTotales = 0;
	estadisticas.bolasPerdidas = 0;
}

void cargarEstadisticas(Estadisticas& estadisticas) {
	system("CLS");
	FILE* archivoEstadisticas;
	errno_t err = fopen_s(&archivoEstadisticas, "Estadisticas.txt", "r");
	if (err != 0 || archivoEstadisticas == NULL) {
		inicializarEstadisticas(estadisticas);
	}
	else {
		if (fscanf_s(archivoEstadisticas, "%i", &estadisticas.contadorLadrillos) != 1 ||
			fscanf_s(archivoEstadisticas, "%i", &estadisticas.vidasGastadas) != 1 ||
			fscanf_s(archivoEstadisticas, "%i", &estadisticas.bolasRebotadas) != 1 ||
			fscanf_s(archivoEstadisticas, "%i", &estadisticas.bolasPerdidas) != 1 ||
			fscanf_s(archivoEstadisticas, "%i", &estadisticas.enemigosDestruidos) != 1 ||
			fscanf_s(archivoEstadisticas, "%i", &estadisticas.puntosTotales) != 1) {
			inicializarEstadisticas(estadisticas);
		}
		fclose(archivoEstadisticas);
	}
}

void guardararchivo(Estadisticas& estadisticas) {

	Estadisticas estadisticasPrevias;
	cargarEstadisticas(estadisticasPrevias);

	// Sumar los valores actuales a los valores previos
	estadisticas.contadorLadrillos += estadisticasPrevias.contadorLadrillos;
	estadisticas.vidasGastadas += estadisticasPrevias.vidasGastadas;
	estadisticas.bolasRebotadas += estadisticasPrevias.bolasRebotadas;
	estadisticas.bolasPerdidas += estadisticasPrevias.bolasPerdidas;
	estadisticas.enemigosDestruidos += estadisticasPrevias.enemigosDestruidos;
	estadisticas.puntosTotales += estadisticasPrevias.puntosTotales;


	FILE* archivoEstadisticas;
	fopen_s(&archivoEstadisticas, "Estadisticas.txt", "w+");
	if (archivoEstadisticas != NULL) {
		fprintf_s(archivoEstadisticas, "%i", estadisticas.contadorLadrillos);
		fprintf_s(archivoEstadisticas, "\n%i", estadisticas.vidasGastadas);
		fprintf_s(archivoEstadisticas, "\n%i", estadisticas.bolasRebotadas);
		fprintf_s(archivoEstadisticas, "\n%i", estadisticas.bolasPerdidas);
		fprintf_s(archivoEstadisticas, "\n%i", estadisticas.enemigosDestruidos);
		fprintf_s(archivoEstadisticas, "\n%i\n\n", estadisticas.puntosTotales);
		fclose(archivoEstadisticas);
	}
}

//funcion que deja un array sin contenido
void reiniciarArray(char buffer[]) {
	for (int i = 0; i < sizeof(buffer) - 1; ++i) {
		buffer[i] = '\0';
	}
}

ALLEGRO_COLOR colorBlanco = al_map_rgb(255, 255, 255);
ALLEGRO_COLOR colorVerdeMusgo = al_map_rgb(35, 79, 42);
ALLEGRO_COLOR colorAmarilloAsco = al_map_rgb(109, 138, 50);