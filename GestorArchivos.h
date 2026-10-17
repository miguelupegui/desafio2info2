#ifndef GESTORARCHIVOS_H
#define GESTORARCHIVOS_H

#include <fstream>
#include <string>
#include "Fecha.h"
#include "Usuario.h"
#include "MatrizAdyacencia.h"

// Lee y guarda los 6 archivos de Ude@Book. No usa cin/cout.
// Todos los metodos reciben 'iter' por referencia para la metrica de iteraciones.
// Los metodos cargarX devuelven false si el archivo es invalido: en ese caso el
// estado cargado hasta ese punto debe descartarse (excepto cargarUsuarios, que se limpia sola).
class GestorArchivos {
private:
    std::string rutaUsuarios;
    std::string rutaAmistades;
    std::string rutaSolicitudes;
    std::string rutaAlbumes;
    std::string rutaFotos;
    std::string rutaMensajes;

    // ---- Lectura basica ----
    bool leerRegistro(std::ifstream& f, char** campos, int numCampos, long& iter,
                      char sep = ',', int maxLen = 2048) const;
    bool abrirYContar(std::ifstream& f, const std::string& ruta, int& total, long& iter) const;
    void liberar(char** campos, int n, long& iter) const;
    // ---- Conversiones manuales ----
    bool textoAEntero(const char* t, int& valor, long& iter) const;
    bool textoAFecha(const char* t, Fecha& fecha, long& iter) const;
    bool textoAHora(const char* t, int& h, int& m, long& iter) const;
    int contarCaracteres(const char* t, long& iter) const;

    // ---- Escritura basica ----
    bool textoSeguro(const std::string& t, char sep, long& iter) const;
    void escribirFecha(std::ofstream& f, const Fecha& d) const;
    void escribirHora(std::ofstream& f, int h, int m) const;

public:
    // Sin memoria dinamica propia: el constructor de copia por defecto es suficiente
    // y no se necesita destructor.
    GestorArchivos();

    // Getters y setters
    std::string getRutaUsuarios() const { return rutaUsuarios; }
    std::string getRutaAmistades() const { return rutaAmistades; }
    std::string getRutaSolicitudes() const { return rutaSolicitudes; }
    std::string getRutaAlbumes() const { return rutaAlbumes; }
    std::string getRutaFotos() const { return rutaFotos; }
    std::string getRutaMensajes() const { return rutaMensajes; }
    void setRutaUsuarios(const std::string& r) { rutaUsuarios = r; }
    void setRutaAmistades(const std::string& r) { rutaAmistades = r; }
    void setRutaSolicitudes(const std::string& r) { rutaSolicitudes = r; }
    void setRutaAlbumes(const std::string& r) { rutaAlbumes = r; }
    void setRutaFotos(const std::string& r) { rutaFotos = r; }
    void setRutaMensajes(const std::string& r) { rutaMensajes = r; }

    // Orden de carga obligatorio:
    // usuarios -> amistades -> solicitudes -> albumes -> fotos -> mensajes
    bool cargarUsuarios(Usuario**& usuarios, int& n, long& iter) const;
    bool cargarAmistades(MatrizAdyacencia& m, int numUsuarios, long& iter) const;
    bool cargarSolicitudes(Usuario** usuarios, int numUsuarios,
                           const MatrizAdyacencia& m, long& iter) const;
    bool cargarAlbumes(Usuario** usuarios, int numUsuarios, long& iter) const;
    bool cargarFotos(Usuario** usuarios, int numUsuarios, long& iter) const;
    bool cargarMensajes(Usuario** usuarios, int numUsuarios,
                        const MatrizAdyacencia& m, long& iter) const;

    // Se llaman al volver al menu principal. usuarios y albumes no se guardan
    // (el enunciado no los modifica); fotos si, por los likes.
    bool guardarAmistades(const MatrizAdyacencia& m, int n, long& iter) const;
    bool guardarSolicitudes(Usuario** usuarios, int n, long& iter) const;
    bool guardarMensajes(Usuario** usuarios, int n, long& iter) const;
    bool guardarFotos(Usuario** usuarios, int n, long& iter) const;
};

#endif
