bool Usuario::textoMensajeValido(const std::string& t, long& iter) const {
    int bytes = (int)t.length();
    if (bytes == 0 || bytes > 2048) return false;

    int caracteres = 0;
    bool hayTexto = false;
    for (int i = 0; i < bytes; i++) {
        iter++;
        if (t[i] == '|' || t[i] == '\n' || t[i] == '\r') return false;
        if (t[i] != ' ' && t[i] != '\t') hayTexto = true;
        if ((t[i] & 0xC0) != 0x80) caracteres++;
    }
    return hayTexto && caracteres <= 512;
}


bool Usuario::enviarMensaje(Usuario& receptor, const std::string& texto, long& iter) {
    if (&receptor == this) return false;                  // no se escribe a si mismo
    if (!textoMensajeValido(texto, iter)) return false;

    Mensaje* m = new Mensaje(texto, this, &receptor);     // una sola llamada a time()
    if (!agregarEnviado(m)) { delete m; return false; }
    receptor.agregarRecibido(m);                          // el receptor solo guarda el puntero
    return true;
}
// tambien lo ve leido.
int Usuario::leerNuevosMensajes(long& iter) {
    int nuevos = 0;
    for (int i = 0; i < numRecibidos; i++) {
        iter++;
        if (!recibidos[i]->getLeido()) {
            recibidos[i]->mostrar();
            recibidos[i]->marcarLeido();
            nuevos++;
        }
    }
    return nuevos;
}
int Usuario::buscarMensajes(const Fecha& inicio, const Fecha& fin, long& iter) const {
    if (!(inicio <= fin)) return -1;

    int lo = 0, hi = numRecibidos;
    while (lo < hi) {
        iter++;
        int mid = lo + (hi - lo) / 2;
        if (recibidos[mid]->getFecha() < inicio) lo = mid + 1;
        else hi = mid;
    }

    int encontrados = 0;
    for (int i = lo; i < numRecibidos; i++) {
        iter++;
        if (!(recibidos[i]->getFecha() <= fin)) break;
        recibidos[i]->mostrar();
        encontrados++;
    }
    return encontrados;
}
void Usuario::ordenarRecibidos(long& iter) {
    if (numRecibidos < 2) return;

    Mensaje** aux = new Mensaje*[numRecibidos];
    for (int ancho = 1; ancho < numRecibidos; ancho *= 2) {
        for (int ini = 0; ini < numRecibidos; ini += 2 * ancho) {
            int med = ini + ancho;
            if (med > numRecibidos) med = numRecibidos;
            int fin = ini + 2 * ancho;
            if (fin > numRecibidos) fin = numRecibidos;

            int i = ini, j = med, k = ini;
            while (i < med && j < fin) {
                iter++;
                if (*recibidos[j] < *recibidos[i]) aux[k++] = recibidos[j++];
                else aux[k++] = recibidos[i++];
            }
            while (i < med) { iter++; aux[k++] = recibidos[i++]; }
            while (j < fin) { iter++; aux[k++] = recibidos[j++]; }
        }
        for (int x = 0; x < numRecibidos; x++) { iter++; recibidos[x] = aux[x]; }
    }
    delete[] aux;
}

void Menu::descartarLinea(long& iter) const {
    char c;
    while (std::cin.get(c)) {
        iter++;
        if (c == '\n') break;
    }
}
bool Menu::leerLinea(std::string& destino, int tam, long& iter) const {
    char* buf = new char[tam];
    std::cin.getline(buf, tam);
    bool ok = !std::cin.fail();

    if (!ok && !std::cin.eof()) {
        std::cin.clear();
        descartarLinea(iter);
    }
    if (ok) {
        int len = 0;
        while (buf[len] != '\0') { iter++; len++; }
        if (len > 0 && buf[len - 1] == '\r') buf[len - 1] = '\0';
        destino = buf;
    }
    delete[] buf;
    return ok;
}

bool Menu::leerTextoMensaje(std::string& texto, long& iter) const {
    std::cout << "Mensaje (maximo 512 caracteres): ";
    return leerLinea(texto, 2049, iter);
}

bool Menu::leerNombre(std::string& nombre, long& iter) const {
    std::cout << "Nombre de usuario: ";
    return leerLinea(nombre, 256, iter);
}

// Fecha como tres enteros: dia mes anio (ej: 15 3 2026). Valida rangos basicos.
bool Menu::leerFecha(const char* pregunta, Fecha& f, long& iter) const {
    int d, m, a;
    std::cout << pregunta;
    std::cin >> d >> m >> a;
    if (std::cin.fail()) {
        std::cin.clear();
        descartarLinea(iter);
        return false;
    }
    descartarLinea(iter);
    if (d < 1 || d > 31 || m < 1 || m > 12 || a < 1) return false;
    f = Fecha(d, m, a);
    return true;
}

void Menu::avisar(const char* texto) const {
    std::cout << texto << '\n';
}

Usuario* Plataforma::buscarUsuario(const std::string& nombre, long& iter) const {
    for (int i = 0; i < numUsuarios; i++) {
        iter++;
        if (usuarios[i]->getNombreUsuario() == nombre) return usuarios[i];   // provisional
    }
    return nullptr;
}
void Plataforma::funcionEnviarMensaje() {
    mediador.reiniciar();
    long iter = 0;
    std::string nombre, texto;
    Usuario* amigo = nullptr;

    if (!menu.leerNombre(nombre, iter)) menu.avisar("No se pudo leer el nombre.");
    else {
        amigo = buscarUsuario(nombre, iter);
        if (amigo == nullptr) menu.avisar("Ese usuario no existe.");
        else if (!matriz->sonAmigos(usuarioActivo->getIndice(), amigo->getIndice()))
            menu.avisar("Solo puedes escribirle a tus amigos.");
        else if (!menu.leerTextoMensaje(texto, iter)) menu.avisar("No se pudo leer el mensaje.");
        else if (!usuarioActivo->enviarMensaje(*amigo, texto, iter))
            menu.avisar("Mensaje invalido: vacio, de mas de 512 caracteres o con el caracter '|'.");
        else {
            mediador.registrarTime();
            menu.avisar("Mensaje enviado.");
        }
    }

    mediador.sumarIteraciones(iter);
    mediador.sumarMemoria(memoriaTotal()
        + (long)(sizeof(iter) + sizeof(nombre) + nombre.length()
               + sizeof(texto) + texto.length() + sizeof(amigo))
        + 2049);
    mediador.mostrarConsumo();
}

// VI.b Leer nuevos mensajes
void Plataforma::funcionLeerNuevos() {
    mediador.reiniciar();
    long iter = 0;

    int nuevos = usuarioActivo->leerNuevosMensajes(iter);
    if (nuevos == 0) menu.avisar("No tienes mensajes nuevos.");

    mediador.sumarIteraciones(iter);
    mediador.sumarMemoria(memoriaTotal() + (long)(sizeof(iter) + sizeof(nuevos)));
    mediador.mostrarConsumo();
}

// VI.c Buscar mensajes recibidos por rango de fechas (ambas inclusive)
void Plataforma::funcionBuscarPorRango() {
    mediador.reiniciar();
    long iter = 0;
    Fecha ini, fin;

    if (!menu.leerFecha("Fecha de inicio (dia mes anio): ", ini, iter) ||
        !menu.leerFecha("Fecha de fin (dia mes anio): ", fin, iter)) {
        menu.avisar("Fecha invalida.");
    } else {
        int n = usuarioActivo->buscarMensajes(ini, fin, iter);
        if (n < 0) menu.avisar("La fecha de inicio es posterior a la de fin.");
        else if (n == 0) menu.avisar("No hay mensajes en ese rango.");
    }

    mediador.sumarIteraciones(iter);
    mediador.sumarMemoria(memoriaTotal()
        + (long)(sizeof(iter) + sizeof(ini) + sizeof(fin)));
    mediador.mostrarConsumo();
}


// ---------------------------------------------------------------------
// 4) GESTORARCHIVOS::cargarDatos - ordenar los recibidos despues de cargar
// ---------------------------------------------------------------------
/* En cargarDatos, reemplaza el final (despues del "if (!ok) {...}") por:

    for (int i = 0; i < n; i++) {
        iter++;
        usuarios[i]->ordenarRecibidos(iter);
    }
    matriz = m;
    return true;
*/
