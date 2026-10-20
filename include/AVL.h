//
// Created by Fernanda Sanchez on 19/10/26.
//

#ifndef MINIDB_AVL_H
#define MINIDB_AVL_H

#include "Registro.h"

struct Node {
    Registro* reg;
    Node* left;
    Node* right;
    int height;
    int size;           // cantidad de nodos

    Node(Registro* r)
        : reg(r), left(nullptr), right(nullptr), height(1), size(1) {}
};

class AVLTree {
public:
    AVLTree();
    ~AVLTree();

    AVLTree(const AVLTree&) = delete;
    AVLTree operator=(const AVLTree&) = delete;

    bool insert(Registro* reg);
    bool remove(Registro* reg);
    int getHeight() const;

private:

}


#endif //MINIDB_AVL_H
