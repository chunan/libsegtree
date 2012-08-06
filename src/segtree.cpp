#include <iostream>
#include <vector>
#include <cassert>
#include <list>
#include <cmath>
#include "segtree.h"

const int sizeInt = sizeof(int);
const int sizeFloat = sizeof(float);

void SegTree::Init() {/*{{{*/
  start_t   = NULL;
  end_t     = NULL;
  parent    = NULL;
  child     = NULL;
  merge_loss  = NULL;
  merge_mean = 0.0;
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
  merge_mean = 0.0;
  merge_std = 0.0;
  node_size  = 0;
  non_leaf  = 0;
  num_leaf  = 0;
  num_child = 0;
  child_array_size = 0;
}/*}}}*/

void SegTree::Reallocate(int n_node) {/*{{{*/
  num_node = n_node;
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
}/*}}}*/

void SegTree::Load_segtree(string fname) {/*{{{*/
  FILE *fd = FOPEN(fname.c_str(),"r");
  size_t ret;
  int n_node;
  /* num_node, non_leaf, num_leaf, child */
  ret = fread(&n_node,sizeInt,1,fd);
  assert(ret == 1);
  Reallocate(n_node);

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
  int bseg_th;
  ret = fread(&bseg_th, sizeFloat, 1, fd);
  assert(ret == 1);
  ret = fread(&merge_std, sizeFloat, 1, fd);
  assert(ret == 1);
  fclose(fd);

  //cout << "file std = " << merge_std << endl;

  /* Calculate Mean & Std */
  double sum = 0.0;
  double sqr_sum = 0.0;
  for (int i = 0; i < non_leaf; ++i) {
    sum += merge_loss[i];
    sqr_sum += merge_loss[i] * merge_loss[i];
  }
  merge_mean = sum / non_leaf;
  merge_std = sqrt(sqr_sum / non_leaf - merge_mean * merge_mean);

  int n_merge = non_leaf;
  double cutoff = merge_mean + 10.0 * merge_std;
  for (int i = non_leaf - 1; i >= 0; --i) {
    if (merge_loss[i] > cutoff) {
      n_merge--;
      sum -= merge_loss[i];
      sqr_sum -= merge_loss[i] * merge_loss[i];
    }
  }
  merge_mean = sum / n_merge;
  merge_std = sqrt(sqr_sum / n_merge - merge_mean * merge_mean);

  //cout << "new std  = " << merge_std << endl;


}/*}}}*/

void SegTree::ConstructTree(const Feature& feat) {
  Reallocate(2 * feat.LT() - 1);
}

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
  if (seg_idx < 0) {
    cerr << "SegTree::StartT(" << seg_idx << "): out of bound\n";
  }
  if (seg_idx >= num_node) {
    cerr << "SegTree::StartT(" << seg_idx << "): out of bound, num_node = "
        << num_node << "\n";
  }
  assert(seg_idx >= 0 && seg_idx < num_node);
  if (seg_idx < num_leaf) {
    return seg_idx;
  } else {
    return start_t[seg_idx - num_leaf];
  }
}/*}}}*/

int SegTree::EndT(int seg_idx) const {/*{{{*/
  if (seg_idx < 0) {
    cerr << "SegTree::StartT(" << seg_idx << "): out of bound\n";
  }
  if (seg_idx >= num_node) {
    cerr << "SegTree::StartT(" << seg_idx << "): out of bound, num_node = "
        << num_node << "\n";
  }
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

void SegTree::GetBasicSeg(vector<int>* index, float threshold) {
  index->clear();
  std::list<int> nodes_to_visit;
  nodes_to_visit.push_front(num_node - 1);

  while (!nodes_to_visit.empty()) {
    int visiting = nodes_to_visit.front();
    nodes_to_visit.pop_front();
    if (MergeLoss(visiting) <= threshold) {
      index->push_back(visiting);
    } else {
      nodes_to_visit.push_front(child[visiting - num_leaf][1]);
      nodes_to_visit.push_front(child[visiting - num_leaf][0]);
    }
  }
}
