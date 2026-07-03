#ifndef TRIE_HPP
#define TRIE_HPP

#include <string>
#include "LinkedList.h"
#include "../Classes/Cancion.hpp"

class TrieNodo {
public:
    TrieNodo* hijos[256];
    LinkedList<Cancion> canciones;

    TrieNodo() {
        for (int i = 0; i < 256; i++) hijos[i] = nullptr;
    }

    ~TrieNodo() {
        for (int i = 0; i < 256; i++) {
            if (hijos[i] != nullptr) delete hijos[i];
        }
    }
};

class Trie {
private:
    TrieNodo* raiz;

    std::string normalizar(const std::string& s) {
        std::string resultado = "";
        for (char c : s) {
            resultado += std::tolower(static_cast<unsigned char>(c));
        }
        return resultado;
    }

public:
    Trie() { raiz = new TrieNodo(); }
    ~Trie() { delete raiz; }


    void insertar(const std::string& texto, Cancion c) {
        std::string s = normalizar(texto);

        for (size_t inicio = 0; inicio < s.length(); inicio++) {
            TrieNodo* actual = raiz;
            for (size_t i = inicio; i < s.length(); i++) {
                unsigned char index = s[i];
                if (actual->hijos[index] == nullptr) {
                    actual->hijos[index] = new TrieNodo();
                }
                actual = actual->hijos[index];


                bool yaExiste = false;
                for (int j = 0; j < actual->canciones.size(); j++) {
                    if (actual->canciones.get(j).getId() == c.getId()) {
                        yaExiste = true;
                        break;
                    }
                }
                if (!yaExiste) actual->canciones.append(c);
            }
        }
    }


    void buscar(const std::string& subcadena, LinkedList<Cancion>& resultados) {
        std::string s = normalizar(subcadena);
        TrieNodo* actual = raiz;
        for (char c : s) {
            unsigned char index = c;
            if (actual->hijos[index] == nullptr) {
                return;
            }
            actual = actual->hijos[index];
        }

        for (int i = 0; i < actual->canciones.size(); i++) {
            resultados.append(actual->canciones.get(i));
        }
    }
};

#endif