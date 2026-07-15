CONTRIBUCIONES

Integrantes: Leighann, Santiago Marcial
Proyecto: Space Shooter por turnos 

Leighann - Jerarquia de clases y comportamiento


Archivos implementados:
- Entidad.h / Entidad.cpp
- Nave.h / Nave.cpp
- Enemigo.h / Enemigo.cpp
- Bala.h / Bala.cpp

Detalle:

Entidad (clase abstracta base)
- Atributos protected: x, y, vida
- Metodos virtuales puros: actuar() y getSimbolo(), obligan a cada clase hija a definir su propio comportamiento
- recibirDano(int dano): valida que la vida nunca quede negativa (la deja en 0)
- Destructor virtual

Nave (clase concreta, hereda de Entidad)
- Atributos private: puntaje, direccionPendiente, anchoMapa
- actuar() override: aplica el movimiento pendiente (izquierda/derecha) y evita que la nave se salga del mapa
- getSimbolo() override: devuelve '^'
- moverIzquierda() / moverDerecha(): guardan la direccion elegida por el jugador
- sumarPuntaje(int p): valida que el puntaje sumado sea positivo antes de sumarlo

Enemigo (clase concreta, hereda de Entidad)
- Atributo private: velocidad
- actuar() override: baja una cantidad de filas igual a su velocidad
- getSimbolo() override: devuelve 'V'

Bala (clase de apoyo, no hereda de Entidad)
- Atributos private: x, y, activa
- mover(): sube una fila cada turno
- desactivar() / estaActiva(): controlan si la bala sigue en juego

Esta parte cubre principalmente:
- Criterio 1: Encapsulamiento
- Criterio 2: Herencia y polimorfismo

Santiago - Contenedor, memoria dinamica y logica del juego


Archivos implementados:
- Colisiones.h / Colisiones.cpp
- Juego.h / Juego.cpp
- main.cpp

Detalle:

Colisiones
- hayColision(Bala, Enemigo): compara posicion exacta para detectar impacto
- hayColision(Enemigo, Nave): compara columna y si el enemigo alcanzo o paso la fila de la nave
- Son dos versiones de la misma funcion, sobrecargadas, con comportamiento distinto justificado por el dominio (sobrecarga de funciones, no de operadores)

Juego (clase contenedora)
- entidades: arreglo dinamico de punteros Entidad* (incluye la nave y los enemigos, usados de forma polimorfica)
- balas: arreglo dinamico de punteros Bala*
- crecerEntidades() / crecerBalas(): duplican la capacidad del arreglo cuando se llena
- generarOleada(): agrega enemigos nuevos en columnas aleatorias cada cierto numero de turnos (rand())
- avanzarTurno(): mueve entidades, mueve balas, revisa colisiones, controla cuando termina la partida
- mostrarMapa(): dibuja el mapa recorriendo el arreglo de entidades de forma polimorfica
- guardarPartida() / cargarPartida(): persistencia en archivo de texto, maneja el caso de archivo inexistente
- jugar(): bucle principal que conecta menu, turno, mapa y persistencia
- Destructor: libera con delete cada entidad y cada bala, y libera los arreglos con delete[]

main.cpp
- Punto de entrada del programa
- srand() para que las oleadas de enemigos salgan en columnas distintas cada partida
- Crea el objeto Juego y llama a cargarPartida() y jugar()

Esta parte cubre principalmente:
- Criterio 3: Memoria dinamica
- Criterio 4: Persistencia en archivo
- Criterio 5: Sobrecarga

