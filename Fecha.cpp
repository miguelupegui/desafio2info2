#include "Fecha.h"
#include <ctime>
#include <iostream>

Fecha::Fecha() : dia(1), mes(1), anio(1) {}

Fecha::Fecha(short d, short m, short a) : dia(d), mes(m), anio(a) {}

Fecha::Fecha(const Fecha& otra) : dia(otra.dia), mes(otra.mes), anio(otra.anio) {}

bool Fecha::actualizarAHoy(int& hora, int& minuto) {
    std::time_t t = std::time(nullptr);
    std::tm* l = std::localtime(&t);
    if (l == nullptr) return false;

    dia = (short)l->tm_mday;
    mes = (short)(l->tm_mon + 1);
    anio = (short)(l->tm_year + 1900);
    hora = l->tm_hour;
    minuto = l->tm_min;
    return true;
}

void Fecha::mostrar() const {
    std::cout << (dia / 10) << (dia % 10) << '/'
              << (mes / 10) << (mes % 10) << '/' << anio;
}

bool Fecha::operator<(const Fecha& otra) const {
    if (anio != otra.anio) return anio < otra.anio;
    if (mes != otra.mes) return mes < otra.mes;
    return dia < otra.dia;
}

bool Fecha::operator==(const Fecha& otra) const {
    return anio == otra.anio && mes == otra.mes && dia == otra.dia;
}

bool Fecha::operator<=(const Fecha& otra) const {
    return *this < otra || *this == otra;
}
