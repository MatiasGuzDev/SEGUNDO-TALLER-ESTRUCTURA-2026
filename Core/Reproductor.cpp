#include "Reproductor.hpp"
#include "Trie.h"
#include "AVLArtista.h"
#include "MaxHeap.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>


void limpiarConsola() {
    std::cout << std::flush;
#ifdef _WIN32
    std::system("cls");
#else
    std::system("clear");
#endif
    std::cout << std::flush;
}

Reproductor::Reproductor() {
    this->id_actual = 0;
    this->estado = "Reproduccion Detenida";
    this->modo_aleatorio = false;
    this->modo_repeticion = 0;
    std::srand(std::time(nullptr));
}

void Reproductor::cargarArchivos() {
    std::ifstream archivoStatus("../status.cfg");
    if (!archivoStatus.is_open()) {
        std::ofstream nuevoStatus("../status.cfg");
        nuevoStatus << "CONTADOR_IDS 0\nMODO_ALEATORIO 0\nREPETICION 0\n";
        nuevoStatus.close();
    } else {
        std::string parametro, valor;
        while (archivoStatus >> parametro >> valor) {
            if (parametro == "MODO_ALEATORIO") this->modo_aleatorio = (valor == "1");
            else if (parametro == "REPETICION") this->modo_repeticion = std::stoi(valor);
        }
        archivoStatus.close();
    }

    std::ifstream archivoCanciones("../data_structures/music_source.txt");
    if (!archivoCanciones.is_open()) {
        std::cout << "\n[ERROR]: No se encuentra 'music_source.txt' en la carpeta data_structures.\n";
        return;
    }

    if (archivoCanciones.peek() == std::ifstream::traits_type::eof()) {
        std::cout << "\n[ALERTA]: El archivo de biblioteca esta vacio.\n";
        archivoCanciones.close();
        return;
    }

    std::string linea;
    while (std::getline(archivoCanciones, linea)) {
        if (linea.empty()) continue;
        try {
            std::stringstream ss(linea);
            std::string id_str, nombre, artista, album, ano_str, duracion_str, ubicacion, repro_str;

            std::getline(ss, id_str, ',');
            std::getline(ss, nombre, ',');
            std::getline(ss, artista, ',');
            std::getline(ss, album, ',');
            std::getline(ss, ano_str, ',');
            std::getline(ss, duracion_str, ',');
            std::getline(ss, ubicacion, ',');

            int repros = 0;
            if (std::getline(ss, repro_str, ',')) {
                repros = std::stoi(repro_str);
            }

            int id = std::stoi(id_str);
            int ano = std::stoi(ano_str);
            int duracion = std::stoi(duracion_str);

            Cancion nuevaCancion(id, nombre, artista, album, ano, duracion, ubicacion, repros);
            lista_canciones.append(nuevaCancion);
            id_actual = id;
        } catch (...) {
            continue;
        }
    }
    archivoCanciones.close();
    seleccionarCancionInicial();
}

void Reproductor::seleccionarCancionInicial() {
    if (!lista_canciones.isEmpty()) {
        Cancion primeraCancion = lista_canciones.getFirst();
        this->cancion_actual = primeraCancion;

        if (lista_reproduccion.isEmpty()) {
            lista_reproduccion.append(cancion_actual);
        }
    }
}

void Reproductor::guardarEstado() {
    std::ofstream archivoStatus("../status.cfg");
    if (archivoStatus.is_open()) {
        archivoStatus << "CONTADOR_IDS " << this->id_actual << "\n";
        archivoStatus << "MODO_ALEATORIO " << (this->modo_aleatorio ? 1 : 0) << "\n";
        archivoStatus << "REPETICION " << this->modo_repeticion << "\n";
        archivoStatus.close();
    }

    std::ofstream archivoCanciones("../data_structures/music_source.txt");
    if (archivoCanciones.is_open()) {
        for (int i = 0; i < lista_canciones.size(); i++) {
            Cancion c = lista_canciones.get(i);
            archivoCanciones << c.getId() << "," << c.getNombreCancion() << ","
                             << c.getNombreArtista() << "," << c.getAlbum() << ","
                             << c.getAno() << "," << c.getDuracion() << ","
                             << c.getUbicacion() << "," << c.getReproducciones() << "\n";
        }
        archivoCanciones.close();
    }
}

void Reproductor::forzarReproduccionDirecta(Cancion c) {
    cancion_actual = c;
    cancion_actual.incrementarReproduccion();
    this->estado = "Reproduciendo";

    for(int i = 0; i < lista_canciones.size(); i++) {
        if(lista_canciones.get(i).getId() == cancion_actual.getId()) {
            Cancion temp = lista_canciones.get(i);
            temp.incrementarReproduccion();
            lista_canciones.set(i, temp);
            break;
        }
    }
    guardarEstado();

    lista_reproduccion = LinkedList<Cancion>();

    LinkedList<Cancion> copiaBiblioteca;
    for(int i = 0; i < lista_canciones.size(); i++) copiaBiblioteca.append(lista_canciones.get(i));

    int n = copiaBiblioteca.size();
    for (int i = n - 1; i > 0; --i) {
        int j = std::rand() % (i + 1);
        Cancion temp = copiaBiblioteca.get(i);
        copiaBiblioteca.set(i, copiaBiblioteca.get(j));
        copiaBiblioteca.set(j, temp);
    }

    for(int i=0; i<copiaBiblioteca.size(); i++) {
        lista_reproduccion.append(copiaBiblioteca.get(i));
    }
    std::cout << "\nReproduciendo ahora mismo: " << cancion_actual.getNombreCancion() << "\n";
    std::cout << "La lista de reproduccion actual ha sido reestructurada aleatoriamente.\n";
}

void Reproductor::reproducirPausar() {
    if (this->estado == "Reproduciendo") {
        this->estado = "En pausa";
    } else {
        this->estado = "Reproduciendo";
        cancion_actual.incrementarReproduccion();

        for(int i=0; i<lista_canciones.size(); i++) {
            if(lista_canciones.get(i).getId() == cancion_actual.getId()) {
                Cancion temp = lista_canciones.get(i);
                temp.incrementarReproduccion();
                lista_canciones.set(i, temp);
                break;
            }
        }
        guardarEstado();
    }
}


void Reproductor::menuBuscarCanciones() {
    limpiarConsola();
    std::cout << "Busqueda de canciones\n";
    std::cout << "Buscar canciones que contengan: ";
    std::string cadenaBusqueda;

    std::cin.clear();
    std::cin.ignore(10000, '\n');
    std::getline(std::cin, cadenaBusqueda);

    if (cadenaBusqueda.empty()) return;

    Trie* arbolTrie = new Trie();
    for (int i = 0; i < lista_canciones.size(); i++) {
        Cancion c = lista_canciones.get(i);
        arbolTrie->insertar(c.getNombreCancion(), c);
        arbolTrie->insertar(c.getNombreArtista(), c);
    }

    LinkedList<Cancion> resultados;
    arbolTrie->buscar(cadenaBusqueda, resultados);
    delete arbolTrie;

    while (true) {
        limpiarConsola();
        std::cout << "Busqueda de canciones\n";
        std::cout << "Canciones que contienen \"" << cadenaBusqueda << "\":\n";

        if (resultados.isEmpty()) {
            std::cout << " No se encontraron coincidencias.\n";
        } else {
            for (int i = 0; i < resultados.size(); i++) {
                std::cout << (i + 1) << ". " << resultados.get(i).getNombreCancion()
                          << "   " << resultados.get(i).getNombreArtista() << "\n";
            }
        }

        std::cout << "\nOpciones:\n";


        if (!resultados.isEmpty()) {
            std::cout << "R<num> Reproducir cancion seleccionada\n";
            std::cout << "A<num> Agregar cancion seleccionada al final de la cola\n";
        }


        std::cout << "F-    Repetir busqueda con un texto diferente\n";
        std::cout << "V     Volver al menu principal\n";
        std::cout << "Ingrese Opcion: ";

        std::string opcion;
        std::cin >> opcion;
        char cmd = std::toupper(opcion[0]);


        if (cmd == 'R' && !resultados.isEmpty()) {
            try {
                int num = std::stoi(opcion.substr(1)) - 1;
                if (num >= 0 && num < resultados.size()) {
                    forzarReproduccionDirecta(resultados.get(num));
                    return;
                }
            } catch(...) { std::cout << "Opcion no valida.\n"; }
        } else if (cmd == 'A' && !resultados.isEmpty()) {
            try {
                int num = std::stoi(opcion.substr(1)) - 1;
                if (num >= 0 && num < resultados.size()) {
                    lista_reproduccion.append(resultados.get(num));
                    std::cout << "\nCancion \"" << resultados.get(num).getNombreCancion() << "\" agregada a la cola.\n";
                    std::cout << "Presione Enter para continuar...";
                    std::cin.ignore(10000, '\n'); std::cin.get();
                }
            } catch(...) { std::cout << "Opcion no valida.\n"; }
        } else if (cmd == 'F') {
            menuBuscarCanciones();
            return;
        } else if (cmd == 'V') {
            return;
        } else {
            std::cout << "Opcion no valida. Presione Enter para continuar...";
            std::cin.ignore(10000, '\n');
            std::cin.get();
        }
    }
}

void Reproductor::menuTopRankings() {
    while (true) {
        limpiarConsola();
        std::cout << "Ranking TOP\n";
        std::cout << "C- Top 10 canciones mas escuchadas\n";
        std::cout << "A- Top 10 artistas mas escuchados\n";
        std::cout << "X- Salir\n";
        std::cout << "Ingrese Opcion: ";

        std::string opStr;
        std::cin >> opStr;
        char op = std::toupper(opStr[0]);

        if (op == 'C') desplegarTopCanciones();
        else if (op == 'A') desplegarTopArtistas();
        else if (op == 'X') return;
    }
}

void Reproductor::desplegarTopCanciones() {
    limpiarConsola();
    int totalItems = lista_canciones.size();
    MaxHeapCanciones heap(totalItems > 0 ? totalItems : 1);

    for (int i = 0; i < lista_canciones.size(); i++) {
        ItemCancion item = {lista_canciones.get(i), lista_canciones.get(i).getReproducciones()};
        heap.insertar(item);
    }

    LinkedList<ItemCancion> topTen;
    int n = (totalItems < 10) ? totalItems : 10;
    for (int i = 0; i < n; i++) {
        topTen.append(heap.extraerMax());
    }

    while (true) {
        limpiarConsola();
        std::cout << "Ranking TOP 10 Canciones mas escuchadas:\n";
        for (int i = 0; i < topTen.size(); i++) {
            std::cout << (i + 1) << ". [" << topTen.get(i).reproducciones << "] "
                      << topTen.get(i).cancion.getNombreCancion() << "   "
                      << topTen.get(i).cancion.getNombreArtista() << "\n";
        }

        std::cout << "\nOpciones:\n";
        std::cout << "R<num> Reproducir cancion seleccionada\n";
        std::cout << "A<num> Agregar cancion seleccionada al final de la cola\n";
        std::cout << "A-    Top 10 artistas mas escuchados\n";
        std::cout << "V     Volver al menu anterior\n";
        std::cout << "Ingrese Opcion: ";

        std::string opcion;
        std::cin >> opcion;
        char cmd = std::toupper(opcion[0]);

        if (cmd == 'R') {
            try {
                int num = std::stoi(opcion.substr(1)) - 1;
                if (num >= 0 && num < topTen.size()) {
                    forzarReproduccionDirecta(topTen.get(num).cancion);
                    return;
                }
            } catch(...) {}
        }  else if (cmd == 'A') {
            if (opcion.length() == 1 || opcion == "A-") {
                desplegarTopArtistas();
                return;
            } else {
                try {
                    int num = std::stoi(opcion.substr(1)) - 1;
                    if (num >= 0 && num < topTen.size()) {
                        lista_reproduccion.append(topTen.get(num).cancion);


                        std::cout << "\nCancion \"" << topTen.get(num).cancion.getNombreCancion()
                                  << "\" agregada a la cola.\n";
                        std::cout << "Presione Enter para continuar...";
                        std::cin.ignore(10000, '\n');
                        std::cin.get();

                    }
                } catch(...) {}
            }
        } else if (cmd == 'V') {
            return;
        } else {
            std::cout << "Opcion no valida. Presione Enter para continuar...";
            std::cin.ignore(10000, '\n');
            std::cin.get();
        }
    }
}

void Reproductor::desplegarTopArtistas() {
    limpiarConsola();

    LinkedList<ItemArtista> listaAgregada;
    for(int i = 0; i < lista_canciones.size(); i++) {
        Cancion c = lista_canciones.get(i);
        bool encontrado = false;
        for(int j = 0; j < listaAgregada.size(); j++) {
            if(listaAgregada.get(j).artista == c.getNombreArtista()) {
                ItemArtista tempArtista = listaAgregada.get(j);
                tempArtista.reproducciones += c.getReproducciones();
                listaAgregada.set(j, tempArtista);
                encontrado = true;
                break;
            }
        }
        if(!encontrado) {
            ItemArtista nuevo = {c.getNombreArtista(), c.getReproducciones()};
            listaAgregada.append(nuevo);
        }
    }

    int totalArtistas = listaAgregada.size();
    int limite = (totalArtistas < 10) ? totalArtistas : 10;

    for(int i = 0; i < totalArtistas - 1; i++) {
        for(int j = 0; j < totalArtistas - i - 1; j++) {
            if(listaAgregada.get(j).reproducciones < listaAgregada.get(j+1).reproducciones ||
              (listaAgregada.get(j).reproducciones == listaAgregada.get(j+1).reproducciones && listaAgregada.get(j).artista > listaAgregada.get(j+1).artista)) {
                ItemArtista temp = listaAgregada.get(j);
                listaAgregada.set(j, listaAgregada.get(j+1));
                listaAgregada.set(j+1, temp);
            }
        }
    }

    while (true) {
        limpiarConsola();
        std::cout << "Ranking TOP 10 Artistas mas escuchados:\n";
        for (int i = 0; i < limite; i++) {
            std::cout << (i + 1) << ". [" << listaAgregada.get(i).reproducciones << "] " << listaAgregada.get(i).artista << "\n";
        }

        std::cout << "\nOpciones:\n";
        std::cout << "S<num> Mostrar canciones del artista\n";
        std::cout << "C-    Top 10 canciones mas escuchadas\n";
        std::cout << "V     Volver al menu anterior\n";
        std::cout << "Ingrese Opcion: ";

        std::string opcion;
        std::cin >> opcion;
        char cmd = std::toupper(opcion[0]);

        if (cmd == 'S') {
            try {
                int num = std::stoi(opcion.substr(1)) - 1;
                if (num >= 0 && num < limite) {
                    desplegarCancionesDeArtista(listaAgregada.get(num).artista);
                    return;
                }
            } catch(...) {}
        } else if (cmd == 'C') {
            desplegarTopCanciones();
            return;
        } else if (cmd == 'V') {
            return;
        } else {
            std::cout << "Opcion no valida. Presione Enter para continuar...";
            std::cin.ignore(10000, '\n');
            std::cin.get();
        }
    }
}

void Reproductor::desplegarCancionesDeArtista(std::string nombreArtista) {
    ArbolAVL* avl = new ArbolAVL();
    for(int i=0; i<lista_canciones.size(); i++) {
        if(lista_canciones.get(i).getNombreArtista() == nombreArtista) {
            avl->insertar(lista_canciones.get(i));
        }
    }

    LinkedList<Cancion> ordenadas;
    avl->obtenerListaOrdenada(ordenadas);
    delete avl;

    while (true) {
        limpiarConsola();
        std::cout << "Ranking TOP 10 Artistas mas escuchados:\n";
        std::cout << "Artista: " << nombreArtista << "\n";
        for(int i=0; i<ordenadas.size(); i++) {
            std::cout << (i+1) << ". " << ordenadas.get(i).getNombreCancion() << "\n";
        }

        std::cout << "\nOpciones:\n";
        std::cout << "R<num> Reproducir cancion seleccionada\n";
        std::cout << "A<num> Agregar cancion a la cola\n";
        std::cout << "V-    Volver al listado de TOP 10 artistas\n";
        std::cout << "X-    Volver al menu principal\n";
        std::cout << "Ingrese Opcion: ";

        std::string sub_opcion;
        std::cin >> sub_opcion;
        char cmd = std::toupper(sub_opcion[0]);

        if (cmd == 'R') {
            try {
                int num = std::stoi(sub_opcion.substr(1)) - 1;
                if (num >= 0 && num < ordenadas.size()) {
                    forzarReproduccionDirecta(ordenadas.get(num));
                    return;
                }
            } catch(...) {}
        } else if (cmd == 'A') {
            try {
               int num = std::stoi(sub_opcion.substr(1));
            if (num >= 1 && num <= lista_canciones.size()) {
                lista_reproduccion.append(lista_canciones.get(num - 1));

                std::cout << "\nCancion \"" << lista_canciones.get(num - 1).getNombreCancion() << "\" agregada a la cola.\n";
                std::cout << "Presione Enter para continuar...";
                std::cin.ignore(10000, '\n'); std::cin.get();
                }
            } catch(...) {}
        } else if (cmd == 'V') {
            desplegarTopArtistas();
            return;
        } else if (cmd == 'X') {
            return;
        } else {
            std::cout << "Opcion no valida. Presione Enter para continuar...";
            std::cin.ignore(10000, '\n');
            std::cin.get();
        }
    }
}


void Reproductor::pistaSiguiente() {
    if (lista_canciones.isEmpty()) return;

    int idx = 0;
    int nuevo = 0;


    for (int i = 0; i < lista_canciones.size(); i++) {
        if (lista_canciones.get(i).getId() == cancion_actual.getId()) {
            idx = i;
            break;
        }
    }


    if (modo_repeticion == 1) {

        nuevo = idx;

        modo_repeticion = 0;
    }
    else if (modo_aleatorio) {

        nuevo = std::rand() % lista_canciones.size();
    }
    else {

        nuevo = (idx + 1) % lista_canciones.size();
    }

    cancion_actual = lista_canciones.get(nuevo);
    cancion_actual.incrementarReproduccion();


    Cancion temp = lista_canciones.get(nuevo);
    temp.incrementarReproduccion();
    lista_canciones.set(nuevo, temp);

    guardarEstado();
}
void Reproductor::pistaAnterior() {
    if (lista_canciones.isEmpty()) return;
    int idx = 0;
    for (int i = 0; i < lista_canciones.size(); i++) {
        if (lista_canciones.get(i).getId() == cancion_actual.getId()) { idx = i; break; }
    }
    int nuevo = (idx - 1 + lista_canciones.size()) % lista_canciones.size();
    cancion_actual = lista_canciones.get(nuevo);
    cancion_actual.incrementarReproduccion();

    Cancion temp = lista_canciones.get(nuevo);
    temp.incrementarReproduccion();
    lista_canciones.set(nuevo, temp);

    guardarEstado();
}

void Reproductor::alternarModoAleatorio() { this->modo_aleatorio = !this->modo_aleatorio; guardarEstado(); }
void Reproductor::alternarRepeticion() { this->modo_repeticion = (this->modo_repeticion + 1) % 3; guardarEstado(); }

void Reproductor::verListaReproduccion() {
    std::cout << "Actual: " << cancion_actual.getNombreCancion() << " - " << cancion_actual.getNombreArtista() << "\n";
    std::cout << "Lista de reproduccion actual:\n";
    if (lista_reproduccion.isEmpty()) {
        std::cout << " Vacia\n";
    } else {
        lista_reproduccion.display();
    }

    std::cout << "\nOpciones:\n";
    std::cout << "S<num> - Saltar a la cancion seleccionada\n";
    std::cout << "V - Volver al menu principal\n";
    std::cout << "Seleccione opcion (Ej. S2, o V): ";

    std::string sub_opcion;
    std::cin >> sub_opcion;
    char cmd = std::toupper(sub_opcion[0]);

    if (cmd == 'S') {
        try {
            int num = std::stoi(sub_opcion.substr(1));
            if (num >= 1 && num <= lista_reproduccion.size()) {
                cancion_actual = lista_reproduccion.get(num - 1);
                cancion_actual.incrementarReproduccion();

                // SINCRONIZACIÓN MAESTRA
                for(int i = 0; i < lista_canciones.size(); i++) {
                    if(lista_canciones.get(i).getId() == cancion_actual.getId()) {
                        Cancion temp = lista_canciones.get(i);
                        temp.incrementarReproduccion();
                        lista_canciones.set(i, temp);
                        break;
                    }
                }

                guardarEstado();
                std::cout << "Saltando a: " << cancion_actual.getNombreCancion() << "\n";
            }
        } catch (...) {}
    }
}

void Reproductor::verListadoCanciones() {
    limpiarConsola();
    std::cout << "Actual: " << cancion_actual.getNombreCancion() << " - " << cancion_actual.getNombreArtista() << "\n";
    std::cout << "Canciones registradas:\n";
    lista_canciones.display();

    std::cout << "\nOpciones:\n";
    std::cout << "R<num> - Reproducir cancion seleccionada\n";
    std::cout << "A<num> - Agregar cancion al final de la cola\n";
    std::cout << "N - Agregar cancion al registro\n";
    std::cout << "V - Volver al menu principal\n";
    std::cout << "Seleccione opcion: ";

    std::string sub_opcion;
    std::cin >> sub_opcion;
    char cmd = std::toupper(sub_opcion[0]);

    if (cmd == 'R') {
        try {
            int num = std::stoi(sub_opcion.substr(1));
            if (num >= 1 && num <= lista_canciones.size()) {
                forzarReproduccionDirecta(lista_canciones.get(num - 1));
            }
        } catch (...) {}
    } else if (cmd == 'A') {
        try {
            int num = std::stoi(sub_opcion.substr(1));
            if (num >= 1 && num <= lista_canciones.size()) {
                lista_reproduccion.append(lista_canciones.get(num - 1));
                // --- MENSAJE AÑADIDO ---
                std::cout << "\nCancion \"" << lista_canciones.get(num - 1).getNombreCancion() << "\" agregada a la cola.\n";
                std::cout << "Presione Enter para continuar...";
                std::cin.ignore(10000, '\n'); std::cin.get();
            }
        } catch (...) {}
    } else if (cmd == 'N') {
        std::string n_nombre, n_artista, n_album, n_ubicacion;
        int n_ano, n_duracion;

        std::cout << "Ingrese nombre: ";
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::getline(std::cin, n_nombre);
        std::cout << "Ingrese artista: ";
        std::getline(std::cin, n_artista);
        std::cout << "Ingrese album: ";
        std::getline(std::cin, n_album);
        std::cout << "Ingrese ano: ";
        std::cin >> n_ano;
        std::cout << "Ingrese duracion (segundos): ";
        std::cin >> n_duracion;
        std::cout << "Ingrese ubicacion de archivo: ";
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::getline(std::cin, n_ubicacion);

        id_actual++;
        Cancion nuevaCancion(id_actual, n_nombre, n_artista, n_album, n_ano, n_duracion, n_ubicacion, 0);
        lista_canciones.append(nuevaCancion);
        guardarEstado();
        std::cout << "Cancion registrada exitosamente.\n";
    }
}

void Reproductor::mostrarMenuPrincipal() {

    std::string txt_repeticion;
    if (modo_repeticion == 0) txt_repeticion = "Repeticion: OFF";
    else if (modo_repeticion == 1) txt_repeticion = "Repeticion: R1 (Una)";
    else txt_repeticion = "Repeticion: RA (Todas)";


    std::string txt_aleatorio = modo_aleatorio ? "Aleatorio: ON" : "Aleatorio: OFF";


    std::cout << "=== " << this->estado << " ===\n";
    std::cout << "[" << txt_repeticion << "] | [" << txt_aleatorio << "]\n";
    std::cout << "Pista:   " << cancion_actual.getNombreCancion() << "\n";
    std::cout << "Artista: " << cancion_actual.getNombreArtista() << "\n";
    std::cout << "Album:   " << cancion_actual.getAlbum() << "\n\n";

    std::cout << "Opciones:\n";
    std::cout << "W- Reproducir/Pausar\n0- Pista Anterior\nE- Pista Siguiente\n";
    std::cout << "S- Activar/Desactivar modo aleatorio\nR- Repeticion\n";
    std::cout << "A- Ver lista de reproduccion actual\nL- Listado de canciones\n";
    std::cout << "F- Buscar canciones\nT- TOP 10 Artistas y Canciones\nX- Salir\n";
    std::cout << "Ingrese Opcion: " << std::flush;
}