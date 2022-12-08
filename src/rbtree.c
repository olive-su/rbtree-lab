#include "rbtree.h"

#include <stdlib.h>
#include <stdio.h>

/**
 * @todo initialize struct if needed
 * @brief [CREATE] RB tree 구조체 생성
 * @details root 노드 SENTINEL로 구현
 * 
 * @param void
 * @return rbtree* 생성한 구조체의 포인터
*/
rbtree *new_rbtree(void) 
{
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree)); 
  node_t *nil_node = (node_t *)calloc(1, sizeof(node_t));
  nil_node -> color = RBTREE_BLACK;
  p -> root = nil_node;
  p -> nil = nil_node;
  return p;
}

/**
 * @brief [INSERT] node x에 대한 left-rotation 연산 수행
 * @details - Related Function @n     - `rbtree_insert` @n     - `rbtree_erase`
 * 
 * @param rbtree* t 트리 포인터
 * @param node_t* x 노드 포인터 
 * @return void
*/
void left_rotate(rbtree *t, node_t *x) 
{
  node_t *y = x -> right;
  x -> right = y -> left; // y의 왼쪽 서브 트리를 x의 오른쪽 서브 트리로 옮긴다.
  if (y -> left != t -> nil) { // y의 왼쪽 서브 트리의 부모를 연결한다.
    y -> left -> parent = x;
  }
  y -> parent = x -> parent;  // x의 부모를 y로 연결한다.
  if (x -> parent == t -> nil){ // 트리 생성 후, 첫 노드 삽입
    t -> root = y;
  } else if (x == x -> parent -> left){ // x 노드가 부모 노드의 왼쪽 자식일 경우
    x -> parent -> left = y;
  } else { // x 노드가 부모 노드의 오른쪽 자식일 경우
    x -> parent -> right = y;
  }
  y -> left = x;  // x를 y의 왼쪽으로 놓는다.
  x -> parent = y;
}


/**
 * @brief [INSERT] node x에 대한 right-rotation 연산 수행
 * @details - Related Function @n     - `rbtree_insert` @n     - `rbtree_erase`
 * 
 * @param rbtree* t 트리 포인터
 * @param node_t* x 노드 포인터 
 * @return void
*/
void right_rotate(rbtree *t, node_t *x) 
{
  node_t *y = x -> left;
  x -> left = y -> right; // y의 오른쪽 서브 트리를 x의 왼쪽 서브 트리로 옮긴다.
  if (y -> right != t -> nil) {
    y -> right -> parent = x; // y 오른쪽 서브 트리의 부모에 x를 연결한다.
  }
  y -> parent = x -> parent; // y의 부모를 x로 연결한다.
  if (x -> parent == t -> nil){ // 트리 생성 후, 첫 노드 삽입
    t -> root = y;
  } else if (x == x -> parent -> right){ // x 노드가 부모 노드의 오른쪽 자식일 경우
    x -> parent -> right = y;
  } else { // x 노드가 부모 노드의 왼쪽 자식일 경우
    x -> parent -> left = y;
  }
  y -> right = x; // x를 y의 오른쪽으로 놓는다.
  x -> parent = y;
}

/**
 * @brief [INSERT] 새로 삽입하는 노드를 항상 RED로 지정함으로써 발생하는 규칙 위반 교정
 * @details - Related Function @n     - `rbtree_insert` @n     - `rotate_left` @n     - `rotate_right`
 * 
 * @param rbtree* t 트리 포인터
 * @param node_t* z 노드 포인터 
 * @return void
*/
void rbtree_insert_fixup(rbtree *t, node_t *z) 
{
  while (z -> parent -> color == RBTREE_RED){ 
    // 부모가 조부모의 왼쪽 자식일 때
    if (z -> parent == z -> parent -> parent -> left){ 
      node_t *y = z -> parent -> parent -> right; // y is uncle.
      
      // [CASE 1] : y is RED.
      if (y -> color == RBTREE_RED){ 
        z -> parent -> color = RBTREE_BLACK; // 부모 노드의 색을 블랙으로 바꾼다.
        y -> color = RBTREE_BLACK; // 삼촌 노드의 색을 블랙으로 바꾼다.
        z -> parent -> parent -> color = RBTREE_RED;
        z = z -> parent -> parent;
      } else { 
        // [CASE 2] : y is BLACK, z is right-child. 
        if (z == z -> parent -> right){
        z = z -> parent;
        left_rotate(t, z); // z가 오른쪽 자식이므로 왼쪽으로 회전을 수행한다.
        } 
        // [CASE 3] : y is BLACK, z is left-child. 
        z -> parent -> color = RBTREE_BLACK;
        z -> parent -> parent -> color = RBTREE_RED; 
        right_rotate(t, z -> parent -> parent); // z가 왼쪽 자식이므로 오른쪽으로 회전을 수행한다.
      } 
    }
    // 부모가 조부모의 오른쪽 자식일 때 (위의 로직과 동일)
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
  t -> root -> color = RBTREE_BLACK; // 특성 2(root : BLACK)에 대한 복구
}

/**
 * @brief [INSERT] RB Tree에 key 추가
 * @details - Related Function @n     - `rbtree_insert_fixup`
 * 
 * @param rbtree* t 트리 포인터
 * @param key_t key 삽입하려는 키 값 
 * @return node_t* 삽입한 노드의 포인터
*/
node_t *rbtree_insert(rbtree *t, const key_t key) 
{
  node_t *z = (node_t *)calloc(1, sizeof(node_t)); // key값을 갖는 노드를 새로 만든다.
  z -> key = key; // node z : key 확정

  node_t *y = t -> nil;
  node_t *x = t -> root;

  while(x != t -> nil){ // 리프의 부모 노드(NIL 직전 노드)까지 내려간다. (level : int(log N))
    y = x;
    if(z -> key < x -> key){
      x = x -> left;
    } else{
      x = x -> right;
    }
  }
  z -> parent = y; // 부모 노드 지정

  if (y == t -> nil){ // 첫 번째 원소 삽입
    t -> root = z;
  } else if (z -> key < y -> key){ // 리프의 부모 노드가 key보다 작으면 왼쪽 자식으로 삽입
    y -> left = z; 
  } else { // 리프의 부모 노드가 key보다 크거나 같으면 오른쪽 자식으로 삽입
    y -> right = z;
  }

  z -> left = t -> nil;
  z -> right = t -> nil;
  z -> color = RBTREE_RED; // INSERT 규칙 적용 "새 노드 z를 삽입하고 해당 노드의 색상을 RED로 지정한다."

  rbtree_insert_fixup(t, z);

  return t -> root;
}

/**
 * @todo implement find
 * @brief [FIND] BST-SEARCH
 * 
 * @param rbtree* t 트리 포인터 
 * @param key_t key 찾으려는 키 값
 * @return node_t* 찾은 노드의 포인터
 */
node_t *rbtree_find(const rbtree *t, const key_t key) 
{
  node_t *x = t -> root; // 시작 노드를 루트로 지정

  while (x != t -> nil && key != x -> key){
    if (key < x -> key){ // 찾으려는 키가 현재 노드의 키 값보다 작으면 왼쪽 자식 탐색
      x = x -> left;
    } else {
      x = x -> right; // 찾으려는 키가 현재 노드의 키 값보다 크면 오른쪽 자식 탐색
    }
  }
  if (x == t -> nil) { // 만약 찾으려는 key가 트리에 없으면 NULL 리턴
    return NULL;
  }
  return x;
}

/**
 * @todo implement find
 * @brief [MIN] RB Tree에서 가장 작은 값을 반환
 * 
 * @param rbtree* t 트리 포인터 
 * @return node_t* 찾은 노드의 포인터
 */
node_t *rbtree_min(const rbtree *t) 
{
  node_t *x = t -> root; // 시작 노드를 루트로 지정 

  while (x -> left != t -> nil)
    x = x -> left; // 왼쪽 자식을 순환적으로 반복 탐색하며 가장 작은 값 탐색
  return x;
}

/**
 * @todo implement find
 * @brief [MAX] RB Tree에서 가장 큰 값을 반환
 * 
 * @param rbtree* t 트리 포인터 
 * @return node_t* 찾은 노드의 포인터
 */
node_t *rbtree_max(const rbtree *t) 
{
  node_t *x = t -> root; // 시작 노드를 루트로 지정
  
  while (x -> right != t -> nil)
    x = x -> right; // 오른쪽 자식을 순환적으로 반복 탐색하며 가장 큰 값 탐색
  return x;
}

/**
 * @brief [ERASE] 노드 u가 루트인 서브 트리(A)를 노드 v가 루트인 서브 트리(B)로 교체
 * @details - Related Function @n     - `rbtree_erase`
 * 
 * @param rbtree* t 트리 포인터 
 * @param node_t* u 서브 트리 A의 루트 노드 
 * @param node_t* v 서브 트리 B의 루트 노드  
 * @return void
 */
void rbtree_transplant(rbtree *t, node_t *u, node_t *v)
{
  if (u -> parent == t -> nil){ // u가 루트 노드일 떄
    t -> root = v;
  } else if (u == u -> parent -> left) { // u가 왼쪽 자식일 때
    u -> parent -> left = v;
  } else {
    u -> parent -> right = v; // u가 오른쪽 자식일 때
  }

  v -> parent = u -> parent; // v의 부모 연결
}

/**
 * @brief [ERASE] 블랙 노드를 삭제함으로써 발생하는 규칙 위반 교정 
 * @details - Related Function @n     - `rbtree_erase`
 * 
 * @param rbtree* t 트리 포인터 
 * @param node_t* x 삭제한 노드
 * @return void
 */
void rbtree_erase_fixup(rbtree *t, node_t *x)
{
  node_t *w;

  while (x != t -> root && x -> color == RBTREE_BLACK){
    if (x == x -> parent -> left) { // x가 왼쪽 자식일 때
      w = x -> parent -> right; // w is sibling.
      // [CASE 1] w is RED.
      if (w -> color == RBTREE_RED){
        w -> color = RBTREE_BLACK; // w와 p의 색을 바꾼다.
        x -> parent -> color = RBTREE_RED;
        left_rotate(t, x -> parent); // p를 기준으로 left-rotate를 수행한다.
        w = x -> parent -> right;
      }
      // [CASE 2] w is BLACK, w's children are BLACK.
      if (w -> left -> color == RBTREE_BLACK && w -> right -> color == RBTREE_BLACK){
        w -> color = RBTREE_RED; // w를 레드로 바꾼다.
        x = x -> parent;
      } else {
        // [CASE 3] w is BLACK, w's left-child is RED, w's right-child is BLACK.
        if (w -> right -> color == RBTREE_BLACK) {
          w -> left -> color = RBTREE_BLACK; // w와 w의 왼쪽 자식의 색을 바꾼다.
          w -> color = RBTREE_RED;
          right_rotate(t, w); // w를 기준으로 right-rotate를 수행한다.
          w = x -> parent -> right;
        }
        // [CASE 4]
        w -> color = x -> parent -> color; // w와 p의 색을 바꾼다.
        x -> parent -> color = RBTREE_BLACK;
        w -> right -> color = RBTREE_BLACK; // p를 기준으로 left-rotate를 수행한다.
        left_rotate(t, x -> parent);
        x = t -> root;
      }
    } else { // x가 오른쪽 자식일 때 (위의 로직과 동일)
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

/**
 * @brief [ERASE] 노드 x의 직후 원소(successor) 탐색
 * @details - Related Function @n     - `rbtree_erase`
 * 
 * @param rbtree* t 트리 포인터 
 * @param node_t* x successor를 찾고자하는 노드
 * @return node_t* x의 successor
 */
node_t *rbtree_successor(rbtree *t, node_t *x)
{
  node_t *y = x -> right; // y는 x의 오른쪽 자식

  while (y -> left != t -> nil) { // 리프 이전까지 반복
  y = y -> left; // y에 y의 왼쪽 자식을 연속해서 갱신
  }
  return y;  
}

/**
 * @todo implement erase
 * @brief [ERASE] 트리에서 특정 노드 삭제
 * @details - Related Function @n     - `rbtree_erase_fixup` @n     - `rbtree_successor` @n     - `rbtree_transplant`
 * 
 * @param rbtree* t 트리 포인터
 * @param node_t* p 노드 포인터 
 * @return int 성공 시 0 리턴
 */
int rbtree_erase(rbtree *t, node_t *p) 
{
  if (p == NULL){
    return 0;
  }
  node_t *x; // y의 원래 위치로 이동하는 노드
  node_t *y = p; // y를 삭제하려는 노드 p로 지정한다.
  color_t y_original_color = y -> color; // CASE 1. 삭제색 : 삭제되는 노드의 색상

  // [CASE 1] p가 하나 이하의 자식을 가지고 있을 때 
  // 단순히 하나 또는 nil(자식이 없음) 서브 트리의 레벨을 현재 삭제하려는 노드 위치로 올린다.
  if (p -> left == t -> nil){
    x = p -> right;
    rbtree_transplant(t, p, p -> right);
  } else if (p -> right == t -> nil){
    x = p -> left;
    rbtree_transplant(t, p, p -> left);

  // [CASE 2] p가 두 개의 자식을 가지고 있을 때 
  } else {
    y = rbtree_successor(t, p); // p가 삭제되고 p의 위치로 올 노드
    y_original_color = y -> color; // CASE 2. 삭제색 : successor의 색상
    x = y -> right; // y의 오른쪽 자식
    if (y == p -> right){ // p의 오른쪽 자식이 왼쪽 서브 트리를 가지고 있지않은 경우
      x -> parent = y;
    } else { // p의 오른쪽 자식이 왼쪽 서브 트리를 가지고 있는 경우
      rbtree_transplant(t, y, y -> right); // successor의 서브 트리를 분리한다.
      y -> right = p -> right; 
      y -> right -> parent = y;
    }
    rbtree_transplant(t, p, y); // 삭제하려는 노드의 오른쪽 자식으로 successor를 붙인다.
    y -> left = p -> left;
    y -> left -> parent = y;
    y -> color = p -> color;
  }

  if (y_original_color == RBTREE_BLACK){ // 삭제되는 색이 블랙이면 재조정을 한다.
    rbtree_erase_fixup(t, x);
  }
  
  free(p); // p에 할당되어있던 메모리를 해제해준다.
  p = NULL; // dangling pointer를 방지하기 위해 해제된 포인터 값으로 NULL을 넣어준다.
  
  return 0;
}

/**
 * @brief [DELETE] RB tree 구조체의 내부 노드들 메모리 반환
 * @details - Related Function @n     - `delete_rbtree`
 * 
 * @param rbtree* t 트리 포인터
 * @param node_t* x 노드 포인터
 * @return void
*/
void delete_inner_node(rbtree *t, node_t *x)
{
  if (x != t -> nil){
    delete_inner_node(t, x -> left);  // 왼쪽 자식들 모두 제거
    delete_inner_node(t, x -> right); // 오른쪽 자식들 모두 제거
    
    free(x); // 단말 노드 : 메모리 해제
    x = NULL;
  }
}

/**
 * @todo reclaim the tree nodes's memory
 * @brief [DELETE] RB tree 구조체 자체의 메모리 반환
 * @details - Related Function @n     - `delete_inner_node`
 * 
 * @param rbtree* t 트리 포인터
 * @return void
*/
void delete_rbtree(rbtree *t) 
{
  delete_inner_node(t, t -> root); // 연결된 내부 노드들 모두 제거
  free(t -> nil);   // 내부 노드 모두 삭제 후, NIL 노드 해제
  t -> nil = NULL;
  free(t);          // RB tree 구조체 자체 해제
  t = NULL;
}

/**
 * @brief inorder traversal
 * @details - Related Function @n     - `rbtree_to_array'
 * 
 * @param node_t* p 루트 노드  
 * @param key_t* arr 중위 순회 결과를 담는 배열 
 * @param rbtree* t 트리 포인터
 * @param int i 배열 인덱스
 * @return int 배열 인덱스
 */
int inorder_tree_walk(node_t *p, key_t *arr, const rbtree *t, int i)
{
  if (p == t -> nil) {
    return i;
  }
  i = inorder_tree_walk(p -> left, arr, t, i);
  arr[i++] = p -> key; // 인덱스를 하나씩 올려가며 중위 순회 결과를 배열에 담는다.
  i = inorder_tree_walk(p -> right, arr, t, i);
  return i;
}

/**
 * @todo implement to_array
 * 
 * @param rbtree* t 트리 포인터
 * @param key_t* arr 중위 순회 결과를 담는 배열 
 * @param size_t n 배열 크기
 * @return int 성공 시 0 리턴
 */
int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n)
{
  node_t *x = t->root;
  inorder_tree_walk(x, arr, t, 0); 
  return 0;
}
