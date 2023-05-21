#ifndef BIN_TREE_H
#define BIN_TREE_H

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

typedef struct Node {
  size_t r_index ;
  size_t l_index ;
  size_t val ;
  bool populated ;
}Node;

typedef struct Tree {
  size_t size;
  size_t capacity;
  Node *nodes;
}Tree;

void tree_init(Tree* t, size_t sz) {
  t->capacity = sz;
  t->size = 0;
  t->nodes = (Node*)malloc(t->capacity * sizeof(Node));
  for(int i = 0; i < t->capacity; i++) {
    t->nodes[i].populated = false;
    t->nodes[i].r_index = SIZE_MAX;
    t->nodes[i].l_index = SIZE_MAX;
    t->nodes[i].val = 0;
  }
}
int tree_insert(Tree *t, size_t val) {

  int i = 0;
  size_t prev;
  if(!t->nodes[0].populated) {
    t->nodes[0].populated = true;
    t->nodes[0].val = val;
    t->size++;
    return 0;
  }

  while(i < t->capacity) {
    if(val > t->nodes[i].val) {
      if(t->nodes[t->nodes[i].r_index].populated){
        i = t->nodes[i].r_index;
        continue;
      } else {
        int j = 0;
        while(j < t->capacity) {
          if(!t->nodes[j].populated) {
            t->nodes[j].val = val;
            t->nodes[j].populated = true;
            t->nodes[i].r_index = j;
            t->size++;
            return j;
          }
          j++;
        }
      }
    }else if(val <= t->nodes[i].val) {
      if(t->nodes[t->nodes[i].l_index].populated) {
        i = t->nodes[i].l_index;
        continue;
      } else {
        int j = 0;
        while(j < t->capacity) {
          if(!t->nodes[j].populated) {
            t->nodes[j].val = val;
            t->nodes[j].populated = true;
            t->nodes[i].l_index = j;
            t->size++;
            return j;
          }
          j++;
        }
      }
    }
  }
  return -1;
}

void tree_print_inorder(Tree tree, size_t idx) {
  size_t i = idx;
  if(tree.nodes[i].l_index && tree.nodes[tree.nodes[i].l_index].populated == true) {
    tree_print_inorder(tree, tree.nodes[i].l_index);
  }
  if(tree.nodes[i].r_index && tree.nodes[tree.nodes[i].r_index].populated == true) {
    tree_print_inorder(tree, tree.nodes[i].r_index);
  }
  if(tree.nodes[i].populated == true) {
    printf("idx: %ld, val : %ld, lptr : %ld, rptr : %ld\n", i, tree.nodes[i].val, tree.nodes[i].l_index, tree.nodes[i].r_index);
  }
  return;
}

void tree_height(Tree tree, size_t idx, size_t h, size_t *mx) {

  if(tree.nodes[idx].l_index && tree.nodes[tree.nodes[idx].l_index].populated == true) {
    h++;
    if(h > *mx) *mx = h;
    printf("left : cur h = %ld\n", h);
    tree_height(tree, tree.nodes[idx].l_index, h, mx);
  }
  if(tree.nodes[idx].r_index && tree.nodes[tree.nodes[idx].r_index].populated == true) {
    h++;
    if(h > *mx) *mx = h;
    tree_height(tree, tree.nodes[idx].r_index, h, mx);
  }
  return;
  
}


#define TREE_LEFT(x) t->nodes[t->nodes[x].l_index]
#define TREE_RIGHT(x) t->nodes[t->nodes[x].l_index]
#define TREE_CUR(x) t->nodes[x]

size_t tree_min(Tree *t, size_t n) {

  while(t->nodes[n].populated) {
    size_t left = t->nodes[n].l_index;
    if(t->nodes[left].populated && t->nodes[n].l_index != 0) {
      n = left;
    }
    else return n;
  }
  return n;
}

int tree_delete(Tree *t, size_t val) {
  size_t i = 0;
  size_t prev = 0;
  uint8_t prev_lr = false;
  while(t->nodes[i].populated) {
    if(t->nodes[i].val == val) {
      // do the delete
      if(TREE_LEFT(i).populated && !TREE_RIGHT(i).populated) {
        TREE_LEFT(i).populated = false;
        TREE_CUR(i).l_index = TREE_LEFT(i).l_index;
        TREE_CUR(i).r_index = TREE_LEFT(i).r_index;
        TREE_CUR(i).val = val;
      } 
      else if(TREE_RIGHT(i).populated && !TREE_LEFT(i).populated) {
        TREE_RIGHT(i).populated = false;
        TREE_CUR(i).r_index = TREE_RIGHT(i).r_index;
        TREE_CUR(i).l_index = TREE_LEFT(i).l_index;
        TREE_CUR(i).val = val;
      } 
      else if(!TREE_RIGHT(i).populated && !TREE_LEFT(i).populated) {
        TREE_CUR(i).populated = false;
      } 
      else {
        // FIND THE MINIMUM OF THE TREE_RIGHT CHILD TO GET NEW VAL
        size_t min = tree_min(t, t->nodes[i].r_index);
        t->nodes[i].val = t->nodes[min].val;
        t->nodes[min].populated = false;
        if(prev_lr == 1) t->nodes[prev].l_index = 0;
        else if(prev_lr == 2) t->nodes[prev].r_index = 0;
      }
      return 0;
    } else if(t->nodes[i].val > val) {
      prev = i;
      prev_lr = 1;
      if(TREE_LEFT(i).populated) i = t->nodes[i].l_index;
      else return -1;
    } else if(t->nodes[i].val <= val) {
      prev = i;
      prev_lr = 2;
      if(TREE_RIGHT(i).populated) i = t->nodes[i].r_index;
      else return -1;
    } else if(!t->nodes->populated) {
      return -1;
    }
  }

  return 0;
}

size_t tree_retrieve(Tree *t, size_t s) {

  int i = 0;
  while(t->nodes[i].populated) {
    if(t->nodes[i].val < s) i = t->nodes[i].r_index;
    else if(t->nodes[i].val == s) return i;
    else i = t->nodes[i].l_index;
  }
  return SIZE_MAX;
}

#endif
