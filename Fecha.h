#ifndef FECHA_H
#define FECHA_H

// Fecha calendario (dia, mes, anio). Sin memoria dinamica: el constructor de copia
// y el operador de asignacion por defecto serian suficientes; se declara el de copia
// porque el diagrama lo pide.
class Fecha {
private:
    short dia;
    short mes;
    short anio;

public:
    Fecha();                                   // 1/1/1
    Fecha(short d, short m, short a);
    Fecha(const Fecha& otra);

    // Getters
    short getDia() const { return dia; }
    short getMes() const { return mes; }
    short getAnio() const { return anio; }

    // Setters
    void setDia(short d) { dia = d; }
    void setMes(short m) { mes = m; }
    void setAnio(short a) { anio = a; }

    // Toma la fecha y hora del computador con UNA sola llamada a time()
    // (asi fecha y hora nunca quedan de instantes distintos).
    // Deja la fecha en este objeto y entrega hora y minuto por referencia.
    // Devuelve false si el sistema no pudo dar la hora (el objeto no cambia).
    bool actualizarAHoy(int& hora, int& minuto);

    // Operaciones de despliegue: escribe dd/mm/aaaa en consola
    void mostrar() const;

    // Operadores sobrecargados (comparan solo la fecha)
    bool operator<(const Fecha& otra) const;
    bool operator==(const Fecha& otra) const;
    bool operator<=(const Fecha& otra) const;
};

#endif
