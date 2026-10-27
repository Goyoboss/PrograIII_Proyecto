// =============================================================================
//  Motor.cpp — ESTE ES TU PROYECTO. Todo lo de aquí es un esqueleto.
// =============================================================================
#include "Motor.h"

#include <cstring>

Motor::Motor() {}
Motor::~Motor() {}

bool Motor::insertar(const Registro& r) {
    // se asigna un espacio de memoria con una copia dinámica realizada con "new"
    Registro* nuevoRegistro = new Registro(r);

    // se inserta en la tabla Hash
    if (!table.insert(nuevoRegistro)) {
        delete nuevoRegistro; // ID duplicado, liberamos memoria
        return false;
    }

    // si la tabla lo acepta, se inserta en el árbol AVL
    avl.insert(nuevoRegistro);
    return true;
}
bool Motor::eliminar(int id) {
    // se obtiene el puntero del registro existente antes de borrarlo
    Registro* reg = table.get(id);
    if (reg == nullptr) {
        return false; // en caso de que el ID no existe
    }

    // se quita del árbol
    avl.remove(reg);
    // se quita de la tabla
    return table.remove(id);
}

bool Motor::actualizar(int id, int newVal) {
    Registro* reg = table.get(id);
    if (reg == nullptr) {
        return false; // El ID no existe
    }
    // si el valor ya existe entonces no se hace nada
    if (reg->valor == newVal) {
        return true;
    }
    // quitar el valor viejo del árbol
    avl.remove(reg);

    // actualizar en la tabla hash
    table.update(id, newVal);

    // insertar el nuevo valor en el avl
    avl.insert(reg);
    return true;
}

bool Motor::obtener(int id, Registro& salida) {
    // se llama a la función get y se guarda con su dirección en memoria
    Registro* found = table.get(id);

    // si es que se encuentra se copian sus datos
    if (found != nullptr) {
        salida = *found;
        return true;
    }
    return false;   // si no se encuentra
}

void Motor::rango(int a, int b, int limite, ResultadoRango& salida) {
        // se llama a la función getRango del avl
        avl.getRango(a, b, limite, salida);
}

int  Motor::contar(int a, int b) {
    // se llama al avl
    // devuelve un entero para saber cuantos registros existen entre a y b
    return avl.contarRango(a, b);
}

bool Motor::kesimo(int k, Registro& salida) {
    Registro* regFound = nullptr;
    // se llama al avl y la función getKesimo, si encuentra el valor
    // guardará en memoria su dirección y se guardará en regEncontrado
    if (avl.getKesimo(k, regFound)) {
        salida = *regFound; // Copiamos todos los datos del registro a la variable 'salida'
        return true;
    }

    return false; // Retorna false si 'k' está fuera de rango o el árbol está vacío
}
int Motor::top(int k, int categoria, Registro* salida) {
    return avl.getTop(k, categoria, salida);    // llama al avl
}

void Motor::iniciarTransaccion() {}
bool Motor::confirmar() { return false; }
bool Motor::revertir() { return false; }

int  Motor::altura() const {
    return avl.getHeight();
}

void Motor::configurarCache(int capacidad) { (void)capacidad; }

EstadisticasCache Motor::estadisticasCache() const {
    EstadisticasCache e;
    std::memset(&e, 0, sizeof(e));
    return e;
}

bool Motor::verificar(char* mensaje, int capacidad) {
    if (capacidad > 0) mensaje[0] = '\0';
    return true;
}

void Motor::imprimirStats(std::ostream& os) {
    os << "Casillas ocupadas: " << table.getElementCount();
}
