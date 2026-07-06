cmake_minimum_required(VERSION 3.20)
project(Taller_Estructura_Reproductor_de_Musica_2026)

set(CMAKE_CXX_STANDARD 14)


include_directories(data_structures classes core)

add_executable(untitled
        Core/Reproductor.cpp
        Classes/Cancion.cpp
        Classes/Cancion.hpp
        Classes/Reproductor.hpp
        data_structures/LinkedList.h
        data_structures/Nodo.h
        data_structures/Trie.h
        data_structures/AVLArtista.h
        data_structures/MaxHeap.h
        Core/main.cpp
)