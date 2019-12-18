/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   pc.h
 * Author: manucorcos
 *
 * Created on December 16, 2019, 7:21 PM
 */

#ifndef PC_H
#define PC_H

typedef struct { 
    unsigned int x;
    unsigned int y;

}dcoord_t;




int inicializacion(void);
int move (void);
int print_menu (void);
void leavestop(int n);
void print_stopmenu(void);
void copy_board(int *pllegada,int *psalida);    
void closepro(void);
void play_sample (void);
void create_pantalla(void);
void update_board(void);
void finish_game (void);
void print_screen (int n2);

#endif /* PC_H */

