//
// Created by Fernanda Sanchez on 13/10/26.
//
#include "TablaHash.h"
#include<iostream>

// función hash: obtenemos el módulo del id entre la capacidad de la tabla, que inicialmente es 17
int HashTable::hashFunction(int id) const {
    int index = id%capacity;
    return index;
}

// métod0 dentro de atributos privados para devolver el indice de algún id
int HashTable::searchIndex(int id) {
    // ciclo for que empieza por el primer índice  y ve todos los demás gracias al ++i
    for (int i=0; i<capacity; ++i) {
        // busca el indice donde debería estar llamando a la función hash
        int index = (hashFunction(id)+i)%capacity;
        // si el estado de la tabla se retorna como vacío entonces no está dentro de la tabla
        if (table[index].state==SlotState::EMPTY_SLOT) {
            return -1;
        }
        // si el índice sale como ocupado y coinciden entonces se retorna el índice
        if (table[index].state==SlotState::OCCUPIED && table[index].reg->id==id) {
            return index;
        }
    }
    return -1;
}

// función <get>, obtener los datos dentro de la tabla
Registro* HashTable::get(int id) {
    int index = searchIndex(id);
    if (index != false){ // si el índice no es igual a 'false' entoces si existe
        return table[index].reg; // si es que existe retorna los datos
    }
    return nullptr; //si no existe entonces no retorna nada
}

// función de eliminar
bool HashTable::remove(int id) {
    int index = searchIndex(id); // se llama a la función

    if (index != false){                        // si el indice si existe
        table[index].state=SlotState::DELETED;  // se elimina
    delete table[index].reg;                    //se borra la fila
    // se actualizan los contadores de las tablas y de los eliminados
    --elementCount;
    ++deletedCount;
    return true;
    }
    return false;
}

// función para actualizar un valor
bool HashTable::update(int id, int newVal) {
    Registro* row = get(id);    // se llama a la función get para obtener su índice
    if (row!=nullptr) {         // si la fila si existe entonces...
        row->valor = newVal;    //se cambia el valor por el nuevo valor
        return true;
    }
    else {
        return false;
    }
    return false;
}

bool HashTable::insert(Registro* reg) {
    int id = reg->id;       // puntero hacia el valor del id
    int deleted=-1;         // contar los espacios que han sido eliminados
    for (int i=0; i<capacity; ++i) {                    // buscar en toda la tabla
        // obtenemos el índice ideal con la función hashFunction
        int index = (hashFunction(id)+i)%capacity;
        // si la casilla está ocupada y el id coincide con el id que queremos insertar
        // significa que queremos insertar un valor duplicado
        if (table[index].state == SlotState::OCCUPIED && table[index].reg->id==id) {
            return false;
        }
        // si vemos una que sea deleted se guarda el espacio del indice
        if (table[index].state==SlotState::DELETED) {
            deleted=index;
        }
        // si el índice está marcado como vacio entonces se usa ese espacio
        if (table[index].state==SlotState::EMPTY_SLOT) {
            // Se usa el espacio  si es que hay una casilla borrada antes
            int obj = (deleted != -1) ? deleted : index;
            // si se usa una DELETED o EMPTY_SLOT, se actualiza a OCCUPIED
            table[obj].state=SlotState::OCCUPIED;
            table[obj].reg = reg;       // puntero
            ++elementCount;             // se añade al element count
            if (obj == deleted) {
                --deletedCount;         // se le quita al deleted count
            }
            return true;
        }
    }
    // si solo hay deleted entonces se toma el primero y se cambia a occupied
    if (deleted != -1) {
        table[deleted].state=SlotState::OCCUPIED;
        table[deleted].reg = reg;       // puntero al valor
        ++elementCount;
        --deletedCount;
        return true;
    }
    return false;
}


// función que determina si un número es primo o no
bool HashTable::isPrime(std::size_t n) {
    if (n < 2) return false;
    if (n == 2 || n == 3) return true;
    if (n % 2 == 0) return false;
    for (std::size_t i = 3; i * i <= n; i += 2) {
        if (n % i == 0) return false;
    }
    return true;
}

// función que llama a isPrime para determinar el próximo número primo
int HashTable::nextPrime(std::size_t n) {
    if (n <= 2) return 2;
    std::size_t candidate = (n % 2 == 0) ? n + 1 : n;
    while (!isPrime(candidate)) {
        candidate += 2;
    }
    return candidate;
}


// función rehash para que la tabla se haga más grande
void HashTable::rehash() {
    std::size_t oldCapacity = capacity;         // declaramos una capacidad vieja
    Entry* oldTable = table;
    // calcular la nueva capacidad que al menos sea el doble de grande a la inicial
    capacity = nextPrime(oldCapacity * 2);
    // se crea la nueva tabla con la nueva capacidad
    Entry* newTable = new Entry[capacity];
    for (int i = 0; i < capacity; ++i) {
        newTable[i].state = SlotState::EMPTY_SLOT;
        newTable[i].reg = nullptr;
    }
    // se busca las casillas marcadas como ocupadas y con el puntero
    // obtenemos los valores para meterlos a una nueva función hash
    for (int i = 0; i < oldCapacity; ++i) {
        if (oldTable[i].state == SlotState::OCCUPIED) {
            Registro* reg = oldTable[i].reg;
            int id = reg->id;

            for (int j = 0; j < capacity; ++j) {
                // función hash a los valores que se obtuvieron de la tabla vieja
                int index = (hashFunction(id) + j) % capacity;
                // sondeo lineal -> si esta vacío se pone como ocupado al meter el nuevo hash
                if (newTable[index].state == SlotState::EMPTY_SLOT) {
                    newTable[index].state = SlotState::OCCUPIED;
                    newTable[index].reg = reg;      // copiamos la dirección del Registro
                    break;
                }
            }
        }
    }
    // se libera la tabla vieja
    delete[] oldTable;
    table = newTable;

    // se quitan los deleted
    deletedCount = 0;
}