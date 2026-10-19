#include "Usuario.h"
#include "Album.h"
#include "Mensaje.h"

namespace {
template <class T>
bool agregarPuntero(T**& arreglo, int& cantidad, int& capacidad, T* elemento) {
    if (elemento == nullptr) return false;
    if (cantidad == capacidad) {
        int nuevaCap = capacidad * 2;
        T** nuevo = new T*[nuevaCap];
        for (int i = 0; i < cantidad; i++) nuevo[i] = arreglo[i];
        delete[] arreglo;
        arreglo = nuevo;
        capacidad = nuevaCap;
    }
    arreglo[cantidad++] = elemento;
    return true;
}

// saca el puntero y corre los de la derecha un puesto a la izquierda
template <class T>
bool quitarPuntero(T** arreglo, int& cantidad, T* elemento, long& iter) {
    for (int i = 0; i < cantidad; i++) {
        iter++;
        if (arreglo[i] == elemento) {
            for (int j = i; j < cantidad - 1; j++) {
                iter++;
                arreglo[j] = arreglo[j + 1];
            }
            cantidad--;
            return true;
        }
    }
    return false;
}

bool contiene(Usuario** arreglo, int cantidad, const Usuario* buscado) {
    for (int i = 0; i < cantidad; i++)
        if (arreglo[i] == buscado) return true;
    return false;
}
}

Usuario::Usuario(const std::string& nombre, const std::string& clave_, const Fecha& nac,
                 const std::string& correo_, const std::string& rutaFoto,
                 const std::string& ciudad_, const std::string& pais_,
                 const Fecha& ins, int indice_)
    : nombreUsuario(nombre), clave(clave_), correo(correo_), rutaFotoPerfil(rutaFoto),
    ciudad(ciudad_), pais(pais_), fechaNacimiento(nac), fechaInscripcion(ins),
    indice(indice_),
    albumes(new Album*[4]), numAlbumes(0), capAlbumes(4),
    enviados(new Mensaje*[4]), numEnviados(0), capEnviados(4),
    recibidos(new Mensaje*[4]), numRecibidos(0), capRecibidos(4),
    solEnviadas(new Usuario*[4]), numSolEnviadas(0), capSolEnviadas(4),
    solRecibidas(new Usuario*[4]), numSolRecibidas(0), capSolRecibidas(4) {}

// copia solo los datos del perfil. Los albumes y mensajes pertenecen al usuario original:
// copiar los punteros haria que dos usuarios los liberaran (doble delete).
Usuario::Usuario(const Usuario& otro)
    : Usuario(otro.nombreUsuario, otro.clave, otro.fechaNacimiento, otro.correo,
              otro.rutaFotoPerfil, otro.ciudad, otro.pais, otro.fechaInscripcion,
              otro.indice) {}

// libera lo que posee. Recibidos y solicitudes apuntan a objetos ajenos: solo se libera el arreglo.
Usuario::~Usuario() {
    for (int i = 0; i < numAlbumes; i++) delete albumes[i];
    for (int i = 0; i < numEnviados; i++) delete enviados[i];
    delete[] albumes;
    delete[] enviados;
    delete[] recibidos;
    delete[] solEnviadas;
    delete[] solRecibidas;
}

bool Usuario::operator==(const Usuario& otro) const {
    return nombreUsuario == otro.nombreUsuario;
}

bool Usuario::agregarAlbum(Album* a) {
    return agregarPuntero(albumes, numAlbumes, capAlbumes, a);
}

bool Usuario::agregarEnviado(Mensaje* m) {
    return agregarPuntero(enviados, numEnviados, capEnviados, m);
}

bool Usuario::agregarRecibido(Mensaje* m) {
    return agregarPuntero(recibidos, numRecibidos, capRecibidos, m);
}

bool Usuario::agregarSolicitudEnviada(Usuario* otro) {
    if (otro == nullptr || otro == this) return false;
    if (contiene(solEnviadas, numSolEnviadas, otro)) return false;    // ya enviada
    if (contiene(solRecibidas, numSolRecibidas, otro)) return false;  // el otro ya me envio una
    return agregarPuntero(solEnviadas, numSolEnviadas, capSolEnviadas, otro);
}

bool Usuario::agregarSolicitudRecibida(Usuario* otro) {
    if (otro == nullptr || otro == this) return false;
    if (contiene(solRecibidas, numSolRecibidas, otro)) return false;  // ya recibida
    if (contiene(solEnviadas, numSolEnviadas, otro)) return false;    // yo ya le envie una
    return agregarPuntero(solRecibidas, numSolRecibidas, capSolRecibidas, otro);
}

bool Usuario::eliminarSolicitudEnviada(Usuario* otro, long& iter) {
    return quitarPuntero(solEnviadas, numSolEnviadas, otro, iter);
}

bool Usuario::eliminarSolicitudRecibida(Usuario* otro, long& iter) {
    return quitarPuntero(solRecibidas, numSolRecibidas, otro, iter);
}

bool Usuario::tieneSolicitudPendiente(const Usuario* otro, long& iter) const {
    iter += numSolEnviadas + numSolRecibidas;
    return contiene(solEnviadas, numSolEnviadas, otro)
           || contiene(solRecibidas, numSolRecibidas, otro);
}