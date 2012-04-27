#ifndef __SEGTREE_H__
#define __SEGTREE_H__

#include "ugoc_utility.h"

class SegTree{
	public:
	SegTree() { Init(); }
	SegTree(char * const fname) { Load_segtree(fname); }
	~SegTree(){ Free(); }
	void Init();
	void Free();
	void Load_segtree(char fname[]);
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
	// Data
	int *start_t;    // start_t[non_leaf]
	int *end_t;      // end_t[non_leaf]
	int *parent;     // parent[num_node]
	int **child;     // child[non_leaf]
	float *merge_loss; // merge_loss[non_leaf]

	float bseg_th;
	float h_bseg_th;
	float merge_std;

	int node_size;
  int non_leaf_size;

	int num_node;
	int non_leaf;
	int num_leaf;
  int num_child;
	int child_array_size;
};
#endif
