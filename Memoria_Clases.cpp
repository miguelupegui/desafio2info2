#include "Plataforma.h"
#include "GestorArchivos.h"
#include "Matrizadyacencia.h"
#include "Usuario.h"
#include "Album.h"
#include "Foto.h"
#include "Mensaje.h"

long Mensaje::memoria() const {
    // La Fecha, hora, minuto, leido y los punteros emisor/receptor ya estan en sizeof.
    return (long)(sizeof(Mensaje) + texto.length());
}


long Usuario::memoria() const {
    // Fechas y numeros ya estan en sizeof
    long total = (long)sizeof(Usuario);
    total += (long)(nombreUsuario.length() + clave.length() + correo.length()
                     + rutaFotoPerfil.length() + ciudad.length() + pais.length());

    // Arreglos de punteros (capacidad reservada)
    total += (long)(capAlbumes * sizeof(Album*)
                     + capEnviados * sizeof(Mensaje*)
                     + capRecibidos * sizeof(Mensaje*)
                     + capSolEnviadas * sizeof(Usuario*)
                     + capSolRecibidas * sizeof(Usuario*));

    // Objetos que posee
    for (int i = 0; i < numAlbumes; i++) total += albumes[i]->memoria();
    for (int i = 0; i < numEnviados; i++) total += enviados[i]->memoria();
    // 'recibidos' y solicitudes apuntan a objetos ajenos: NO se cuentan aqui.
    return total;
}
long MatrizAdyacencia::memoria() const {
    long n = numUsuarios;
    return (long)sizeof(MatrizAdyacencia)
           + n * (long)sizeof(bool*)                    // arreglo de filas
           + n * n * (long)sizeof(bool);                // n filas de n bool
}
long GestorArchivos::memoria() const {
    return (long)(sizeof(GestorArchivos)
                   + rutaUsuarios.length() + rutaAmistades.length()
                   + rutaSolicitudes.length() + rutaAlbumes.length()
                   + rutaFotos.length() + rutaMensajes.length());
}
long Plataforma::memoriaTotal() const {
    long total = (long)sizeof(Plataforma);
    total += (long)(gestor.memoria() - sizeof(GestorArchivos));  // solo el texto de las rutas

    total += (long)(numUsuarios * sizeof(Usuario*));             // arreglo de punteros
    for (int i = 0; i < numUsuarios; i++)
        total += usuarios[i]->memoria();                         // cada usuario con todo lo suyo

    if (matriz != nullptr) total += matriz->memoria();
    return total;
}