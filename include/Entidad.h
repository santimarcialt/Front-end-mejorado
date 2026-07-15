#ifndef ENTIDAD_H
#define ENTIDAD_H

class Entidad {
protected:
    int x;
    int y;
    int vida;
public:
    Entidad(int x, int y, int vida);
    virtual void actuar() = 0;
    virtual char getSimbolo() const = 0;
    int getX() const;
    int getY() const;
    int getVida() const;
    void recibirDano(int dano);
    void establecerPosicion(int nuevoX, int nuevoY);
    void establecerVida(int nuevaVida);
    bool estaViva() const;
    virtual ~Entidad();
};

#endif
