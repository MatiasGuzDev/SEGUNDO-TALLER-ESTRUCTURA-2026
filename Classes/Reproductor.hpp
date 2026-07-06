#ifndef REPRODUCTOR_HPP
#define REPRODUCTOR_HPP

#include "../data_structures/LinkedList.h"
#include "Cancion.hpp"
#include <string>

class Reproductor {
private:
    int id_actual;
    std::string estado;
    bool modo_aleatorio;
    int modo_repeticion;
    Cancion cancion_actual;
    LinkedList<Cancion> lista_canciones;
    LinkedList<Cancion> lista_reproduccion;


    void forzarReproduccionDirecta(Cancion c); // Asegurado sin errores de escritura
    void seleccionarCancionInicial();
    void desplegarTopCanciones();
    void desplegarTopArtistas();
    void desplegarCancionesDeArtista(std::string nombreArtista);

public:
    Reproductor();

    void cargarArchivos();
    void guardarEstado();
    void reproducirPausar();
    void pistaSiguiente();
    void pistaAnterior();
    void alternarModoAleatorio();
    void alternarRepeticion();
    void verListaReproduccion();
    void verListadoCanciones();
    void mostrarMenuPrincipal();


    void menuBuscarCanciones();
    void menuTopRankings();
};

#endif