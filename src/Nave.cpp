#include "Nave.h"

Nave::Nave(int x, int y, int vida, int anchoMapa) : Entidad(x, y, vida) {
    puntaje = 0;
    direccionPendiente = 0;
    this->anchoMapa = anchoMapa;
}

void Nave::actuar() {
    x += direccionPendiente;
    if (x < 0) {
        x = 0;
    }
    if (x >= anchoMapa) {
        x = anchoMapa - 1;
    }
    direccionPendiente = 0;
}

char Nave::getSimbolo() const {
    return '^';
}

void Nave::sumarPuntaje(int p) {
    if (p > 0) {
        puntaje += p;
    }
}

void Nave::establecerPuntaje(int p) {
    // Validacion: el puntaje guardado nunca puede ser negativo
    if (p < 0) {
        puntaje = 0;
    } else {
        puntaje = p;
    }
}

int Nave::getPuntaje() const {
    return puntaje;
}

void Nave::moverIzquierda() {
    direccionPendiente = -1;
}

void Nave::moverDerecha() {
    direccionPendiente = 1;
}
