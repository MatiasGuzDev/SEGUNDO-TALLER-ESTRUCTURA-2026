#ifndef AVL_ARTISTA_HPP
#define AVL_ARTISTA_HPP

#include <string>
#include <iostream>
#include "../Classes/Cancion.hpp"
#include "LinkedList.h"

class NodoAVL {
public:
    Cancion cancion;
    NodoAVL* izquierdo;
    NodoAVL* derecho;
    int altura;

    NodoAVL(Cancion c) : cancion(c), izquierdo(nullptr), derecho(nullptr), altura(1) {}
};

class ArbolAVL {
private:
    NodoAVL* raiz;

    int obtenerAltura(NodoAVL* n) { return n ? n->altura : 0; }
    int max(int a, int b) { return (a > b) ? a : b; }

    int obtenerBalance(NodoAVL* n) { return n ? obtenerAltura(n->izquierdo) - obtenerAltura(n->derecho) : 0; }

    NodoAVL* rotarDerecha(NodoAVL* y) {
        NodoAVL* x = y->izquierdo;
        NodoAVL* T2 = x->derecho;
        x->derecho = y;
        y->izquierdo = T2;
        y->altura = max(obtenerAltura(y->izquierdo), obtenerAltura(y->derecho)) + 1;
        x->altura = max(obtenerAltura(x->izquierdo), obtenerAltura(x->derecho)) + 1;
        return x;
    }

    NodoAVL* rotarIzquierda(NodoAVL* x) {
        NodoAVL* y = x->derecho;
        NodoAVL* T2 = y->izquierdo;
        y->izquierdo = x;
        x->derecho = T2;
        x->altura = max(obtenerAltura(x->izquierdo), obtenerAltura(x->derecho)) + 1;
        y->altura = max(obtenerAltura(y->izquierdo), obtenerAltura(y->derecho)) + 1;
        return y;
    }

    NodoAVL* insertar(NodoAVL* nodo, Cancion c) {
        if (!nodo) return new NodoAVL(c);


        if (c.getNombreCancion() < nodo->cancion.getNombreCancion()) {
            nodo->izquierdo = insertar(nodo->izquierdo, c);
        } else {
            nodo->derecho = insertar(nodo->derecho, c);
        }

        nodo->altura = 1 + max(obtenerAltura(nodo->izquierdo), obtenerAltura(nodo->derecho));
        int balance = obtenerBalance(nodo);

        if (balance > 1 && c.getNombreCancion() < nodo->izquierdo->cancion.getNombreCancion())
            return rotarDerecha(nodo);

        if (balance < -1 && c.getNombreCancion() > nodo->derecho->cancion.getNombreCancion())
            return rotarIzquierda(nodo);

        if (balance > 1 && c.getNombreCancion() > nodo->izquierdo->cancion.getNombreCancion()) {
            nodo->izquierdo = rotarIzquierda(nodo->izquierdo);
            return rotarDerecha(nodo);
        }

        if (balance < -1 && c.getNombreCancion() < nodo->derecho->cancion.getNombreCancion()) {
            nodo->derecho = rotarDerecha(nodo->derecho);
            return rotarIzquierda(nodo);
        }

        return nodo;
    }


    void enOrden(NodoAVL* raiz, LinkedList<Cancion>& lista) {
        if (raiz) {
            enOrden(raiz->izquierdo, lista);
            lista.append(raiz->cancion);
            enOrden(raiz->derecho, lista);
        }
    }

    void destruir(NodoAVL* nodo) {
        if (nodo) {
            destruir(nodo->izquierdo);
            destruir(nodo->derecho);
            delete nodo;
        }
    }

public:
    ArbolAVL() : raiz(nullptr) {}
    ~ArbolAVL() { destruir(raiz); }

    void insertar(Cancion c) { raiz = insertar(raiz, c); }


    void obtenerListaOrdenada(LinkedList<Cancion>& lista_salida) {
        enOrden(raiz, lista_salida);
    }
};

#endif