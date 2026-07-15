#ifndef NAVE_H
#define NAVE_H

#include "Entidad.h"

class Nave : public Entidad {
private:
    int puntaje;
    int direccionPendiente;
    int anchoMapa;
public:
    Nave(int x, int y, int vida, int anchoMapa);
    void actuar() override;
    char getSimbolo() const override;
    void sumarPuntaje(int p);
    void establecerPuntaje(int p);
    int getPuntaje() const;
    void moverIzquierda();
    void moverDerecha();
};

#endif
