#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;

struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
  TreeMap* newTreeMap= (TreeMap*)malloc(sizeof(TreeMap));
  if(newTreeMap==NULL){
    return NULL;
  }
  //new->lower_than = lower_than;
  newTreeMap->lower_than=lower_than;

  return newTreeMap;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
  TreeNode *Nodo = createTreeNode(key, value);
  TreeNode *Aux;
  
  searchTreeMap(tree, key);
  
  Aux = tree->current;
  Nodo->parent = Aux;
  
  if (tree->lower_than(Aux->pair->key, key) == 1) {
    Aux->right = Nodo;
  }else if (tree->lower_than(key, Aux->pair->key) == 1) {
    Aux->left = Nodo;
  }
  tree->current = Nodo;
}
  
TreeNode * minimum(TreeNode * x){
  if(x==NULL){
    return NULL;
  }
  
  while(x->left!=NULL){
    x=x->left;
  }
  return x;
}

void removeNode(TreeMap * tree, TreeNode* node) {

}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);
}

Pair * searchTreeMap(TreeMap * tree, void* key){
  tree->current = tree->root;
  if (tree->current == NULL) {
      return NULL;
  }

  while (tree->current != NULL){
    if (is_equal(tree, key, tree->current->pair->key) == 1){
      break;
    }
    if (tree->lower_than(key, tree->current->pair->key)== 1 ){
            tree->current = tree->current->left;
    }  else{
            tree->current = tree->current->right;
    }
    
    return tree->current->pair;
  }
}

Pair * upperBound(TreeMap * tree, void* key) {
    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
  TreeNode *aux = minimum(tree->root);
  tree->current = aux;
  return aux->pair;
}

Pair * nextTreeMap(TreeMap * tree) {
    if (tree == NULL || tree->root == NULL || tree->current == NULL) return NULL;
    
    if (tree->current->right != NULL) {
        TreeNode * aux = tree->current->right;

        tree->current = minimum(aux);
        
        return tree->current->pair;
    }
    
    TreeNode * aux = tree->current->parent;
    
    while (aux != NULL && tree->current == aux->right) {
        tree->current = aux;
        aux = aux->parent;
    }
    
    tree->current = aux;
    
    if (aux == NULL) return NULL;
    
    return aux->pair;
}
