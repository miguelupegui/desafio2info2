#ifndef ALBUM_H
#define ALBUM_H

#include <string>
#include "Foto.h"


class Album {
private:
    std::string nombre;
    std::string ruta;
    char privacidad;
    Foto** fotos;          // arreglo dinamico de punteros a Foto
    short numFotos;        // usadas
    short capFotos;        // capacidad

public:
    Album();
    Album(const std::string& n, const std::string& r, char p);
    Album(const Album& otro);
    Album& operator=(const Album&) = delete;
    ~Album();

    // Getters (const)
    const std::string& getNombre() const { return nombre; }
    const std::string& getRuta() const { return ruta; }
    char getPrivacidad() const { return privacidad; }
    short getNumFotos() const { return numFotos; }
    short getCapFotos() const { return capFotos; }
    Foto* getFoto(int i) const;

    // Setters
    void setNombre(const std::string& n) { nombre = n; }
    void setRuta(const std::string& r) { ruta = r; }
    void setPrivacidad(char p) { privacidad = p; }

    bool agregarFoto(Foto* f);

    // Carrusel circular: 1 siguiente, 2 previa, 3 dar/quitar like, 4 salir.
    void recorrerCarrusel(long& iter);

    // Bytes de locales y parametros de recorrerCarrusel (para la metrica)
    long memoriaCarrusel() const;

    // sizeof(Album) + textos + capacidad del arreglo + memoria de cada foto
    long memoria() const;
};

#endif
