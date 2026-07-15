#include "Enemigo.h"

Enemigo::Enemigo(int x, int y, int vida, int velocidad) : Entidad(x, y, vida) {
    this->velocidad = velocidad;
}

void Enemigo::actuar() {
    y += velocidad;
}

char Enemigo::getSimbolo() const {
    return 'V';
}

int Enemigo::getVelocidad() const {
    return velocidad;
}
