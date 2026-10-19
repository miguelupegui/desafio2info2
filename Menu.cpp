#include "Menu.h"
#include <iostream>

bool Menu::leerClave(std::string& clave, long& iter) const {
    std::cout << "Contrasena: ";
    return leerLinea(clave, 256, iter);
}

int Menu::leerOpcion(long& iter) const {
    int op;
    std::cout << "Opcion: ";
    if (!(std::cin >> op)) {
        if (std::cin.eof()) return -2;
        std::cin.clear();
        descartarLinea(iter);
        return -1;
    }
    descartarLinea(iter);
    return op;
}

void Menu::mostrarPrincipal() const {
    std::cout << "\n===== Ude@Book =====\n"
              << "1. Administrar amistades\n"
              << "2. Visitar perfil de un amigo\n"
              << "3. Ver sugerencias de amistad\n"
              << "4. Administrar mensajes\n"
              << "5. Actualizar fotos de amigos (solo una vez)\n"
              << "0. Salir\n";
}

void Menu::mostrarMensajes() const {
    std::cout << "\n--- Mensajes ---\n"
              << "1. Enviar mensaje\n"
              << "2. Leer nuevos mensajes\n"
              << "3. Buscar mensajes por rango de fecha\n"
              << "0. Volver\n";
}

void Menu::mostrarAmistades() const {
    std::cout << "\n--- Amistades ---\n"
              << "1. Enviar solicitud de amistad\n"
              << "2. Aceptar solicitudes recibidas\n"
              << "0. Volver\n";
}

int Menu::leerNumero(const char* pregunta, long& iter) const {
    int n;
    std::cout << pregunta;
    if (!(std::cin >> n)) {
        if (std::cin.eof()) return -2;
        std::cin.clear();
        descartarLinea(iter);
        return -1;
    }
    descartarLinea(iter);
    return n;
}

void Menu::mostrarFotoAmigo(int pos, const std::string& nombre, const std::string& ruta) const {
    if (pos == 1) std::cout << "\n--- Fotos de perfil de tus amigos ---\n";
    std::cout << pos << ". " << nombre << ": " << ruta << '\n';
}

void Menu::mostrarSolicitud(int pos, const std::string& nombre) const {
    if (pos == 1) std::cout << "\nSolicitudes recibidas:\n";
    std::cout << pos << ". " << nombre << '\n';
}

void Menu::mostrarSugerencia(int pos, const std::string& nombre, int comunes) const {
    if (pos == 1) std::cout << "\nSugerencias de amistad:\n";
    std::cout << pos << ". " << nombre << " (" << comunes << " amigos en comun)\n";
}

// el numero es lo que devuelve Plataforma::enviarSolicitud
void Menu::avisarResultadoSolicitud(int codigo) const {
    if (codigo == 0) avisar("Solicitud enviada.");
    else if (codigo == 1) avisar("Ese usuario no existe.");
    else if (codigo == 2) avisar("No puedes enviarte una solicitud a ti mismo.");
    else if (codigo == 3) avisar("Ya son amigos.");
    else avisar("Ya hay una solicitud pendiente con ese usuario.");
}