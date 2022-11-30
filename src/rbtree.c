#include "rbtree.h"

#include <stdlib.h>

/*
  [CREATE]
  TODO: initialize struct if needed
*/
rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  node_t *nil_node = (node_t *)calloc(1, sizeof(node_t));
  nil_node -> color = RBTREE_BLACK;
  p -> root = nil_node;
  p -> nil = nil_node;
  return p;
}

void delete_inner_node(rbtree *t, node_t *x){
  if (x != t -> nil){
    delete_inner_node(t, x -> left);
    delete_inner_node(t, x -> right);
    
    free(x);
    x = NULL;
  }
}

/*
  [DELETE]
  TODO: reclaim the tree nodes's memory
*/
void delete_rbtree(rbtree *t) {
  delete_inner_node(t, t -> root); // 내부 노드 삭제
  free(t -> nil);
  t -> nil = NULL;
  free(t);
  t = NULL;
}

/* INSERT */
void left_rotate(rbtree *t, node_t *x) {
  node_t *y = x -> right;
  x -> right = y -> left;
  if (y -> left != t -> nil) {
    y -> left -> parent = x;
  }
  y -> parent = x -> parent;
  if (x -> parent == t -> nil){
    t -> root = y;
  } else if (x == x -> parent -> left){
    x -> parent -> left = y;
  } else {
    x -> parent -> right = y;
  }
  y -> left = x;
  x -> parent = y;
  return;
}

void right_rotate(rbtree *t, node_t *x) {
  node_t *y = x -> left;
  x -> left = y -> right;
  if (y -> right != t -> nil) {
    y -> right -> parent = x;
  }
  y -> parent = x -> parent;
  if (x -> parent == t -> nil){
    t -> root = y;
  } else if (x == x -> parent -> right){
    x -> parent -> right = y;
  } else {
    x -> parent -> left = y;
  }
  y -> right = x;
  x -> parent = y;
  return;
}


// 새로 삽입하는 노드를 항상 RED로 지정함으로써 
// RB Tree의 특성이 위반되므로 이를 교정하는 작업 수행
void rbtree_insert_fixup(rbtree *t, node_t *z) {
  while (z -> parent -> color == RBTREE_RED){
    if (z -> parent == z -> parent -> parent -> left){
      node_t *y = z -> parent -> parent -> right;
      if (y -> color == RBTREE_RED){
        z -> parent -> color = RBTREE_BLACK;
        y -> color = RBTREE_BLACK;
        z -> parent -> parent -> color = RBTREE_RED;
        z = z -> parent -> parent;
      } else {
        if (z == z -> parent -> right){
        z = z -> parent;
        left_rotate(t, z);
        }
        z -> parent -> color = RBTREE_BLACK;
        z -> parent -> parent -> color = RBTREE_RED;
        right_rotate(t, z -> parent -> parent);
      } 
    }
    else {
      node_t *y = z -> parent -> parent -> left;
      if (y -> color == RBTREE_RED){
        z -> parent -> color = RBTREE_BLACK;
        y -> color = RBTREE_BLACK;
        z -> parent -> parent -> color = RBTREE_RED;
        z = z -> parent -> parent;
      } else{ 
      if (z == z -> parent -> left){
        z = z -> parent;
        right_rotate(t, z);
      }
      z -> parent -> color = RBTREE_BLACK;
      z -> parent -> parent -> color = RBTREE_RED;
      left_rotate(t, z -> parent -> parent);
      }
    }
  }
  t -> root -> color = RBTREE_BLACK;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert

  // key 노드를 새로 만듦
  node_t *z = (node_t *)calloc(1, sizeof(node_t));
  z -> key = key; // 아직은 key만 확정된 상태

  node_t *y = t -> nil;
  node_t *x = t -> root;

  while(x != t -> nil){
    y = x;
    if(z -> key < x -> key){
      x = x -> left;
    } else{
      x = x -> right;
    }
  }
  z -> parent = y;

  if (y == t -> nil){
    t -> root = z;
  } else if (z -> key < y -> key){
    y -> left = z;
  } else {
    y -> right = z;
  }

  z -> left = t -> nil;
  z -> right = t -> nil;
  z -> color = RBTREE_RED;

  rbtree_insert_fixup(t, z);

  return t -> root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find

  node_t *x = t -> root;
  while (x != t -> nil && key != x -> key){
    if (key < x -> key){
      x = x -> left;
    } else {
      x = x -> right;
    }
  }
  if (x == t -> nil) {
    return NULL;
  }
  return x;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find

  node_t *x = t -> root;
  while (x -> left != t -> nil)
    x = x -> left;
  return x;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  
  node_t *x = t -> root;
  while (x -> right != t -> nil)
    x = x -> right;
  return x;
}

// 노드 u가 루트인 서브 트리를 노드 v가 루트인 서브트리로 교체
void rbtree_transplant(rbtree *t, node_t *u, node_t *v){
  v -> parent = u -> parent;
  
  if (u -> parent == t -> nil){
    t -> root = v;
  } else if (u == u -> parent -> left) {
    u -> parent -> left = v;
  } else {
    u -> parent -> right = v;
  }
  // v -> parent = u -> parent;
}

// RB Tree의 특성이 위반되므로 이를 교정하는 작업 수행
void rbtree_delete_fixup(rbtree *t, node_t *x){
  node_t *w;
  while (x != t -> root && x -> color == RBTREE_BLACK){
    if (x == x -> parent -> left) {
      w = x -> parent -> right;
      if (w -> color == RBTREE_RED){
        w -> color = RBTREE_BLACK;
        x -> parent -> color = RBTREE_RED;
        left_rotate(t, x -> parent);
        w = x -> parent -> right;
      }
      if (w -> left -> color == RBTREE_BLACK && w -> right -> color == RBTREE_BLACK){
        w -> color = RBTREE_RED;
        x = x -> parent;
      } else {
        if (w -> right -> color == RBTREE_BLACK) {
          w -> left -> color = RBTREE_BLACK;
          w -> color = RBTREE_RED;
          right_rotate(t, w);
          w = x -> parent -> right;
        }
        w -> color = x -> parent -> color;
        x -> parent -> color = RBTREE_BLACK;
        w -> right -> color = RBTREE_BLACK;
        left_rotate(t, x -> parent);
        x = t -> root;
      }
    } else {
      w = x -> parent -> left;
      if (w -> color == RBTREE_RED){
        w -> color = RBTREE_BLACK;
        x -> parent -> color = RBTREE_RED;
        right_rotate(t, x -> parent);
        w = x -> parent -> left;
      }
      if (w -> right -> color == RBTREE_BLACK && w -> left -> color == RBTREE_BLACK){
        w -> color = RBTREE_RED;
        x = x -> parent;
      } else {
        if (w -> left -> color == RBTREE_BLACK) {
          w -> right -> color = RBTREE_BLACK;
          w -> color = RBTREE_RED;
          left_rotate(t, w);
          w = x -> parent -> left;
        }
        w -> color = x -> parent -> color;
        x -> parent -> color = RBTREE_BLACK;
        w -> left -> color = RBTREE_BLACK;
        right_rotate(t, x -> parent);
        x = t -> root;
      }
    } 
  }
  x -> color = RBTREE_BLACK;
}

// min 과 똑같이 구현, but 시작 노드(루트)를 x로 지정
node_t *rbtree_successor(rbtree *t, node_t *x){

  node_t *y = x;                  // y는 x
  while (y -> left != t -> nil) { // y의 왼쪽 자식이 nil이 아닐 때 반복
    y = y -> left;                // y는 y의 왼쪽 자식
  }
  return y;  
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase

  if (p == NULL){
    return 0;
  }
  node_t *x;
  node_t *y = p;
  color_t y_original_color = y -> color;

  if (p -> left == t -> nil){
    x = p -> right;
    rbtree_transplant(t, p, p -> right);
  } else if (p -> right == t -> nil){
    x = p -> left;
    rbtree_transplant(t, p, p -> left);
  } else {
    y = rbtree_successor(t, p -> right);
    y_original_color = y -> color;
    x = y -> right;
    if (y != p -> right){
      rbtree_transplant(t, y, y -> right);
      y -> right = p -> right;
      y -> right -> parent = y;
    } else {
      x -> parent = y;
    }
    rbtree_transplant(t, p, y);
    y -> left = p -> left;
    y -> left -> parent = y;
    y -> color = p -> color;
  }

  if (y_original_color == RBTREE_BLACK){
    rbtree_delete_fixup(t, x);
  }
  
  free(p);
  p = NULL;
  
  return 0;
}

// 중위 순회
int inorder_tree_walk(node_t *p, key_t *arr, const rbtree *t, int i){
  if (p == t->nil) {
    return i;
  }
  i = inorder_tree_walk(p->left, arr, t, i);
  arr[i++] = p->key;
  i = inorder_tree_walk(p->right, arr, t, i);
  return i;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n){
  // TODO: implement to_array
  node_t *x = t->root;
  inorder_tree_walk(x, arr, t, 0); 
  return 0;
}

// void print_tree(rbtree* t, node_t* node, int level) {
//     if (node == t->nil)
//         return;
//     print_tree(t, node->right, level + 1);
//     for (int i = 0; i < level; i++) {
//         printf("    ");
//     }
//     printf("%d:%d\n", node->key, node->color);
//     print_tree(t, node->left, level + 1);
// }


// int main(int argc, char *argv[]) {
//     rbtree *p = new_rbtree();
//     p -> root = rbtree_insert(p, 8);
//     p -> root = rbtree_insert(p, 1);
//     p -> root = rbtree_insert(p, 11);
//     p -> root = rbtree_insert(p, 6);
//     p -> root = rbtree_insert(p, 15);
//     p -> root = rbtree_insert(p, 17);
//     p -> root = rbtree_insert(p, 25);
//     p -> root = rbtree_insert(p, 22);
//     p -> root = rbtree_insert(p, 13);
//     p -> root = rbtree_insert(p, 27);
    

//     node_t *min_node = rbtree_min(p);
//     node_t *max_node = rbtree_max(p);
//     printf("min_node : %d\n", min_node -> key);
//     printf("max_node : %d\n", max_node -> key);
//     print_tree(p, p -> root, 0);
//     rbtree_erase(p, p->root);
//     delete_rbtree(p);
//     printf("%p\n", p);
//     printf("delete\n");
//     // print_tree(p, p -> root, 0);
// }
