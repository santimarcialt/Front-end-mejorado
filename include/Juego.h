#ifndef JUEGO_H
#define JUEGO_H

#include <string>
#include "Entidad.h"
#include "Nave.h"
#include "Enemigo.h"
#include "Bala.h"

class Juego {
private:
    Entidad** entidades;
    int cantidadEntidades;
    int capacidadEntidades;

    Bala** balas;
    int cantidadBalas;
    int capacidadBalas;

    Nave* jugador;
    int turno;
    bool partidaTerminada;
    std::string ultimoMensaje;

    void crecerEntidades();
    void crecerBalas();
    void moverEntidades();
    void moverBalas();
    void revisarColisiones();
    void generarOleada();
    void limpiarEnemigosYBalas();
    bool hayPartidaGuardada(const std::string& archivo) const;
    std::string barraVida() const;

public:
    Juego(int capacidadInicial);
    ~Juego();

    void agregarEnemigo(int x, int y, int vida, int velocidad);
    void agregarBala(int x, int y);
    void dispararBala();
    void avanzarTurno();

    void mostrarHistoria() const;
    void mostrarMapa() const;
    void mostrarMenu() const;

    void guardarPartida(const std::string& archivo) const;
    void cargarPartida(const std::string& archivo);

    void jugar();
};

#endif
