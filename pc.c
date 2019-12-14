/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

void update_board(void);
int inicializacion(void);
int move (void);
void print_menu (void);




#include "structs.h"


#include "game.h"
#include <stdio.h>

#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>

#include "termlib.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h> 
#include <allegro5/allegro_color.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_audio.h>

#define FPS    60.0
#define HEIGHT 700
#define WIDTH  1100
#define CUADRADITO_SIZE 32
#define MOVE_RATE  4.0


    ALLEGRO_DISPLAY  *display;     //punteros que apuntan a un estructuras de allegro, se los apuntan a NULL para controlar errores
    ALLEGRO_BITMAP *imagen;
    ALLEGRO_EVENT_QUEUE * event_queue;
    ALLEGRO_SAMPLE *sample;
    
    ALLEGRO_EVENT ev;
    
    
    
    
void closepro(void);

void create_pantalla(void);
void update_board(void);
int close_display = 0;

int inicializacion (void){
    

    int portAux;        //puerto que me guarda la configuracion actual del puerto para hacerlo parpadear
    
    
    if (!al_init()){        //inicializacion general del allegro
        fprintf (stderr, "error al inicializar el allegro\n");
        return -1;
    }
    
   if (!al_install_keyboard()) {
        fprintf(stderr, "failed to initialize the keyboard!\n");
        return -1;
    }

 
    
    
    event_queue = al_create_event_queue();       //se inicializa los eventos
    
    if (!event_queue) {                         //se controla si fallo la init de los eventos
        fprintf(stderr, "failed to create event_queue!\n");
        return -1;
    }
   
   
    if (!al_init_primitives_addon()){       //se controla si fallo la inicializacion de las primitivas
        fprintf (stderr, "error al inicializar las primitivas\n");
        return -1;
    }
    
    /*if (!al_init_image_addon()) { // ADDON necesario para manejo(no olvidar el freno de mano) de imagenes 
        fprintf(stderr, "failed to initialize image addon !\n");
        return -1;
    }
    
    image = al_load_bitmap("image.jpg");
    
    if (!image) {
        fprintf(stderr, "failed to load image !\n");
        return 0;
    }
    */
    
    
    
    display=al_create_display(WIDTH,HEIGHT); 
     
    //se crea el display
    
    al_register_event_source(event_queue, al_get_display_event_source(display)); //se registra la fuente de los eventos de cierre de display
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    if(!display){
        al_shutdown_primitives_addon();      //se destruye la imagen porque ocupa espacio en heap y el programa fallo por otro motivo
       fprintf(stderr,"failed to create display");
        
        return -1;
    }
    
    
    al_clear_to_color(al_color_name("blue"));          //se pinta las dos caras del display con blanco
    al_flip_display();
    al_clear_to_color(al_color_name("blue"));
    //al_draw_bitmap(image,0 , 0 , 0);
   
    
    create_pantalla();  //se crea el puerto A 
    
    
    
}

int print_menu (int palabra,int wait1,int numero,int wait2){
    return 0;
}

void create_pantalla(void){           //se crea el grafico del puerto A(los 8flags)
    int i,j;
    
    for(i=0;i<16;i++)
    for(j=0;j<12;j++){  
        
     al_draw_filled_rectangle( WIDTH*(j+6)/25, HEIGHT*(i+2)/19 ,WIDTH*(j+6+1)/25,HEIGHT*(i+1+2)/19,  al_color_name("black"));
                al_draw_rectangle( WIDTH*(j+6)/25, HEIGHT*(i+2)/19 ,WIDTH*(j+6+1)/25,HEIGHT*(i+1+2)/19,  al_color_name("grey"),0);
                
                
                
    }
        al_draw_rectangle( WIDTH*6/25, HEIGHT*2/19 ,WIDTH*18/25,HEIGHT*18/19,  al_color_name("grey"),10);
        al_flip_display();
        
    
        
        
}



int move(void){
   
   
    al_get_next_event(event_queue, &ev);
    int get_move=0;
    if(ev.type == ALLEGRO_EVENT_KEY_DOWN) {
                switch (ev.keyboard.keycode) {
                    case ALLEGRO_KEY_UP:
                        get_move=2;
                        break;

                    case ALLEGRO_KEY_DOWN:
                        get_move=-2;
                        break;

                    case ALLEGRO_KEY_LEFT:
                        get_move=-1;
                        break;

                    case ALLEGRO_KEY_RIGHT:
                        get_move=1;
                        break;
                    
                    case ALLEGRO_KEY_ENTER:
                        get_move='\n';
                        break;
               
                }
            }
    else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
                switch (ev.keyboard.keycode) {
                    case ALLEGRO_KEY_UP:
                        get_move=0;
                        break;

                    case ALLEGRO_KEY_DOWN:
                        get_move=0;
                        break;

                    case ALLEGRO_KEY_LEFT:
                        get_move=0;
                        break;

                    case ALLEGRO_KEY_RIGHT:
                        get_move=0;
                        break;
                    
                    case ALLEGRO_KEY_ENTER:
                        get_move=0;
                        break;
                    
                    
                }
                
    }
    else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
               
                    get_move = 10;
                 
    }
    return get_move;
}  


void closepro(void){
    
    al_destroy_bitmap(imagen);       //se libera la memoria dinamica , destruyendo los elemntos usados
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
    al_shutdown_primitives_addon();
    al_destroy_sample(sample);
    al_uninstall_audio();
    printf ("bye\n");
    
    
}

void update_board(void){
    
    int i,j;
    
    for(i=0;i<16;i++){
        
        
        for(j=0;j<12;j++){
            
            
            if(gameboard[i+4][j]){
                
                switch(gameboard[i+4][j]){
                    
                    case 1: 
                    case 11:
                        al_draw_filled_rectangle( WIDTH*(j+6)/25, HEIGHT*(i+2)/19 ,WIDTH*(j+6+1)/25,HEIGHT*(i+1+2)/19, al_map_rgb(3,240,240));
                         al_draw_rectangle( WIDTH*(j+6)/25, HEIGHT*(i+2)/19 ,WIDTH*(j+6+1)/25,HEIGHT*(i+1+2)/19,  al_color_name("grey"),0);
                        break;
                    case 2:
                    case 12:
                        al_draw_filled_rectangle( WIDTH*(j+6)/25, HEIGHT*(i+2)/19 ,WIDTH*(j+6+1)/25,HEIGHT*(i+1+2)/19,  al_color_name("yellow"));
                         al_draw_rectangle( WIDTH*(j+6)/25, HEIGHT*(i+2)/19 ,WIDTH*(j+6+1)/25,HEIGHT*(i+1+2)/19,  al_color_name("grey"),0);
                        break;
                    case 3:
                    case 13:
                        al_draw_filled_rectangle( WIDTH*(j+6)/25, HEIGHT*(i+2)/19 ,WIDTH*(j+6+1)/25,HEIGHT*(i+1+2)/19,  al_color_name("red"));
                         al_draw_rectangle( WIDTH*(j+6)/25, HEIGHT*(i+2)/19 ,WIDTH*(j+6+1)/25,HEIGHT*(i+1+2)/19,  al_color_name("grey"),0);
                        break;
                    case 4:
                    case 14:
                        al_draw_filled_rectangle( WIDTH*(j+6)/25, HEIGHT*(i+2)/19 ,WIDTH*(j+6+1)/25,HEIGHT*(i+1+2)/19,  al_map_rgb(4,247,28));
                         al_draw_rectangle( WIDTH*(j+6)/25, HEIGHT*(i+2)/19 ,WIDTH*(j+6+1)/25,HEIGHT*(i+1+2)/19,  al_color_name("grey"),0);
                        break;
                    case 5:
                    case 15:
                        al_draw_filled_rectangle( WIDTH*(j+6)/25, HEIGHT*(i+2)/19 ,WIDTH*(j+6+1)/25,HEIGHT*(i+1+2)/19,  al_map_rgb(4,85,247));
                         al_draw_rectangle( WIDTH*(j+6)/25, HEIGHT*(i+2)/19 ,WIDTH*(j+6+1)/25,HEIGHT*(i+1+2)/19,  al_color_name("grey"),0);
                        break;
                    case 6:
                    case 16:
                        al_draw_filled_rectangle( WIDTH*(j+6)/25, HEIGHT*(i+2)/19 ,WIDTH*(j+6+1)/25,HEIGHT*(i+1+2)/19,  al_map_rgb(255,196,0));
                         al_draw_rectangle( WIDTH*(j+6)/25, HEIGHT*(i+2)/19 ,WIDTH*(j+6+1)/25,HEIGHT*(i+1+2)/19,  al_color_name("grey"),0);
                        break;
                    case 7:
                    case 17: 
                        al_draw_filled_rectangle( WIDTH*(j+6)/25, HEIGHT*(i+2)/19 ,WIDTH*(j+6+1)/25,HEIGHT*(i+1+2)/19,  al_color_name("violet"));
                         al_draw_rectangle( WIDTH*(j+6)/25, HEIGHT*(i+2)/19 ,WIDTH*(j+6+1)/25,HEIGHT*(i+1+2)/19,  al_color_name("grey"),0);
                        break;

               
                }
            }
            else{
                
                al_draw_filled_rectangle( WIDTH*(j+6)/25, HEIGHT*(i+2)/19 ,WIDTH*(j+6+1)/25,HEIGHT*(i+1+2)/19,  al_color_name("black"));
                al_draw_rectangle( WIDTH*(j+6)/25, HEIGHT*(i+2)/19 ,WIDTH*(j+6+1)/25,HEIGHT*(i+1+2)/19,  al_color_name("grey"),0);
                
                
            }
        }
  
    }
    
        al_flip_display();
    
   
        
    }

