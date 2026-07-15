#ifndef ENEMIGO_H
#define ENEMIGO_H

#include "Entidad.h"

class Enemigo : public Entidad {
private:
    int velocidad;
public:
    Enemigo(int x, int y, int vida, int velocidad);
    void actuar() override;
    char getSimbolo() const override;
    int getVelocidad() const;
};

#endif
