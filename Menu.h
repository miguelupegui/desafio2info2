#ifndef MENU_H
#define MENU_H

#include <string>
#include "Fecha.h"

// Toda la lectura y escritura por consola. No guarda datos.
class Menu {
private:
    void descartarLinea(long& iter) const;
    bool leerLinea(std::string& destino, int tam, long& iter) const;

public:
    bool leerTextoMensaje(std::string& texto, long& iter) const;
    bool leerNombre(std::string& nombre, long& iter) const;
    bool leerFecha(const char* pregunta, Fecha& f, long& iter) const;
    void avisar(const char* texto) const;

    bool leerClave(std::string& clave, long& iter) const;
    int leerOpcion(long& iter) const;
    void mostrarPrincipal() const;
    void mostrarMensajes() const;
    void mostrarAmistades() const;

    int leerNumero(const char* pregunta, long& iter) const;
    void mostrarFotoAmigo(int pos, const std::string& nombre, const std::string& ruta) const;
    void mostrarSolicitud(int pos, const std::string& nombre) const;
    void mostrarSugerencia(int pos, const std::string& nombre, int comunes) const;
    void avisarResultadoSolicitud(int codigo) const;
};

#endif