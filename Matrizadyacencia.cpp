#include "Matrizadyacencia.h"

MatrizAdyacencia::MatrizAdyacencia(short n) : filas(nullptr), numUsuarios(n) {
    filas = new bool*[n];
    for (int i = 0; i < n; i++) {
        filas[i] = new bool[n];
        for (int j = 0; j < n; j++) filas[i][j] = false;
    }
}

MatrizAdyacencia::~MatrizAdyacencia() {
    for (int i = 0; i < numUsuarios; i++) delete[] filas[i];
    delete[] filas;
}

bool MatrizAdyacencia::indiceValido(int i) const {
    return i >= 0 && i < numUsuarios;
}

short MatrizAdyacencia::getNumUsuarios() const { return numUsuarios; }

bool MatrizAdyacencia::sonAmigos(int a, int b) const {
    if (!indiceValido(a) || !indiceValido(b)) return false;
    return filas[a][b];
}

bool MatrizAdyacencia::conectar(int a, int b) {
    if (!indiceValido(a) || !indiceValido(b) || a == b) return false;
    filas[a][b] = true;
    filas[b][a] = true;
    return true;
}

bool MatrizAdyacencia::desconectar(int a, int b) {
    if (!indiceValido(a) || !indiceValido(b)) return false;
    filas[a][b] = false;
    filas[b][a] = false;
    return true;
}

//Cuantos amigos tienen en comun para ordenar las sugerencias.
int MatrizAdyacencia::contarAmigosComunes(int a, int b, long& iter) const {
    if (!indiceValido(a) || !indiceValido(b)) return 0;
    int comunes = 0;
    for (int k = 0; k < numUsuarios; k++) {
        iter++;
        if (filas[a][k] && filas[b][k]) comunes++;
    }
    return comunes;
}