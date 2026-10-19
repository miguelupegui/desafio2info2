#include "Album.h"
#include <iostream>

Album::Album()
    : nombre(""), ruta(""), privacidad('P'), fotos(nullptr), numFotos(0), capFotos(0) {}

Album::Album(const std::string& n, const std::string& r, char p)
    : nombre(n), ruta(r), privacidad(p), fotos(nullptr), numFotos(0), capFotos(0) {}

Album::Album(const Album& otro)
    : nombre(otro.nombre), ruta(otro.ruta), privacidad(otro.privacidad),
      fotos(nullptr), numFotos(otro.numFotos), capFotos(otro.numFotos) {
    if (numFotos > 0) {
        fotos = new Foto*[capFotos];
        for (int i = 0; i < numFotos; i++) fotos[i] = new Foto(*otro.fotos[i]);
    }
}

Album::~Album() {
    for (int i = 0; i < numFotos; i++) delete fotos[i];
    delete[] fotos;
}

Foto* Album::getFoto(int i) const {
    if (i < 0 || i >= numFotos) return nullptr;
    return fotos[i];
}

bool Album::agregarFoto(Foto* f) {
    if (f == nullptr) return false;
    if (numFotos == capFotos) {
        int nueva = (capFotos == 0) ? 4 : capFotos * 2;
        Foto** tmp = new Foto*[nueva];
        for (int i = 0; i < numFotos; i++) tmp[i] = fotos[i];
        delete[] fotos;
        fotos = tmp;
        capFotos = (short)nueva;
    }
    fotos[numFotos++] = f;
    return true;
}


void Album::recorrerCarrusel(long& iter) {
    if (numFotos == 0) {
        std::cout << "Este album no tiene fotos.\n";
        return;
    }

    bool* likeDado = new bool[numFotos];
    for (int i = 0; i < numFotos; i++) { iter++; likeDado[i] = false; }

    int actual = 0;
    bool salir = false;
    while (!salir) {
        iter++;
        std::cout << "\nFoto " << (actual + 1) << " de " << numFotos
                  << " - album " << nombre << '\n';
        fotos[actual]->mostrar();
        std::cout << "1. Siguiente  2. Previa  3. Dar/quitar like  4. Salir del album\n"
                  << "Opcion: ";

        char op, c;
        if (!std::cin.get(op)) { salir = true; continue; }
        if (op != '\n') {
            while (std::cin.get(c) && c != '\n') iter++;
        }

        if (op == '1') {
            actual = (actual + 1) % numFotos;
        } else if (op == '2') {
            actual = (actual - 1 + numFotos) % numFotos; //el modulo de un negativo es negativo
        } else if (op == '3') {
            if (!likeDado[actual]) { fotos[actual]->darLike(); likeDado[actual] = true; }
            else { fotos[actual]->quitarLike(); likeDado[actual] = false; }
        } else if (op == '4') {
            salir = true;
        } else {
            std::cout << "Opcion invalida.\n";
        }
    }
    delete[] likeDado;
}

long Album::memoriaCarrusel() const {
    return (long)(numFotos * sizeof(bool) + sizeof(bool*) + sizeof(int) + sizeof(bool)
                  + 2 * sizeof(char) + sizeof(void*));
}

long Album::memoria() const {
    long total = (long)(sizeof(Album) + nombre.length() + ruta.length());
    total += (long)(capFotos * sizeof(Foto*));      // arreglo de punteros
    for (int i = 0; i < numFotos; i++)
        total += fotos[i]->memoria();               // Album es dueño de fotos
    return total;
}
