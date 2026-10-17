#include "Mensaje.h"
#include "Usuario.h"
#include <iostream>

Mensaje::Mensaje()
    : texto(""), emisor(nullptr), receptor(nullptr), leido(false),
      fecha(), hora(0), minuto(0) {}

Mensaje::Mensaje(const std::string& t, Usuario* e, Usuario* r)
    : texto(t), emisor(e), receptor(r), leido(false),
      fecha(), hora(0), minuto(0) {
    int h = 0, m = 0;
    if (fecha.actualizarAHoy(h, m)) {
        hora = (short)h;
        minuto = (short)m;
    }
}

Mensaje::Mensaje(const std::string& t, Usuario* e, Usuario* r,
                 const Fecha& f, int h, int m, bool yaLeido)
    : texto(t), emisor(e), receptor(r), leido(yaLeido),
      fecha(f), hora((short)h), minuto((short)m) {}

Mensaje::Mensaje(const Mensaje& otro)
    : texto(otro.texto), emisor(otro.emisor), receptor(otro.receptor),
      leido(otro.leido), fecha(otro.fecha), hora(otro.hora), minuto(otro.minuto) {}

void Mensaje::mostrar() const {
//veo hand aqui tienes que poner el nombre de la clase ya que getnombreusuario es provisional
    std::cout << "De: " << emisor->getNombreUsuario() << "  [";
    fecha.mostrar();
    std::cout << ' ' << (hora / 10) << (hora % 10) << ':'
              << (minuto / 10) << (minuto % 10) << "]\n"
              << texto << '\n';
}

bool Mensaje::operator<(const Mensaje& otro) const {
    if (!(fecha == otro.fecha)) return fecha < otro.fecha;
    if (hora != otro.hora) return hora < otro.hora;
    return minuto < otro.minuto;
}
