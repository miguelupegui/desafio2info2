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
bool leerRegistro(std::ifstream& f, char** campos, int numCampos, long& iter,
    char sep = ',', int maxLen = 2048) {
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
//Sigue la funcion de Cargar las Amistades
// Convierte texto a entero como en la primera funcion, pero se llamará mas adelante
bool textoAEntero(const char* t, int& valor, long& iter) {
    if (t[0] == '\0') return false;
    int v = 0;
    for (int i = 0; t[i] != '\0'; i++) {
        iter++;
        if (t[i] < '0' || t[i] > '9') return false;
        v = v * 10 + (t[i] - '0');
    }
    valor = v;
    return true;
}
// Formato: 1ª línea = número de amistades; luego "indiceA,indiceB" por línea.
bool cargarAmistades(const char* ruta, MatrizAdyacencia& m,
                     int numUsuarios, long& iter) {
    std::ifstream f(ruta);
    if (!f.is_open()) return false;

    char* cuenta[1];
    if (!leerRegistro(f, cuenta, 1)) return false;
    int total;
    bool ok = textoAEntero(cuenta[0], total, iter);
    delete[] cuenta[0];
    if (!ok) return false;

    char* campos[2];
    for (int i = 0; i < total; i++) {
        iter++;
        if (!leerRegistro(f, campos, 2)) return false;   // ya se liberó solo

        int a, b;
        ok = textoAEntero(campos[0], a, iter) && textoAEntero(campos[1], b, iter)
             && a < numUsuarios && b < numUsuarios && a != b;
        delete[] campos[0];
        delete[] campos[1];
        if (!ok) return false;

        m.conectar(a, b);
    }
    return true;
}

//Cargar Solicitudes de amistad
// Debe llamarse después de cargar usuarios y amistades.
bool cargarSolicitudes(const char* ruta, Usuario** usuarios, int numUsuarios,
                       const MatrizAdyacencia& m, long& iter) {
    std::ifstream f(ruta);
    if (!f.is_open()) return false;

    char* cuenta[1];
    if (!leerRegistro(f, cuenta, 1)) return false;
    int total;
    bool ok = textoAEntero(cuenta[0], total, iter);
    delete[] cuenta[0];
    if (!ok) return false;

    char* campos[2];
    for (int i = 0; i < total; i++) {
        iter++;
        if (!leerRegistro(f, campos, 2)) return false;

        int e, r;
        ok = textoAEntero(campos[0], e, iter) && textoAEntero(campos[1], r, iter);
        delete[] campos[0];
        delete[] campos[1];

        if (!ok || e >= numUsuarios || r >= numUsuarios || e == r) return false;
        if (m.sonAmigos(e, r)) return false;       // ya son amigos: archivo inválido

        // Métodos Miguel con nombres provicionales, se devuelve un false si
        // la solicitud ya existía o existe la inversa.
        if (!usuarios[r]->agregarSolicitudRecibida(e)) return false;
        if (!usuarios[e]->agregarSolicitudEnviada(r)) return false;
    }
    return true;
}

// "dd/mm/aaaa" -> Fecha. Devuelve false si el formato o los rangos son inválidos.
bool textoAFecha(const char* t, Fecha& fecha, long& iter) {
    int partes[3] = {0, 0, 0};
    int idx = 0;
    bool hayDigito = false;
    for (int i = 0; t[i] != '\0'; i++) {
        iter++;
        if (t[i] == '/') {
            if (!hayDigito || idx >= 2) return false;
            idx++;
            hayDigito = false;
        } else if (t[i] >= '0' && t[i] <= '9') {
            partes[idx] = partes[idx] * 10 + (t[i] - '0');
            if (partes[idx] > 9999) return false;
            hayDigito = true;
        } else {
            return false;
        }
    }
    if (idx != 2 || !hayDigito) return false;
    if (partes[0] < 1 || partes[0] > 31 || partes[1] < 1 || partes[1] > 12 || partes[2] < 1)
        return false;
    fecha = Fecha(partes[0], partes[1], partes[2]);
    return true;
}

// Cuelga cada álbum de su dueño. Llamar después de cargar usuarios.
bool cargarAlbumes(const char* ruta, Usuario** usuarios, int numUsuarios, long& iter) {
    std::ifstream f(ruta);
    if (!f.is_open()) return false;

    char* cuenta[1];
    if (!leerRegistro(f, cuenta, 1)) return false;
    int total;
    bool ok = textoAEntero(cuenta[0], total, iter);
    delete[] cuenta[0];
    if (!ok) return false;

    char* c[4];
    for (int i = 0; i < total; i++) {
        iter++;
        if (!leerRegistro(f, c, 4)) return false;

        int dueno;
        ok = textoAEntero(c[0], dueno, iter) && dueno < numUsuarios
             && c[1][0] != '\0' && c[2][0] != '\0'
             && c[3][0] != '\0' && c[3][1] == '\0'
             && (c[3][0] == 'P' || c[3][0] == 'R' || c[3][0] == 'A');

        if (ok) {
            Album* a = new Album(c[1], c[2], c[3][0]);
            if (!usuarios[dueno]->agregarAlbum(a)) { delete a; ok = false; }
        }
        for (int j = 0; j < 4; j++) delete[] c[j];
        if (!ok) return false;
    }
    return true;
}

// Cuelga cada foto del álbum (dueño, posición). Llamar después de cargarAlbumes.
bool cargarFotos(const char* ruta, Usuario** usuarios, int numUsuarios, long& iter) {
    std::ifstream f(ruta);
    if (!f.is_open()) return false;

    char* cuenta[1];
    if (!leerRegistro(f, cuenta, 1)) return false;
    int total;
    bool ok = textoAEntero(cuenta[0], total, iter);
    delete[] cuenta[0];
    if (!ok) return false;

    char* c[6];
    for (int i = 0; i < total; i++) {
        iter++;
        if (!leerRegistro(f, c, 6)) return false;

        int dueno, pos, tam, likes;
        Fecha fecha;
        ok = textoAEntero(c[0], dueno, iter) && dueno < numUsuarios
             && textoAEntero(c[1], pos, iter)
             && pos < usuarios[dueno]->getNumAlbumes()
             && c[2][0] != '\0'
             && textoAFecha(c[3], fecha, iter)
             && textoAEntero(c[4], tam, iter)
             && textoAEntero(c[5], likes, iter);

        if (ok) {
            Album* al = usuarios[dueno]->getAlbum(pos);
            al->agregarFoto(new Foto(c[2], fecha, tam, likes));
        }
        for (int j = 0; j < 6; j++) delete[] c[j];
        if (!ok) return false;
    }
    return true;
}
//carga de mensajes
// "hh:mm" exacto. Devuelve false si el formato o el rango son inválidos.
bool textoAHora(const char* t, int& h, int& m, long& iter) {
    for (int i = 0; i < 5; i++) {
        iter++;
        if (t[i] == '\0') return false;          // evita leer más allá del final
    }
    if (t[5] != '\0' || t[2] != ':') return false;
    int idx[4] = {0, 1, 3, 4};
    for (int i = 0; i < 4; i++)
        if (t[idx[i]] < '0' || t[idx[i]] > '9') return false;
    h = (t[0] - '0') * 10 + (t[1] - '0');
    m = (t[3] - '0') * 10 + (t[4] - '0');
    return h <= 23 && m <= 59;
}

// Cuenta caracteres (no bytes): los bytes 10xxxxxx de UTF-8 son continuación.
int contarCaracteres(const char* t, long& iter) {
    int n = 0;
    for (int i = 0; t[i] != '\0'; i++) {
        iter++;
        if ((t[i] & 0xC0) != 0x80) n++;
    }
    return n;
}

// Llamar después de usuarios y amistades. El emisor es dueño del Mensaje;
// el receptor solo guarda el puntero.
bool cargarMensajes(const char* ruta, Usuario** usuarios, int numUsuarios,
                    const MatrizAdyacencia& m, long& iter) {
    std::ifstream f(ruta);
    if (!f.is_open()) return false;

    char* cuenta[1];
    if (!leerRegistro(f, cuenta, 1, iter)) return false;
    int total;
    bool ok = textoAEntero(cuenta[0], total, iter);
    delete[] cuenta[0];
    if (!ok) return false;

    char* c[6];
    for (int i = 0; i < total; i++) {
        iter++;
        if (!leerRegistro(f, c, 6, iter, '|', 2048)) return false;

        int e, r, hora, min;
        Fecha fecha;
        ok = textoAEntero(c[0], e, iter) && e < numUsuarios
             && textoAEntero(c[1], r, iter) && r < numUsuarios && e != r
             && m.sonAmigos(e, r)
             && textoAFecha(c[2], fecha, iter)
             && textoAHora(c[3], hora, min, iter)
             && (c[4][0] == '0' || c[4][0] == '1') && c[4][1] == '\0'
             && c[5][0] != '\0'
             && contarCaracteres(c[5], iter) <= 512;

        if (ok) {
            Mensaje* msg = new Mensaje(c[5], usuarios[e], usuarios[r],
                                       fecha, hora, min, c[4][0] == '1');
            if (!usuarios[e]->agregarEnviado(msg)) { delete msg; ok = false; }
            else if (!usuarios[r]->agregarRecibido(msg)) ok = false;
        }
        for (int j = 0; j < 6; j++) delete[] c[j];
        if (!ok) return false;
    }
    return true;
}