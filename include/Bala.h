#ifndef BALA_H
#define BALA_H

class Bala {
private:
    int x;
    int y;
    bool activa;
public:
    Bala(int x, int y);
    void mover();
    int getX() const;
    int getY() const;
    bool estaActiva() const;
    void desactivar();
};

#endif
