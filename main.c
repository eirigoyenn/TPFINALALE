/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: manucorcos
 *
 * Created on December 4, 2019, 4:21 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "structs.h"
#include "game.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h> 
#include <allegro5/allegro_color.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_audio.h>
#include <unistd.h>
#include <pthread.h>



#define  ONE_MS    	1000	   // One mseg
#define  time_delay(x)  (x/100)    // x in ms , minvalue 100mseg


int gameboard[VNFil+1][NCol];


void update_board(void);
void inicializacion(void);
int move (void);

void printboard (void);



    
  /*
    
  int main (void){  
   
    
  create_floor();
    clean_struct(0);
    clean_struct(1);
    clean_struct(2);
    clean_struct(3);
    clean_struct(4);
    clean_struct(5);
    clean_struct(6);
    
    init_blocks();
    inicializacion();
    
    
    
    do{
        
       int entrada,loop=1;    //entrada es una variable que me permite almacenar el dato aportado por el usuario, loop, me permite permanecer en el ciclo
    
        ALLEGRO_EVENT ev;   //se crea la estructura ev
        if (al_get_next_event(event_queue, &ev)) //Toma un evento de la cola
        {
            if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE){    
                close_display =1;
                loop=0;
            }
        }
        
        
        int n;
        n=gen_pieza();
        printf("pieza numero:%d\n",n);
        int cont;
        cont=1;
        
        
        print_pieza(n);
        update_board();
        
        
        while(cont ){
            int get_move;
            get_move=move();
            
            cont=check_down(n);
            
            if(cont){
            
                piece_down(n); 
            }  
              
            if( get_move==2 && cont){
                 
                int rot;
                rot=rotate(n);
                
                if(rot){
                    print_pieza(n);
                }
            }
            
            if( get_move==1 && !check_right(n) && cont ){
                 
                 
                piece_right(n);
                
            
            }

            if( get_move==-1 && !check_left(n) && cont ){
                
                piece_left(n);
                 
                 
            }
            
            
            
            update_board();
            
            
            
                   
            
            usleep(200000);
    }
        stayed_blocks();
        clean_struct(n);
        check_board();
        
        
    }while( !close_display );

    closepro();

} 
*/

    
    
    
int piece_set_down=0,finish=0;
unsigned int TimerTick=2;
int n,end=1;
int get_move,conta,timex=150;
void closepro(void);



void* thread_timer()  // Time base Thread
{	       
    while(end)
    {
        if(conta>=7){
            timex-=15;
            conta=0;
        }
        usleep(timex*ONE_MS); // 100ms * 
        if (TimerTick){

            TimerTick--;
                printf("soy tick--\n\n");
        }
    }
}

void* thread_down(){ // Periodic Task Thread


	
	while (end) {
            if( piece_set_down || (!TimerTick)){
                    printf("soy Down\n\n");
                    if(check_down(n)){

                        printf("bajo");

                        piece_down(n);

                    }
                    else{
                        piece_set_down=1;
                    }
                
                    if(piece_set_down){

                        printf("soy newpiece\n\n");

                        stayed_blocks(); 
                        check_board();
                        clean_struct(n);

                        n=gen_pieza();
                        printf("pieza numero:%d\n",n);
                        print_pieza(n);
                        piece_set_down=0;
                        conta++;

                    }
                    TimerTick=4;
            }
    }
}


void * thread_joy(){ // Periodic Task Thread



        while (end) 
        {   

            if(move()==10){
                end=0;
                closepro();
            }

        }
}







void * thread_move(){ // The APP


        while(end){
            
           
            if(TimerTick && check_down(n) && piece_set_down==0){
                printf("soy move\n\n");                
                get_move=move();
                
                               
                
                if( get_move==2 ){

                    int rot;
                    rot=rotate(n);
                    
                    if(rot){
                        print_pieza(n);
                    }
                }

                if( get_move==1 && !check_right(n) ){


                    piece_right(n);
                    printf("soy move derecha \n\n");

                }

                if( get_move==-1 && !check_left(n)){

                    piece_left(n);


                }
                if( get_move==-2 ){

                    down(n);
                }    
                
                 if(move()==10){
                    end=0;
                }
                
                if (!check_down(n)){
                    
                    usleep((timex-10)*ONE_MS);
                    
                }

            }

        }

}


void * thread_init(){
    
    create_floor();
    clean_struct(0);
    clean_struct(1);
    clean_struct(2);
    clean_struct(3);
    clean_struct(4);
    clean_struct(5);
    clean_struct(6);


    init_blocks();
    inicializacion();



    n=gen_pieza();
    printf("pieza numero:%d\n",n);

    print_pieza(n);

            
    while(end){
            update_board();
            usleep(1000);
        }
}



int main()
{
        pthread_t tid1,tid2,tid3,tid4;
        srand(time(NULL));    
        
        pthread_create(&tid4,NULL,thread_init,NULL);
        usleep(1000000);
        pthread_create(&tid1,NULL,thread_timer,NULL);
        usleep(1000000);
        pthread_create(&tid3,NULL,thread_move,NULL);
        usleep(1000000);
        pthread_create(&tid2,NULL,thread_down,NULL);
        usleep(1000000);
      
        pthread_join(tid1,NULL);
        pthread_join(tid2,NULL);
        pthread_join(tid3,NULL);     
        pthread_join(tid4,NULL);
        
        return 0;
        
}

   
 