//
// Created by Fernanda Sanchez on 19/10/26.
//

#ifndef MINIDB_AVL_H
#define MINIDB_AVL_H

#include "Registro.h"

// estructura que tendrán los nodos
struct Node {
    Registro* reg;      // guarda la dirección de memoria
    Node* left;         // puntero hacia la izquierda
    Node* right;        // puntero a la derecha
    int height;         // altura del árbol
    int size;           // cantidad de nodos

    // constructor del nodo que se le asigna un registo
    Node(Registro* r)
        // inicia con un registro e inicializa con izq y der vacío
        // tamaño y altura 1
        : reg(r), left(nullptr), right(nullptr), height(1), size(1) {}
};

class AVLTree {
public:
    // constructor
    AVLTree();
    // destructor
    ~AVLTree();

    // eliminar constructor de copia
    AVLTree(const AVLTree&) = delete;
    // eliminar el operados de asignación
    AVLTree operator=(const AVLTree&) = delete;

    bool insert(Registro* reg);     // bool para insertar un registro del árbol
    bool remove(Registro* reg);     // bool para quitar un registro del árbol
    int getHeight() const;          // obtener la altura del árbol

private:
    Node* root;         // puntero a la punta del árbol

    int getHeight(Node* node) const;        // altura del árbol
    int getSize(Node* node) const;          // tamaño del subárbol
    int getBalance(Node* node) const;       // factor de balance
    void updateNode(Node* node);            // actualizar

    // cuando el lado derecho es más pesado entonces se rota a la izquierda
    Node* rotateLeft(Node* x);
    // cuando el lado izquierdo es más pesado entonces se rota a la derecha
    Node* rotateRight(Node* y);

    // buscar la posición óptima para insertar algo
    Node* insertRecursive(Node* node, Registro* reg, bool& inserted);
    // para eliminar algo y balancear el árbol
    Node* removeRecursive(Node* node, int id, int valor, bool& removed);
    // busca el valor mínimo
    Node* getMinNode(Node* node);
    // auxiliar para el destructor
    void destroyRecursive(Node* node);
};


#endif //MINIDB_AVL_H
