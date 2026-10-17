#include "GestorArchivos.h"
#include "Album.h"
#include "Foto.h"
#include "Mensaje.h"

namespace {
//mano le puse un limite porque aja, toca ponerle un limite a eso, si no que decarp
    const int MAX_USUARIOS = 100000;
    const int MAX_TEXTO_BYTES = 2048;
    const int MAX_TEXTO_CARACTERES = 512;
}

GestorArchivos::GestorArchivos()
    : rutaUsuarios("usuarios.csv"), rutaAmistades("amistades.csv"),
      rutaSolicitudes("solicitudes.csv"), rutaAlbumes("albumes.csv"),
      rutaFotos("fotos.csv"), rutaMensajes("mensajes.csv") {}
bool GestorArchivos::leerRegistro(std::ifstream& f, char** campos, int numCampos,
                                  long& iter, char sep, int maxLen) const {
    int cap = 16, n = 0, campo = 0;
    char* buf = new char[cap];
    char c;
    bool hayDatos = false, lineaCompleta = false, error = false;

    while (f.get(c)) {
        iter++;
        hayDatos = true;
        if (c == '\r') continue;

        if (c == sep || c == '\n') {
            buf[n] = '\0';
            if (campo >= numCampos) { error = true; break; }
            campos[campo++] = buf;
            buf = nullptr;

            if (c == '\n') { lineaCompleta = true; break; }
            if (campo == numCampos) { error = true; break; }

            cap = 16; n = 0;
            buf = new char[cap];
        } else {
            if (n >= maxLen) { error = true; break; }
            if (n + 1 >= cap) {
                char* tmp = new char[cap * 2];
                for (int i = 0; i < n; i++) { iter++; tmp[i] = buf[i]; }
                delete[] buf;
                buf = tmp;
                cap *= 2;
            }
            buf[n++] = c;
        }
    }

    // Ultima linea sin '\n': cierra el campo pendiente
    if (!error && !lineaCompleta && hayDatos && buf != nullptr && campo < numCampos) {
        buf[n] = '\0';
        campos[campo++] = buf;
        buf = nullptr;
    }

    bool ok = !error && campo == numCampos;
    if (!ok) {
        for (int i = 0; i < campo; i++) { iter++; delete[] campos[i]; }
    }
    delete[] buf;      // delete[] sobre nullptr es seguro
    return ok;
}

// Abre el archivo y lee la primera linea
bool GestorArchivos::abrirYContar(std::ifstream& f, const std::string& ruta,
                                  int& total, long& iter) const {
    f.open(ruta);
    if (!f.is_open()) return false;

    char* cuenta[1];
    if (!leerRegistro(f, cuenta, 1, iter)) return false;
    bool ok = textoAEntero(cuenta[0], total, iter);
    delete[] cuenta[0];
    return ok;
}

void GestorArchivos::liberar(char** campos, int n, long& iter) const {
    for (int i = 0; i < n; i++) {
        iter++;
        delete[] campos[i];
    }
}

//Aqui van las conversiones de datos
bool GestorArchivos::textoAEntero(const char* t, int& valor, long& iter) const {
    if (t[0] == '\0') return false;
    int v = 0;
    for (int i = 0; t[i] != '\0'; i++) {
        iter++;
        if (t[i] < '0' || t[i] > '9') return false;
        v = v * 10 + (t[i] - '0');
        if (v > 100000000) return false;
    }
    valor = v;
    return true;
}
bool GestorArchivos::textoAFecha(const char* t, Fecha& fecha, long& iter) const {
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

bool GestorArchivos::textoAHora(const char* t, int& h, int& m, long& iter) const {
    for (int i = 0; i < 5; i++) {
        iter++;
        if (t[i] == '\0') return false;
    }
    if (t[5] != '\0' || t[2] != ':') return false;
    int idx[4] = {0, 1, 3, 4};
    for (int i = 0; i < 4; i++) {
        iter++;
        if (t[idx[i]] < '0' || t[idx[i]] > '9') return false;
    }
    h = (t[0] - '0') * 10 + (t[1] - '0');
    m = (t[3] - '0') * 10 + (t[4] - '0');
    return h <= 23 && m <= 59;
}
int GestorArchivos::contarCaracteres(const char* t, long& iter) const {
    int n = 0;
    for (int i = 0; t[i] != '\0'; i++) {
        iter++;
        if ((t[i] & 0xC0) != 0x80) n++;
    }
    return n;
}

//Las Funciones que cargan los datos desde los archivos, extrayendo todo, y maneja indices

bool GestorArchivos::cargarUsuarios(Usuario**& usuarios, int& n, long& iter) const {
    usuarios = nullptr;
    n = 0;

    std::ifstream f;
    int total;
    if (!abrirYContar(f, rutaUsuarios, total, iter) || total > MAX_USUARIOS) return false;

    Usuario** arr = new Usuario*[total];
    int creados = 0;
    bool ok = true;
    char* c[8];

    for (int i = 0; i < total && ok; i++) {
        iter++;
        if (!leerRegistro(f, c, 8, iter)) { ok = false; break; }

        Fecha nac, ins;
        ok = c[0][0] != '\0'
             && textoAFecha(c[2], nac, iter)
             && textoAFecha(c[7], ins, iter);
        if (ok) {
            // AQUI VA UN CONSTRUCTOR PROVISIONAL, PONGA EL QUE USTED USE CUANDO LEA ESTO
            arr[creados++] = new Usuario(c[0], c[1], nac, c[3], c[4], c[5], c[6], ins, i);
        }
        liberar(c, 8);
    }

    if (!ok) {
        for (int j = 0; j < creados; j++) { iter++; delete arr[j]; }
        delete[] arr;
        return false;
    }
    usuarios = arr;
    n = total;
    return true;
}

bool GestorArchivos::cargarAmistades(MatrizAdyacencia& m, int numUsuarios, long& iter) const {
    std::ifstream f;
    int total;
    if (!abrirYContar(f, rutaAmistades, total, iter)) return false;

    char* c[2];
    for (int i = 0; i < total; i++) {
        iter++;
        if (!leerRegistro(f, c, 2, iter)) return false;

        int a, b;
        bool ok = textoAEntero(c[0], a, iter) && textoAEntero(c[1], b, iter);
        liberar(c, 2);
        if (!ok || a >= numUsuarios || b >= numUsuarios || a == b) return false;

        m.conectar(a, b);
    }
    return true;
}

bool GestorArchivos::cargarSolicitudes(Usuario** usuarios, int numUsuarios,
                                       const MatrizAdyacencia& m, long& iter) const {
    std::ifstream f;
    int total;
    if (!abrirYContar(f, rutaSolicitudes, total, iter)) return false;

    char* c[2];
    for (int i = 0; i < total; i++) {
        iter++;
        if (!leerRegistro(f, c, 2, iter)) return false;

        int e, r;
        bool ok = textoAEntero(c[0], e, iter) && textoAEntero(c[1], r, iter);
        liberar(c, 2);
        if (!ok || e >= numUsuarios || r >= numUsuarios || e == r) return false;
        if (m.sonAmigos(e, r)) return false;

        if (!usuarios[r]->agregarSolicitudRecibida(e)) return false;
        if (!usuarios[e]->agregarSolicitudEnviada(r)) return false;
    }
    return true;
}

bool GestorArchivos::cargarAlbumes(Usuario** usuarios, int numUsuarios, long& iter) const {
    std::ifstream f;
    int total;
    if (!abrirYContar(f, rutaAlbumes, total, iter)) return false;

    char* c[4];
    for (int i = 0; i < total; i++) {
        iter++;
        if (!leerRegistro(f, c, 4, iter)) return false;

        int dueno;
        bool ok = textoAEntero(c[0], dueno, iter) && dueno < numUsuarios
                  && c[1][0] != '\0' && c[2][0] != '\0'
                  && c[3][0] != '\0' && c[3][1] == '\0'
                  && (c[3][0] == 'P' || c[3][0] == 'R' || c[3][0] == 'A');

        if (ok) {
            Album* a = new Album(c[1], c[2], c[3][0]);
            if (!usuarios[dueno]->agregarAlbum(a)) { delete a; ok = false; }
        }
        liberar(c, 4);
        if (!ok) return false;
    }
    return true;
}

bool GestorArchivos::cargarFotos(Usuario** usuarios, int numUsuarios, long& iter) const {
    std::ifstream f;
    int total;
    if (!abrirYContar(f, rutaFotos, total, iter)) return false;

    char* c[6];
    for (int i = 0; i < total; i++) {
        iter++;
        if (!leerRegistro(f, c, 6, iter)) return false;

        int dueno, pos, tam, likes;
        Fecha fecha;
        bool ok = textoAEntero(c[0], dueno, iter) && dueno < numUsuarios
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
        liberar(c, 6);
        if (!ok) return false;
    }
    return true;
}

bool GestorArchivos::cargarMensajes(Usuario** usuarios, int numUsuarios,
                                    const MatrizAdyacencia& m, long& iter) const {
    std::ifstream f;
    int total;
    if (!abrirYContar(f, rutaMensajes, total, iter)) return false;

    char* c[6];
    for (int i = 0; i < total; i++) {
        iter++;
        if (!leerRegistro(f, c, 6, iter, '|', MAX_TEXTO_BYTES)) return false;

        int e, r, hora, min;
        Fecha fecha;
        bool ok = textoAEntero(c[0], e, iter) && e < numUsuarios
                  && textoAEntero(c[1], r, iter) && r < numUsuarios && e != r
                  && m.sonAmigos(e, r)
                  && textoAFecha(c[2], fecha, iter)
                  && textoAHora(c[3], hora, min, iter)
                  && (c[4][0] == '0' || c[4][0] == '1') && c[4][1] == '\0'
                  && c[5][0] != '\0'
                  && contarCaracteres(c[5], iter) <= MAX_TEXTO_CARACTERES;

        if (ok) {
            Mensaje* msg = new Mensaje(c[5], usuarios[e], usuarios[r],
                                       fecha, hora, min, c[4][0] == '1');
            if (!usuarios[e]->agregarEnviado(msg)) { delete msg; ok = false; }
            else if (!usuarios[r]->agregarRecibido(msg)) ok = false;
        }
        liberar(c, 6);
        if (!ok) return false;
    }
    return true;
}

//Funciones que actualizan los datos en lel archivo
bool GestorArchivos::textoSeguro(const std::string& t, char sep, long& iter) const {
    for (int i = 0; i < (int)t.length(); i++) {
        iter++;
        if (t[i] == sep || t[i] == '\n' || t[i] == '\r') return false;
    }
    return true;
}

void GestorArchivos::escribirFecha(std::ofstream& f, const Fecha& d) const {
    f << d.getDia() << '/' << d.getMes() << '/' << d.getAnio();
}

void GestorArchivos::escribirHora(std::ofstream& f, int h, int m) const {
    f << h / 10 << h % 10 << ':' << m / 10 << m % 10;      // siempre hh:mm
}
bool GestorArchivos::guardarAmistades(const MatrizAdyacencia& m, int n, long& iter) const {
    int total = 0;
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++) {
            iter++;
            if (m.sonAmigos(i, j)) total++;
        }

    std::ofstream f(rutaAmistades);
    if (!f.is_open()) return false;
    f << total << '\n';
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++) {
            iter++;
            if (m.sonAmigos(i, j)) f << i << ',' << j << '\n';
        }
    return f.good();
}

bool GestorArchivos::guardarSolicitudes(Usuario** usuarios, int n, long& iter) const {
    int total = 0;
    for (int i = 0; i < n; i++) {
        iter++;
        total += usuarios[i]->getNumSolicitudesEnviadas();
    }

    std::ofstream f(rutaSolicitudes);
    if (!f.is_open()) return false;
    f << total << '\n';
    for (int i = 0; i < n; i++) {
        int k = usuarios[i]->getNumSolicitudesEnviadas();
        for (int j = 0; j < k; j++) {
            iter++;
            f << i << ',' << usuarios[i]->getSolicitudEnviada(j)->getIndice() << '\n';
        }
    }
    return f.good();
}

bool GestorArchivos::guardarMensajes(Usuario** usuarios, int n, long& iter) const {
    int total = 0;
    for (int i = 0; i < n; i++) {
        int k = usuarios[i]->getNumEnviados();
        for (int j = 0; j < k; j++) {
            iter++;
            if (!textoSeguro(usuarios[i]->getEnviado(j)->getTexto(), '|', iter)) return false;
            total++;
        }
    }

    std::ofstream f(rutaMensajes);
    if (!f.is_open()) return false;
    f << total << '\n';
    for (int i = 0; i < n; i++) {
        int k = usuarios[i]->getNumEnviados();
        for (int j = 0; j < k; j++) {
            iter++;
            const Mensaje* msg = usuarios[i]->getEnviado(j);
            f << i << '|' << msg->getReceptor()->getIndice() << '|';
            escribirFecha(f, msg->getFecha());
            f << '|';
            escribirHora(f, msg->getHora(), msg->getMinuto());
            f << '|' << (msg->getLeido() ? 1 : 0) << '|' << msg->getTexto() << '\n';
        }
    }
    return f.good();
}
bool GestorArchivos::guardarFotos(Usuario** usuarios, int n, long& iter) const {
    int total = 0;
    for (int i = 0; i < n; i++)
        for (int a = 0; a < usuarios[i]->getNumAlbumes(); a++) {
            const Album* al = usuarios[i]->getAlbum(a);
            for (int p = 0; p < al->getNumFotos(); p++) {
                iter++;
                if (!textoSeguro(al->getFoto(p)->getRuta(), ',', iter)) return false;
                total++;
            }
        }
    std::ofstream f(rutaFotos);
    if (!f.is_open()) return false;
    f << total << '\n';
    for (int i = 0; i < n; i++)
        for (int a = 0; a < usuarios[i]->getNumAlbumes(); a++) {
            const Album* al = usuarios[i]->getAlbum(a);
            for (int p = 0; p < al->getNumFotos(); p++) {
                iter++;
                const Foto* fo = al->getFoto(p);
                f << i << ',' << a << ',' << fo->getRuta() << ',';
                escribirFecha(f, fo->getFecha());
                f << ',' << fo->getTamano() << ',' << fo->getLikes() << '\n';
            }
        }
    return f.good();
}
