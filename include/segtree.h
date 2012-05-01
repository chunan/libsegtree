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
	void Load_segtree(char fname[]);
  void Save_segtree(string fname);
  // TODO: not implemented yet
  void ConstructTree(const Feature& feat);
  /* mutators */
	void SetHBsegTh(const double th) { h_bseg_th = th; }
  /* accessors */
  int StartT(int seg_idx) const;
  int EndT(int seg_idx) const;
	int Parent(int child_id) const;
	int Child(int parent_id, int which_child) const;
  int NumNode() const { return num_node; };
  int NumLeaf() const { return num_leaf; };
  int NumIntNode() const { return non_leaf; };
  float BsegTh() const { return bseg_th; }
  float HBsegTh() const { return h_bseg_th; }
	float MergeLoss(int seg_idx) const;
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

	float bseg_th;
	float h_bseg_th;
	float merge_std;

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
