#ifndef PLATAFORMA_H
#define PLATAFORMA_H

#include <string>
#include "GestorArchivos.h"
#include "MediadorIteraciones.h"
#include "Menu.h"
#include "Usuario.h"
#include "MatrizAdyacencia.h"

// Esto carga los datos, autentica al usuario y reparte el trabajo del menu.
// Es dueña de los usuarios y de la matriz de amistades.
class Plataforma {
private:
    GestorArchivos gestor;
    Menu menu;
    MediadorIteraciones mediador;
    Usuario** usuarios;
    int numUsuarios;
    MatrizAdyacencia* matriz;
    Usuario* usuarioActivo;

    bool iniciarSesion(long& iter, long& memLocal);
    void menuPrincipal();
    void menuMensajes();
    void guardarCambios();

    Usuario* buscarUsuario(const std::string& nombre, long& iter) const;
    void funcionEnviarMensaje();
    void funcionLeerNuevos();
    void funcionBuscarPorRango();
    void funcionVisitarPerfil();

public:
    Plataforma();
    Plataforma(const Plataforma&) = delete;// es dueña de memoria dinamica
    Plataforma& operator=(const Plataforma&) = delete;
    ~Plataforma();

    void ejecutar();
    long memoriaTotal() const;
};

#endif