//
// Created by marti on 03-05-2026.
//

#ifndef TALLER_ESTRUCTURA_REPRODUCTOR_DE_MUSICA_2026_CANCION_HPP
#define TALLER_ESTRUCTURA_REPRODUCTOR_DE_MUSICA_2026_CANCION_HPP

#include <string>

class Cancion {
private:
    int id;
    std::string nombre;
    std::string artista;
    std::string album;
    int ano;
    int duracion;
    std::string ubicacion;
    int reproducciones;

public:

    Cancion();


    Cancion(int id, std::string nombre, std::string artista, std::string album, int ano, int duracion, std::string ubicacion, int reproducciones = 0);

    int getId() const;
    std::string getNombreCancion() const;
    std::string getNombreArtista() const;
    std::string getAlbum() const;
    int getAno() const;
    int getDuracion() const;
    std::string getUbicacion() const;


    int getReproducciones() const;
    void incrementarReproduccion();
};

#endif