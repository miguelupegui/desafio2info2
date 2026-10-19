#ifndef MATRIZADYACENCIA_H
#define MATRIZADYACENCIA_H

// Guarda quien es amigo de quien: filas[i][j] es true si los usuarios i y j son amigos.
// La amistad es simetrica, asi que siempre se marcan las dos casillas.
class MatrizAdyacencia {
private:
    bool** filas;
    short numUsuarios;

    bool indiceValido(int i) const;

public:
    explicit MatrizAdyacencia(short n);
    MatrizAdyacencia(const MatrizAdyacencia&) = delete;
    MatrizAdyacencia& operator=(const MatrizAdyacencia&) = delete;
    ~MatrizAdyacencia();

    short getNumUsuarios() const;
    bool sonAmigos(int a, int b) const;
    bool conectar(int a, int b);
    bool desconectar(int a, int b);
    int contarAmigosComunes(int a, int b, long& iter) const;
    long memoria() const;
};

#endif