#include "rbtree.h"

#include <stdlib.h>

/**
 * @todo initialize struct if needed
 * @brief [CREATE] RB tree 구조체 생성
 * @details root 노드 SENTINEL로 구현
 * @param void
 * @return rbtree* 생성한 구조체의 포인터
*/
rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree)); 
  node_t *nil_node = (node_t *)calloc(1, sizeof(node_t));
  nil_node -> color = RBTREE_BLACK;
  p -> root = nil_node;
  p -> nil = nil_node;
  return p;
}

/**
 * @brief [DELETE] RB tree 구조체의 내부 노드들 메모리 반환
 * @details - Related Function @n     - `delete_rbtree`
 * @param rbtree* t 트리 포인터
 * @param node_t* x 노드 포인터
 * @return void
*/
void delete_inner_node(rbtree *t, node_t *x){
  if (x != t -> nil){
    delete_inner_node(t, x -> left);  // 왼쪽 자식 노드들 모두 제거
    delete_inner_node(t, x -> right); // 오른쪽 자식 노드들 모두 제거
    
    free(x); // 단말 노드 : 메모리 해제
    x = NULL;
  }
}

/**
 * @todo reclaim the tree nodes's memory
 * @brief [DELETE] RB tree 구조체 자체의 메모리 반환
 * @details - Related Function @n     - `delete_inner_node`
 * @param rbtree* t 트리 포인터
 * @return void
*/
void delete_rbtree(rbtree *t) {
  delete_inner_node(t, t -> root); // 연결된 내부 노드들 모두 제거
  free(t -> nil);   // 내부 노드 모두 삭제 후, NIL 노드 해제
  t -> nil = NULL;
  free(t);          // RB tree 구조체 자체 해제
  t = NULL;
}

/**
 * @brief [INSERT] node x에 대한 left-rotation 연산 수행
 * @details - Related Function @n     - `rbtree_insert`
 * @param rbtree* t 트리 포인터
 * @param node_t* x 노드 포인터 
 * @return void
*/
void left_rotate(rbtree *t, node_t *x) {
  node_t *y = x -> right;
  x -> right = y -> left; // y의 왼쪽 서브 트리를 x의 오른쪽 서브 트리로 옮긴다.
  if (y -> left != t -> nil) {
    y -> left -> parent = x;
  }
  y -> parent = x -> parent;  // x의 부모를 y로 연결한다.
  if (x -> parent == t -> nil){
    t -> root = y;
  } else if (x == x -> parent -> left){
    x -> parent -> left = y;
  } else {
    x -> parent -> right = y;
  }
  y -> left = x;  // x를 y의 왼쪽으로 놓는다.
  x -> parent = y;
}


/**
 * @brief [INSERT] node x에 대한 right-rotation 연산 수행
 * @details - Related Function @n     - `rbtree_insert`
 * @param rbtree* t 트리 포인터
 * @param node_t* x 노드 포인터 
 * @return void
*/
void right_rotate(rbtree *t, node_t *x) {
  node_t *y = x -> left;
  x -> left = y -> right; // y의 오른쪽 서브 트리를 x의 왼쪽 서브 트리로 옮긴다.
  if (y -> right != t -> nil) {
    y -> right -> parent = x;
  }
  y -> parent = x -> parent; // y의 부모를 x로 연결한다.
  if (x -> parent == t -> nil){
    t -> root = y;
  } else if (x == x -> parent -> right){
    x -> parent -> right = y;
  } else {
    x -> parent -> left = y;
  }
  y -> right = x; // x를 y의 오른쪽으로 놓는다.
  x -> parent = y;
}

/**
 * @brief [INSERT] 새로 삽입하는 노드를 항상 RED로 지정함으로써 발생하는 위반된 규칙 교정
 * @details - Related Function @n     - `rbtree_insert`
 * @param rbtree* t 트리 포인터
 * @param node_t* x 노드 포인터 
 * @return void
*/
void rbtree_insert_fixup(rbtree *t, node_t *z) {
  while (z -> parent -> color == RBTREE_RED){
    if (z -> parent == z -> parent -> parent -> left){
      node_t *y = z -> parent -> parent -> right;
      
      if (y -> color == RBTREE_RED){ // case 1
        z -> parent -> color = RBTREE_BLACK;
        y -> color = RBTREE_BLACK;
        z -> parent -> parent -> color = RBTREE_RED;
        z = z -> parent -> parent;
      } else { 
        if (z == z -> parent -> right){ // case 2
        z = z -> parent;
        left_rotate(t, z);
        } // case 3
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

/**
 * @brief [INSERT] RB Tree에 key 추가
 * @details
 * @param rbtree* t 트리 포인터
 * @param node_t* x 노드 포인터  
 * @return void
*/
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

