#include "MediadorIteraciones.h"
#include <iostream>

MediadorIteraciones::MediadorIteraciones()
    : iteraciones(0), memoriaBytes(0), usosTime(0), usosRand(0) {}

MediadorIteraciones::MediadorIteraciones(const MediadorIteraciones& otro)
    : iteraciones(otro.iteraciones), memoriaBytes(otro.memoriaBytes),
      usosTime(otro.usosTime), usosRand(otro.usosRand) {}

void MediadorIteraciones::reiniciar() {
    iteraciones = 0;
    memoriaBytes = 0;
    usosTime = 0;
    usosRand = 0;
}

void MediadorIteraciones::mostrarConsumo() const {
    std::cout << "\n--- Consumo de recursos ---\n"
              << "Iteraciones: " << iteraciones << '\n'
              << "Memoria: " << memoriaBytes << " bytes ("
              << memoriaBytes / 1024 << " KB)\n";
    if (usosTime > 0)
        std::cout << "Componente externo: time() invocado " << usosTime << " vez/veces\n";
    if (usosRand > 0)
        std::cout << "Componente externo: rand() invocado " << usosRand << " vez/veces\n";
    std::cout << "---------------------------\n";
}
