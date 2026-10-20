//
// Created by Fernanda Sanchez on 13/10/26.
//

#ifndef MINIDB_TABLAHASH_H
#define MINIDB_TABLAHASH_H

#include <cstddef>

#include "Registro.h"


// objeto estático global
enum class SlotState {EMPTY_SLOT, OCCUPIED, DELETED};
struct Entry {
    SlotState state;
    Registro* reg; 		//el * guarda la dirección de memoria en lugar de copiar t0do
};
class HashTable {
private:

    Entry* table;			// guardando la dirección de memoria del arreglo
    int capacity; 			// capacidad de la tabla
    int elementCount;		// conteo de elementos en la tabla
    int deletedCount;		// conteo de elementos eliminados (tumbas?)

public:
    // constructor con una capacidad inicial predeterminada
    HashTable(int initialCapacity = 17);
    // destructor de clase porque no podemos utilizar librería estándar
    ~HashTable();

    // eliminar explicitamente la copia (= delete)
    // eliminar constructor de copia
    HashTable(const HashTable&) = delete;
    // eliminar operador de asignación
    HashTable& operator=(const HashTable&) = delete;

    bool isPrime(std::size_t n);
    int nextPrime(std::size_t n);

    bool remove(int id);				// quitar una clave
    bool insert(Registro* reg); 		// insertar algo en la tabla
    Registro* get(int id); 				// buscar una llave dentro de la tabla
    bool update(int id, int newValue);	// actualizar un valor

    int searchIndex(int id);                    //buscar un índice - la utiliza remove y get
    [[nodiscard]] int hashFunction(int key) const; 		    // función hash a utiliza
    void rehash();				                // cuando la tabla está muy llena la va a agrandar

    int getElementCount() const;          // obtener el tamaño de la tabla
};
#endif //MINIDB_TABLAHASH_H
