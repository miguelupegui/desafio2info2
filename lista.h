#ifndef LISTA_H
#define LISTA_H

template <class T>
class Lista {
private:
    T* elementos;
    int capacidad;
    int cantidad;

    void expandir() {
        int nuevaCapacidad = capacidad * 2;
        T* nuevo = new T[nuevaCapacidad];
        for (int i = 0; i < cantidad; i++) {
            nuevo[i] = elementos[i];
        }
        delete[] elementos;
        elementos = nuevo;
        capacidad = nuevaCapacidad;
    }

public:
    Lista() {
        capacidad = 4;
        cantidad = 0;
        elementos = new T[capacidad];
    }

    Lista(const Lista<T>& otra) {
        capacidad = otra.capacidad;
        cantidad = otra.cantidad;
        elementos = new T[capacidad];
        for (int i = 0; i < cantidad; i++) {
            elementos[i] = otra.elementos[i];
        }
    }

    ~Lista() {
        delete[] elementos;
    }

    Lista<T>& operator=(const Lista<T>& otra) {
        if (this != &otra) {
            delete[] elementos;
            capacidad = otra.capacidad;
            cantidad = otra.cantidad;
            elementos = new T[capacidad];
            for (int i = 0; i < cantidad; i++) {
                elementos[i] = otra.elementos[i];
            }
        }
        return *this;
    }

    T& operator[](int indice) {
        return elementos[indice];
    }

    const T& operator[](int indice) const {
        return elementos[indice];
    }

    void agregar(const T& elemento) {
        if (cantidad == capacidad) {
            expandir();
        }
        elementos[cantidad] = elemento;
        cantidad = cantidad + 1;
    }

    void eliminar(int indice) {
        if (indice < 0 || indice >= cantidad) {
            return;
        }
        for (int i = indice; i < cantidad - 1; i++) {
            elementos[i] = elementos[i + 1];
        }
        cantidad = cantidad - 1;
    }

    int tamano() const {
        return cantidad;
    }

    long memoriaUsada() const {
        return (long)sizeof(*this) + (long)capacidad * (long)sizeof(T);
    }
};

#endif