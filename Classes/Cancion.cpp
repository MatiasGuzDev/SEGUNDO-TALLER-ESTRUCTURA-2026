//
// Created by marti on 03-05-2026.
//

#include "Cancion.hpp"


Cancion::Cancion() {
    this->id = 0;
    this->nombre = "";
    this->artista = "";
    this->album = "";
    this->ano = 0;
    this->duracion = 0;
    this->ubicacion = "";
    this->reproducciones = 0;
}


Cancion::Cancion(int id, std::string nombre, std::string artista, std::string album, int ano, int duracion, std::string ubicacion, int reproducciones) {
    this->id = id;
    this->nombre = nombre;
    this->artista = artista;
    this->album = album;
    this->ano = ano;
    this->duracion = duracion;
    this->ubicacion = ubicacion;
    this->reproducciones = reproducciones;
}

int Cancion::getId() const { return this->id; }
std::string Cancion::getNombreCancion() const { return this->nombre; }
std::string Cancion::getNombreArtista() const { return this->artista; }
std::string Cancion::getAlbum() const { return this->album; }
int Cancion::getAno() const { return this->ano; }
int Cancion::getDuracion() const { return this->duracion; }
std::string Cancion::getUbicacion() const { return this->ubicacion; }


int Cancion::getReproducciones() const {
    return this->reproducciones;
}

void Cancion::incrementarReproduccion() {
    this->reproducciones++;
}