#pragma once
#include "funciones.h"
using namespace std;
#define FPS 60.0

#pragma once

bool estaInicializado(const Estadisticas& estadisticas) {
    return estadisticas.contadorLadrillos != -1;
}

void menuEstadisticas() {
	//CARGAR ARCHIVOS
	Estadisticas estadisticas;
    cargarEstadisticas(estadisticas);
	printf("oli");

	if (!al_init()) {
		al_show_native_message_box(NULL, "Error", "Error", "No se pudo inicializar Allegro", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return;
	}

	al_install_keyboard();
	al_install_mouse();


	ALLEGRO_MONITOR_INFO monitor;
	al_get_monitor_info(0, &monitor);
	const int ResolucionX = monitor.x2 - monitor.x1;
	const int ResolucionY = monitor.y2 - monitor.y1;
	bool hecho = true;
	int mousex = 0;
	int mousey = 0;

	al_set_new_display_flags(ALLEGRO_FULLSCREEN);
	ALLEGRO_DISPLAY* display;
	display = al_create_display(ResolucionX, ResolucionY); //se crea el display del tamaño de la pantalla
	al_set_window_title(display, "ArkanoidEstadisticas");

	int X = al_get_display_width(display);
	int Y = al_get_display_height(display);

	ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS);
	ALLEGRO_EVENT_QUEUE* cola_eventos = al_create_event_queue();

	al_register_event_source(cola_eventos, al_get_timer_event_source(timer));
	al_register_event_source(cola_eventos, al_get_mouse_event_source());
	al_register_event_source(cola_eventos, al_get_keyboard_event_source());

	al_start_timer(timer);

	ALLEGRO_FONT* font1 = al_load_font("Fuentes/04b_30__.TTF", 50, NULL);
	ALLEGRO_FONT* font2 = al_load_font("Fuentes/ka1.ttf", 30, NULL);
	ALLEGRO_BITMAP* Fondo = al_load_bitmap("Imagenes/Fondo.png");
	ALLEGRO_SAMPLE* click = al_load_sample("Musica/Click.mp3");

	while (hecho) {
		ALLEGRO_EVENT eventos;
		al_wait_for_event(cola_eventos, &eventos);

		if (eventos.type == ALLEGRO_EVENT_KEY_UP) {
			switch (eventos.keyboard.keycode) {
			case ALLEGRO_KEY_ESCAPE:
				hecho = false;
				break;
			}

		}
		if (eventos.type == ALLEGRO_EVENT_MOUSE_AXES)
		{
			mousex = eventos.mouse.x;
			mousey = eventos.mouse.y;

		}
		if (eventos.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
			hecho = false;

		//ojooo
		if (eventos.type == ALLEGRO_EVENT_TIMER) {
			al_clear_to_color(al_map_rgb(0, 0, 0));
			al_draw_scaled_bitmap(Fondo, 0, 0, al_get_bitmap_width(Fondo), al_get_bitmap_height(Fondo), 0, 0, ResolucionX, ResolucionY, 0);
			al_draw_text(font1, colorBlanco, AnchoDeFrame / 2, 200, ALLEGRO_ALIGN_CENTER, "ESTADISTICAS");
			al_draw_text(font1, colorVerdeMusgo, (AnchoDeFrame / 2) + 1, 200 + 1, ALLEGRO_ALIGN_CENTER, "ESTADISTICAS");
			//Cantidad de ladrillos destruidos
			al_draw_text(font2, colorAmarilloAsco, (AnchoDeFrame / 2) - 300, 350, ALLEGRO_ALIGN_CENTRE, "LADRILLOS DESTRUIDOS: ");
			char buffer[20];
			_itoa_s(estadisticas.contadorLadrillos, buffer, 20, 10);
			al_draw_text(font2, colorAmarilloAsco, (AnchoDeFrame / 2) + 400, 350, ALLEGRO_ALIGN_CENTRE, buffer);
			//Cantidad de bolas rebotadas
			reiniciarArray(buffer);
			al_draw_text(font2, colorAmarilloAsco, (AnchoDeFrame / 2) - 300, 400, ALLEGRO_ALIGN_CENTRE, "BOLAS REBOTADAS: ");
			_itoa_s(estadisticas.bolasRebotadas, buffer, 20, 10);
			al_draw_text(font2, colorAmarilloAsco, (AnchoDeFrame / 2) + 400, 400, ALLEGRO_ALIGN_CENTRE, buffer);
			// Cantidad de bolas perdidas
			reiniciarArray(buffer);
			al_draw_text(font2, colorAmarilloAsco, (AnchoDeFrame / 2) - 300, 450, ALLEGRO_ALIGN_CENTRE, "BOLAS PERDIDAS: ");
			_itoa_s(estadisticas.bolasPerdidas, buffer, 20, 10);
			al_draw_text(font2, colorAmarilloAsco, (AnchoDeFrame / 2) + 400, 450, ALLEGRO_ALIGN_CENTRE, buffer);
			//Cantidad de vidas gastadas
			reiniciarArray(buffer);
			al_draw_text(font2, colorAmarilloAsco, (AnchoDeFrame / 2) - 300, 500, ALLEGRO_ALIGN_CENTRE, "VIDAS GASTADAS: ");
			_itoa_s(estadisticas.vidasGastadas, buffer, 20, 10);
			al_draw_text(font2, colorAmarilloAsco, (AnchoDeFrame / 2) + 400, 500, ALLEGRO_ALIGN_CENTRE, buffer);
			//Cantidad de enemigosDestruidos
			reiniciarArray(buffer);
			al_draw_text(font2, colorAmarilloAsco, (AnchoDeFrame / 2) - 300, 550, ALLEGRO_ALIGN_CENTRE, "ENEMIGOS DESTRUIDOS: ");
			_itoa_s(estadisticas.enemigosDestruidos, buffer, 20, 10);
			al_draw_text(font2, colorAmarilloAsco, (AnchoDeFrame / 2) + 400, 550, ALLEGRO_ALIGN_CENTRE, buffer);
			//Cantidad de puntos totales
			reiniciarArray(buffer);
			al_draw_text(font2, colorAmarilloAsco, (AnchoDeFrame / 2) - 300, 600, ALLEGRO_ALIGN_CENTRE, "PUNTOS TOTALES: ");
			_itoa_s(estadisticas.puntosTotales, buffer, 20, 10);
			al_draw_text(font2, colorAmarilloAsco, (AnchoDeFrame / 2) + 400, 600, ALLEGRO_ALIGN_CENTRE, buffer);

		}

		//Si se posiciona el mouse en las coordenadas de SALIR
		if ((mousex >= (AnchoDeFrame / 2) + 405 - 140 && mousex <= (AnchoDeFrame / 2) + 405 + 140) && (mousey >= 900 - 35 && mousey <= 900 + 35)) {
			al_draw_text(font2, colorBlanco, (AnchoDeFrame / 2) + 405, 900, ALLEGRO_ALIGN_CENTRE, "SALIR");
			if (eventos.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
				if (eventos.mouse.button & 1) { //si se da click:
					al_play_sample(click, 0.3, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
					al_draw_text(font2, colorVerdeMusgo, (AnchoDeFrame / 2) + 405, 900, ALLEGRO_ALIGN_CENTRE, "SALIR");
					al_flip_display();
					al_rest(0.2);
					al_draw_text(font2, colorAmarilloAsco, (AnchoDeFrame / 2) + 405, 900, ALLEGRO_ALIGN_CENTRE, "SALIR");
					al_flip_display();
					hecho = false;
				}

			}
		}
		else { //el mouse no paso por ahi
			al_draw_text(font2, colorAmarilloAsco, (AnchoDeFrame / 2) + 405, 900, ALLEGRO_ALIGN_CENTRE, "SALIR");
		}
		al_flip_display();
	}
	al_destroy_display(display);
	al_destroy_font(font1);
	al_destroy_font(font2);
	al_destroy_bitmap(Fondo);
	al_destroy_timer(timer);
	al_destroy_event_queue(cola_eventos);
	al_destroy_sample(click);
}
