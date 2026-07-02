Reproductor de Música - ReMusik
Este proyecto es un sistema de gestión y reproducción de música desarrollado como parte del taller de Estructuras de Datos. 
El software permite organizar una biblioteca musical, gestionar colas de reproducción y aplicar algoritmos de búsqueda y ordenamiento de manera eficiente. Características Principales:
Gestión de Biblioteca: Carga y persistencia de datos desde un archivo local (music_source.txt).
Algoritmos usados:
Trie: Utilizado para búsquedas rápidas de canciones y artistas.
AVL (Árbol Binario de Búsqueda Equilibrado): Implementado para mantener canciones ordenadas por artista
MaxHeap: Utilizado para generar rankings (TOP 10) de canciones más escuchadas.
Modos de Reproducción: Soporta modo secuencial, aleatorio y repetición (R1 y RA).
Persistencia: El estado actual (id, modo aleatorio, repetición) se guarda en status.cfg.
 Tecnologías Utilizadas:
 Lenguaje: C++ .
 IDE: JetBrains CLion.
Estructuras de Datos Propias: LinkedList, Trie, AVL Tree, MaxHeap. 
"Para compilar el proyecto fuera de un IDE, utilice el script proporcionado según su sistema operativo (build.bat para Windows).
Estos scripts automatizan la configuración de CMake y el proceso de construcción, asegurando un entorno de compilación estándar."
 [Diagrama de clases taller 2.pdf](https://github.com/user-attachments/files/29616138/Diagrama.de.clases.taller.2.pdf)

Integrantes: Matías Guzman - Martín Canales
