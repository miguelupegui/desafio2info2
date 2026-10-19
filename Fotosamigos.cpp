#include "Plataforma.h"
#include <cstdlib>

namespace {
// el peso va por 2 para no usar decimales: 1 -> 2, 2.5 -> 5, 3.5 -> 7
int pesoPorComunes(int comunes) {
    if (comunes <= 10) return 2;
    if (comunes <= 20) return 5;
    return 7;
}
}

void Plataforma::elegirFotosAmigos(long& iter, long& memLocal, bool evitarPrevias) {
    int yo = usuarioActivo->getIndice();
    int* candidatos = new int[numUsuarios];
    int* pesos = new int[numUsuarios];
    int cuantos = 0;

    for (int j = 0; j < numUsuarios; j++) {
        iter++;
        if (!matriz->sonAmigos(yo, j)) continue;
        candidatos[cuantos] = j;
        pesos[cuantos] = pesoPorComunes(matriz->contarAmigosComunes(yo, j, iter));
        cuantos++;
    }

    int objetivo = cuantos < 3 ? cuantos : 3;

    // si quedan 3 amigos distintos a los de antes, los saco del sorteo
    if (evitarPrevias && cuantos - numFotosAmigos >= objetivo) {
        for (int i = 0; i < cuantos; i++) {
            for (int k = 0; k < numFotosAmigos; k++) {
                iter++;
                if (candidatos[i] == fotosAmigos[k]) pesos[i] = 0;
            }
        }
    }

    int nuevas[3];
    for (int k = 0; k < objetivo; k++) {
        long suma = 0;
        for (int i = 0; i < cuantos; i++) {
            iter++;
            suma += pesos[i];
        }
        long r = std::rand() % suma;
        mediador.registrarRand();

        for (int i = 0; i < cuantos; i++) {
            iter++;
            if (pesos[i] == 0) continue;
            if (r < pesos[i]) {
                nuevas[k] = candidatos[i];
                pesos[i] = 0;   // ya salio, que no se repita en este grupo
                break;
            }
            r -= pesos[i];
        }
    }

    for (int k = 0; k < objetivo; k++) fotosAmigos[k] = nuevas[k];
    numFotosAmigos = objetivo;

    memLocal = (long)(2 * numUsuarios * sizeof(int) + sizeof(yo) + sizeof(cuantos)
                       + sizeof(objetivo) + sizeof(nuevas) + 2 * sizeof(int*));
    delete[] candidatos;
    delete[] pesos;
}

void Plataforma::mostrarFotosAmigos() const {
    if (numFotosAmigos == 0) {
        menu.avisar("Aun no tienes amigos para mostrar sus fotos.");
        return;
    }
    for (int k = 0; k < numFotosAmigos; k++) {
        Usuario* amigo = usuarios[fotosAmigos[k]];
        menu.mostrarFotoAmigo(k + 1, amigo->getNombreUsuario(), amigo->getRutaFotoPerfil());
    }
}

void Plataforma::funcionActualizarFotos() {
    if (fotosActualizadas) {
        menu.avisar("Ya usaste la actualizacion de fotos.");
        return;
    }
    if (numFotosAmigos == 0) {
        menu.avisar("No tienes amigos cuyas fotos actualizar.");
        return;
    }

    mediador.reiniciar();
    long iter = 0, memLocal = 0;
    elegirFotosAmigos(iter, memLocal, true);
    fotosActualizadas = true;
    mostrarFotosAmigos();

    mediador.sumarIteraciones(iter);
    mediador.sumarMemoria(memoriaTotal() + memLocal + (long)sizeof(iter));
    mediador.mostrarConsumo();
}