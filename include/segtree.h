#ifndef __SEGTREE_H__
#define __SEGTREE_H__

#include "ugoc_utility.h"
#include "feature.h"

class SegTree{
  public:
    SegTree() { Init(); }
    SegTree(char * const fname) { Load_segtree(fname); }
    ~SegTree(){ Free(); }
    void Init();
    void Free();
    void Load_segtree(string fname);
    void Save_segtree(string fname);
    void GetBasicSeg(vector<int>* index, float threshold);
    // TODO: not implemented yet
    void ConstructTree(const Feature& feat);
    /* accessors */
    int StartT(int seg_idx) const;
    int EndT(int seg_idx) const;
    int Parent(int child_id) const;
    int Child(int parent_id, int which_child) const;
    int NumNode() const { return num_node; };
    int NumLeaf() const { return num_leaf; };
    int NumIntNode() const { return non_leaf; };
    float MergeLoss(int seg_idx) const;
    float MergeMean() const { return merge_mean; }
    float MergeStd() const { return merge_std; }
    void DumpData() const;

  private:
    void Reallocate(int n_node);
    // Data
    int *start_t;    // start_t[non_leaf]
    int *end_t;      // end_t[non_leaf]
    int *parent;     // parent[num_node]
    int **child;     // child[non_leaf]
    float *merge_loss; // merge_loss[non_leaf]

    float merge_std;
    float merge_mean;

    int num_node;
    int non_leaf;
    int num_leaf; // number of frames

    /*
     * node_size:
     *   record the size of start_t, end_t, parent
     * non_leaf_size, num_child, child_array_size:
     *   record the child 2d array size
     *   (non_leaf_size * num_child)  <= child_array_size
     *
     * They are used to record actural memory size because SegTree
     * allows to be reloaded. See SegTree::Load_segtree() for detail.
     */
    int node_size;
    int non_leaf_size;
    int num_child; // 2
    int child_array_size;

};
#endif
