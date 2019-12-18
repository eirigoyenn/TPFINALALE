/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */





#include "structs.h"


#include "game.h"
#include <stdio.h>

#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>

#include "termlib.h"
#include "pc.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h> 
#include <allegro5/allegro_color.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_audio.h>

#define FPS    60.0
#define HEIGHT 499
#define WIDTH  625
#define CUADRADITO_SIZE 32
#define MOVE_RATE  4.0



    ALLEGRO_DISPLAY  *display;  //punteros que apuntan a un estructuras de allegro, se los apuntan a NULL para controlar errores
    ALLEGRO_BITMAP *imagen;
    ALLEGRO_EVENT_QUEUE * event_queue;
    ALLEGRO_SAMPLE *Tetris=NULL;
    ALLEGRO_SAMPLE *LevelClear=NULL;
    ALLEGRO_SAMPLE *GameOver=NULL;
    ALLEGRO_SAMPLE *TitleTheme=NULL;
    ALLEGRO_SAMPLE_ID id;
   
    ALLEGRO_EVENT ev;
 
    
    ALLEGRO_BITMAP *NewGame = NULL;
    ALLEGRO_BITMAP *LoadGame = NULL;
    ALLEGRO_BITMAP *Fondo = NULL;
    ALLEGRO_BITMAP *Diff1 = NULL;
    ALLEGRO_BITMAP *Diff2 = NULL; 
    ALLEGRO_BITMAP *Diff3 = NULL;
    ALLEGRO_BITMAP *Diff4 = NULL;  
    ALLEGRO_BITMAP *CoverON = NULL;
    ALLEGRO_BITMAP *CoverOFF = NULL;     
    ALLEGRO_BITMAP *PNewGame = NULL; 
    ALLEGRO_BITMAP *PLoadGame = NULL;
    ALLEGRO_BITMAP *PSaveGame = NULL;  
    ALLEGRO_BITMAP *PContinue = NULL;
    ALLEGRO_BITMAP *PExit = NULL; 
    ALLEGRO_BITMAP *IGameOver = NULL; 
    ALLEGRO_BITMAP *IP0 = NULL; 
    ALLEGRO_BITMAP *IP1 = NULL; 
    ALLEGRO_BITMAP *IP2 = NULL; 
    ALLEGRO_BITMAP *IP3 = NULL; 
    ALLEGRO_BITMAP *IP4 = NULL;     
    ALLEGRO_BITMAP *IP5 = NULL; 
    ALLEGRO_BITMAP *IP6 = NULL; 
    ALLEGRO_BITMAP *N0 = NULL; 
    ALLEGRO_BITMAP *N1 = NULL; 
    ALLEGRO_BITMAP *N2 = NULL; 
    ALLEGRO_BITMAP *N3 = NULL; 
    ALLEGRO_BITMAP *N4 = NULL;     
    ALLEGRO_BITMAP *N5 = NULL; 
    ALLEGRO_BITMAP *N6 = NULL; 
    ALLEGRO_BITMAP *N7 = NULL; 
    ALLEGRO_BITMAP *N8 = NULL; 
    ALLEGRO_BITMAP *N9 = NULL; 



    
int close_display = 0;
extern int pausa,finish,score,set_menu,n2,n;
extern int storage[][12],block;
extern int end,menu,piece_set_down,lines,P0,P1,P2,P3,P4,P5,P6;


int inicializacion (void){

    int portAux;//puerto que me guarda la configuracion actual del puerto para hacerlo parpadear
    
    
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
    
   if (!al_init_image_addon()) { // ADDON necesario para manejo(no olvidar el freno de mano) de imagenes 
        fprintf(stderr, "failed to initialize image addon !\n");
        return -1;
    }

    if (!al_install_audio()) {
        fprintf(stderr, "failed to initialize audio!\n");
        return -1;
    }

    if (!al_init_acodec_addon()) {
        fprintf(stderr, "failed to initialize audio codecs!\n");
        return -1;
    }

    if (!al_reserve_samples(4)) {
        fprintf(stderr, "failed to reserve samples!\n");
        return -1;
    }

    
    Tetris = al_load_sample("Tetris.ogg");

    if (!Tetris) {
        printf("Audio clip sample not loaded!\n");
        return -1;
    }    
    
    LevelClear = al_load_sample("LevelClear.ogg");

    if (!LevelClear) {
        printf("Audio clip sample not loaded!\n");
        return -1;
    }        
    
    GameOver = al_load_sample("GameOver.ogg");

    if (!GameOver) {
        printf("Audio clip sample not loaded!\n");
        return -1;
    }      
    
    TitleTheme = al_load_sample("TitleTheme.ogg");

    if (!TitleTheme) {
        printf("Audio clip sample not loaded!\n");
        return -1;
    }      
    
    display=al_create_display(WIDTH,HEIGHT); 
     
    //se crea el display
    
    al_register_event_source(event_queue, al_get_display_event_source(display)); //se registra la fuente de los eventos de cierre de display
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    
    if(!display){
        al_shutdown_primitives_addon();      //se destruye la imagen porque ocupa espacio en heap y el programa fallo por otro motivo
       fprintf(stderr,"failed to create display");
        
        return -1;
    }
    

    
    Diff1 = al_load_bitmap("Diff1.bmp");

    if (!Diff1) {
        fprintf(stderr, "failed to load image Diff1!\n");
        return 0;
    }
    
    NewGame = al_load_bitmap("NewGame.bmp");
    
    if (!NewGame) {
        fprintf(stderr, "failed to load image NewGame !\n");
        return 0;
    }
    
    LoadGame = al_load_bitmap("LoadGame.bmp");

    if (!LoadGame) {
        fprintf(stderr, "failed to load image LoadGame !\n");
        return 0;
    }
    
    Fondo = al_load_bitmap("Fondo.bmp");

    if (!Fondo) {
        fprintf(stderr, "failed to load image LoadGame !\n");
        return 0;
    }
    

    Diff2 = al_load_bitmap("Diff2.bmp");

    if (!Diff2) {
        fprintf(stderr, "failed to load image Diff2!\n");
        return 0;
    }
    
    Diff3 = al_load_bitmap("Diff3.bmp");

    if (!Diff3) {
        fprintf(stderr, "failed to load image  Diff3!\n");
        return 0;
    }
    
    Diff4 = al_load_bitmap("Diff4.bmp");

    if (!Diff4) {
        fprintf(stderr, "failed to load image Diff4!\n");
        return 0;
    }
    
    
    CoverON = al_load_bitmap("CoverON.bmp");

    if (!CoverON) {
        fprintf(stderr, "failed to load image COVERON!\n");
        return 0;
    }
    
    CoverOFF = al_load_bitmap("CoverOFF.bmp");

    if (!CoverOFF) {
        fprintf(stderr, "failed to load image COVEROFF!\n");
        return 0;
    }
        
    PNewGame = al_load_bitmap("PNewGame.bmp");

    if (!PNewGame) {
        fprintf(stderr, "failed to load image NEWGAME!\n");
        return 0;
    }
    PLoadGame = al_load_bitmap("PLoadGame.bmp");

    if (!PLoadGame) {
        fprintf(stderr, "failed to load image LOADGAME!\n");
        return 0;
    }
    PSaveGame = al_load_bitmap("PSaveGame.bmp");

    if (!PSaveGame) {
        fprintf(stderr, "failed to load image SAVEGAME!\n");
        return 0;
    }
    PContinue = al_load_bitmap("PContinue.bmp");

    if (!PContinue) {
        fprintf(stderr, "failed to load image CONTINUE!\n");
        return 0;
    }
    PExit = al_load_bitmap("PExit.bmp");

    if (!PExit) {
        fprintf(stderr, "failed to load image PEXIT!\n");
        return 0;
    }    
  
    IGameOver = al_load_bitmap("GameOver.bmp");

    if (!IGameOver) {
        fprintf(stderr, "failed to load image GAMEOVER!\n");
        return 0;
    }   
  
    IP0 = al_load_bitmap("P0.bmp");

    if (!IP0) {
        fprintf(stderr, "failed to load image IP0!\n");
        return 0;
    }       
    IP1 = al_load_bitmap("P1.bmp");

    if (!IP1) {
        fprintf(stderr, "failed to load image IP1!\n");
        return 0;
    }   
    IP2 = al_load_bitmap("P2.bmp");

    if (!IP2) {
        fprintf(stderr, "failed to load image IP2!\n");
        return 0;
    }   
    IP3 = al_load_bitmap("P3.bmp");

    if (!IP3) {
        fprintf(stderr, "failed to load image IP3!\n");
        return 0;
    }   
    IP4 = al_load_bitmap("P4.bmp");

    if (!IP4) {
        fprintf(stderr, "failed to load image IP4!\n");
        return 0;
    }   
    IP5 = al_load_bitmap("P5.bmp");

    if (!IP5) {
        fprintf(stderr, "failed to load image IP5!\n");
        return 0;
    }       
    IP6 = al_load_bitmap("P6.bmp");

    if (!IP6) {
        fprintf(stderr, "failed to load image IP6!\n");
        return 0;
    }       
    
    N0 = al_load_bitmap("0.bmp");

    if (!N0) {
        fprintf(stderr, "failed to load image N0!\n");
        return 0;
    }       
    
    N1 = al_load_bitmap("1.bmp");

    if (!N1) {
        fprintf(stderr, "failed to load image N1!\n");
        return 0;
    }       
    
    N2 = al_load_bitmap("2.bmp");

    if (!N2) {
        fprintf(stderr, "failed to load image N2!\n");
        return 0;
    }       
    
    N3 = al_load_bitmap("3.bmp");

    if (!N3) {
        fprintf(stderr, "failed to load image N3!\n");
        return 0;
    }       
    
    N4 = al_load_bitmap("4.bmp");

    if (!N4) {
        fprintf(stderr, "failed to load image N4!\n");
        return 0;
    }       
    
    N5 = al_load_bitmap("5.bmp");

    if (!N5) {
        fprintf(stderr, "failed to load image N5!\n");
        return 0;
    }       
    
    N6 = al_load_bitmap("6.bmp");

    if (!N6) {
        fprintf(stderr, "failed to load image N6!\n");
        return 0;
    }       
    
    N7 = al_load_bitmap("7.bmp");

    if (!N7) {
        fprintf(stderr, "failed to load image N7!\n");
        return 0;
    }       
    
    N8 = al_load_bitmap("8.bmp");

    if (!N8) {
        fprintf(stderr, "failed to load image N8!\n");
        return 0;
    }       
    
    N9 = al_load_bitmap("9.bmp");

    if (!N9) {
        fprintf(stderr, "failed to load image N9!\n");
        return 0;
    }       
    
    
    
    
    
    
    
    
    
    
    al_clear_to_color(al_color_name("black"));          //se pinta las dos caras del display con blanco
    al_flip_display();
    al_clear_to_color(al_color_name("black"));
    
}

void erase_events (void){
    al_flush_event_queue(event_queue);
}



int print_menu (void){
    
    al_set_target_backbuffer(display);
    al_stop_sample(&id);
    al_play_sample(TitleTheme, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &id); 
    int get_move=1,pc=1,px=1,chosen_mode,chosen_diff,conta=-5;
    



    while(get_move != 8){
        
        get_move=move();
        
                al_draw_bitmap(CoverON,0,0,0);
                al_flip_display();
                usleep(200000);
                al_draw_bitmap(CoverOFF,0,0,0);
                al_flip_display();
                usleep(200000);

    }

    

    get_move=1;
    
    while(get_move != 8 ){
        
        get_move=move();
        
        switch(get_move){
            case 2:
                if(pc>1){
                    pc--;
                }
                break;
            case -2:
                if(pc<2){
                    pc++;
                }
        }
        switch(pc){
            case 1: al_draw_bitmap(NewGame,0,0,0);
                    al_flip_display();
                break;
                
            case 2: al_draw_bitmap(LoadGame,0,0,0);
                    al_flip_display();
                break;
        }
        
    }

    get_move=1;
    chosen_mode=pc;
    
    while(get_move != 8){
        
        get_move=move();
        
        switch(get_move){
            case 1:
                    if(px<4){
                        px++;
                    }
                    break;
            case -1:
                    if(px>1){
                        px--;
                    }
                    break;
        }
        switch(px){
            
            case 1: al_draw_bitmap(Diff1,0,0,0);
                    al_flip_display();
                break;
            case 2:al_draw_bitmap(Diff2,0,0,0);
                   al_flip_display();
                break;
            case 3:al_draw_bitmap(Diff3,0,0,0);
                    al_flip_display();
                break;
            case 4:al_draw_bitmap(Diff4,0,0,0);
                    al_flip_display();
        } 
    }
    
    chosen_diff=px;
    al_stop_sample(&id);
    al_play_sample(Tetris, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &id);
    al_draw_bitmap(Fondo,0,0,0);
    al_flip_display();
    init_game(chosen_mode,chosen_diff);
    
}

void create_pantalla(void){           //se crea el grafico del puerto A(los 8flags)
    int i,j;
    
    for(i=0;i<16;i++)
        for(j=0;j<12;j++){  

            al_draw_filled_rectangle( WIDTH*(j+6)/25, HEIGHT*(i+2)/19 ,WIDTH*(j+6+1)/25,HEIGHT*(i+1+2)/19,  al_color_name("black"));
            al_draw_rectangle( WIDTH*(j+6)/25, HEIGHT*(i+2)/19 ,WIDTH*(j+6+1)/25,HEIGHT*(i+1+2)/19,  al_map_rgb(0,0,0),0);



        }
        al_draw_rectangle( WIDTH*6/25, HEIGHT*2/19 ,WIDTH*18/25,HEIGHT*18/19,  al_map_rgb(70,70,70),3);
        al_flip_display();
            
        
}

void print_stopmenu(void){
    
    int i,j,palabra=0,numero,get_move;
    al_set_target_backbuffer(display);
    get_move=1;
    while(get_move != 8){

        get_move=move();
        switch(get_move){
            case 2: 
                    if(palabra>0){
                        palabra--;
                    }
                    break;

            case -2:                          
                    if(palabra<4){
                        palabra++;
                    }
                    break;
        }
        switch(palabra){

            case 0: al_draw_bitmap(PNewGame,150,100,0);
                    al_flip_display();
                break;
            case 1: al_draw_bitmap(PLoadGame,150,100,0);
                    al_flip_display();
                break;       
            case 2:al_draw_bitmap(PSaveGame,150,100,0);
                   al_flip_display();
                break;
            case 3:al_draw_bitmap(PContinue,150,100,0);
                    al_flip_display();
                break;
            case 4:al_draw_bitmap(PExit,150,100,0);
                    al_flip_display();
                break;
        }
        usleep(90000);
    }

        usleep(500000);
        leavestop(palabra);
        al_draw_bitmap(Fondo,0,0,0);
        print_screen(n2);
        usleep(100000);
        menu=0;
}




void leavestop(int n){
    
    static int scoret,p0,p1,p2,p3,p4,p5,p6;
    
    switch(n){
        
        case 0: menu=1;
                usleep(70000);
                clear_board();
                clean_struct(n);
                create_floor();
                score=0;
                P0=0;
                P1=0; 
                P2=0; 
                P3=0; 
                P4=0; 
                P5=0; 
                P6=0; 
                menu=0;
                usleep(30000);
                break;
       
        case 1: menu=1;
                usleep(70000);
                copy_board((int*)gameboard,(int*)storage);
                clean_struct(n);
                create_floor();
                score=scoret;
                P0=p0;
                P1=p1;
                P2=p2;
                P3=p3;
                P4=p4;
                P5=p5;
                P6=p6;
                menu=0;
                usleep(30000);
                break;
       
        case 2: menu=1;
                usleep(70000);
                copy_board((int*)storage,(int*)gameboard);
                clear_board();
                copy_board((int*)gameboard,(int*)storage);
                scoret=score;
                p0=P0;
                p1=P1;
                p2=P2;
                p3=P3;
                p4=P4;
                p5=P5;
                p6=P6;
                create_floor();
                menu=0;
                usleep(30000);
                break;
                
        case 3: menu=1;
                usleep(70000);
                create_floor();
                menu=0;
                usleep(30000);
                      
                break; 
                
        case 4:  
                end=0;
                finish=0;
                set_menu=0;

                break;       
        
    }
}




void copy_board(int *pllegada,int *psalida){
    
    int i,j;
    
    for(i=0;i<20;i++){
        for(j=0;j<12;j++){
            
            if(psalida[i*12+j]>7 || psalida[i*12+j]==0){
                
            
                pllegada[i*12+j]=psalida[i*12+j];
           }
        }
    } 
}

void play_sample (void){
    
    al_stop_sample(&id);
    al_play_sample(LevelClear,1.3,0.0,1.0,ALLEGRO_PLAYMODE_LOOP,&id);
    usleep(2300000);
    al_stop_sample(&id);
    al_play_sample(Tetris,1.0,0.0,1.0,ALLEGRO_PLAYMODE_LOOP,&id);
 
}






void print_screen (int n2){
    int k,i,scorep,numero,linep;
    static int P0p=1,P1p=1,P2p=1,P3p=1,P4p=1,P5p=1,P6p=1,P7p=1,P8p=1,P9p=1;
    
    menu=1;
    usleep(70000);
    
    al_set_target_backbuffer(display);
    
    switch(n2){
        
            case 0: al_draw_bitmap(IP0,464,252,0);
                    al_flip_display();
                    break;
            case 1: al_draw_bitmap(IP1,464,252,0);
                    al_flip_display();
                    break;
            case 2: al_draw_bitmap(IP2,464,252,0);
                    al_flip_display();
                    break;
            case 3: al_draw_bitmap(IP3,464,252,0);
                    al_flip_display();
                    break;
            case 4: al_draw_bitmap(IP4,464,252,0);
                    al_flip_display();
                    break;
            case 5: al_draw_bitmap(IP5,464,252,0);
                    al_flip_display();
                    break;
            case 6: al_draw_bitmap(IP6,464,252,0);
                    al_flip_display();
                    break;
    }
    
    scorep=score;
    for(k=560,i=0;i<5;i++){
        
        numero=scorep%10;
        switch(numero){

                case 0: al_draw_bitmap(N0,k,150,0);
                        al_flip_display();
                        break;
                case 1: al_draw_bitmap(N1,k,150,0);
                        al_flip_display();
                        break;
                case 2: al_draw_bitmap(N2,k,150,0);
                        al_flip_display();
                        break;
                case 3: al_draw_bitmap(N3,k,150,0);
                        al_flip_display();
                        break;
                case 4: al_draw_bitmap(N4,k,150,0);
                        al_flip_display();
                        break;
                case 5: al_draw_bitmap(N5,k,150,0);
                        al_flip_display();
                        break;
                case 6: al_draw_bitmap(N6,k,150,0);
                        al_flip_display();
                        break;
                case 7: al_draw_bitmap(N7,k,150,0);
                        al_flip_display();
                        break;
                case 8: al_draw_bitmap(N8,k,150,0);
                        al_flip_display();
                        break;
                case 9: al_draw_bitmap(N9,k,150,0);
                        al_flip_display();
                        break;                        
        }
        scorep/=10;
        k-=15;
    } 
    
    
    linep=lines;
    for(k=395,i=0;i<5;i++){
        
        numero=linep%10;
        switch(numero){

                case 0: al_draw_bitmap(N0,k,17,0);
                        al_flip_display();
                        break;
                case 1: al_draw_bitmap(N1,k,17,0);
                        al_flip_display();
                        break;
                case 2: al_draw_bitmap(N2,k,17,0);
                        al_flip_display();
                        break;
                case 3: al_draw_bitmap(N3,k,17,0);
                        al_flip_display();
                        break;
                case 4: al_draw_bitmap(N4,k,17,0);
                        al_flip_display();
                        break;
                case 5: al_draw_bitmap(N5,k,17,0);
                        al_flip_display();
                        break;
                case 6: al_draw_bitmap(N6,k,17,0);
                        al_flip_display();
                        break;
                case 7: al_draw_bitmap(N7,k,17,0);
                        al_flip_display();
                        break;
                case 8: al_draw_bitmap(N8,k,17,0);
                        al_flip_display();
                        break;
                case 9: al_draw_bitmap(N9,k,17,0);
                        al_flip_display();
                        break;                        
        }
        linep/=10;
        k-=15;
    } 
    P0p=P0;
    for(k=120,i=0;i<5;i++){

        numero=P0p%10;
        switch(numero){

                case 0: al_draw_bitmap(N0,k,175,0);
                        al_flip_display();
                        break;
                case 1: al_draw_bitmap(N1,k,175,0);
                        al_flip_display();
                        break;
                case 2: al_draw_bitmap(N2,k,175,0);
                        al_flip_display();
                        break;
                case 3: al_draw_bitmap(N3,k,175,0);
                        al_flip_display();
                        break;
                case 4: al_draw_bitmap(N4,k,175,0);
                        al_flip_display();
                        break;
                case 5: al_draw_bitmap(N5,k,175,0);
                        al_flip_display();
                        break;
                case 6: al_draw_bitmap(N6,k,175,0);
                        al_flip_display();
                        break;
                case 7: al_draw_bitmap(N7,k,175,0);
                        al_flip_display();
                        break;
                case 8: al_draw_bitmap(N8,k,175,0);
                        al_flip_display();
                        break;
                case 9: al_draw_bitmap(N9,k,175,0);
                        al_flip_display();
                        break;                        
        }
        P0p/=10;
        k-=15;
    }

    P1p=P1;
    for(k=120,i=0;i<5;i++){

        numero=P1p%10;
        switch(numero){

                case 0: al_draw_bitmap(N0,k,195,0);
                        al_flip_display();
                        break;
                case 1: al_draw_bitmap(N1,k,195,0);
                        al_flip_display();
                        break;
                case 2: al_draw_bitmap(N2,k,195,0);
                        al_flip_display();
                        break;
                case 3: al_draw_bitmap(N3,k,195,0);
                        al_flip_display();
                        break;
                case 4: al_draw_bitmap(N4,k,195,0);
                        al_flip_display();
                        break;
                case 5: al_draw_bitmap(N5,k,195,0);
                        al_flip_display();
                        break;
                case 6: al_draw_bitmap(N6,k,195,0);
                        al_flip_display();
                        break;
                case 7: al_draw_bitmap(N7,k,195,0);
                        al_flip_display();
                        break;
                case 8: al_draw_bitmap(N8,k,195,0);
                        al_flip_display();
                        break;
                case 9: al_draw_bitmap(N9,k,195,0);
                        al_flip_display();
                        break;                        
        }
        P1p/=10;
        k-=15;
    }


    P2p=P2;
    for(k=120,i=0;i<5;i++){

        numero=P2p%10;
        switch(numero){

                case 0: al_draw_bitmap(N0,k,215,0);
                        al_flip_display();
                        break;
                case 1: al_draw_bitmap(N1,k,215,0);
                        al_flip_display();
                        break;
                case 2: al_draw_bitmap(N2,k,215,0);
                        al_flip_display();
                        break;
                case 3: al_draw_bitmap(N3,k,215,0);
                        al_flip_display();
                        break;
                case 4: al_draw_bitmap(N4,k,215,0);
                        al_flip_display();
                        break;
                case 5: al_draw_bitmap(N5,k,215,0);
                        al_flip_display();
                        break;
                case 6: al_draw_bitmap(N6,k,215,0);
                        al_flip_display();
                        break;
                case 7: al_draw_bitmap(N7,k,215,0);
                        al_flip_display();
                        break;
                case 8: al_draw_bitmap(N8,k,215,0);
                        al_flip_display();
                        break;
                case 9: al_draw_bitmap(N9,k,215,0);
                        al_flip_display();
                        break;                        
        }
        P2p/=10;
        k-=15;
    }       

    P3p=P3;
    for(k=120,i=0;i<5;i++){

        numero=P3p%10;
        switch(numero){

                case 0: al_draw_bitmap(N0,k,235,0);
                        al_flip_display();
                        break;
                case 1: al_draw_bitmap(N1,k,235,0);
                        al_flip_display();
                        break;
                case 2: al_draw_bitmap(N2,k,235,0);
                        al_flip_display();
                        break;
                case 3: al_draw_bitmap(N3,k,235,0);
                        al_flip_display();
                        break;
                case 4: al_draw_bitmap(N4,k,235,0);
                        al_flip_display();
                        break;
                case 5: al_draw_bitmap(N5,k,235,0);
                        al_flip_display();
                        break;
                case 6: al_draw_bitmap(N6,k,235,0);
                        al_flip_display();
                        break;
                case 7: al_draw_bitmap(N7,k,235,0);
                        al_flip_display();
                        break;
                case 8: al_draw_bitmap(N8,k,235,0);
                        al_flip_display();
                        break;
                case 9: al_draw_bitmap(N9,k,235,0);
                        al_flip_display();
                        break;                        
        }
        P3p/=10;
        k-=15;
    }       

    P4p=P4;
    for(k=120,i=0;i<5;i++){

        numero=P4p%10;
        switch(numero){

                case 0: al_draw_bitmap(N0,k,255,0);
                        al_flip_display();
                        break;
                case 1: al_draw_bitmap(N1,k,255,0);
                        al_flip_display();
                        break;
                case 2: al_draw_bitmap(N2,k,255,0);
                        al_flip_display();
                        break;
                case 3: al_draw_bitmap(N3,k,255,0);
                        al_flip_display();
                        break;
                case 4: al_draw_bitmap(N4,k,255,0);
                        al_flip_display();
                        break;
                case 5: al_draw_bitmap(N5,k,255,0);
                        al_flip_display();
                        break;
                case 6: al_draw_bitmap(N6,k,255,0);
                        al_flip_display();
                        break;
                case 7: al_draw_bitmap(N7,k,255,0);
                        al_flip_display();
                        break;
                case 8: al_draw_bitmap(N8,k,255,0);
                        al_flip_display();
                        break;
                case 9: al_draw_bitmap(N9,k,255,0);
                        al_flip_display();
                        break;                        
        }
        P4p/=10;
        k-=15;
    }

    P5p=P5;
    for(k=120,i=0;i<5;i++){

        numero=P5p%10;
        switch(numero){

                case 0: al_draw_bitmap(N0,k,275,0);
                        al_flip_display();
                        break;
                case 1: al_draw_bitmap(N1,k,275,0);
                        al_flip_display();
                        break;
                case 2: al_draw_bitmap(N2,k,275,0);
                        al_flip_display();
                        break;
                case 3: al_draw_bitmap(N3,k,275,0);
                        al_flip_display();
                        break;
                case 4: al_draw_bitmap(N4,k,275,0);
                        al_flip_display();
                        break;
                case 5: al_draw_bitmap(N5,k,275,0);
                        al_flip_display();
                        break;
                case 6: al_draw_bitmap(N6,k,275,0);
                        al_flip_display();
                        break;
                case 7: al_draw_bitmap(N7,k,275,0);
                        al_flip_display();
                        break;
                case 8: al_draw_bitmap(N8,k,275,0);
                        al_flip_display();
                        break;
                case 9: al_draw_bitmap(N9,k,275,0);
                        al_flip_display();
                        break;                        
        }
        P5p/=10;
        k-=15;
    }       



    P6p=P6;
    for(k=120,i=0;i<5;i++){

        numero=P6p%10;
        switch(numero){

                case 0: al_draw_bitmap(N0,k,295,0);
                        al_flip_display();
                        break;
                case 1: al_draw_bitmap(N1,k,295,0);
                        al_flip_display();
                        break;
                case 2: al_draw_bitmap(N2,k,295,0);
                        al_flip_display();
                        break;
                case 3: al_draw_bitmap(N3,k,295,0);
                        al_flip_display();
                        break;
                case 4: al_draw_bitmap(N4,k,295,0);
                        al_flip_display();
                        break;
                case 5: al_draw_bitmap(N5,k,295,0);
                        al_flip_display();
                        break;
                case 6: al_draw_bitmap(N6,k,295,0);
                        al_flip_display();
                        break;
                case 7: al_draw_bitmap(N7,k,295,0);
                        al_flip_display();
                        break;
                case 8: al_draw_bitmap(N8,k,295,0);
                        al_flip_display();
                        break;
                case 9: al_draw_bitmap(N9,k,295,0);
                        al_flip_display();
                        break;                        
        }
        P6p/=10;
        k-=15;
    }       
   
         
    menu=0;
    usleep(10000);
}

void finish_game (void){
    menu=1;
    piece_set_down=0;
    usleep(50000);
    al_set_target_backbuffer(display);
    al_draw_bitmap(IGameOver,0,0,0);
    al_flip_display();
    usleep(4000000);
    set_menu=1;
    usleep(20000);
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
                        al_flush_event_queue(event_queue);
                        break;

                    case ALLEGRO_KEY_LEFT:
                        get_move=-1;
                        if(check_down(n)==0){
                            block=0;
                        }
                        break;

                    case ALLEGRO_KEY_RIGHT:
                       
                        if(check_down(n)==0){
                            block=0;
                        }
                        get_move=1;
                        break;
                    
                    case ALLEGRO_KEY_ENTER:
                        get_move=8;
                        break;
                        
                    case ALLEGRO_KEY_ESCAPE:
                        get_move=9;
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
                    case ALLEGRO_KEY_ESCAPE:
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
    
    al_stop_sample(&id);
    al_play_sample(GameOver, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &id);
    usleep(3000000);
    al_stop_sample(&id); 
    
    
    al_destroy_bitmap(imagen);       //se libera la memoria dinamica , destruyendo los elemntos usados
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
    al_shutdown_primitives_addon();
    al_destroy_sample(TitleTheme);
    al_destroy_sample(LevelClear);
    al_destroy_sample(GameOver);
    al_destroy_sample(Tetris);
    al_uninstall_audio();
    al_destroy_bitmap(NewGame);
    al_destroy_bitmap(LoadGame);
    al_destroy_bitmap(Diff1);
    al_destroy_bitmap(Diff2);
    al_destroy_bitmap(Diff3);
    al_destroy_bitmap(Diff4);  
    al_destroy_bitmap(CoverON);
    al_destroy_bitmap(CoverOFF);    
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
                        al_draw_rectangle( WIDTH*(j+6)/25, HEIGHT*(i+2)/19 ,WIDTH*(j+6+1)/25,HEIGHT*(i+1+2)/19, al_map_rgb(70,70,70),0);
                        break;
                    case 2:
                    case 12:
                        al_draw_filled_rectangle( WIDTH*(j+6)/25, HEIGHT*(i+2)/19 ,WIDTH*(j+6+1)/25,HEIGHT*(i+1+2)/19,  al_color_name("yellow"));
                         al_draw_rectangle( WIDTH*(j+6)/25, HEIGHT*(i+2)/19 ,WIDTH*(j+6+1)/25,HEIGHT*(i+1+2)/19,  al_map_rgb(70,70,70),0);
                        break;
                    case 3:
                    case 13:
                        al_draw_filled_rectangle( WIDTH*(j+6)/25, HEIGHT*(i+2)/19 ,WIDTH*(j+6+1)/25,HEIGHT*(i+1+2)/19,  al_color_name("red"));
                         al_draw_rectangle( WIDTH*(j+6)/25, HEIGHT*(i+2)/19 ,WIDTH*(j+6+1)/25,HEIGHT*(i+1+2)/19,  al_map_rgb(70,70,70),0);
                        break;
                    case 4:
                    case 14:
                        al_draw_filled_rectangle( WIDTH*(j+6)/25, HEIGHT*(i+2)/19 ,WIDTH*(j+6+1)/25,HEIGHT*(i+1+2)/19,  al_map_rgb(4,247,28));
                         al_draw_rectangle( WIDTH*(j+6)/25, HEIGHT*(i+2)/19 ,WIDTH*(j+6+1)/25,HEIGHT*(i+1+2)/19, al_map_rgb(70,70,70),0);
                        break;
                    case 5:
                    case 15:
                        al_draw_filled_rectangle( WIDTH*(j+6)/25, HEIGHT*(i+2)/19 ,WIDTH*(j+6+1)/25,HEIGHT*(i+1+2)/19,  al_map_rgb(4,85,247));
                         al_draw_rectangle( WIDTH*(j+6)/25, HEIGHT*(i+2)/19 ,WIDTH*(j+6+1)/25,HEIGHT*(i+1+2)/19, al_map_rgb(70,70,70),0);
                        break;
                    case 6:
                    case 16:
                        al_draw_filled_rectangle( WIDTH*(j+6)/25, HEIGHT*(i+2)/19 ,WIDTH*(j+6+1)/25,HEIGHT*(i+1+2)/19,  al_map_rgb(255,196,0));
                         al_draw_rectangle( WIDTH*(j+6)/25, HEIGHT*(i+2)/19 ,WIDTH*(j+6+1)/25,HEIGHT*(i+1+2)/19,  al_map_rgb(70,70,70),0);
                        break;
                    case 7:
                    case 17: 
                        al_draw_filled_rectangle( WIDTH*(j+6)/25, HEIGHT*(i+2)/19 ,WIDTH*(j+6+1)/25,HEIGHT*(i+1+2)/19,  al_color_name("violet"));
                         al_draw_rectangle( WIDTH*(j+6)/25, HEIGHT*(i+2)/19 ,WIDTH*(j+6+1)/25,HEIGHT*(i+1+2)/19,  al_map_rgb(70,70,70),0);
                        break;

               
                }
            }
            else{
                
                al_draw_filled_rectangle( WIDTH*(j+6)/25, HEIGHT*(i+2)/19 ,WIDTH*(j+6+1)/25,HEIGHT*(i+1+2)/19,  al_color_name("black"));
                al_draw_rectangle( WIDTH*(j+6)/25, HEIGHT*(i+2)/19 ,WIDTH*(j+6+1)/25,HEIGHT*(i+1+2)/19,  al_map_rgb(70,70,70),0);
                
                
            }
        }
  
    }
        al_draw_rectangle( WIDTH*6/25, HEIGHT*2/19 ,WIDTH*18/25,HEIGHT*18/19,  al_map_rgb(70,70,70),3);
        al_flip_display();
    
   
        
    }

