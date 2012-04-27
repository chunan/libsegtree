#include <iostream>
#include <vector>
#include <cassert>
#include "segtree.h"

const int sizeInt = sizeof(int);
const int sizeFloat = sizeof(float);

void SegTree::Init() {/*{{{*/
  start_t   = NULL;
  end_t     = NULL;
  parent    = NULL;
  child     = NULL;
  merge_loss  = NULL;
  bseg_th   = 0.0;
  h_bseg_th = 0.0;
  merge_std = 0.0;
  num_node  = 0;
  node_size  = 0;
  non_leaf  = 0;
  num_leaf  = 0;
  num_child = 0;
  child_array_size = 0;
}/*}}}*/

void SegTree::Free() {/*{{{*/
  if (Free_1d_array(start_t)) {
    end_t = NULL;
    parent = NULL;
  }
  mem_op<int>::delete_2d_array(&child);
  Free_1d_array(merge_loss);
  bseg_th   = 0.0;
  h_bseg_th = 0.0;
  merge_std = 0.0;
  node_size  = 0;
  non_leaf  = 0;
  num_leaf  = 0;
  num_child = 0;
  child_array_size = 0;
}/*}}}*/

void SegTree::Load_segtree(char fname[]) {/*{{{*/
  FILE *fd = FOPEN(fname,"r");
  size_t ret;
  /* num_node, non_leaf, num_leaf, child */
  ret = fread(&num_node,sizeInt,1,fd);
  assert(ret == 1);
  num_leaf = (num_node + 1) / 2;
  non_leaf = num_node - num_leaf;
  mem_op<int>::reallocate_2d_array(&child, non_leaf, 2,
                                   &non_leaf_size,
                                   &num_child,
                                   &child_array_size);

  if (num_node > node_size) { /* Reallocat Mem */
    /* start_t, end_t, parents */
    Free_1d_array(start_t);
    start_t = new int [2 * non_leaf + num_node];
    /* merge loss */
    Free_1d_array(merge_loss);
    merge_loss = new float[non_leaf];
    node_size = num_node;
  }
  end_t = start_t + non_leaf;
  parent = end_t + non_leaf;

  ret = fread(start_t, sizeInt, non_leaf, fd);
  assert(ret == static_cast<unsigned>(non_leaf));
  ret = fread(end_t, sizeInt, non_leaf, fd);
  assert(ret == static_cast<unsigned>(non_leaf));
  ret = fread(parent, sizeInt, num_node, fd);
  assert(ret == static_cast<unsigned>(num_node));
  ret = fread(child[0], sizeInt, 2 * non_leaf, fd);
  assert(ret == 2 * static_cast<unsigned>(non_leaf));
  ret = fread(merge_loss, sizeFloat, non_leaf, fd);
  assert(ret == static_cast<unsigned>(non_leaf));
  ret = fread(&bseg_th, sizeFloat, 1, fd);
  assert(ret == 1);
  ret = fread(&merge_std, sizeFloat, 1, fd);
  assert(ret == 1);
  fclose(fd);
}/*}}}*/

void SegTree::DumpData() const {/*{{{*/
  printf("Parent: \n");
  for (int i = 0; i < num_node; ++i) {
    printf("Parent[%3d] = %3d\n", i, Parent(i));
  }
  printf("Children: \n");
  for (int i = NumLeaf(); i < NumNode(); ++i) {
    printf("Child[%3d] = {%3d, %3d}\n", i, Child(i, 0), Child(i, 1));
  }


}/*}}}*/

int SegTree::StartT(int seg_idx) const {/*{{{*/
  assert(seg_idx >= 0 && seg_idx < num_node);
  if (seg_idx < num_leaf) {
    return seg_idx;
  } else {
    return start_t[seg_idx - num_leaf];
  }
}/*}}}*/

int SegTree::EndT(int seg_idx) const {/*{{{*/
  assert(seg_idx >= 0 && seg_idx < num_node);
  if (seg_idx < num_leaf) {
    return seg_idx;
  } else {
    return end_t[seg_idx - num_leaf];
  }
}/*}}}*/

int SegTree::Parent(int child_id) const {/*{{{*/
  assert(child_id >= 0 && child_id < num_node);
  return (child_id != num_node - 1) ? parent[child_id] : -1;
}/*}}}*/

int SegTree::Child(int parent_id, int which_child) const {/*{{{*/
  assert(parent_id >= 0 && parent_id < num_node);
  return (parent_id < num_leaf) ? -1 : child[parent_id - num_leaf][which_child];
}/*}}}*/

float SegTree::MergeLoss(int seg_idx) const {/*{{{*/
  assert(seg_idx >= 0 && seg_idx < num_node);
  return (seg_idx < num_leaf) ? 0.0 : merge_loss[seg_idx - num_leaf];
}/*}}}*/
