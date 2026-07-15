# Space Shooter por Turnos

## 1. Introducción

**Space Shooter** es un videojuego de consola desarrollado en **C++** como proyecto de Programación Avanzada en C++. El jugador controla una nave espacial que debe resistir oleadas de enemigos que descienden por el mapa. 

El objetivo principal del juego es sobrevivir la mayor cantidad de turnos posible, destruir enemigos y acumular puntaje. En cada turno, el jugador puede mover la nave hacia la izquierda, moverla hacia la derecha, disparar una bala o guardar la partida. Después de cada acción, el sistema actualiza el estado del juego: mueve enemigos, desplaza balas, revisa colisiones, suma puntos y verifica si la nave sigue con vida.

El proyecto fue diseñado para aplicar varios conceptos importantes de C++, especialmente programación orientada a objetos. Por eso, el juego no se basa únicamente en mostrar símbolos en consola, sino en organizar correctamente sus clases y relaciones internas. Entre los elementos principales se encuentran una clase base abstracta llamada `Entidad`, clases derivadas como `Nave` y `Enemigo`, una clase de apoyo llamada `Bala`, un sistema de colisiones y una clase principal `Juego` que controla toda la ejecución.

Además, el proyecto incluye memoria dinámica, persistencia en archivo `.txt`, sobrecarga de funciones y uso de polimorfismo. Estos elementos permiten que el programa cumpla con los criterios técnicos solicitados en la rúbrica, manteniendo una estructura comprensible y defendible.

## 2. Controles del juego

El juego funciona mediante un sistema de **turnos**. En cada turno, el jugador debe ingresar una opción numérica para decidir qué acción realizará la nave. Después de elegir una acción válida, el programa actualiza el estado de la partida: mueve las entidades, desplaza las balas, revisa colisiones, genera nuevas oleadas si corresponde y muestra nuevamente el mapa.

Los controles principales son los siguientes:

| Comando | Acción |
|---|---|
| `1` | Mover la nave hacia la izquierda |
| `2` | Mover la nave hacia la derecha |
| `3` | Disparar una bala |
| `4` | Guardar la partida y salir |

### Funcionamiento de cada comando

El comando `1` permite mover la nave una posición hacia la izquierda. Si la nave ya se encuentra en el borde izquierdo del mapa, el programa evita que salga del límite permitido.

El comando `2` permite mover la nave una posición hacia la derecha. De la misma manera, si la nave llega al borde derecho, el programa limita su movimiento para mantenerla dentro del mapa.

El comando `3` permite disparar una bala desde la posición actual de la nave. La bala se crea sobre la nave y avanza hacia arriba en los siguientes turnos. Si la bala impacta contra un enemigo, este recibe daño y el jugador gana puntos.

El comando `4` permite guardar la partida en un archivo `.txt` y salir del juego. Esto forma parte del sistema de persistencia, ya que permite conservar datos importantes como el turno, la vida actual de la nave y el puntaje acumulado.

## 3. Estructura del proyecto

La estructura general del proyecto es la siguiente:
```txt
Proyecto-final/
main.cpp
Configuracion.h
Entidad.h
Entidad.cpp
Nave.h
Nave.cpp
Enemigo.h
Enemigo.cpp
Bala.h
Bala.cpp
Colisiones.h
Colisiones.cpp
Juego.h
Juego.cpp
partida.txt
README.md
CONTRIBUCIONES.md  
```

## 4. Explicación de clases

### 4.1 Clase `Entidad`

La clase `Entidad` es la base principal de la jerarquía del juego. Su función es representar cualquier objeto importante que tenga una posición dentro del mapa y una cantidad de vida. En el proyecto, `Nave` y `Enemigo` heredan de esta clase, por lo que ambas comparten atributos y comportamientos básicos, aunque cada una actúa de forma distinta.

Esta clase es fundamental porque permite organizar el juego usando programación orientada a objetos. En lugar de manejar la nave y los enemigos como elementos completamente separados, ambos se pueden tratar como entidades del juego. Esto permite aplicar herencia, polimorfismo y encapsulamiento de forma clara.

#### Declaración general de la clase

```cpp
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
    bool estaViva() const;
    virtual ~Entidad();
}; 
``` 
**Protected** se usa porque la nave necesita modificar su posición horizontal cuando el jugador decide moverse. El enemigo necesita modificar su posición vertical cuando avanza hacia abajo en el mapa. Por eso, si x, y y vida fueran private, las clases hijas no podrían usarlas directamente y el código sería más largo o menos práctico.

Sin embargo, tampoco conviene que estos atributos sean public, porque cualquier parte del programa podría modificar la posición o la vida sin control. Por ejemplo, una parte externa podría colocar un enemigo fuera del mapa o asignar una vida negativa. Por eso, protected es una decisión equilibrada: permite la herencia, pero mantiene los datos protegidos frente al exterior. 

**Public** en recibirDano(), porque el sistema de colisiones necesita reducir la vida de una entidad cuando ocurre un impacto. Si una bala golpea a un enemigo, el enemigo recibe daño. Si un enemigo alcanza a la nave, la nave también recibe daño. 

**Virtual puro** Gracias a los métodos virtuales puros, cada clase hija está obligada a implementar su propia versión de actuar() y getSimbolo(). Esto asegura que todas las entidades tengan una acción y una representación visual. 

### 4.2 Clase `Nave`

La clase `Nave` representa al jugador dentro del juego. Esta clase hereda de `Entidad`, por lo que conserva los atributos básicos de una entidad, como posición `x`, posición `y` y `vida`, pero agrega elementos propios como el puntaje, la dirección pendiente y el ancho del mapa.

Esta clase es fundamental porque conecta directamente las decisiones del jugador con la lógica interna del programa. La nave puede moverse hacia la izquierda, moverse hacia la derecha, acumular puntos y mantenerse dentro de los límites del mapa. Además, al heredar de `Entidad`, puede ser tratada de forma polimórfica junto con otros objetos del juego.

#### Declaración general de la clase

```cpp
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
    int getPuntaje() const;
    void moverIzquierda();
    void moverDerecha();
};
```

**Private** se usa en `puntaje`, `direccionPendiente` y `anchoMapa`, porque estos datos pertenecen al estado interno de la nave. El puntaje no debe modificarse directamente desde fuera, ya que podría alterarse con valores incorrectos. La dirección pendiente debe cambiarse únicamente mediante `moverIzquierda()` o `moverDerecha()`. El ancho del mapa también debe mantenerse protegido porque se usa para controlar los límites del movimiento. Si estos atributos fueran públicos, cualquier parte del programa podría cambiar el puntaje, mover la nave de forma incorrecta o alterar sus límites. Por eso, `private` permite que la nave conserve control sobre sus propios datos.

**Public** se usa en los métodos que el resto del programa necesita utilizar para comunicarse con la nave. Por ejemplo, `Juego` necesita llamar a `moverIzquierda()` y `moverDerecha()` cuando el jugador elige una opción. También necesita consultar el puntaje mediante `getPuntaje()` para mostrarlo en pantalla y guardar el progreso de la partida. Estos métodos públicos funcionan como una interfaz segura entre la nave y el resto del programa. Gracias a esto, otras clases pueden usar la nave sin modificar directamente sus atributos privados. De esta forma, el programa mantiene encapsulamiento sin impedir la interacción necesaria para jugar.

**Override** se usa en `actuar()` y `getSimbolo()` porque ambos métodos vienen de la clase abstracta `Entidad`. La nave está obligada a implementar su propia forma de actuar y su propia representación visual. En este caso, `actuar()` aplica el movimiento pendiente del jugador, mientras que `getSimbolo()` devuelve `'^'`. Esto confirma que la clase está usando correctamente polimorfismo. Además, `override` ayuda a detectar errores si el método no coincide exactamente con el definido en la clase base. Por eso, su uso hace que el código sea más seguro y más claro al trabajar con herencia.

**Const** se usa en `getSimbolo()` y `getPuntaje()` porque estos métodos solo consultan información. `getSimbolo()` devuelve el carácter que representa a la nave, pero no modifica su posición ni su vida. `getPuntaje()` devuelve el puntaje actual sin alterarlo. Esto hace que el código sea más seguro, porque separa claramente los métodos de consulta de los métodos que cambian el estado del objeto. Además, permite que el programa lea datos de la nave sin riesgo de modificarla accidentalmente. En general, `const` mejora la confiabilidad del código y hace más clara la intención de cada método. 

### 4.3 Clase `Enemigo`

La clase `Enemigo` representa a los rivales que descienden por el mapa e intentan alcanzar la posición de la nave. Esta clase hereda de `Entidad`, por lo que conserva atributos generales como `x`, `y` y `vida`, pero agrega un atributo propio llamado `velocidad`.

Esta clase es importante porque permite que el juego tenga una amenaza activa. Mientras la nave responde a las decisiones del jugador, el enemigo tiene un comportamiento automático: avanzar hacia abajo en cada turno. Gracias a esto, el jugador debe moverse, disparar y evitar que los enemigos lleguen hasta su fila.

#### Declaración general de la clase

```cpp
class Enemigo : public Entidad {
private:
    int velocidad;

public:
    Enemigo(int x, int y, int vida, int velocidad);
    void actuar() override;
    char getSimbolo() const override;
    int getVelocidad() const;
};
```

**Private** se usa en `velocidad` porque este atributo pertenece al comportamiento interno del enemigo. La velocidad determina cuántas filas baja el enemigo durante su turno, por lo que no debería modificarse libremente desde cualquier parte del programa. Si fuera pública, otra clase podría cambiarla sin control y provocar enemigos demasiado rápidos, inmóviles o con comportamiento incoherente. Al mantenerla privada, el enemigo conserva control sobre su propia forma de desplazarse. Esto también ayuda a que cada enemigo tenga una velocidad definida desde su creación. Por eso, `private` permite proteger una característica clave del enemigo sin impedir que el programa pueda usarlo correctamente.

**Public** se usa en los métodos que el resto del programa necesita utilizar para interactuar con el enemigo. El constructor permite crear enemigos indicando posición inicial, vida y velocidad. El método `actuar()` debe poder ser llamado por la clase `Juego` cuando avanza cada turno. También se usa `getVelocidad()` para consultar la velocidad sin modificarla directamente. Estos métodos públicos funcionan como la interfaz del enemigo frente al resto del programa. Gracias a esto, `Juego` puede mover enemigos, revisar su estado y procesar colisiones sin acceder directamente a sus atributos privados. Así se mantiene el encapsulamiento sin bloquear la lógica necesaria para la partida.

**Override** se usa en `actuar()` y `getSimbolo()` porque ambos métodos provienen de la clase abstracta `Entidad`. El enemigo debe implementar su propia versión de `actuar()`, ya que no se mueve igual que la nave. En este caso, `actuar()` aumenta la posición `y` según la velocidad, haciendo que el enemigo baje por el mapa. `getSimbolo()` devuelve `'V'`, que es la representación visual del enemigo en la consola. El uso de `override` confirma que estos métodos están sobrescribiendo correctamente los métodos definidos en la clase base. Además, ayuda a detectar errores de escritura o de parámetros si el método no coincide con el original. Esto hace que la herencia y el polimorfismo sean más seguros.

**Const** se usa en `getSimbolo()` y `getVelocidad()` porque ambos métodos solo consultan información del enemigo. `getSimbolo()` devuelve el carácter que representa al enemigo, pero no modifica su posición, vida ni velocidad. `getVelocidad()` permite conocer la velocidad del enemigo sin alterarla. Esto es importante porque otras partes del programa pueden necesitar leer datos del enemigo sin cambiar su estado interno. El uso de `const` separa claramente los métodos de consulta de los métodos que modifican el objeto. También mejora la seguridad del código porque evita cambios accidentales. En general, `const` hace que la intención de estos métodos sea más clara y confiable. 

### 4.4 Clase `Bala`

La clase `Bala` representa los disparos realizados por la nave del jugador. A diferencia de `Nave` y `Enemigo`, esta clase no hereda de `Entidad`, porque su comportamiento es más simple y no necesita formar parte de la jerarquía polimórfica principal.

Esta clase es importante porque permite que el jugador interactúe ofensivamente con los enemigos. Cada vez que el jugador elige disparar, el juego crea una bala en la posición de la nave. Luego, en cada turno, la bala avanza hacia arriba hasta impactar contra un enemigo o salir del mapa.

#### Declaración general de la clase

```cpp
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
```

**Private** se usa en `x`, `y` y `activa`, porque estos atributos forman parte del estado interno de la bala. La posición de la bala no debería modificarse directamente desde cualquier parte del programa, ya que eso podría romper la lógica del disparo. El atributo `activa` también debe protegerse, porque determina si la bala sigue participando en la partida. Si estos datos fueran públicos, cualquier clase podría cambiar la posición o activar/desactivar la bala sin control. Al mantenerlos privados, la bala controla su propio estado mediante métodos específicos. Esto permite que el programa maneje los disparos de forma más segura y ordenada.

**Public** se usa en los métodos que el resto del programa necesita para interactuar con la bala. El constructor permite crear una bala en una posición inicial concreta. El método `mover()` permite actualizar su posición durante cada turno. Los métodos `getX()` y `getY()` permiten consultar su ubicación para dibujar el mapa o revisar colisiones. `estaActiva()` permite saber si la bala debe seguir existiendo dentro del juego. `desactivar()` permite retirarla cuando impacta o sale del mapa. Estos métodos públicos permiten usar la bala sin acceder directamente a sus atributos privados.

**Const** se usa en `getX()`, `getY()` y `estaActiva()` porque estos métodos solo consultan información. `getX()` y `getY()` devuelven la posición actual de la bala, pero no la modifican. `estaActiva()` informa si la bala sigue en juego, pero tampoco cambia su estado. Esto permite que otras funciones, como las de colisión o dibujo del mapa, puedan leer datos de la bala sin alterarla accidentalmente. El uso de `const` separa claramente las funciones de consulta de las funciones que modifican el objeto. También mejora la seguridad del código porque evita cambios inesperados. En general, hace que la intención de estos métodos sea más clara.

## 5. Cumplimiento de la rúbrica

### 5.1 Encapsulamiento

El proyecto cumple el criterio de encapsulamiento porque cada clase mantiene control sobre sus propios datos y evita que el resto del programa modifique directamente información sensible. En lugar de trabajar con atributos públicos, el diseño utiliza `private` y `protected` para proteger datos como vida, posición, puntaje, velocidad, dirección de movimiento y estado de las balas.

Este criterio se cumple en un nivel alto porque el encapsulamiento no se limita a ocultar variables, sino que también incluye métodos que controlan cómo se modifican esos datos. Por ejemplo, la vida de una entidad no se reduce de forma directa desde fuera de la clase, sino mediante un método que valida que el valor no termine siendo negativo. De igual forma, el puntaje de la nave se incrementa mediante un método que solo acepta valores positivos.

La clase `Juego` puede consultar la información necesaria para mostrar el mapa, revisar colisiones y actualizar la partida, pero no necesita manipular directamente los atributos internos de cada objeto. Esto demuestra que existe una separación adecuada entre el estado interno de las clases y la lógica general del programa.

Además, el uso de métodos `const` en funciones de consulta refuerza la seguridad del diseño, porque deja claro qué métodos solo leen información y cuáles modifican el estado del objeto. Esto mejora la claridad del código y reduce el riesgo de cambios accidentales.

En conclusión, el proyecto cumple el nivel más alto del criterio porque usa correctamente especificadores de acceso, métodos de consulta, métodos de modificación controlada y validaciones reales. Cada clase administra su propio estado, lo que hace que el programa sea más ordenado, seguro y fácil de mantener.

### 5.2 Herencia y polimorfismo

El proyecto cumple el criterio de herencia y polimorfismo porque organiza sus objetos principales mediante una jerarquía de clases. La clase `Entidad` funciona como base común para los elementos principales del juego, mientras que `Nave` y `Enemigo` extienden esa base para implementar comportamientos propios.

Este criterio se cumple en un nivel alto porque la herencia no se usa solo como una relación superficial entre clases, sino como parte activa de la lógica del juego. La clase base define una estructura común, pero las clases derivadas determinan cómo actúan y cómo se representan dentro del mapa. Esto permite que objetos diferentes compartan una misma interfaz sin comportarse de la misma manera.

El polimorfismo aparece cuando la clase `Juego` almacena y maneja objetos mediante punteros a la clase base `Entidad`. Gracias a esto, el programa puede recorrer un conjunto de entidades y llamar a sus métodos sin conocer necesariamente el tipo específico de cada objeto. La nave y los enemigos responden de forma distinta, pero pueden ser administrados desde una misma estructura.

La presencia de métodos virtuales puros convierte a `Entidad` en una clase abstracta, lo cual fortalece el diseño. Esto obliga a las clases hijas a definir su propio comportamiento y evita crear objetos genéricos sin sentido dentro del dominio del juego. Además, el uso de `override` hace explícita la sobrescritura de métodos y ayuda a prevenir errores.

También se incluye un destructor virtual, lo que es importante porque el programa usa memoria dinámica y punteros a clase base. Esto permite liberar correctamente objetos derivados cuando se eliminan desde un puntero de tipo `Entidad*`.

En conclusión, el proyecto cumple el nivel más alto de este criterio porque incluye clase abstracta, métodos virtuales puros, sobrescritura, uso real de punteros polimórficos y destructor virtual. La herencia y el polimorfismo no son elementos decorativos, sino que participan directamente en el movimiento, representación y administración de las entidades del juego. 

### 5.3 Memoria dinámica

El proyecto cumple el criterio de memoria dinámica porque utiliza creación y liberación explícita de objetos durante la ejecución del programa. En lugar de trabajar únicamente con variables fijas, el juego maneja arreglos dinámicos para almacenar entidades y balas, lo que permite que la cantidad de objetos pueda cambiar mientras avanza la partida.

Este criterio se cumple en un nivel alto porque el uso de memoria dinámica tiene una función real dentro del juego. Los enemigos y las balas no son elementos completamente fijos, sino que pueden aparecer durante la ejecución. Por eso, el programa necesita reservar memoria cuando se agregan nuevos objetos y liberar esa memoria cuando la partida termina.

La clase `Juego` administra un arreglo dinámico de punteros a `Entidad`, donde se almacenan objetos como la nave y los enemigos. También administra un arreglo dinámico de punteros a `Bala`, donde se guardan los disparos realizados por el jugador. Esto demuestra que la memoria dinámica no se usa de forma aislada, sino como parte central de la lógica del juego.

Además, el programa incluye funciones para aumentar la capacidad de estos arreglos cuando se llenan. Esto permite que el juego pueda seguir creando entidades o balas sin depender de un tamaño completamente fijo desde el inicio. Este proceso demuestra un manejo más avanzado de memoria, porque no solo se usa `new`, sino que también se reorganiza el almacenamiento cuando es necesario.

El proyecto también cumple con la liberación de memoria mediante `delete` y `delete[]`. En el destructor de la clase `Juego`, se recorren los arreglos, se elimina cada objeto creado dinámicamente y luego se liberan los arreglos. Esto es importante porque evita fugas de memoria y muestra una administración responsable de los recursos.

En conclusión, el proyecto cumple el nivel más alto de este criterio porque usa `new`, `delete`, `delete[]`, arreglos dinámicos de tamaño variable, punteros a objetos y liberación explícita de memoria. La memoria dinámica no aparece como un requisito forzado, sino como una necesidad real para administrar enemigos, balas y entidades durante la partida.

### 5.4 Persistencia en archivo

El proyecto cumple el criterio de persistencia en archivo porque permite guardar y cargar información de la partida mediante un archivo de texto. Esto hace que los datos no se pierdan inmediatamente al cerrar el programa, sino que puedan mantenerse entre diferentes ejecuciones.

Este criterio se cumple en un nivel alto porque la persistencia tiene una utilidad clara dentro del juego. El jugador puede guardar el estado de la partida y luego volver a cargarlo. Para esto, el programa utiliza un archivo `.txt`, donde se almacenan datos importantes como el turno actual, la vida de la nave y el puntaje acumulado.

La clase `Juego` incluye métodos específicos para manejar esta funcionalidad. El método de guardado escribe los datos principales de la partida en el archivo, mientras que el método de carga intenta leerlos al iniciar el programa. Esto demuestra que la persistencia está integrada dentro de la estructura del proyecto y no aparece como una función separada sin relación con el juego.

También se considera el caso en el que el archivo no exista. Si el programa no encuentra el archivo de partida, no se detiene ni produce un error grave, sino que continúa con una partida nueva. Esto es importante porque demuestra que el sistema de archivos fue pensado de manera segura y práctica.

El uso de un archivo de texto también facilita revisar manualmente los datos guardados. Al ser un `.txt`, el contenido puede abrirse y leerse fácilmente, lo que ayuda a comprobar que la información realmente se está almacenando. Esto permite evidenciar que los datos persisten fuera de la memoria temporal del programa.

En conclusión, el proyecto cumple el nivel más alto de este criterio porque guarda datos relevantes en un archivo, puede cargarlos en una ejecución posterior, maneja el caso de archivo inexistente y usa la persistencia como parte funcional del juego. La partida no depende únicamente de la memoria temporal, sino que puede conservar información entre ejecuciones.

### 5.5 Sobrecarga

El proyecto cumple el criterio de sobrecarga porque incluye dos versiones de una función con el mismo nombre, pero con parámetros diferentes. En este caso, la función `hayColision()` se encuentra sobrecargada para revisar distintos tipos de colisiones dentro del juego.

Este criterio se cumple en un nivel alto porque la sobrecarga tiene una función observable y justificada dentro del programa. No se usa solamente para cumplir el requisito de forma artificial, sino porque el juego necesita detectar más de un tipo de choque. Una colisión entre una bala y un enemigo no representa lo mismo que una colisión entre un enemigo y la nave.

La primera versión de `hayColision()` se usa para detectar si una bala impacta contra un enemigo. Esta colisión permite eliminar o dañar al enemigo, desactivar la bala y sumar puntos al jugador. Por lo tanto, está relacionada con el sistema ofensivo del juego.

La segunda versión de `hayColision()` se usa para detectar si un enemigo alcanza a la nave. Esta colisión afecta directamente al jugador, porque puede reducir la vida de la nave y acercar la partida a su final. Aunque ambas funciones revisan colisiones, cada una representa una situación diferente dentro de la lógica del juego.

La sobrecarga es útil porque mantiene un mismo nombre para una acción general: revisar colisiones. Sin embargo, permite que el comportamiento se adapte según los objetos recibidos como parámetros. Esto hace que el código sea más claro, porque el nombre de la función conserva el mismo significado general, pero se especializa según el caso.

Además, esta implementación demuestra que la sobrecarga tiene efecto real en la ejecución. El programa llama a la versión correcta de `hayColision()` dependiendo de si está comparando una bala con un enemigo o un enemigo con la nave. Esto ayuda a mantener la lógica de colisiones organizada y fácil de entender.

En conclusión, el proyecto cumple el nivel más alto de este criterio porque utiliza sobrecarga con funciones diferenciadas, parámetros distintos y comportamiento observable dentro del juego. La sobrecarga está justificada por el dominio del proyecto, ya que existen distintos tipos de colisiones que deben ser evaluadas de manera específica.

## 6. Compilación y ejecución

Para compilar el proyecto desde una terminal, todos los archivos `.cpp` deben estar en la misma carpeta junto con sus respectivos archivos `.h`.

```bash
g++ main.cpp Bala.cpp Colisiones.cpp Enemigo.cpp Entidad.cpp Juego.cpp Nave.cpp -o SpaceShooter
```

Para ejecutar en Windows:

```bash
SpaceShooter.exe
```

Para ejecutar en Linux o macOS:

```bash
./SpaceShooter
```

También se puede compilar desde Dev-C++ abriendo el proyecto, verificando que los archivos `.cpp` estén agregados correctamente y ejecutando la opción de compilar y correr.

El archivo `partida.txt` debe estar en la misma carpeta del ejecutable para que el sistema de guardado y carga funcione correctamente.
