#ifndef FOTO_H
#define FOTO_H

#include <string>
#include "Fecha.h"

class Foto {
private:
    std::string ruta;// ruta absoluta al archivo
    Fecha fecha;// fecha de captura
    long tamano;// capacidad
    int likes;

public:
    Foto();
    Foto(const std::string& r, const Fecha& f, long t, int l);
    Foto(const Foto& otra);

    // Getters (const)
    const std::string& getRuta() const { return ruta; }
    const Fecha& getFecha() const { return fecha; }
    long getTamano() const { return tamano; }
    int getLikes() const { return likes; }

    // Setters
    void setRuta(const std::string& r) { ruta = r; }
    void setFecha(const Fecha& f) { fecha = f; }
    void setTamano(long t) { tamano = t; }
    void setLikes(int l) { likes = l; }

    // El like se alterna por sesion en Albu
    void darLike() { likes++; }
    void quitarLike() { if (likes > 0) likes--; }
    void mostrar() const;

    long memoria() const;
};

#endif
