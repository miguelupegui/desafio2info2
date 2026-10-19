#include "Plataforma.h"
#include <cstdlib>
#include <ctime>

Plataforma::Plataforma()
    : usuarios(nullptr), numUsuarios(0), matriz(nullptr), usuarioActivo(nullptr),
    numFotosAmigos(0), fotosActualizadas(false) {}

Plataforma::~Plataforma() {
    for (int i = 0; i < numUsuarios; i++) delete usuarios[i];
    delete[] usuarios;
    delete matriz;
}

void Plataforma::ejecutar() {
    mediador.reiniciar();
    long iter = 0;
    if (!gestor.cargarDatos(usuarios, numUsuarios, matriz, iter)) {
        menu.avisar("No se pudieron cargar los datos.");
        return;
    }
    mediador.sumarIteraciones(iter);
    mediador.sumarMemoria(memoriaTotal() + (long)sizeof(iter));
    mediador.mostrarConsumo();

    mediador.reiniciar();
    iter = 0;
    long memLocal = 0;
    std::srand((unsigned)std::time(nullptr));      // para que las fotos al azar cambien en cada ejecucion
    mediador.registrarTime();
    if (!iniciarSesion(iter, memLocal)) {
        menu.avisar("No fue posible iniciar sesion.");
        return;
    }
    long memFotos = 0;
    elegirFotosAmigos(iter, memFotos, false);
    mostrarFotosAmigos();

    mediador.sumarIteraciones(iter);
    mediador.sumarMemoria(memoriaTotal() + memLocal + memFotos + (long)sizeof(iter));
    mediador.mostrarConsumo();

    menuPrincipal();
}

bool Plataforma::iniciarSesion(long& iter, long& memLocal) {
    for (int intento = 0; intento < 3; intento++) {
        iter++;
        std::string nombre, clave;
        if (!menu.leerNombre(nombre, iter) || !menu.leerClave(clave, iter)) return false;

        Usuario* u = buscarUsuario(nombre, iter);
        memLocal = (long)(sizeof(intento) + sizeof(nombre) + nombre.length()
                           + sizeof(clave) + clave.length() + sizeof(u) + 2 * sizeof(void*));
        if (u != nullptr && u->claveCorrecta(clave)) {
            usuarioActivo = u;
            return true;
        }
        menu.avisar("Usuario o contrasena incorrectos.");
    }
    return false;
}

void Plataforma::menuPrincipal() {
    bool salir = false;
    while (!salir) {
        menu.mostrarPrincipal();
        long iter = 0;
        int op = menu.leerOpcion(iter);

        if (op == 0 || op == -2) salir = true;
        else if (op == 1) menuAmistades();
        else if (op == 2) funcionVisitarPerfil();
        else if (op == 3) funcionSugerencias();
        else if (op == 4) menuMensajes();
        else if (op == 5) funcionActualizarFotos();
        else menu.avisar("Opcion invalida.");

        if (op >= 1 && op <= 4) guardarCambios();
    }
}

void Plataforma::menuAmistades() {
    bool volver = false;
    while (!volver) {
        menu.mostrarAmistades();
        long iter = 0;
        int op = menu.leerOpcion(iter);

        if (op == 1) funcionEnviarSolicitud();
        else if (op == 2) funcionAceptarSolicitudes();
        else if (op == 0 || op == -2) volver = true;
        else menu.avisar("Opcion invalida.");
    }
}

void Plataforma::menuMensajes() {
    bool volver = false;
    while (!volver) {
        menu.mostrarMensajes();
        long iter = 0;
        int op = menu.leerOpcion(iter);

        if (op == 1) funcionEnviarMensaje();
        else if (op == 2) funcionLeerNuevos();
        else if (op == 3) funcionBuscarPorRango();
        else if (op == 0 || op == -2) volver = true;
        else menu.avisar("Opcion invalida.");
    }
}

void Plataforma::guardarCambios() {
    long iter = 0;
    gestor.guardarDatos(usuarios, numUsuarios, *matriz, iter);
}