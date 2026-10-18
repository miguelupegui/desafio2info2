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
              << "0. Salir\n";
}

void Menu::mostrarMensajes() const {
    std::cout << "\n--- Mensajes ---\n"
              << "1. Enviar mensaje\n"
              << "2. Leer nuevos mensajes\n"
              << "3. Buscar mensajes por rango de fecha\n"
              << "0. Volver\n";
}