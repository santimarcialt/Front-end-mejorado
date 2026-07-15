#include "Bala.h"

Bala::Bala(int x, int y) {
    this->x = x;
    this->y = y;
    activa = true;
}

void Bala::mover() {
    y -= 1;
}

int Bala::getX() const {
    return x;
}

int Bala::getY() const {
    return y;
}

bool Bala::estaActiva() const {
    return activa;
}

void Bala::desactivar() {
    activa = false;
}
