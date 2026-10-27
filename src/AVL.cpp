//
// Created by Fernanda Sanchez on 19/10/26.
//

#include "AVL.h"

// se usa inline porque es una función bastante pequeña
// porque se usará cada vez que se le agregue algo al árbol y es más rápido
inline int maximo(int a, int b) {
    return a > b ? a : b;
}

// constructor
AVLTree::AVLTree() {
    root = nullptr;
}

// destructor
AVLTree::~AVLTree() {
    destroyRecursive(root); // borra los nodos del árbol
}

// para el destructor
void AVLTree::destroyRecursive(Node* node) {
    if (node != nullptr) {
        destroyRecursive(node->left);
        destroyRecursive(node->right);
        delete node;
    }
}

int AVLTree::getHeight(Node* n) const{
    // si n es nulo devuelve 0, si no lo es devuelve la altura
    return (n==nullptr) ? 0 : n->height;
}

int AVLTree::getSize(Node* n) const {
    // si n es nulo devuelve 0, si no lo es devuelve el tamaño
    return (n==nullptr) ? 0 : n->size;
}

int AVLTree::getBalance(Node* n) const {
    // si es nulo devuelve 0
    if (n==nullptr) return 0;
    // si no lo es devuelve la altura del laso izquierdo - la altura del lado derecho
    return getHeight(n->left) - getHeight(n->right);
}

void AVLTree::updateNode(Node* n) {
    if (n != nullptr) {
        // llama a la función máximo
        n->height = 1 + maximo(getHeight(n->left), getHeight(n->right));
        // El tamaño de un subárbol es: 1 + hijos izquierdos + hijos derechos
        n->size = 1 + getSize(n->left) + getSize(n->right);
    }
}

Node* AVLTree::rotateRight(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;
    x->right = y;           // realiza una rotación
    y->left = T2;
    // Actualizar alturas y tamaños llamando a la función updateNode
    updateNode(y);
    updateNode(x);
    return x;               // devuelve una nueva raíz
}

Node* AVLTree::rotateLeft(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;
    y->left = x;           // realiza una rotación
    x->right = T2;
    // Actualizar alturas y tamaños llamando a la función updateNode
    updateNode(x);
    updateNode(y);
    return y;               // devuelve una nueva raíz
}

// función para insertar que devuelve un booleano
bool AVLTree::insert(Registro* reg) {
    bool inserted = false;
    // llama a la función insertRecursive
    root = insertRecursive(root, reg, inserted);
    return inserted;
}

Node* AVLTree::insertRecursive(Node* node, Registro* reg, bool& inserted) {
    // --- Caso 1 ---
    // se encuentra un espacio vacío
    if (node == nullptr) {
        inserted = true;
        return new Node(reg);
    }
    // --- ordenamiento principal ---
    // se compara el valor si el valor es menor a los que están en el árbol entonces se
    // manda a la izquierda, se vuelve a llamar a la misma función
    if (reg->valor < node->reg->valor ||        // los || significan 'or'
       (reg->valor == node->reg->valor && reg->id < node->reg->id)) {
        node->left = insertRecursive(node->left, reg, inserted);
       }
    // si en cambio el valor es más grande, entonces se manda a la derecha llamando
    // a la función insertRecursive
    else if (reg->valor > node->reg->valor ||
            (reg->valor == node->reg->valor && reg->id > node->reg->id)) {
        node->right = insertRecursive(node->right, reg, inserted);
            }
    else {
        // Si valor y id son idénticos entonces es duplicado
        inserted = false;
        return node;
    }

    // si no se insertó regresa el nodo
    if (!inserted) return node;

    // se actualizan los nodos llamando a la función updateNode
    updateNode(node);

    // se checa que el árbol esté balanceado
    int balance = getBalance(node);

    // si el balance es mayor a 1 (altura izquierda- altura derecha > 1,
    // entonces está desbalanceado hacia la izquierda
    if (balance > 1 && (reg->valor < node->left->reg->valor ||
                       (reg->valor == node->left->reg->valor && reg->id < node->left->reg->id))) {
        // hace una rotación a la derecha para balancearlo
        return rotateRight(node);
                       }
    // si el balance es menor a -1 (altura izquierda- altura derecha > 1,
    // entonces está desbalanceado hacia la derecha
    if (balance < -1 && (reg->valor > node->right->reg->valor ||
                        (reg->valor == node->right->reg->valor && reg->id > node->right->reg->id))) {
        // rotación a la izquierda
        return rotateLeft(node);
                        }
    // la rama izquierda es más alta que la derecha
    if (balance > 1 && (reg->valor > node->left->reg->valor ||
                       (reg->valor == node->left->reg->valor && reg->id > node->left->reg->id))) {
        // se necesitan dos rotaciones
        // rotación a la izquierda
        node->left = rotateLeft(node->left);
        // rotación a la derecha
        return rotateRight(node);
                       }
    // la rama de la derecha es más alta que la izquierda
    if (balance < -1 && (reg->valor < node->right->reg->valor ||
                        (reg->valor == node->right->reg->valor && reg->id < node->right->reg->id))) {
        // se necesitan dos rotaciones
        // rotación a la derecha
        node->right = rotateRight(node->right);
        // rotación a la izquierda
        return rotateLeft(node);
                        }
    return node;
}

// función para obtener el nodo más pequeño
Node* AVLTree::getMinNode(Node* node) {
    // asigna una variable para ese nodo
    Node* current = node;
    // while loop hasta encontrar el nodo que se encuentre más a la izquierda
    while (current->left != nullptr)
        current = current->left;
    // retorna la variable
    return current;
}

bool AVLTree::remove(Registro* reg) {
    bool removed = false;
    // se busca el nodo exactamente por la misma tupla que lo insertamos: (id, valor)
    root = removeRecursive(root, reg->id, reg->valor, removed);
    return removed;
}

Node* AVLTree::removeRecursive(Node* node, int id, int valor, bool& removed) {
    if (node == nullptr) return node;

    // Buscar el nodo a eliminar usando  valor, id
    if (valor < node->reg->valor || (valor == node->reg->valor && id < node->reg->id)) {
        // se llama la función a sí misma
        node->left = removeRecursive(node->left, id, valor, removed);
    }
    else if (valor > node->reg->valor || (valor == node->reg->valor && id > node->reg->id)) {
        node->right = removeRecursive(node->right, id, valor, removed);
    }
    else {
        // aquí se remueve
        removed = true;

        // si el nodo tiene 1 hijo o no
        if (node->left == nullptr || node->right == nullptr) {
            Node* temp = node->left ? node->left : node->right;

            if (temp == nullptr) {      // si no tiene hijos
                delete node;            // se borra su espacio
                node = nullptr;
            } else {                    // si tiene un hijo
                delete node;
                node = temp;            // el hijo toma el lugar del padre
            }
        }
        // si tiene dos hijos
        else {
            Node* temp = getMinNode(node->right);       // se busca el más pequeño
            node->reg = temp->reg;                      // puntero al registro
            // se elimina el sucesor que acabamos de copiar
            node->right = removeRecursive(node->right, temp->reg->id, temp->reg->valor, removed);
        }
    }
    if (node == nullptr) return node;

    // Actualizar y rebalancear, igual que en insertar
    updateNode(node);
    int balance = getBalance(node);

    if (balance > 1 && getBalance(node->left) >= 0) return rotateRight(node);
    if (balance > 1 && getBalance(node->left) < 0) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }
    if (balance < -1 && getBalance(node->right) <= 0) return rotateLeft(node);
    if (balance < -1 && getBalance(node->right) > 0) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}

int AVLTree::getHeight() const {
    return getHeight(root);
}

bool AVLTree::getKesimo(int k, Registro*& salida) const {
    // validación para k
    if (k <= 0 || k > getSize(root)) {
        return false;
    }
    return getKesimoRecursive(root, k, salida);
}

bool AVLTree::getKesimoRecursive(Node* node, int k, Registro*& salida) const {
    if (node == nullptr) return false;
    // nodos de la izquierda
    int leftSize = getSize(node->left);

    if (k == leftSize + 1) {
        salida = node->reg;
        return true;
    }
    else if (k <= leftSize) {
        // Buscamos en la izquierda manteniendo el mismo 'k'
        return getKesimoRecursive(node->left, k, salida);
    }
    else {
        // Buscamos en la derecha
        return getKesimoRecursive(node->right, k - leftSize - 1, salida);
    }
}

// hacer una búsqueda binaria porque cada nodo sabe cuantos hijos tiene (tamaño)
// se le da un valor máximo y pasa por la derecha y/o la izquierda
int AVLTree::countLessOrEqual(int max_val) const {
    int count = 0;
    Node* current = root;
    while (current != nullptr) {
        if (current->reg->valor <= max_val) {
            count += getSize(current->left) + 1;
            current = current->right;
        } else {
            current = current->left;
        }
    }
    return count;
}

// busca el valor mínimo en el árbol
int AVLTree::countStrictlyLess(int min_val) const {
    int count = 0;
    Node* current = root;

    while (current != nullptr) {
        if (current->reg->valor < min_val) {
            // El nodo actual y TODOS los de su izquierda son estrictamente menores
            count += getSize(current->left) + 1;
            // Seguimos buscando más a la derecha
            current = current->right;
        } else {
            // El valor es mayor o igual, buscamos algo más pequeño en la izquierda
            current = current->left;
        }
    }
    return count;
}

// hace la resta del rango
// cuántos registros tienen un valor entre a y b?
int AVLTree::contarRango(int a, int b) const {
    if (a > b) return 0;
    return countLessOrEqual(b) - countStrictlyLess(a);
}

void AVLTree::getRango(int a, int b, int limite, ResultadoRango& salida) const {
    // Inicializamos la salida desde cero (limpiamos basuras en memoria)
    salida.nFilas = 0;
    // llama a la función getRangoRecursive
    getRangoRecursive(root, a, b, limite, salida);
}

// se da un rango entre a y b, tmb un límite, es decir que en cuanto encuentre un número (denominado límite)
// dejará de buscar más resultados. Si pones a=6, b=25, limite=4, devolverá los primeros 4 registros entre 6 y 25.
void AVLTree::getRangoRecursive(Node* node, int a, int b, int limite, ResultadoRango& salida) const {
    if (node == nullptr) return;

    // si ya se llega al límite máximo que pidieron ya no se continua
    if (salida.nFilas >= limite) return;

    // se va a la izquierda si hay posibilidad de que haya registros
    if (node->reg->valor > a) {
        getRangoRecursive(node->left, a, b, limite, salida);
    }

    // si con lo de la izquierda ya se llenó el límite, entonces se para
    if (salida.nFilas >= limite) return;

    // si llega a la raíz valora si entra entre el rango a y b
    if (node->reg->valor >= a && node->reg->valor <= b) {
        salida.nFilas++; // Contabilizamos que hallamos un registro válido
    }

    // si ya está lleno el límite no va a la derecha, si no lo está la recorre
    if (salida.nFilas >= limite) return;

    // va a la izquierda a por registros
    if (node->reg->valor < b) {
        getRangoRecursive(node->right, a, b, limite, salida);
    }
}

// función que devuelve un entero que busca los valores más grandes
int AVLTree::getTop(int k, int categoria, Registro* salida) const {
    int count = 0; // inicializa en 0
    // si se pide 0 elementos se devuelve 0
    if (k <= 0) return 0;
    // llama a otra función
    getTopRecursive(root, k, categoria, salida, count);
    return count; // Devolvemos la cantidad real de registros que escribimos
}

void AVLTree::getTopRecursive(Node* node, int k, int categoria, Registro* salida, int& count) const {
    // si el nodo es nulo o (||) si la k ya se llenó para aquí
    if (node == nullptr || count >= k) return;

    // va a la derecha - los valores más grandes
    getTopRecursive(node->right, k, categoria, salida, count);

    // si ya se llenó la k se para
    if (count >= k) return;
    // si coincide la categoría entonces se guarda el registro
    if (node->reg->categoria == categoria) {
        salida[count] = *(node->reg);
        ++count;  // incremento
    }

    // si ya se llenó las k, entonces para aquí
    if (count >= k) return;

    // ahora va hacia la izquierda a ver que no se hayan llenado las k
    getTopRecursive(node->left, k, categoria, salida, count);
}