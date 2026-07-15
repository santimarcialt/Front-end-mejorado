#include "Entidad.h"

Entidad::Entidad(int x, int y, int vida) {
    this->x = x;
    this->y = y;
    this->vida = vida;
}

int Entidad::getX() const {
    return x;
}

int Entidad::getY() const {
    return y;
}

int Entidad::getVida() const {
    return vida;
}

void Entidad::recibirDano(int dano) {
    vida -= dano;
    if (vida < 0) {
        vida = 0;
    }
}

void Entidad::establecerPosicion(int nuevoX, int nuevoY) {
    x = nuevoX;
    y = nuevoY;
}

void Entidad::establecerVida(int nuevaVida) {
    // Validacion: no se permite vida negativa
    if (nuevaVida < 0) {
        vida = 0;
    } else {
        vida = nuevaVida;
    }
}

bool Entidad::estaViva() const {
    return vida > 0;
}

Entidad::~Entidad() {
}
