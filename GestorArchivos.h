#ifndef GESTORARCHIVOS_H
#define GESTORARCHIVOS_H

#include <fstream>
#include <string>
#include "Fecha.h"
#include "Usuario.h"
#include "MatrizAdyacencia.h"

// Lee y guarda los 6 archivos de Ude@Book. No usa cin/cout.
class GestorArchivos {
private:
    void liberarCarga(Usuario** usuarios, int n, MatrizAdyacencia* matriz, long& iter) const;
    std::string rutaUsuarios;
    std::string rutaAmistades;
    std::string rutaSolicitudes;
    std::string rutaAlbumes;
    std::string rutaFotos;
    std::string rutaMensajes;


    bool leerRegistro(std::ifstream& f, char** campos, int numCampos, long& iter,
                      char sep = ',', int maxLen = 2048) const;
    bool abrirYContar(std::ifstream& f, const std::string& ruta, int& total, long& iter) const;
    void liberar(char** campos, int n, long& iter) const;

    bool textoAEntero(const char* t, int& valor, long& iter) const;
    bool textoAFecha(const char* t, Fecha& fecha, long& iter) const;
    bool textoAHora(const char* t, int& h, int& m, long& iter) const;
    int contarCaracteres(const char* t, long& iter) const;


    bool textoSeguro(const std::string& t, char sep, long& iter) const;
    void escribirFecha(std::ofstream& f, const Fecha& d) const;
    void escribirHora(std::ofstream& f, int h, int m) const;

public:
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

    bool cargarUsuarios(Usuario**& usuarios, int& n, long& iter) const;
    bool cargarAmistades(MatrizAdyacencia& m, int numUsuarios, long& iter) const;
    bool cargarSolicitudes(Usuario** usuarios, int numUsuarios,
                           const MatrizAdyacencia& m, long& iter) const;
    bool cargarAlbumes(Usuario** usuarios, int numUsuarios, long& iter) const;
    bool cargarFotos(Usuario** usuarios, int numUsuarios, long& iter) const;
    bool cargarMensajes(Usuario** usuarios, int numUsuarios,
                        const MatrizAdyacencia& m, long& iter) const;


    bool guardarAmistades(const MatrizAdyacencia& m, int n, long& iter) const;
    bool guardarSolicitudes(Usuario** usuarios, int n, long& iter) const;
    bool guardarMensajes(Usuario** usuarios, int n, long& iter) const;
    bool guardarFotos(Usuario** usuarios, int n, long& iter) const;
    bool cargarDatos(Usuario**& usuarios, int& n, MatrizAdyacencia*& matriz, long& iter) const;

    int guardarDatos(Usuario** usuarios, int n, const MatrizAdyacencia& matriz, long& iter) const;
};

#endif
