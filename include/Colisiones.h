#ifndef COLISIONES_H
#define COLISIONES_H

#include "Bala.h"
#include "Enemigo.h"
#include "Nave.h"

bool hayColision(const Bala& b, const Enemigo& e);
bool hayColision(const Enemigo& e, const Nave& n);

#endif
