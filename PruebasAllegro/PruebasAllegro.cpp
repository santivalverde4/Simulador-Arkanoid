#include <stdio.h>
#include <iostream>
#include <math.h>
#include <windows.h>
#include <stdlib.h>
#include <cstdlib>
#include <time.h>
#include <string>
#include <cstdio>

#include "dos_jugadores.h"

#include "un_jugador.h"

#include "estadisticas.h"

// librerias de allegro
#include <allegro5/allegro.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
using namespace std;


int main()
{
    //**************************************************************************************************************************************************************
    //**********************************************************CONFIGURACION DE PANTALLA***************************************************************************
    //**************************************************************************************************************************************************************


    if (!al_init())
    {
        al_show_native_message_box(NULL, "ERROR", NULL, "Error: El display no ha sido inicializado", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }

    //Se obtiene la información del monitor como su resolución mediante diferencia de puntos finales e iniciales
    ALLEGRO_MONITOR_INFO monitor;
    al_get_monitor_info(0, &monitor);
    const int ResolucionX = monitor.x2 - monitor.x1;
    const int ResolucionY = monitor.y2 - monitor.y1;

    al_set_new_display_flags(ALLEGRO_FULLSCREEN);//Se modifica el display para que sea pantalla completa
    ALLEGRO_DISPLAY* display;
    display = al_create_display(ResolucionX, ResolucionY); //se crea el display del tamaño de la pantalla

    al_set_window_title(display, "Arkanoid");

    //Se obtienen las dimensiones de la pantalla en donde se proyecta el juego para que este se ajuste a las dimensiones
    int X = al_get_display_width(display);
    int Y = al_get_display_height(display);

    if (!display)
    {
        al_show_native_message_box(display, "ERROR", "Display", "EL display no fue creado correctamente", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }
    //se inician complementos
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_image_addon();
    al_install_keyboard();
    al_install_mouse();
    al_install_audio();
    al_init_acodec_addon();
    al_reserve_samples(7);

    //configuracion de colas
    bool hecho = true;
    bool yaDestruido = false;

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS); //Pp
    ALLEGRO_EVENT_QUEUE* cola_eventos = al_create_event_queue();
    al_register_event_source(cola_eventos, al_get_keyboard_event_source());
    al_register_event_source(cola_eventos, al_get_timer_event_source(timer));
    al_register_event_source(cola_eventos, al_get_display_event_source(display));
    al_register_event_source(cola_eventos, al_get_mouse_event_source());
    int mousex = 0;
    int mousey = 0;
    al_start_timer(timer);

    //**************************************************************************************************************************************************************
    //**********************************************************CONFIGURACION DE PANTALLA***************************************************************************
    //**************************************************************************************************************************************************************


    //**************************************************************************************************************************************************************
    //**********************************************************CONFIGURACION DE MENU*******************************************************************************
    //**************************************************************************************************************************************************************
    ALLEGRO_FONT* font1 = al_load_font("Fuentes/04b_30__.TTF", 50, NULL);
    ALLEGRO_FONT* font2 = al_load_font("Fuentes/ka1.ttf", 30, NULL);
    ALLEGRO_BITMAP* Fondo = al_load_bitmap("Imagenes/Fondo.png");
    ALLEGRO_SAMPLE* musica = al_load_sample("Musica/MusicaFondo.mp3");
    ALLEGRO_SAMPLE* click = al_load_sample("Musica/Click.mp3");
    //ALLEGRO_BITMAP* paredFoto = al_load_bitmap("Imagenes/marco.jpeg");
    //ALLEGRO_BITMAP* bala = al_load_bitmap("Imagenes/Bala.png");
    //ALLEGRO_BITMAP* fotoBola = al_load_bitmap("Imagenes/Bala.png");
    

    //**************************************************************************************************************************************************************
    //**********************************************************CONFIGURACION DE MENU*******************************************************************************
    //**************************************************************************************************************************************************************
    al_play_sample(musica, 0.3, 0, 1, ALLEGRO_PLAYMODE_LOOP, NULL);
    // timers
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
        
        if (eventos.type == ALLEGRO_EVENT_TIMER) {
            al_draw_scaled_bitmap(Fondo, 0, 0, al_get_bitmap_width(Fondo), al_get_bitmap_height(Fondo), 0, 0, ResolucionX, ResolucionY, 0);
            al_draw_text(font1, colorBlanco, AnchoDeFrame / 2, 200, ALLEGRO_ALIGN_CENTER, "ARKANOID");
            al_draw_text(font1, colorVerdeMusgo, (AnchoDeFrame / 2) + 1, 200 + 1, ALLEGRO_ALIGN_CENTER, "ARKANOID");
            al_draw_text(font2, colorAmarilloAsco, AnchoDeFrame / 2, 350, ALLEGRO_ALIGN_CENTRE, "1 JUGADOR");
            al_draw_text(font2, colorAmarilloAsco, AnchoDeFrame / 2, 450, ALLEGRO_ALIGN_CENTRE, "2 JUGADORES");
            al_draw_text(font2, colorAmarilloAsco, AnchoDeFrame / 2, 550, ALLEGRO_ALIGN_CENTRE, "ESTADISTICAS");
            al_draw_text(font2, colorAmarilloAsco, AnchoDeFrame / 2, 650, ALLEGRO_ALIGN_CENTRE, "SALIR");


        }
        //Si se posiciona el mouse en las coordenadas de 1 jugador
        if ((mousex >= (X / 2) - 115 && mousex <= (X / 2) + 115) && (mousey >= 350 && mousey <= 385)) {
            al_draw_text(font2, colorBlanco, (AnchoDeFrame / 2), 350, ALLEGRO_ALIGN_CENTRE, "1 JUGADOR");
            if (eventos.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
                if (eventos.mouse.button & 1) { //si se da click:
                    al_play_sample(click, 0.3, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
                    al_draw_text(font2, colorVerdeMusgo, (AnchoDeFrame / 2), 350, ALLEGRO_ALIGN_CENTRE, "1 JUGADOR");
                    al_flip_display();
                    al_rest(0.2);
                    al_draw_text(font2, colorAmarilloAsco, AnchoDeFrame / 2, 350, ALLEGRO_ALIGN_CENTRE, "1 JUGADOR");
                    al_flip_display();
                    al_destroy_sample(musica);
                    al_destroy_display(display);
                    int vida = 3;
                    int nivel = 1;
                    while (vida != 0) {
                        vida = un_jugador(vida, nivel);
                        nivel += 1;
                    }

                    main();
                    yaDestruido = true;
                    hecho = false;
                }

            }
        }
        else { //el mouse no paso por ahi 
            al_draw_text(font2, colorAmarilloAsco, AnchoDeFrame / 2, 350, ALLEGRO_ALIGN_CENTRE, "1 JUGADOR");
        }
        //Si se posiciona el mouse en las coordenadas de 2 jugadores
        if ((mousex >= (X / 2) - 138 && mousex <= (X / 2) + 138) && (mousey >= 450 && mousey <= 485)) {
            al_draw_text(font2, colorBlanco, (AnchoDeFrame / 2), 450, ALLEGRO_ALIGN_CENTRE, "2 JUGADORES");
            if (eventos.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
                if (eventos.mouse.button & 1) { //si se da click:
                    al_play_sample(click, 0.3, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
                    al_draw_text(font2, colorVerdeMusgo, (AnchoDeFrame / 2), 450, ALLEGRO_ALIGN_CENTRE, "2 JUGADORES");
                    al_flip_display();
                    al_rest(0.2);
                    al_play_sample(click, 0.3, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
                    al_draw_text(font2, colorBlanco, (AnchoDeFrame / 2), 450, ALLEGRO_ALIGN_CENTRE, "2 JUGADORES");
                    al_flip_display();
                    al_destroy_sample(musica);
                    al_destroy_display(display);
                    int vida = 3;
                    int nivel = 1;
                    while (vida != 0) {
                        vida = dos_jugadores(vida, nivel);
                        nivel += 1;
                    }

                    main();
					yaDestruido = true;
                    hecho = false;
                }
            }
        }
        else {
            al_draw_text(font2, colorAmarilloAsco, (AnchoDeFrame / 2), 450, ALLEGRO_ALIGN_CENTRE, "2 JUGADORES");
        }

        //si se posiciona el mouse en las coordenadas de Estadisticas
        if ((mousex >= (X / 2) - 140 && mousex <= (X / 2) + 140) && (mousey >= 550 && mousey <= 585)) {
            al_draw_text(font2, colorBlanco, (AnchoDeFrame / 2), 550, ALLEGRO_ALIGN_CENTRE, "ESTADISTICAS");
            if (eventos.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
                if (eventos.mouse.button & 1) { //si se da click:
                    al_play_sample(click, 0.3, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
                    al_draw_text(font2, colorVerdeMusgo, (AnchoDeFrame / 2), 550, ALLEGRO_ALIGN_CENTRE, "ESTADISTICAS");
                    al_flip_display();
                    al_rest(0.2);
                    al_draw_text(font2, colorBlanco, (AnchoDeFrame / 2), 550, ALLEGRO_ALIGN_CENTRE, "ESTADISTICAS");
                    al_flip_display();
                    //logica de estadisticas
                    al_destroy_display(display);
                    al_destroy_sample(musica);
                    menuEstadisticas();
					yaDestruido = true;
                    main();
                    return -1;
                }
            }
        }
        else {
            al_draw_text(font2, colorAmarilloAsco, (AnchoDeFrame / 2), 550, ALLEGRO_ALIGN_CENTRE, "ESTADISTICAS");
        }

        //si se posiciona el mouse en las coordenadas de Salir
        if ((mousex >= (X / 2) - 60 && mousex <= (X / 2) + 60) && (mousey >= 650 && mousey <= 685)) {
            al_draw_text(font2, colorBlanco, (AnchoDeFrame / 2), 650, ALLEGRO_ALIGN_CENTRE, "SALIR");
            if (eventos.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
                if (eventos.mouse.button & 1) { //si se da click:
                    al_play_sample(click, 0.3, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
                    al_draw_text(font2, colorVerdeMusgo, (AnchoDeFrame / 2), 650, ALLEGRO_ALIGN_CENTRE, "SALIR");
                    al_flip_display();
                    al_rest(0.2);
                    al_draw_text(font2, colorBlanco, (AnchoDeFrame / 2), 650, ALLEGRO_ALIGN_CENTRE, "SALIR");
                    al_flip_display();
                    return -1;
                }
            }
        }
        else {
            al_draw_text(font2, colorAmarilloAsco, (AnchoDeFrame / 2), 650, ALLEGRO_ALIGN_CENTRE, "SALIR");
        }

        al_flip_display();// refresca la pantalla 
    }

    if (!yaDestruido){
		al_destroy_display(display);
		al_destroy_sample(musica);
    }

    
    al_destroy_font(font1);
    al_destroy_font(font2);
    al_destroy_bitmap(Fondo);
    al_destroy_timer(timer);
    al_destroy_event_queue(cola_eventos);
    
    al_destroy_sample(click);
}