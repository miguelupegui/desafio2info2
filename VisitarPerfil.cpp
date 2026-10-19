#include <iostream>
#include "Plataforma.h"
#include "Usuario.h"
#include "Album.h"
#include "Foto.h"
bool Usuario::visitarPerfil(Usuario& amigo, long& iter, long& memLocal) const {
    int total = amigo.getNumAlbumes();
    short* visibles = new short[total];          // posicion REAL de cada album visible
    int numVisibles = 0;

    for (int i = 0; i < total; i++) {
        iter++;
        char p = amigo.getAlbum(i)->getPrivacidad();
        if (p == 'P' || p == 'A') visibles[numVisibles++] = (short)i;
    }

    long maxCarrusel = 0;
    bool resultado = (numVisibles > 0);

    if (!resultado) {
        std::cout << amigo.getNombreUsuario() << " no tiene albumes visibles.\n";
    } else {
        std::cout << "Albumes de " << amigo.getNombreUsuario() << ":\n";
        for (int k = 0; k < numVisibles; k++) {
            iter++;
            Album* a = amigo.getAlbum(visibles[k]);
            std::cout << (k + 1) << ". " << a->getNombre()
                      << " (" << a->getNumFotos() << " fotos)\n";
        }

        bool salir = false;
        while (!salir) {
            iter++;
            int eleccion = 0;
            std::cout << "Elige un album (1-" << numVisibles << ", 0 para salir): ";
            if (!(std::cin >> eleccion)) {
                if (std::cin.eof()) salir = true;
                else {                                        // texto en vez de numero
                    std::cin.clear();
                    char c;
                    while (std::cin.get(c) && c != '\n') iter++;
                    std::cout << "Opcion invalida.\n";
                }
                continue;
            }
            char c;
            while (std::cin.get(c) && c != '\n') iter++;       // descarta el resto de la linea

            if (eleccion == 0) {
                salir = true;
            } else if (eleccion >= 1 && eleccion <= numVisibles) {
                Album* a = amigo.getAlbum(visibles[eleccion - 1]);
                long m = a->memoriaCarrusel();
                if (m > maxCarrusel) maxCarrusel = m;          // los carruseles no son simultaneos
                a->recorrerCarrusel(iter);
            } else {
                std::cout << "Opcion invalida.\n";
            }
        }
    }

    memLocal += (long)(total * sizeof(short) + sizeof(short*) + 3 * sizeof(int)
                        + 2 * sizeof(bool) + 2 * sizeof(char) + sizeof(Album*)
                        + 3 * sizeof(void*))                    // referencias: amigo, iter, memLocal
                + maxCarrusel;
    delete[] visibles;
    return resultado;
}
void Plataforma::funcionVisitarPerfil() {
    mediador.reiniciar();
    long iter = 0, memLocal = 0;
    std::string nombre;
    Usuario* amigo = nullptr;

    if (!menu.leerNombre(nombre, iter)) {
        menu.avisar("No se pudo leer el nombre.");
    } else {
        amigo = buscarUsuario(nombre, iter);
        if (amigo == nullptr) menu.avisar("Ese usuario no existe.");
        else if (!matriz->sonAmigos(usuarioActivo->getIndice(), amigo->getIndice()))
            menu.avisar("Solo puedes visitar el perfil de tus amigos.");
        else usuarioActivo->visitarPerfil(*amigo, iter, memLocal);
    }

    mediador.sumarIteraciones(iter);
    mediador.sumarMemoria(memoriaTotal()
                          + (long)(sizeof(iter) + sizeof(memLocal) + sizeof(nombre) + nombre.length()
                                    + sizeof(amigo))
                          + 256
                          + memLocal);
    mediador.mostrarConsumo();
}