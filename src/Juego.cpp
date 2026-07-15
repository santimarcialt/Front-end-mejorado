#include "Juego.h"
#include "Colisiones.h"
#include "Configuracion.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

namespace {
    // Codigos de color ANSI para darle estilo "arcade" a la consola.
    // Funcionan en la terminal de Linux/Mac y en VS Code / Windows Terminal.
    const std::string RESET    = "\033[0m";
    const std::string CIAN     = "\033[1;36m"; // nave del jugador
    const std::string ROJO     = "\033[1;31m"; // enemigos
    const std::string AMARILLO = "\033[1;33m"; // balas y puntaje
    const std::string VERDE    = "\033[1;32m"; // vida restante
    const std::string GRIS     = "\033[90m";   // celdas vacias
    const std::string AZUL     = "\033[1;34m"; // bordes del mapa
    const std::string MAGENTA  = "\033[1;35m"; // titulo / turno
}

Juego::Juego(int capacidadInicial) {
    capacidadEntidades = capacidadInicial;
    cantidadEntidades = 0;
    entidades = new Entidad*[capacidadEntidades];

    capacidadBalas = capacidadInicial;
    cantidadBalas = 0;
    balas = new Bala*[capacidadBalas];

    Nave* nave = new Nave(ANCHO_MAPA / 2, ALTO_MAPA - 1, VIDA_MAXIMA_NAVE, ANCHO_MAPA);
    jugador = nave;

    entidades[cantidadEntidades] = nave;
    cantidadEntidades++;

    turno = 1;
    partidaTerminada = false;

    generarOleada();
}

Juego::~Juego() {
    for (int i = 0; i < cantidadEntidades; i++) {
        delete entidades[i];
        entidades[i] = nullptr;
    }
    delete[] entidades;

    for (int i = 0; i < cantidadBalas; i++) {
        delete balas[i];
        balas[i] = nullptr;
    }
    delete[] balas;

    jugador = nullptr;
}

void Juego::crecerEntidades() {
    capacidadEntidades *= 2;
    Entidad** nuevo = new Entidad*[capacidadEntidades];
    for (int i = 0; i < cantidadEntidades; i++) {
        nuevo[i] = entidades[i];
    }
    delete[] entidades;
    entidades = nuevo;
}

void Juego::crecerBalas() {
    capacidadBalas *= 2;
    Bala** nuevo = new Bala*[capacidadBalas];
    for (int i = 0; i < cantidadBalas; i++) {
        nuevo[i] = balas[i];
    }
    delete[] balas;
    balas = nuevo;
}

void Juego::agregarEnemigo(int x, int y, int vida, int velocidad) {
    if (cantidadEntidades == capacidadEntidades) {
        crecerEntidades();
    }
    entidades[cantidadEntidades] = new Enemigo(x, y, vida, velocidad);
    cantidadEntidades++;
}

void Juego::generarOleada() {
    for (int i = 0; i < ENEMIGOS_POR_OLEADA; i++) {
        int columna = rand() % ANCHO_MAPA;
        agregarEnemigo(columna, 0, 1, 1);
    }
}

void Juego::agregarBala(int x, int y) {
    if (cantidadBalas == capacidadBalas) {
        crecerBalas();
    }
    balas[cantidadBalas] = new Bala(x, y);
    cantidadBalas++;
}

void Juego::dispararBala() {
    agregarBala(jugador->getX(), jugador->getY() - 1);
}

void Juego::moverEntidades() {
    for (int i = 0; i < cantidadEntidades; i++) {
        entidades[i]->actuar();
    }
}

void Juego::moverBalas() {
    for (int i = 0; i < cantidadBalas; i++) {
        balas[i]->mover();
        if (balas[i]->getY() < 0) {
            balas[i]->desactivar();
        }
    }
}

void Juego::revisarColisiones() {
    for (int i = 0; i < cantidadBalas; i++) {
        if (!balas[i]->estaActiva()) {
            continue;
        }
        for (int j = 0; j < cantidadEntidades; j++) {
            if (!entidades[j]->estaViva()) {
                continue;
            }
            Enemigo* enemigo = dynamic_cast<Enemigo*>(entidades[j]);
            if (enemigo != nullptr && hayColision(*balas[i], *enemigo)) {
                balas[i]->desactivar();
                enemigo->recibirDano(enemigo->getVida());
                jugador->sumarPuntaje(PUNTOS_POR_ENEMIGO);
                ultimoMensaje += VERDE + std::string("  >> Impacto directo! Destruiste un enemigo (+")
                                 + std::to_string(PUNTOS_POR_ENEMIGO) + " puntos)." + RESET + "\n";
            }
        }
    }

    for (int i = 0; i < cantidadEntidades; i++) {
        if (!entidades[i]->estaViva()) {
            continue;
        }
        Enemigo* enemigo = dynamic_cast<Enemigo*>(entidades[i]);
        if (enemigo != nullptr && hayColision(*enemigo, *jugador)) {
            jugador->recibirDano(1);
            enemigo->recibirDano(enemigo->getVida());
            ultimoMensaje += ROJO + std::string("  >> Un enemigo llego hasta tu nave! Pierdes 1 vida.") + RESET + "\n";
        }
    }
}

void Juego::limpiarEnemigosYBalas() {
    // El indice 0 siempre es la nave del jugador, por eso empezamos en 1.
    for (int i = 1; i < cantidadEntidades; i++) {
        delete entidades[i];
        entidades[i] = nullptr;
    }
    cantidadEntidades = 1;

    for (int i = 0; i < cantidadBalas; i++) {
        delete balas[i];
        balas[i] = nullptr;
    }
    cantidadBalas = 0;
}

bool Juego::hayPartidaGuardada(const std::string& archivo) const {
    std::ifstream entrada(archivo);
    return entrada.is_open();
}

void Juego::avanzarTurno() {
    ultimoMensaje = "";

    moverEntidades();
    moverBalas();
    revisarColisiones();
    turno++;

    if (turno % TURNOS_POR_OLEADA == 0) {
        generarOleada();
        ultimoMensaje += ROJO + std::string("  >> Alerta! Una nueva oleada de enemigos aparecio.") + RESET + "\n";
    }

    if (!jugador->estaViva()) {
        partidaTerminada = true;
    }
}

void Juego::mostrarHistoria() const {
    std::cout << MAGENTA << std::string(44, '=') << RESET << std::endl;
    std::cout << MAGENTA << "              SPACE SHOOTER" << RESET << std::endl;
    std::cout << MAGENTA << std::string(44, '=') << RESET << std::endl;
    std::cout << "Una formacion enemiga desciende hacia tu nave," << std::endl;
    std::cout << "oleada tras oleada. Debes resistir el mayor" << std::endl;
    std::cout << "numero de turnos posible." << std::endl << std::endl;

    std::cout << "Simbolos en el mapa:" << std::endl;
    std::cout << "  " << CIAN     << "^" << RESET << "  tu nave" << std::endl;
    std::cout << "  " << ROJO     << "V" << RESET << "  enemigo" << std::endl;
    std::cout << "  " << AMARILLO << "|" << RESET << "  tu bala" << std::endl << std::endl;

    std::cout << "Empiezas con " << VIDA_MAXIMA_NAVE << " vidas." << std::endl;
    std::cout << "Si un enemigo llega hasta tu fila, pierdes 1 vida." << std::endl;
    std::cout << "Si destruyes un enemigo con tu bala, ganas " << PUNTOS_POR_ENEMIGO << " puntos." << std::endl;
    std::cout << "Si tus vidas llegan a 0, termina la partida." << std::endl << std::endl;

    std::cout << "Comandos:" << std::endl;
    std::cout << "  " << CIAN << "1" << RESET << " = mover izquierda    "
               << CIAN << "2" << RESET << " = mover derecha" << std::endl;
    std::cout << "  " << AMARILLO << "3" << RESET << " = disparar           "
               << GRIS << "4" << RESET << " = guardar y salir" << std::endl;
    std::cout << MAGENTA << std::string(44, '=') << RESET << std::endl << std::endl;
}

void Juego::mostrarMapa() const {
    char mapa[ALTO_MAPA][ANCHO_MAPA];
    for (int fila = 0; fila < ALTO_MAPA; fila++) {
        for (int columna = 0; columna < ANCHO_MAPA; columna++) {
            mapa[fila][columna] = '.';
        }
    }

    for (int i = 0; i < cantidadEntidades; i++) {
        if (!entidades[i]->estaViva()) {
            continue;
        }
        int fila = entidades[i]->getY();
        int columna = entidades[i]->getX();
        if (fila >= 0 && fila < ALTO_MAPA && columna >= 0 && columna < ANCHO_MAPA) {
            mapa[fila][columna] = entidades[i]->getSimbolo();
        }
    }

    for (int i = 0; i < cantidadBalas; i++) {
        if (!balas[i]->estaActiva()) {
            continue;
        }
        int fila = balas[i]->getY();
        int columna = balas[i]->getX();
        if (fila >= 0 && fila < ALTO_MAPA && columna >= 0 && columna < ANCHO_MAPA) {
            mapa[fila][columna] = '|';
        }
    }

    std::cout << MAGENTA << "Turno " << turno << RESET
               << "   Vida " << barraVida()
               << " (" << jugador->getVida() << "/" << VIDA_MAXIMA_NAVE << ")"
               << "   " << AMARILLO << "Puntaje: " << jugador->getPuntaje() << RESET
               << std::endl << std::endl;

    std::string borde = "+" + std::string(ANCHO_MAPA * 2 + 1, '-') + "+";

    std::cout << AZUL << borde << RESET << std::endl;
    for (int fila = 0; fila < ALTO_MAPA; fila++) {
        std::cout << AZUL << "|" << RESET << " ";
        for (int columna = 0; columna < ANCHO_MAPA; columna++) {
            char c = mapa[fila][columna];
            if (c == '^') {
                std::cout << CIAN << c << RESET;
            } else if (c == 'V') {
                std::cout << ROJO << c << RESET;
            } else if (c == '|') {
                std::cout << AMARILLO << c << RESET;
            } else {
                std::cout << GRIS << c << RESET;
            }
            std::cout << ' ';
        }
        std::cout << AZUL << "|" << RESET << std::endl;
    }
    std::cout << AZUL << borde << RESET << std::endl << std::endl;
}

std::string Juego::barraVida() const {
    std::string barra = "[";
    for (int i = 0; i < VIDA_MAXIMA_NAVE; i++) {
        if (i < jugador->getVida()) {
            barra += VERDE + "#" + RESET;
        } else {
            barra += GRIS + "." + RESET;
        }
    }
    barra += "]";
    return barra;
}

void Juego::mostrarMenu() const {
    std::cout << "Elige opcion (" << CIAN << "1" << RESET << " izq / "
               << CIAN << "2" << RESET << " der / "
               << AMARILLO << "3" << RESET << " disparar / "
               << GRIS << "4" << RESET << " guardar): ";
}

void Juego::guardarPartida(const std::string& archivo) const {
    std::ofstream salida(archivo);

    // Linea 1: turno. Linea 2: estado de la nave (vida, puntaje, posicion x).
    salida << turno << std::endl;
    salida << jugador->getVida() << " " << jugador->getPuntaje() << " " << jugador->getX() << std::endl;

    // Enemigos vivos: cantidad, y luego una linea por cada uno (x y vida velocidad).
    int enemigosVivos = 0;
    for (int i = 1; i < cantidadEntidades; i++) {
        if (entidades[i]->estaViva()) {
            enemigosVivos++;
        }
    }
    salida << enemigosVivos << std::endl;
    for (int i = 1; i < cantidadEntidades; i++) {
        if (!entidades[i]->estaViva()) {
            continue;
        }
        Enemigo* enemigo = dynamic_cast<Enemigo*>(entidades[i]);
        if (enemigo != nullptr) {
            salida << enemigo->getX() << " " << enemigo->getY() << " "
                   << enemigo->getVida() << " " << enemigo->getVelocidad() << std::endl;
        }
    }

    // Balas activas: cantidad, y luego una linea por cada una (x y).
    int balasActivas = 0;
    for (int i = 0; i < cantidadBalas; i++) {
        if (balas[i]->estaActiva()) {
            balasActivas++;
        }
    }
    salida << balasActivas << std::endl;
    for (int i = 0; i < cantidadBalas; i++) {
        if (balas[i]->estaActiva()) {
            salida << balas[i]->getX() << " " << balas[i]->getY() << std::endl;
        }
    }
}

void Juego::cargarPartida(const std::string& archivo) {
    std::ifstream entrada(archivo);
    if (!entrada.is_open()) {
        // No hay partida guardada: se conserva el estado inicial ya generado.
        return;
    }

    int turnoGuardado;
    int vidaGuardada;
    int puntajeGuardado;
    int xGuardado;
    entrada >> turnoGuardado >> vidaGuardada >> puntajeGuardado >> xGuardado;

    // Se descartan los enemigos y balas iniciales; el mapa se reconstruye
    // exactamente como quedo la ultima vez que se guardo la partida.
    limpiarEnemigosYBalas();

    turno = turnoGuardado;
    jugador->establecerVida(vidaGuardada);
    jugador->establecerPuntaje(puntajeGuardado);
    jugador->establecerPosicion(xGuardado, jugador->getY());

    int enemigosGuardados;
    entrada >> enemigosGuardados;
    for (int i = 0; i < enemigosGuardados; i++) {
        int ex, ey, evida, evelocidad;
        entrada >> ex >> ey >> evida >> evelocidad;
        agregarEnemigo(ex, ey, evida, evelocidad);
    }

    int balasGuardadas;
    entrada >> balasGuardadas;
    for (int i = 0; i < balasGuardadas; i++) {
        int bx, by;
        entrada >> bx >> by;
        agregarBala(bx, by);
    }
}

void Juego::jugar() {
    mostrarHistoria();

    if (hayPartidaGuardada("partida.txt")) {
        std::cout << AMARILLO << "Se encontro una partida guardada." << RESET << std::endl;
        std::cout << "Deseas continuar donde te quedaste? ("
                   << CIAN << "1" << RESET << "=si / "
                   << GRIS << "2" << RESET << "=no, empezar de nuevo): ";

        int opcionInicial;
        std::cin >> opcionInicial;

        if (opcionInicial == 1) {
            cargarPartida("partida.txt");
            std::cout << VERDE << "Partida cargada. Continuas en el turno " << turno << "." << RESET
                       << std::endl << std::endl;
        } else {
            std::cout << "Empezando una partida nueva." << std::endl << std::endl;
        }
    }

    while (!partidaTerminada) {
        mostrarMapa();
        mostrarMenu();

        int opcion;
        std::cin >> opcion;

        std::string mensajeAccion;
        if (opcion == 1) {
            jugador->moverIzquierda();
            mensajeAccion = "Te mueves a la izquierda.";
        } else if (opcion == 2) {
            jugador->moverDerecha();
            mensajeAccion = "Te mueves a la derecha.";
        } else if (opcion == 3) {
            dispararBala();
            mensajeAccion = AMARILLO + std::string("Disparas una bala.") + RESET;
        } else if (opcion == 4) {
            std::cout << "Seguro que deseas guardar y salir? (" << CIAN << "1" << RESET
                       << "=si / " << GRIS << "2" << RESET << "=no): ";
            int confirmacion;
            std::cin >> confirmacion;
            if (confirmacion == 1) {
                guardarPartida("partida.txt");
                std::cout << VERDE << "Partida guardada en partida.txt. Hasta la proxima!" << RESET << std::endl;
                return;
            } else {
                std::cout << "Cancelado, seguimos jugando." << std::endl << std::endl;
                continue;
            }
        } else {
            std::cout << ROJO << "Opcion invalida, intenta de nuevo." << RESET << std::endl << std::endl;
            continue;
        }

        std::cout << mensajeAccion << std::endl;

        avanzarTurno();

        if (!ultimoMensaje.empty()) {
            std::cout << ultimoMensaje;
        }

        if (!partidaTerminada) {
            std::cout << std::endl << "Presiona ENTER para continuar...";
            std::cin.ignore();
            std::cin.get();
        }
        std::cout << std::endl;
    }

    std::cout << ROJO << std::string(48, '=') << RESET << std::endl;
    std::cout << ROJO << "  GAME OVER - Puntaje final: " << jugador->getPuntaje() << RESET << std::endl;
    std::cout << ROJO << std::string(48, '=') << RESET << std::endl;
}
