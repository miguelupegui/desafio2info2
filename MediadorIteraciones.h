#ifndef MEDIADORITERACIONES_H
#define MEDIADORITERACIONES_H

// Mide el consumo de recursos de UNA funcionalidad (enunciado, punto VII).
// Uso por cada funcionalidad:
//   1. reiniciar() al empezar.
//   2. Las funciones con bucles reciben 'long& iter' y lo incrementan por vuelta.
//   3. Al final (dentro de la funcionalidad, para que sus locales sigan vivas):
//        sumarIteraciones(iter); sumarMemoria(memoria total + locales); mostrarConsumo();
// Criterios (deben ir tambien en el informe):
//   - Iteracion = una vuelta de un for/while, directa o indirecta. Se cuentan tambien
//     las vueltas de liberacion de memoria; no se cuentan los destructores al cerrar.
//   - Memoria = sizeof de objetos + capacidad reservada de arreglos dinamicos
//     + locales y parametros por valor (sizeof) + sizeof(void*) por cada referencia.
//   - No incluye la contabilidad interna del gestor de memoria de C++.
// No tiene memoria dinamica: no necesita destructor propio.
class MediadorIteraciones {
private:
    long iteraciones;
    long memoriaBytes;
    long usosTime;      // llamadas a time()/localtime (componente externo)
    long usosRand;      // llamadas a rand() (componente externo)

public:
    MediadorIteraciones();
    MediadorIteraciones(const MediadorIteraciones& otro);

    // Getters
    long getIteraciones() const { return iteraciones; }
    long getMemoria() const { return memoriaBytes; }
    long getUsosTime() const { return usosTime; }
    long getUsosRand() const { return usosRand; }

    // Setters
    void setIteraciones(long n) { iteraciones = n; }
    void setMemoria(long bytes) { memoriaBytes = bytes; }
    void setUsosTime(long n) { usosTime = n; }
    void setUsosRand(long n) { usosRand = n; }

    void sumarIteraciones(long n) { iteraciones += n; }
    void sumarMemoria(long bytes) { memoriaBytes += bytes; }
    void registrarTime(long veces = 1) { usosTime += veces; }
    void registrarRand(long veces = 1) { usosRand += veces; }

    // Deja todos los contadores en cero (al empezar cada funcionalidad)
    void reiniciar();

    // Operacion de despliegue: muestra iteraciones, memoria y usos externos
    void mostrarConsumo() const;
};

#endif
