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

int AVLTree::getHeight(Node* n) {

}

