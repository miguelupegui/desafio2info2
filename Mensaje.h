#ifndef MENSAJE_H
#define MENSAJE_H

#include <string>
#include "Fecha.h"

class Usuario;   // declaracion adelantada: Mensaje solo guarda punteros a Usuario

// Un Mensaje lo posee el emisor (lo libera su Usuario). El receptor solo guarda el puntero.
// emisor y receptor NO son propiedad de Mensaje, por eso no hay destructor:
// los strings y la Fecha se liberan solos.
// Mensaje no lee ni escribe en consola salvo mostrar() (operacion de despliegue).
// La validacion del texto (vacio, 512 caracteres, sin '|') la hace enviarMensaje.
class Mensaje {
private:
    std::string texto;
    Usuario* emisor;
    Usuario* receptor;
    bool leido;
    Fecha fecha;
    short hora;
    short minuto;

public:
    Mensaje();

    // Mensaje NUEVO: toma fecha y hora actuales del computador (una sola llamada a time()).
    // Quien lo invoque debe reportar 1 uso de time() en la metrica.
    Mensaje(const std::string& t, Usuario* e, Usuario* r);

    // Mensaje CARGADO de archivo: recibe fecha, hora y estado.
    Mensaje(const std::string& t, Usuario* e, Usuario* r,
            const Fecha& f, int h, int m, bool yaLeido);

    // Copia superficial a proposito: emisor y receptor son punteros que Mensaje no posee.
    Mensaje(const Mensaje& otro);

    // Getters (const)
    const std::string& getTexto() const { return texto; }
    Usuario* getEmisor() const { return emisor; }
    Usuario* getReceptor() const { return receptor; }
    bool getLeido() const { return leido; }
    const Fecha& getFecha() const { return fecha; }
    int getHora() const { return hora; }
    int getMinuto() const { return minuto; }

    // Setters
    void setTexto(const std::string& t) { texto = t; }
    void setEmisor(Usuario* e) { emisor = e; }
    void setReceptor(Usuario* r) { receptor = r; }
    void setLeido(bool l) { leido = l; }
    void setFecha(const Fecha& f) { fecha = f; }
    void setHora(int h) { hora = (short)h; }
    void setMinuto(int m) { minuto = (short)m; }

    void marcarLeido() { leido = true; }

    // Operacion de despliegue: emisor, fecha, hora y texto
    void mostrar() const;

    // Orden cronologico: fecha, luego hora, luego minuto
    bool operator<(const Mensaje& otro) const;
};

#endif
