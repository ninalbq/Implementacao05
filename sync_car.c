
#include "sync_car.h"
#include <stddef.h>

void station_load_car(struct station *station, int count) {
    pthread_mutex_lock(&station->mutex);
    
    station->assentos_livres = count;
    
    if (count > 0 && station->passageiros_esperando > 0) {
        pthread_cond_broadcast(&station->trem_chegou);
        
        while ((station->assentos_livres > 0 && station->passageiros_esperando > 0) || station->passageiros_embarcando > 0) {
            pthread_cond_wait(&station->lotado_ou_livre, &station->mutex);
        }
    }
    
    station->assentos_livres = 0;
    
    pthread_mutex_unlock(&station->mutex);
}

void station_wait_for_car(struct station *station) {
    pthread_mutex_lock(&station->mutex);
    
    station->passageiros_esperando++;
    
    while (station->assentos_livres == 0) {
        pthread_cond_wait(&station->trem_chegou, &station->mutex);
    }
    
    station->assentos_livres--;
    station->passageiros_esperando--;
    station->passageiros_embarcando++;
    
    pthread_mutex_unlock(&station->mutex);
}

void station_on_board(struct station *station) {
    pthread_mutex_lock(&station->mutex);
    
    station->passageiros_embarcando--;
    
    if ((station->assentos_livres == 0 || station->passageiros_esperando == 0) && station->passageiros_embarcando == 0) {
        pthread_cond_signal(&station->lotado_ou_livre);
    }
    
    pthread_mutex_unlock(&station->mutex);}

void station_init(struct station *station) {
    station->assentos_livres = 0;
    station->passageiros_esperando = 0;
    station->passageiros_embarcando = 0;
    
    pthread_mutex_init(&station->mutex, NULL);
    pthread_cond_init(&station->trem_chegou, NULL);
    pthread_cond_init(&station->lotado_ou_livre, NULL);
}
