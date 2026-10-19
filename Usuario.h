#ifndef USUARIO_H
#define USUARIO_H

#include <string>
#include "Fecha.h"

class Album;     // declaraciones adelantadas: Usuario solo guarda punteros
class Mensaje;

// Un Usuario es dueno de sus albumes y de los mensajes que envia.
// Los mensajes recibidos y las solicitudes solo se guardan como punteros a objetos ajenos.
// Los arreglos crecen al doble cuando se llenan (cap* = espacio reservado, num* = usado).
class Usuario {
private:
    std::string nombreUsuario;
    std::string clave;
    std::string correo;
    std::string rutaFotoPerfil;
    std::string ciudad;
    std::string pais;
    Fecha fechaNacimiento;
    Fecha fechaInscripcion;
    int indice;// posicion en el arreglo de la plataforma y en la matriz

    Album** albumes;
    int numAlbumes, capAlbumes;
    Mensaje** enviados;
    int numEnviados, capEnviados;
    Mensaje** recibidos;
    int numRecibidos, capRecibidos;
    Usuario** solEnviadas;
    int numSolEnviadas, capSolEnviadas;
    Usuario** solRecibidas;
    int numSolRecibidas, capSolRecibidas;

    bool textoMensajeValido(const std::string& t, long& iter) const;

public:
    Usuario(const std::string& nombre, const std::string& clave, const Fecha& nac,
            const std::string& correo, const std::string& rutaFoto,
            const std::string& ciudad, const std::string& pais,
            const Fecha& ins, int indice);
    Usuario(const Usuario& otro);
    Usuario& operator=(const Usuario&) = delete;
    ~Usuario();

    bool operator==(const Usuario& otro) const;

    const std::string& getNombreUsuario() const { return nombreUsuario; }
    const std::string& getCorreo() const { return correo; }
    const std::string& getRutaFotoPerfil() const { return rutaFotoPerfil; }
    const std::string& getCiudad() const { return ciudad; }
    const std::string& getPais() const { return pais; }
    const Fecha& getFechaNacimiento() const { return fechaNacimiento; }
    const Fecha& getFechaInscripcion() const { return fechaInscripcion; }
    int getIndice() const { return indice; }

    void setClave(const std::string& c) { clave = c; }
    void setCorreo(const std::string& c) { correo = c; }
    void setRutaFotoPerfil(const std::string& r) { rutaFotoPerfil = r; }
    void setCiudad(const std::string& c) { ciudad = c; }
    void setPais(const std::string& p) { pais = p; }

    bool claveCorrecta(const std::string& intento) const { return clave == intento; }

    int getNumAlbumes() const { return numAlbumes; }
    Album* getAlbum(int i) const { return albumes[i]; }
    bool agregarAlbum(Album* a);

    int getNumEnviados() const { return numEnviados; }
    Mensaje* getEnviado(int i) const { return enviados[i]; }
    bool agregarEnviado(Mensaje* m);
    int getNumRecibidos() const { return numRecibidos; }
    Mensaje* getRecibido(int i) const { return recibidos[i]; }
    bool agregarRecibido(Mensaje* m);

    int getNumSolicitudesEnviadas() const { return numSolEnviadas; }
    Usuario* getSolicitudEnviada(int i) const { return solEnviadas[i]; }
    bool agregarSolicitudEnviada(Usuario* otro);
    int getNumSolicitudesRecibidas() const { return numSolRecibidas; }
    Usuario* getSolicitudRecibida(int i) const { return solRecibidas[i]; }
    bool agregarSolicitudRecibida(Usuario* otro);
    bool eliminarSolicitudEnviada(Usuario* otro, long& iter);
    bool eliminarSolicitudRecibida(Usuario* otro, long& iter);
    bool tieneSolicitudPendiente(const Usuario* otro, long& iter) const;

    bool enviarMensaje(Usuario& receptor, const std::string& texto, long& iter);
    int leerNuevosMensajes(long& iter);
    int buscarMensajes(const Fecha& inicio, const Fecha& fin, long& iter) const;
    void ordenarRecibidos(long& iter);
    bool visitarPerfil(Usuario& amigo, long& iter, long& memLocal) const;
    long memoria() const;
};

#endif