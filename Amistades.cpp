#include "Plataforma.h"

// devuelve 0 si se envio; si no, un numero con el motivo
int Plataforma::enviarSolicitud(Usuario* destino, long& iter) {
    iter++;
    if (destino == nullptr) return 1;
    if (destino == usuarioActivo) return 2;
    if (matriz->sonAmigos(usuarioActivo->getIndice(), destino->getIndice())) return 3;

    if (!usuarioActivo->agregarSolicitudEnviada(destino)) return 4;
    if (!destino->agregarSolicitudRecibida(usuarioActivo)) {
        usuarioActivo->eliminarSolicitudEnviada(destino, iter);
        return 4;
    }
    return 0;
}

void Plataforma::funcionEnviarSolicitud() {
    mediador.reiniciar();
    long iter = 0;
    std::string nombre;
    Usuario* destino = nullptr;

    if (!menu.leerNombre(nombre, iter)) menu.avisar("No se pudo leer el nombre.");
    else {
        destino = buscarUsuario(nombre, iter);
        menu.avisarResultadoSolicitud(enviarSolicitud(destino, iter));
    }

    mediador.sumarIteraciones(iter);
    mediador.sumarMemoria(memoriaTotal()
                          + (long)(sizeof(iter) + sizeof(nombre) + nombre.length() + sizeof(destino)));
    mediador.mostrarConsumo();
}

void Plataforma::funcionAceptarSolicitudes() {
    mediador.reiniciar();
    long iter = 0;
    int total = usuarioActivo->getNumSolicitudesRecibidas();
    int op = 0;

    if (total == 0) menu.avisar("No tienes solicitudes pendientes.");
    else {
        for (int i = 0; i < total; i++) {
            iter++;
            menu.mostrarSolicitud(i + 1, usuarioActivo->getSolicitudRecibida(i)->getNombreUsuario());
        }
        op = menu.leerNumero("Numero de la solicitud a aceptar (0 para volver): ", iter);

        if (op >= 1 && op <= total) {
            Usuario* otro = usuarioActivo->getSolicitudRecibida(op - 1);
            matriz->conectar(usuarioActivo->getIndice(), otro->getIndice());
            usuarioActivo->eliminarSolicitudRecibida(otro, iter);
            otro->eliminarSolicitudEnviada(usuarioActivo, iter);
            menu.avisar("Ahora son amigos.");
        } else if (op != 0) {
            menu.avisar("Opcion invalida.");
        }
    }

    mediador.sumarIteraciones(iter);
    mediador.sumarMemoria(memoriaTotal() + (long)(sizeof(iter) + sizeof(total) + sizeof(op)));
    mediador.mostrarConsumo();
}

// cada amigo mio le suma 1 a sus amigos y al final saco los k mayores
void Plataforma::funcionSugerencias() {
    mediador.reiniciar();
    long iter = 0;
    int yo = usuarioActivo->getIndice();
    int k = menu.leerNumero("Cuantas sugerencias quieres ver (1-20): ", iter);
    int cantidad = 0, op = 0;
    long memArreglos = 0;

    if (k < 1 || k > 20) {
        menu.avisar("K debe estar entre 1 y 20.");
    } else {
        int* comunes = new int[numUsuarios];
        int* sugeridos = new int[k];
        int* cuentas = new int[k];
        for (int j = 0; j < numUsuarios; j++) {
            iter++;
            comunes[j] = 0;
        }

        for (int f = 0; f < numUsuarios; f++) {
            iter++;
            if (!matriz->sonAmigos(yo, f)) continue;
            for (int j = 0; j < numUsuarios; j++) {
                iter++;
                if (matriz->sonAmigos(f, j)) comunes[j]++;
            }
        }

        // no sugiero a mi mismo, a mis amigos ni a quien ya tiene solicitud
        for (int j = 0; j < numUsuarios; j++) {
            iter++;
            if (comunes[j] > 0 && (j == yo || matriz->sonAmigos(yo, j)
                                   || usuarioActivo->tieneSolicitudPendiente(usuarios[j], iter)))
                comunes[j] = 0;
        }

        for (int pos = 0; pos < k; pos++) {
            int mejor = -1;
            for (int j = 0; j < numUsuarios; j++) {
                iter++;
                if (comunes[j] > 0 && (mejor == -1 || comunes[j] > comunes[mejor])) mejor = j;
            }
            if (mejor == -1) break;
            sugeridos[cantidad] = mejor;
            cuentas[cantidad] = comunes[mejor];
            cantidad++;
            comunes[mejor] = 0;
        }

        memArreglos = (long)((numUsuarios + 2 * k) * sizeof(int) + 3 * sizeof(int*));

        if (cantidad == 0) {
            menu.avisar("No hay sugerencias disponibles.");
        } else {
            for (int i = 0; i < cantidad; i++) {
                iter++;
                menu.mostrarSugerencia(i + 1, usuarios[sugeridos[i]]->getNombreUsuario(), cuentas[i]);
            }
            op = menu.leerNumero("Numero al que quieres enviar solicitud (0 para no enviar): ", iter);
            if (op >= 1 && op <= cantidad)
                menu.avisarResultadoSolicitud(enviarSolicitud(usuarios[sugeridos[op - 1]], iter));
            else if (op != 0)
                menu.avisar("Opcion invalida.");
        }

        delete[] comunes;
        delete[] sugeridos;
        delete[] cuentas;
    }

    mediador.sumarIteraciones(iter);
    mediador.sumarMemoria(memoriaTotal() + memArreglos
                          + (long)(sizeof(iter) + sizeof(yo) + sizeof(k) + sizeof(cantidad) + sizeof(op)));
    mediador.mostrarConsumo();
}