#include <fstream>
#include <iostream>
//clase provisional para que el codigo funcione
class Usuario {
    char* datos[8];
public:
    Usuario(char* a, char* b, char* c, char* d,
            char* e, char* f, char* g, char* h) {
        datos[0]=a; datos[1]=b; datos[2]=c; datos[3]=d;
        datos[4]=e; datos[5]=f; datos[6]=g; datos[7]=h;
    }
    ~Usuario() { for (int i = 0; i < 8; i++) delete[] datos[i]; }
};
//Leer una linea del archivo y extrae cada dato de esta, separando todo por comas
bool leerRegistro(std::ifstream& f, char** campos, int numCampos) {
    int cap = 16, n = 0, campo = 0;
    char* buf = new char[cap];
    char c;
    bool hayDatos = false, lineaCompleta = false, error = false;

    while (f.get(c)) {
        hayDatos = true;
        if (c == '\r') continue;

        if (c == ',' || c == '\n') {
            buf[n] = '\0';
            if (campo >= numCampos) { error = true; break; }
            buf = nullptr;

            if (c == '\n') { lineaCompleta = true; break; }
            if (campo == numCampos) { error = true; break; }

            cap = 16; n = 0;
            buf = new char[cap];
        } else {
            //se reserva mas memoria
            if (n + 1 >= cap) {
                char* tmp = new char[cap * 2];
                for (int i = 0; i < n; i++) tmp[i] = buf[i];
                delete[] buf;
                buf = tmp;
                cap *= 2;
            }
            buf[n++] = c;
        }
    }
    if (!error && !lineaCompleta && hayDatos && buf != nullptr && campo < numCampos) {
        buf[n] = '\0';
        campos[campo++] = buf;
        buf = nullptr;
    }

    bool ok = !error && campo == numCampos;
    if (!ok) {
        for (int i = 0; i < campo; i++) delete[] campos[i];
    }
    delete[] buf;
    return ok;
}
//Cargar los usuarios usando la funcion pasada
bool cargarUsuarios(const char* ruta, Usuario**& usuarios, int& n) {
    usuarios = nullptr;
    n = 0;

    std::ifstream f(ruta);
    if (!f.is_open()) return false;

    // 1. Línea del conteo: un solo campo
    char* cuenta[1];
    if (!leerRegistro(f, cuenta, 1)) return false;
    //conversion para tener el entero
    int total = 0;
    bool valido = (cuenta[0][0] != '\0');
    for (int i = 0; cuenta[0][i] != '\0'; i++) {
        if (cuenta[0][i] < '0' || cuenta[0][i] > '9') { valido = false; break; }
        total = total * 10 + (cuenta[0][i] - '0');
    }
    delete[] cuenta[0];
    if (!valido) return false;

    // 2. Reservar el arreglo y leer N registros
    Usuario** arr = new Usuario*[total];
    int creados = 0;
    char* campos[8];

    for (int i = 0; i < total; i++) {
        if (!leerRegistro(f, campos, 8)) {
            for (int j = 0; j < creados; j++) delete arr[j];
            delete[] arr;
            return false;
        }
        arr[creados++] = new Usuario(campos[0], campos[1], campos[2], campos[3],
                                     campos[4], campos[5], campos[6], campos[7]);
    }

    usuarios = arr;
    n = total;
    return true;
}