#include "Foto.h"
#include <iostream>

Foto::Foto() : ruta(""), fecha(), tamano(0), likes(0) {}

Foto::Foto(const std::string& r, const Fecha& f, long t, int l)
    : ruta(r), fecha(f), tamano(t), likes(l) {}

Foto::Foto(const Foto& otra)
    : ruta(otra.ruta), fecha(otra.fecha), tamano(otra.tamano), likes(otra.likes) {}

// Visualizar una foto
void Foto::mostrar() const {
    std::cout << "Foto: " << ruta << "\n  Fecha: ";
    fecha.mostrar();
    std::cout << "  Tamano: " << tamano << " bytes  Likes: " << likes << '\n';
}

long Foto::memoria() const {
    return (long)(sizeof(Foto) + ruta.length());
}
