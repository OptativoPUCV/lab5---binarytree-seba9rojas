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
  TreeMap* new= (TreeMap*)malloc(sizeof(TreeMap));
  if(new==NULL){
    return NULL;
  }
  //new->lower_than = lower_than;
  new->current=new->root=NULL;
  new->lower_than=lower_than;

  return new;
}

void insertTreeMap(TreeMap * tree, void* key, void * value) {
  TreeNode* new;
  tree->current=tree->root;

  while(tree->current!=NULL){
    if(is_equal(tree,key,tree->current->pair->key)==1){
      return;
    }
    if(tree->lower_than(key,tree->current->pair->key)==1){
      if(tree->current->left==NULL){
        new=createTreeNode(key,value);
        tree->current->left=new;
        new->parent=tree->current;
        return;
      }
      tree->current=tree->current->left;
    }
    else{
      if(tree->current->right==NULL){
        new=createTreeNode(key,value);
        tree->current->right=new;
        new->parent=tree->current;
        return;
      }
      tree->current=tree->current->right;
    }
  }
  new=createTreeNode(key,value);
  tree->root=new;
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
  if (node -> left == NULL && node -> right == NULL)
  {
    if (node == node -> parent -> left)
    {
      node -> parent -> left = NULL;
      free(node);
    }
    else
    {
      node -> parent -> right = NULL;
      free(node);
    }
    return;
  }
  else if (node -> left != NULL && node -> right != NULL)
  {
    TreeNode* aux = minimum(node -> right);
    node -> pair = aux -> pair;
    removeNode(tree, aux);
    return;
  }  
  else if (node -> left != NULL && node -> right == NULL)
  {
    if (node == node -> parent -> left)
    {
      node -> parent -> left = node -> left;
      node -> left -> parent = node -> parent;
      free(node);
    }
    else
    {
      node -> parent -> right = node -> left;
      node -> left -> parent = node -> parent;
      free(node);
    }
    return;
  }
  else
  {
    if (node == node -> parent -> left)
    {
      node -> parent -> left = node -> right;
      node -> right -> parent = node -> parent;
      free(node);
    }
    else
    {
      node -> parent -> right = node -> right;
      node -> right -> parent = node -> parent;
      free(node);
    }
    return;
  } 
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
      return tree->current->pair;
    }
    if (tree->lower_than(key, tree->current->pair->key)== 1 ){
      tree->current = tree->current->left;
    } else{
      tree->current = tree->current->right;
    }
    
  }
  return NULL;
}

Pair* upperBound(TreeMap * tree, void* key) {
  TreeNode* current = tree->root;
  TreeNode* ubNode = NULL;
  
  while (current != NULL) 
  {
    if (is_equal(tree, current->pair->key, key)) 
    {
      return current->pair;
    } else if (tree->lower_than(key, current->pair->key)) {
      ubNode = current;
      current = current->left;
    } else {
      current = current->right;
    }
  }
  
  if (ubNode == NULL) 
  {      
    return NULL;
  } else {
      return ubNode->pair;
  }
  
}
  
Pair * firstTreeMap(TreeMap * tree){
  if(tree == NULL || tree-> root == NULL) return NULL;
  TreeNode* actual = tree->root;

  while (actual->left != NULL)
  {
    actual = actual->left;
  }
  tree->current = actual;
  return actual->pair;
}


Pair * nextTreeMap(TreeMap * tree) {
  if(tree == NULL || tree-> current == NULL) return NULL;
  TreeNode* actual = tree->current;
  
  if(actual ->right != NULL)
  {
    actual = actual->right;
    
    while(actual->left !=NULL)
    {
       actual = actual->left;   
    }    
  }else{
    TreeNode* tata = actual->parent;
    
    while (tata != NULL && actual == tata->right)
    {
      actual = tata;
      tata = tata->parent;
    }

    actual = tata;
  }
  
  tree->current = actual;
  
  return (actual != NULL) ? actual->pair : NULL;
}