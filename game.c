#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <allegro5/allegro_audio.h>
#include "structs.h"
#include "game.h"
#include "pc.h"

#define ENTER '\n'


int score,level=1;

int storage [VNFil+1][NCol];

extern int timer,lines,menu;

typedef struct { 
    unsigned int x;
    unsigned int y;
    
}dcoord_t;

typedef struct {
    
    int *values;
    int size;
    dcoord_t pos;
    int rotacion;
    
}ARR_PIECES; 


typedef struct {
    
    int *values;
    int size;
    dcoord_t pos;    
    
}NUM; 


typedef struct {
    
    int *values;
    int sizex;
    int sizey;
    dcoord_t pos;    
    
}WORD; 


extern int gameboard[VNFil+1][NCol];

extern ARR_PIECES piezas[7];

extern NUM nums[9];

extern WORD words[6];


int gen_pieza(void){ // se usa la funcion rand para recibir un numero aleatorio de pieza
    
    srand(time(NULL));
    
    int piezano= rand()%7; // 7 ya que hay 7 piezas
 
    return piezano;

}



void clean_struct (int gen_pieza){ // clean struct pone las coordenadas de la pieza en x=5 e y=0 para que la pieza pueda caer desde arriba
    
    piezas[gen_pieza].pos.x=5;
    piezas[gen_pieza].pos.y=0;
    
    if(piezas[gen_pieza].rotacion){ // si la pieza esta rotadad
        int i;
        
        
        
        for(i= 4-(piezas[gen_pieza].rotacion); i!=0 ;i--){// llamo a reorder pieza asi la dejamos en su estado original rotandola cuantas veces sea necesario
            reorder_pieza(gen_pieza);
        }
         piezas[gen_pieza].rotacion=0;
    }
}


void clean_words (void){ // clean words establece las coordenadas x e y de las palabras
    int i;
    for(i=0;i<6;i++){    
        words[i].pos.x=12;
        words[i].pos.y=7;
    }    
}
      
int check_board(int level){ // check board se fija si existen filas llenas de piezas y de haberlas las borra y suma su score
    
    int i,j,countf=0,counts=0;
    
    menu=1;
    usleep(70000);
    
    for(i=VNFil-1;i>=4;i-- ){ //suma la cantidad de cuadrados escritos en una fila
        
        for(j=NCol-1;j>=0;j--){
            
            if(gameboard[i][j]){
            
                ++countf;
            
            }
        }
        
                   
        
        if(countf==NCol){// si esa cantidad es igual al numero de columnas, es una fila completa

            counts++;            
            for(j=0;j<NCol;j++){
                
                gameboard[i][j]=0;  //hacer titilar las barras y que se haga todas jntas
            
            }
        descend_board(i); // desciendo el gameboard desde esa posicion
        ++i;
        usleep(100000);
        }
        
        countf=0;
            
        menu=0;
        usleep(20000);
        
    }
    switch(counts){ // uso la formula adecuada para sumar el score
        
        case 1:score+=((level+1)*40);break;
        case 2:score+=((level+1)*100);break;
        case 3:score+=((level+1)*300);break;
        case 4:score+=((level+1)*1200);break;
        
            
        
    }
    lines+=counts;
        
    printf("\nSCORE=%d\n",score);
    return score;  
}

int check_level (int score){ // me fijo el estado del nivel y si es necesario aumentarlo
    
    if(level==1){
        if(score>1000){
            level++;
            play_sample();
            
        }
    }
    if(level==2){
        if(score>2000){
            level++;
            play_sample();
        }
    }
    if(level==3){
        if(score>3000){
            level++;
            play_sample();
        }
    }
    if(level==4){
        if(score>4000){
            level++;
            play_sample();
        }
    }    
    return level;
}

void descend_board(int lastf){ // cuando se completa una fila desciendo el gameboard desde el indice que tenia la fila
    
    int i,j;
    for (i=lastf;i>0;i--){
        
        for(j=0;j<NCol;j++){
            
            gameboard[i][j]=gameboard[i-1][j];
            
        }
    }
    
    for(i=0;j<NCol;j++){
        
        gameboard[0][j]=0;
    }
   
    
}

int check_down (int n){   // check down revisa debajo de cada pieza y devuelve 1 si se puede bajar o 0 caso contrario
    
    int i,j,x=piezas[n].pos.x,y=(piezas[n].pos.y)+1,size=piezas[n].size,suma=0;
    
    if(x>NCol){
        x=x-256;
    }
    if (y<(VNFil+1)){ 
        
    
    for(i=0;i<size;i++){
        for(j=0;j<size;j++){
            
            
            
                
            
                if((piezas[n].values[i*size+j] && gameboard[y+i][j+x]) ){
                
                
               
                    if(piezas[n].values[(i+1)*size+j] == gameboard[y+i][j+x]){
                    
                        suma++;
                    }

                
                }    
                
                else{

                    suma++;
            
                }
                
            
            
        }
    }
    }
    
    if(suma!=(size*size) ){ // si la suma de todos los lugares da distinto al tamanio del arreglo, entonces esa pieza no puede descender
        
        suma=0;
        
        
    }
    
    return suma;
}

void clear_board(void){ // borro el gameboard
    int i,j;
    for(i=0;i<16;i++){
        for(j=0;j<16;j++){
            gameboard[i+4][j]=0;
        }
    }
        
}




void piece_down(int n){ // imprimo la pieza un lugar mas abajo
    
    if(check_down(n)){
        int i,j;
        int py,px;
        
        
        py=++piezas[n].pos.y;
        px=piezas[n].pos.x;
        
        if(px>NCol){
            px=px-256;
        }
        
        for(j=0;j<piezas[n].size;j++){ // borro el lugar donde esta la pieza actualmente
           for(i=0;i<piezas[n].size;i++){
                if(piezas[n].values[i*(piezas[n].size)+j]){
                
                   gameboard[py+i-1][j+px]=0;
                }
            } 
            
        }
        
        for(i=0;i<piezas[n].size;i++){ //dibujo en el tablero la posicion proxima de la pieza para crear la ilusion de que se mueve ahacia abajo
            for(j=0;j<piezas[n].size;j++){
                
                if(piezas[n].values[i*(piezas[n].size)+j]){
                    
                    gameboard[py+i][j+px]=piezas[n].values[i*(piezas[n].size)+j];
                }
                
            }
        }
    }
}


void piece_right(int n){ // imprimo la posicion de la pieza un lugar a la derecga
    
    if(!check_right(n)){
        int i,j;
        int py,px;
        
        py=piezas[n].pos.y;
        px=++(piezas[n].pos.x);
        
        for(j=0;j<piezas[n].size;j++){//borro el lugar actual de la pieza
            for(i=0;i<piezas[n].size;i++){
                if(piezas[n].values[i*(piezas[n].size)+j]){
                
                    gameboard[py+i][px-1+j]=0;
                }
            } 
        }
        
        for(i=0;i<piezas[n].size;i++){
            for(j=0;j<piezas[n].size;j++){ // imprimo la pieza un lugar a la derecha
                
                if(piezas[n].values[i*(piezas[n].size)+j] ){
                    
                
                gameboard[py+i][j+px]=piezas[n].values[i*(piezas[n].size)+j];
                
                }
                
            }
        }
    }
}



void init_word(int palabra){ // inicio una palabra asi puede pasar de nuevo
    
    int i,j;
        
    words[palabra].pos.x= 16;    
    
    for(i=0;i<words[palabra].sizey;i++){
        for(j=0;j<words[palabra].sizex;j++){
            if(words[palabra].values[i*(words[palabra].sizex)+j] == 11 ){ // los lugares que tienen 11 del arreglo de una palabara es por que ya pasaron por el board

                words[palabra].values[i*(words[palabra].sizex)+j]=1;

            }

        }
    }
    
}





void letter_left(int palabra){ //imprimo a un costado una palabra en 
         
    int i,j;
    int py,px;
    
    py= words[palabra].pos.y;
    px=--(words[palabra].pos.x);

    if(px>100){
        px=px-256;
    }    
    
    if(px != 0-words[palabra].sizex){



        for(j=0;j<words[palabra].sizex;j++){
            for(i=0;i<words[palabra].sizey;i++){
                if(words[palabra].values[i*(words[palabra].sizex)+j]){

                    gameboard[py+i][px+1+j]=0;
                }
            } 
        }

        for(i=0;i<words[palabra].sizey;i++){
            for(j=0;j<words[palabra].sizex;j++){


                if(words[palabra].values[i*(words[palabra].sizex)+j]==1 && (px+j)<16 && (px+j)>0){

                    gameboard[py+i][j+px]=words[palabra].values[i*(words[palabra].sizex)+j];

                    if(px+j<=0){
                      words[palabra].values[i*(words[palabra].sizex)+j]=11;  
                    }

                }


            }
        }
    }
    else{
        init_word(palabra);
    }
}

void init_game(int chosen_mode,int chosen_diff){
    switch(chosen_mode){
        case 1: clear_board();
                create_floor();
                break;

        case 2: copy_board((int*)gameboard,(int*)storage);
                create_floor();
                break;
    }
    switch(chosen_diff){
        case 1: timer=80;// set timex slow and gen pieza faciles
            break;
           
        case 2: timer=75;// set timex normal and gen pieza todas
            break;
            
        case 3: timer=60;// gameboard con dificultades y timex mas rapido
            break;
            
        case 4: timer=50;// modo leyenda
            break;            
    }
    
}

void piece_left(int n){
    
    if(!check_left(n)){
        int i,j;
        int py,px;
        
        py=piezas[n].pos.y;
        px=--piezas[n].pos.x;
        
        if(px>NCol){
            px=px-256;
        }
        
        for(j=0;j<piezas[n].size;j++){
            for(i=0;i<piezas[n].size;i++){
                if(piezas[n].values[i*(piezas[n].size)+j]){
                
                    gameboard[py+i][px+1+j]=0;
                }
            } 
            
        }
        
        for(i=0;i<piezas[n].size;i++){
            for(j=0;j<piezas[n].size;j++){
                
                if(piezas[n].values[i*(piezas[n].size)+j] ){
                
                    gameboard[py+i][j+px]=piezas[n].values[i*(piezas[n].size)+j];
                }
                
            }
        }
    }
}

int check_right (int gen_pieza){
    
    int x,y,size,i,j,found_piece=0;
    x=piezas[gen_pieza].pos.x;
    y=piezas[gen_pieza].pos.y;
    size=piezas[gen_pieza].size;
    for(i=0;i<size && found_piece==0;i++){
        for(j=0;j<size && found_piece==0;j++){
            if(piezas[gen_pieza].values[i*size+j]){


                if(j<size-1){
                    found_piece= xor(gameboard[i+y][j+x+1],piezas[gen_pieza].values[i*size+j+1]);
                }            
                else{
                    if(gameboard[i+y][j+x+1]){
                        found_piece=1;
                    }
                }
                if(NCol==j+x+1){
                    found_piece=1;
                }                    
            }
        }
    }
    return found_piece;
}


int xor (int a,int b){
    if (a == b){
        return 0;
    }
    else{
        return 1;
    }
}


int check_left (int gen_pieza){
    
    int x,y,size,i,j,found_piece=0;
    x=piezas[gen_pieza].pos.x;
    y=piezas[gen_pieza].pos.y;
    size=piezas[gen_pieza].size;
    
    if(x>NCol){
        x=x-256;
    }
         
    for(i=0;i<size && found_piece==0;i++){
        for(j=0;j<size && found_piece==0;j++){
            if(piezas[gen_pieza].values[i*size+j]){

                if(j!=0){
                    found_piece= xor(gameboard[i+y][j+x-1],piezas[gen_pieza].values[i*size+j-1]);
                }            
                else{
                    if(gameboard[i+y][j+x-1]){
                        found_piece=1;
                    }
                }
                if((j+x-1)==-1){
                    found_piece=1;
                }
            }
        }
    }
    return found_piece;
}

   
    
void print_pieza(int n){
	
    int i,j;
    
    for(i=0;i<piezas[n].size;i++){
        for(j=0;j<piezas[n].size;j++){
            
                
            
                gameboard[i+piezas[n].pos.y][j+piezas[n].pos.x]=piezas[n].values[i*piezas[n].size+j];
               
            
        }
    }
    
             
        
        
        
}


void check_fin (int n){
    int i,j,conta=0,term;    
    i=4;
    term=0;
    menu=1;
    usleep(30000);
    if(piezas[n].pos.y<=4){
        for(j=0;j<NCol;j++){
            if(gameboard[i][j]>7){
                conta++;
            }
        }
        if(conta){
            finish_game();
            term=1;
            
        }
    }
    if(!term){
      menu=0;
    }
    conta=0;
    usleep(20000);
}

int rotate(int n){
    
    int i,j,cont=0,px;
    
    px=piezas[n].pos.x;
    
    
    
    if(px<0){
        return 0;
    }
    
    
    for(i=0;i<piezas[n].size;i++){
        for(j=0;j<piezas[n].size;j++){
            
                
            if(gameboard[i+piezas[n].pos.y][j+piezas[n].pos.x] != piezas[n].values[i*piezas[n].size+j]){
                cont++;
            } 
            
        }
    }
    
    
    if( piezas[n].pos.x+piezas[n].size <= NCol && cont==0 ){
        
    
    
        int size=piezas[n].size;
	
        int mat[size][size];
	
	int i,j;
	
        for(i=0;i<size;i++){
		
		
		
		for(j=0;j<size;j++){

			mat[i][j]=piezas[n].values[(size-1-j)*size+i];
			
		}
	}
	
	
	for(i=0;i<size;i++){
		
		
		
		for(j=0;j<size;j++){

			piezas[n].values[i*size+j]=mat[i][j];
			
		}
	}
        
        ++piezas[n].rotacion;
        
        if (piezas[n].rotacion ==4){
            
            piezas[n].rotacion=0;
        }
        
      
    }
     return (!cont);   
}

void stayed_blocks(void){
    
    int i,j;
    for(i=0;i<VNFil;i++){
        for(j=0;j<NCol;j++){
            
                
            switch(gameboard[i][j]){
                
                
            case 1: gameboard[i][j]=11;
                 break;
            case 2:gameboard[i][j]=12;
                 break;
            case 3:gameboard[i][j]=13;
                 break;
            case 4:gameboard[i][j]=14;
                 break;
            case 5:gameboard[i][j]=15;
                 break;
            case 6:gameboard[i][j]=16;
                 break;
            case 7:gameboard[i][j]=17;
                 break;

                
            } 
            
        }
    }   
}


void reorder_pieza(int n){
    
    int size=piezas[n].size;
	
        int mat[size][size];
	
	int i,j;
	
        for(i=0;i<size;i++){
		
		
		
		for(j=0;j<size;j++){

			mat[i][j]=piezas[n].values[(size-1-j)*size+i];
			
		}
	}
	
	
	for(i=0;i<size;i++){
		
		
		
		for(j=0;j<size;j++){

			piezas[n].values[i*size+j]=mat[i][j];
			
		}
	}
}

void down(int n){
    int out=1;
    for(;check_down(n) && out;){
        if (move()==-2){
            out=0;
        }        
        piece_down(n);
        usleep(70000);
    }
}