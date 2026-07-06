#include <iostream>
#include <cstdlib>
#include "Reproductor.hpp"

int main() {
    Reproductor reproductor;
    reproductor.cargarArchivos();

    char opcion;
    bool salir = false;

    while (!salir) {

        std::cout << std::flush;
        #ifdef _WIN32
            std::system("cls");
        #else
            std::system("clear");
        #endif
        std::cout << std::flush;

        reproductor.mostrarMenuPrincipal();

        std::cin >> opcion;
        opcion = std::toupper(opcion);

        switch (opcion) {
            case 'W':
                reproductor.reproducirPausar();
                break;
            case '0':
            case 'Q':
                reproductor.pistaAnterior();
                break;
            case 'E':
                reproductor.pistaSiguiente();
                break;
            case 'S':
                reproductor.alternarModoAleatorio();
                break;
            case 'R':
                reproductor.alternarRepeticion();
                break;
            case 'A':

                #ifdef _WIN32
                    std::system("cls");
                #else
                    std::system("clear");
                #endif
                reproductor.verListaReproduccion();
                break;
            case 'L':
                #ifdef _WIN32
                    std::system("cls");
                #else
                    std::system("clear");
                #endif
                reproductor.verListadoCanciones();
                break;
            case 'F':
                reproductor.menuBuscarCanciones();
                break;
            case 'T':
                reproductor.menuTopRankings();
                break;
            case 'X':
                reproductor.guardarEstado();
                salir = true;
                break;
            default:
                break;
        }
    }

    return 0;
}