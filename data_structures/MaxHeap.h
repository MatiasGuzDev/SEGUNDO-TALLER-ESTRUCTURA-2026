#ifndef MAXHEAP_HPP
#define MAXHEAP_HPP

#include <string>
#include "../Classes/Cancion.hpp"

struct ItemCancion {
    Cancion cancion;
    int reproducciones;
};

struct ItemArtista {
    std::string artista;
    int reproducciones;
};


class MaxHeapCanciones {
private:
    ItemCancion* arr;
    int capacidad;
    int tam;

    void heapify(int i) {
        int mayor = i;
        int izq = 2 * i + 1;
        int der = 2 * i + 2;

        if (izq < tam && comparar(arr[izq], arr[mayor])) mayor = izq;
        if (der < tam && comparar(arr[der], arr[mayor])) mayor = der;

        if (mayor != i) {
            ItemCancion temp = arr[i];
            arr[i] = arr[mayor];
            arr[mayor] = temp;
            heapify(mayor);
        }
    }


    bool comparar(const ItemCancion& a, const ItemCancion& b) {
        if (a.reproducciones != b.reproducciones) {
            return a.reproducciones > b.reproducciones;
        }
        if (a.cancion.getNombreCancion() != b.cancion.getNombreCancion()) {
            return a.cancion.getNombreCancion() < b.cancion.getNombreCancion();
        }
        return a.cancion.getNombreArtista() < b.cancion.getNombreArtista();
    }

public:
    MaxHeapCanciones(int cap) {
        capacidad = cap;
        arr = new ItemCancion[capacidad];
        tam = 0;
    }
    ~MaxHeapCanciones() { delete[] arr; }

    void insertar(ItemCancion item) {
        if (tam == capacidad) return;
        arr[tam] = item;
        int i = tam;
        tam++;

        while (i != 0 && comparar(arr[i], arr[(i - 1) / 2])) {
            ItemCancion temp = arr[i];
            arr[i] = arr[(i - 1) / 2];
            arr[(i - 1) / 2] = temp;
            i = (i - 1) / 2;
        }
    }

    ItemCancion extraerMax() {
        if (tam <= 0) return ItemCancion();
        if (tam == 1) {
            tam--;
            return arr[0];
        }
        ItemCancion raiz = arr[0];
        arr[0] = arr[tam - 1];
        tam--;
        heapify(0);
        return raiz;
    }

    int size() { return tam; }
};

#endif