//
// Created by marti on 06-05-2026.
//

#ifndef TALLER_ESTRUCTURA_REPRODUCTOR_DE_MUSICA_2026_NODO_H
#define TALLER_ESTRUCTURA_REPRODUCTOR_DE_MUSICA_2026_NODO_H

template <typename M>
class Nodo {
public:
    M data;
    Nodo* next;

    Nodo(M mar) {
        data = mar;
        next = nullptr;
    }

};


#endif //TALLER_ESTRUCTURA_REPRODUCTOR_DE_MUSICA_2026_NODO_H
