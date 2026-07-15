#include <cstdlib>
#include <ctime>
#include "Juego.h"

int main() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    Juego juego(4);
    juego.jugar();

    return 0;
}
