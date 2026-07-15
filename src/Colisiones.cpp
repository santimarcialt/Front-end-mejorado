#include "Colisiones.h"

bool hayColision(const Bala& b, const Enemigo& e) {
    return b.getX() == e.getX() && b.getY() == e.getY();
}

bool hayColision(const Enemigo& e, const Nave& n) {
    return e.getX() == n.getX() && e.getY() >= n.getY();
}
