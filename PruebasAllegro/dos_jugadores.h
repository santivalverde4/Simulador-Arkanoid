#pragma
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

#include "funciones.h"

using namespace std;

#pragma warning(disable:4996);
#define FPS 60.0


int dos_jugadores(int& vidas, int nivel) {
	if (!al_init()) {
		al_show_native_message_box(NULL, "Error", "Error", "No se pudo inicializar Allegro", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return -1;
	}
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_image_addon();
	al_install_keyboard();
	al_install_mouse();
	Estadisticas estadisticas;
	cargarEstadisticas(estadisticas);

	char buffer[200];
	int puntaje = 0;
	bool hecho = true;

	//--------------------------------------------------------------

	al_uninstall_mouse(); //desactivar las funciones del mouse
	ALLEGRO_MONITOR_INFO monitor;
	al_get_monitor_info(0, &monitor);
	const int ResolucionX = monitor.x2 - monitor.x1;
	const int ResolucionY = monitor.y2 - monitor.y1;

	al_set_new_display_flags(ALLEGRO_FULLSCREEN);
	ALLEGRO_DISPLAY* display;
	display = al_create_display(ResolucionX, ResolucionY); //se crea el display del tamaño de la pantalla

	al_set_window_title(display, "Arkanoid2");

	ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS);
	ALLEGRO_EVENT_QUEUE* cola_eventos = al_create_event_queue();
	al_register_event_source(cola_eventos, al_get_timer_event_source(timer));
	al_register_event_source(cola_eventos, al_get_keyboard_event_source());

	//TIMER DE PODERES
	ALLEGRO_TIMER* powerup2_timer = al_create_timer(15.0); // Temporizador de 10 segundos
	al_register_event_source(cola_eventos, al_get_timer_event_source(powerup2_timer));


	al_start_timer(timer);

	ALLEGRO_FONT* font1 = al_load_font("Fuentes/04b_30__.TTF", 50, NULL);
	ALLEGRO_FONT* font2 = al_load_font("Fuentes/04b_30__.TTF", 30, NULL);
	ALLEGRO_FONT* font3 = al_load_font("Fuentes/04b_30__.TTF", 20, NULL);
	ALLEGRO_FONT* font4 = al_load_font("Fuentes/04b_30__.TTF", 20, NULL);

	ALLEGRO_KEYBOARD_STATE teclado;
	ALLEGRO_BITMAP* Fondo = al_load_bitmap("Imagenes/Fondo.png");
	ALLEGRO_BITMAP* marcoizq = al_load_bitmap("Imagenes/Marco2jugadoresIZQ.png");
	ALLEGRO_BITMAP* marcoder = al_load_bitmap("Imagenes/Marco2jugadoresIZQ.png");
	ALLEGRO_BITMAP* ladrilloVerde = al_load_bitmap("Imagenes/Ladrillo1.png"); /// se puede quitar
	ALLEGRO_BITMAP* fotoPowerUpAzul = al_load_bitmap("Imagenes/Pastilla_azul.png");
	ALLEGRO_BITMAP* fotoPowerUpVerde = al_load_bitmap("Imagenes/Pastilla_verde.png");
	ALLEGRO_BITMAP* fotoPowerUpRoja = al_load_bitmap("Imagenes/Pastilla_roja.png");
	ALLEGRO_BITMAP* fotoBola = al_load_bitmap("Imagenes/Bala.png");
	ALLEGRO_COLOR colorBlanco = al_map_rgb(255, 255, 255);
	ALLEGRO_COLOR colorVerdeMusgo = al_map_rgb(35, 79, 42);
	ALLEGRO_COLOR colorAmarilloAsco = al_map_rgb(109, 138, 50);

	ALLEGRO_BITMAP* imagen_enemigo1 = al_load_bitmap("Imagenes/enemigo1.png");
	ALLEGRO_BITMAP* imagen_enemigo2 = al_load_bitmap("Imagenes/enemigo2.png");
	ALLEGRO_BITMAP* imagen_enemigo3 = al_load_bitmap("Imagenes/enemigo3.png");
	ALLEGRO_BITMAP* imagen_enemigo4 = al_load_bitmap("Imagenes/enemigo4.png");

	plataforma plataforma_abajo = { ResolucionX / 2 - 600, ResolucionY - 100, 7, 150, 30, al_load_bitmap("Imagenes/plataforma1.png") };
	plataforma plataforma_arriba = { ResolucionX / 2 - 600,              225, 7, 150, 30, al_load_bitmap("Imagenes/plataforma2.png") };

	bola bola1 = { 200, 200, -5.0, 5.0, 200, 200, 35, 0, fotoBola };
	bola bola2 = { 200, 200, -5.0, 5.0, 200, 200, 35, 2, fotoBola };
	bola bola3 = { 200, 200, -5.0, 5.0, 200, 200, 35, 2, fotoBola };

	powerUp powerUpAzul = { 0, 0, 50, 25, fotoPowerUpAzul, 2, 2.5, 1 }; //NUEVO DE POWERUP
	powerUp powerUpVerde = { 0, 0, 50, 25, fotoPowerUpVerde, 2, 2.5, 2 }; //NUEVO DE POWERUP
	powerUp powerUpRoja = { 0, 0, 50, 25, fotoPowerUpRoja, 2, 2.5, 3 }; //NUEVO DE POWERUP

	ALLEGRO_BITMAP* imagen;

	enemigo enemigo1 = { 0, 0, 2, 40, 1, 2, imagen_enemigo1, enemigoArribaAbajo() };
	enemigo enemigo2 = { 300, 500, 2, 40, 1, 2, imagen_enemigo2,enemigoArribaAbajo() };
	enemigo enemigo3 = { 400, 500, 2, 40, 1, 2, imagen_enemigo3,enemigoArribaAbajo() };
	enemigo enemigo4 = { 500, 500, 2, 40, 1, 2, imagen_enemigo4,enemigoArribaAbajo() };

	ladrillo* listaLadrillos = NULL; // Lista de ladrillos

	int cantidad_ladrillos;

	if (nivel == 1)
		cantidad_ladrillos = 14;
	if (nivel == 2)
		cantidad_ladrillos = 29;
	if (nivel == 3)
		cantidad_ladrillos = 44;


	// Crear y agregar ladrillos a la lista
	for (int i = 0; i < cantidad_ladrillos; ++i) {
		int x = 240 + (i % 14) * 40; // Calcula la posición x
		int y = 500; // Fija la posición y
		ladrillo* nuevoLadrillo = CrearLadrillo(x, y, 40, 20, 1, ladrilloVerde);
		AgregarInicioladrillos(listaLadrillos, nuevoLadrillo);
	}

	int restriccionderecha = 790;
	int restriccionizquierda = al_get_bitmap_width(marcoizq) * 2;



	while (hecho) {
		ALLEGRO_EVENT eventos;
		al_wait_for_event(cola_eventos, &eventos);

		if (eventos.type == ALLEGRO_EVENT_KEY_UP) {
			switch (eventos.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
				hecho = false;
				break;
			}
		}
		if (eventos.type == ALLEGRO_EVENT_TIMER) {
			//el timer es el de los powerUps
			if (eventos.timer.source == powerup2_timer) {
				al_stop_timer(powerup2_timer);
				//restablece la velocidad de la bola dependiendo de la direccion a la que se dirige
				if (bola1.velocidadx < 0) {
					bola1.velocidadx = -5.0;
					estadisticas.bolasRebotadas += 1;
				}
				else
					bola1.velocidadx = 5.0;


				if (bola1.velocidady > 0) {
					bola1.velocidady = 5.0;
					estadisticas.bolasRebotadas += 1;
				}
				else
					bola1.velocidady = -5.0;

				//existe bola 2
				if (bola2.enPantalla == 0) {
					if (bola2.velocidadx < 0) {
						bola2.velocidadx = -5.0;
						estadisticas.bolasRebotadas += 1;
					}
				}
				else
					bola2.velocidadx = 5.0;

				if (bola2.velocidady > 0) {
					bola2.velocidady = 5.0;
					estadisticas.bolasRebotadas += 1;
				}
				else
					bola2.velocidady = -5.0;
			}

			//existe bola 3
			if (bola3.enPantalla == 0) {
				if (bola3.velocidadx < 0) {
					bola3.velocidadx = -5.0;
					estadisticas.bolasRebotadas += 1;
				}
				else
					bola3.velocidadx = 5.0;


				if (bola3.velocidady > 0) {
					bola3.velocidady = 5.0;
					estadisticas.bolasRebotadas += 1;
				}
				else
					bola3.velocidady = -5.0;
			}

			}

			
			al_draw_scaled_bitmap(Fondo, 0, 0, al_get_bitmap_width(Fondo), al_get_bitmap_height(Fondo), 0, 0, ResolucionX, ResolucionY, 0);
			al_draw_text(font1, colorBlanco, 200, 75, ALLEGRO_ALIGN_CENTER, "ARKANOID");
			al_draw_text(font1, colorVerdeMusgo, 200, 76, ALLEGRO_ALIGN_CENTER, "ARKANOID");



			_itoa_s(puntaje, buffer, 200, 10);
			al_draw_text(font2, al_map_rgb(255, 255, 255), 1500, 400, ALLEGRO_ALIGN_CENTER, "PUNTAJE: ");
			al_draw_text(font2, al_map_rgb(255, 255, 255), 1700, 400, ALLEGRO_ALIGN_CENTER, buffer);

			if (bola1.bouncerx < al_get_bitmap_width(marcoizq) + 40 || bola1.bouncerx > ResolucionX / 2 - al_get_bitmap_width(marcoder)) {
				bola1.velocidadx = -bola1.velocidadx;
			}
			vidas_bolas(bola1, bola2, bola3, ResolucionY, vidas, estadisticas.vidasGastadas);

			borrar_bolas(bola1, bola2, bola3, ResolucionY, plataforma_abajo, vidas, estadisticas.bolasPerdidas);

			char buf[200];
			_itoa_s(vidas, buf, 200, 10);
			al_draw_text(font2, al_map_rgb(255, 255, 255), 1500, 500, ALLEGRO_ALIGN_CENTER, "Vidas: ");
			al_draw_text(font2, al_map_rgb(255, 255, 255), 1600, 500, ALLEGRO_ALIGN_CENTER, buf);

			bola1.bouncerx += bola1.velocidadx;
			bola1.bouncery += bola1.velocidady;


			if (bola2.enPantalla == 0) {
				bola2.bouncerx += bola2.velocidadx;
				bola2.bouncery += bola2.velocidady;
			}

			if (bola3.enPantalla == 0) {
				bola3.bouncerx += bola3.velocidadx;
				bola3.bouncery += bola3.velocidady;
			}


			//existe bola2
			if (bola2.enPantalla == 0) {
				if (bola2.bouncerx < al_get_bitmap_width(marcoizq) + 40 || bola2.bouncerx > ResolucionX / 2 - al_get_bitmap_width(marcoder)) {
					bola2.velocidadx = -bola2.velocidadx;
				}
				if (bola2.bouncery < 160 || bola2.bouncery > ResolucionY - 25) {
					bola2.velocidady = -bola2.velocidady;
				}
				colision_plataforma_bola(plataforma_abajo, bola2.bouncerx, bola2.bouncery, bola2.velocidadx, bola2.velocidady, bola2.size, estadisticas.bolasRebotadas);
				colision_plataforma_bola(plataforma_arriba, bola2.bouncerx, bola2.bouncery, bola2.velocidadx, bola2.velocidady, bola2.size, estadisticas.bolasRebotadas);
			}
			//existe bola3
			if (bola3.enPantalla == 0) {
				if (bola3.bouncerx < al_get_bitmap_width(marcoizq) + 40 || bola3.bouncerx > ResolucionX / 2 - al_get_bitmap_width(marcoder)) {
					bola3.velocidadx = -bola3.velocidadx;
					estadisticas.bolasRebotadas += 1;
				}
				if (bola3.bouncery < 160 || bola3.bouncery > ResolucionY - 25) {
					bola3.velocidady = -bola3.velocidady;
				}
				colision_plataforma_bola(plataforma_abajo, bola3.bouncerx, bola3.bouncery, bola3.velocidadx, bola3.velocidady, bola3.size, estadisticas.bolasRebotadas);
				colision_plataforma_bola(plataforma_arriba, bola3.bouncerx, bola3.bouncery, bola3.velocidadx, bola3.velocidady, bola3.size, estadisticas.bolasRebotadas);

			}
			//colisiones con las plataformas
			colision_plataforma_bola(plataforma_abajo, bola1.bouncerx, bola1.bouncery, bola1.velocidadx, bola1.velocidady, bola1.size, estadisticas.bolasRebotadas);
			colision_plataforma_bola(plataforma_arriba, bola1.bouncerx, bola1.bouncery, bola1.velocidadx, bola1.velocidady, bola1.size, estadisticas.bolasRebotadas);

			colision_bola_enemigo(enemigo1, bola1.bouncerx, bola1.bouncery, bola1.velocidadx, bola1.velocidady, bola1.size, puntaje, estadisticas.bolasRebotadas, estadisticas.enemigosDestruidos);
			colision_bola_enemigo(enemigo2, bola1.bouncerx, bola1.bouncery, bola1.velocidadx, bola1.velocidady, bola1.size, puntaje, estadisticas.bolasRebotadas, estadisticas.enemigosDestruidos);
			colision_bola_enemigo(enemigo3, bola1.bouncerx, bola1.bouncery, bola1.velocidadx, bola1.velocidady, bola1.size, puntaje, estadisticas.bolasRebotadas, estadisticas.enemigosDestruidos);
			colision_bola_enemigo(enemigo4, bola1.bouncerx, bola1.bouncery, bola1.velocidadx, bola1.velocidady, bola1.size, puntaje, estadisticas.bolasRebotadas, estadisticas.enemigosDestruidos);

			colision_plataforma_powerup(plataforma_abajo, powerUpAzul);
			colision_plataforma_powerup(plataforma_abajo, powerUpVerde);
			colision_plataforma_powerup(plataforma_abajo, powerUpRoja);

			colision_plataforma_enemigo(plataforma_abajo, enemigo1, false, vidas, estadisticas.vidasGastadas);
			colision_plataforma_enemigo(plataforma_abajo, enemigo2, false, vidas, estadisticas.vidasGastadas);
			colision_plataforma_enemigo(plataforma_abajo, enemigo3, false, vidas, estadisticas.vidasGastadas);
			colision_plataforma_enemigo(plataforma_abajo, enemigo4, false, vidas, estadisticas.vidasGastadas);
			colision_plataforma_enemigo(plataforma_arriba, enemigo1, false, vidas, estadisticas.vidasGastadas);
			colision_plataforma_enemigo(plataforma_arriba, enemigo2, false, vidas, estadisticas.vidasGastadas);
			colision_plataforma_enemigo(plataforma_arriba, enemigo3, false, vidas, estadisticas.vidasGastadas);
			colision_plataforma_enemigo(plataforma_arriba, enemigo4, false, vidas, estadisticas.vidasGastadas);

			//POWERUP BAJA

			if (powerUpAzul.tomado == 0) {
				powerUpAzul.y += powerUpAzul.velocidad;
			}

			if (powerUpVerde.tomado == 0) {
				powerUpVerde.y += powerUpVerde.velocidad;
			}
			if (powerUpRoja.tomado == 0) {
				powerUpRoja.y += powerUpRoja.velocidad;
			}

			//enemigos
			if (enemigo1.fueraPantalla == 0) {
				if (enemigo1.direccion)
					enemigo1.y -= enemigo1.velocidady;
				else
					enemigo1.y += enemigo1.velocidady;
			}

			if (enemigo2.fueraPantalla == 0) {
				if (enemigo2.direccion)
					enemigo2.y -= enemigo2.velocidady;
				else
					enemigo2.y += enemigo2.velocidady;
			}

			if (enemigo3.fueraPantalla == 0) {
				if (enemigo3.direccion)
					enemigo3.y -= enemigo3.velocidady;
				else
					enemigo3.y += enemigo3.velocidady;
			}

			if (enemigo4.fueraPantalla == 0) {
				if (enemigo4.direccion)
					enemigo4.y -= enemigo4.velocidady;
				else
					enemigo4.y += enemigo4.velocidady;
			}

			if (powerUpVerde.tomado == 1) {
				powerUpVerde.accionPowerUp(bola1, bola2, bola3, powerup2_timer, cola_eventos, vidas);
			}

			if (powerUpRoja.tomado == 1) {
				powerUpRoja.accionPowerUp(bola1, bola2, bola3, powerup2_timer, cola_eventos, vidas);
			}

			if (powerUpAzul.tomado == 1) {
				powerUpAzul.accionPowerUp(bola1, bola2, bola3, powerup2_timer, cola_eventos, vidas);
			}

			powerUpAzul.pintarPowerUp(ResolucionY);
			powerUpRoja.pintarPowerUp(ResolucionY);
			powerUpVerde.pintarPowerUp(ResolucionY);

			enemigo1.pintar_enemigo(ResolucionY);
			enemigo2.pintar_enemigo(ResolucionY);
			enemigo3.pintar_enemigo(ResolucionY);
			enemigo4.pintar_enemigo(ResolucionY);

			//se pintan los ladrillos
			ladrillo* actual = listaLadrillos;
			while (actual != NULL) {
				actual->pintarLadrillo(bola1, bola2, bola3, puntaje, powerUpAzul, enemigo1);
				actual = actual->Siguiente; // Avanza al siguiente ladrillo
			}
			
			hecho = revisar_ladrillos(listaLadrillos);

			if (vidas == 0)
				hecho = false;



			al_get_keyboard_state(&teclado);

			// RESTRICCIONES DE MOVIMIENTO

			if ((plataforma_abajo.x < restriccionderecha) && (al_key_down(&teclado, ALLEGRO_KEY_RIGHT)))
				plataforma_abajo.x += plataforma_abajo.velocidad;

			if ((restriccionizquierda < plataforma_abajo.x) && (al_key_down(&teclado, ALLEGRO_KEY_LEFT)))
				plataforma_abajo.x -= plataforma_abajo.velocidad;


			if ((plataforma_arriba.x < restriccionderecha) && (al_key_down(&teclado, ALLEGRO_KEY_D)))
				plataforma_arriba.x += plataforma_arriba.velocidad;

			if ((restriccionizquierda < plataforma_arriba.x) && (al_key_down(&teclado, ALLEGRO_KEY_A)))
				plataforma_arriba.x -= plataforma_arriba.velocidad;

			al_draw_scaled_bitmap(marcoizq, 0, 0, al_get_bitmap_width(marcoizq), al_get_bitmap_height(marcoizq),
				0, ResolucionY - 925, al_get_bitmap_width(marcoizq) * 2.0, ResolucionY - 150, 0);

			al_draw_scaled_bitmap(marcoder, 0, 0, al_get_bitmap_width(marcoder), al_get_bitmap_height(marcoder),
				ResolucionX / 2 - al_get_bitmap_width(marcoder) / 2, ResolucionY - 925, al_get_bitmap_width(marcoder) * 2.0, ResolucionY - 150, 0);

			//plataforma de abajo
			al_draw_scaled_bitmap(plataforma_abajo.imagen, 0, 0, al_get_bitmap_width(plataforma_abajo.imagen), al_get_bitmap_height(plataforma_abajo.imagen),
				plataforma_abajo.x, plataforma_abajo.y, plataforma_abajo.sizeX, plataforma_abajo.sizeY, 0);

			//plataforma de arriba
			al_draw_scaled_bitmap(plataforma_arriba.imagen, 0, 0, al_get_bitmap_width(plataforma_arriba.imagen), al_get_bitmap_height(plataforma_arriba.imagen),
				plataforma_arriba.x, plataforma_arriba.y, plataforma_arriba.sizeX, plataforma_arriba.sizeY, 0);

			pintarbola(bola1);
			pintarbola(bola2);
			pintarbola(bola3);

			al_flip_display();
		}
		estadisticas.contadorLadrillos = contar_ladrillos(listaLadrillos);

	if (vidas == 0) {
		estadisticas.puntosTotales = puntaje;
		char puntaje[20] = "Puntaje: ";
		strcat_s(puntaje, 20, buffer);
		bool seguir = true; // Asegúrate de que esta variable esté definida

		while (seguir) {
			ALLEGRO_EVENT eventos2;
			al_wait_for_event(cola_eventos, &eventos2);
			al_clear_to_color(al_map_rgb(0, 0, 0));

			if (eventos2.type == ALLEGRO_EVENT_KEY_CHAR) {
				if (eventos2.keyboard.keycode == ALLEGRO_KEY_ENTER) {
					seguir = false;
				}
				
			}
			// Dibuja el texto en pantalla
			al_draw_text(font1, colorBlanco, ResolucionX / 2, ResolucionY / 2 - 250, ALLEGRO_ALIGN_CENTRE, "FIN DEL JUEGO");
			al_draw_text(font2, colorBlanco, ResolucionX / 2, ResolucionY / 2 - 150, ALLEGRO_ALIGN_CENTRE, puntaje);
			al_draw_text(font2, colorBlanco, ResolucionX / 2, ResolucionY / 2 + 200, ALLEGRO_ALIGN_CENTRE, "Presione Enter para finalizar");
			al_flip_display();
		}
		guardararchivo(estadisticas);
	}
	else {//Si salida es distinto a cero, el usuario debe presionar ENTER para continuar con el siguiente nivel
		int seguir = true;
		while (seguir) {
			ALLEGRO_EVENT eventos2;
			al_wait_for_event(cola_eventos, &eventos2);
			al_clear_to_color(al_map_rgb(0, 0, 0));
			al_draw_text(font1, colorBlanco, ResolucionX / 2, ResolucionY / 2 - 50, ALLEGRO_ALIGN_CENTRE, "PRESIONE ENTER PARA CONTINUAR");
			if (eventos2.type == ALLEGRO_EVENT_KEY_UP) {
				if (eventos2.keyboard.keycode == ALLEGRO_KEY_ENTER) {
					seguir = false;
				}
			}
			al_flip_display();

		}
	}

	al_destroy_bitmap(fotoBola);
	al_destroy_bitmap(Fondo);
	al_destroy_bitmap(marcoizq);
	al_destroy_bitmap(marcoder);
	al_destroy_bitmap(plataforma_abajo.imagen);
	al_destroy_bitmap(plataforma_arriba.imagen);
	al_destroy_bitmap(imagen_enemigo1);
	al_destroy_bitmap(imagen_enemigo2);
	al_destroy_bitmap(imagen_enemigo3);
	al_destroy_bitmap(imagen_enemigo4);
	al_destroy_event_queue(cola_eventos);
	al_destroy_timer(timer);
	al_destroy_timer(powerup2_timer);
	al_destroy_font(font1);
	al_destroy_font(font2);
	al_destroy_font(font3);
	al_destroy_font(font4);
	al_destroy_display(display);
	al_destroy_bitmap(ladrilloVerde);
	al_destroy_bitmap(fotoPowerUpAzul);
	al_destroy_bitmap(fotoPowerUpRoja);
	al_destroy_bitmap(fotoPowerUpVerde);

	return vidas;
}

