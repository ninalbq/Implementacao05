#ifndef STATION_H
#define STATION_H

#include <pthread.h>

struct station {
    int assentos_livres;          
    int passageiros_esperando;    
    int passageiros_embarcando;
    pthread_mutex_t mutex;        
    pthread_cond_t trem_chegou;  
    pthread_cond_t lotado_ou_livre;
};

void station_init(struct station *station);
void station_load_car(struct station *station, int free_seats);
void station_wait_for_car(struct station *station);
void station_on_board(struct station *station);

#endif // STATION_H