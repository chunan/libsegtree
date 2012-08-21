#include "ugoc_utility.h"
#include "feature.h"
#include "segtree.h"


int main() {

  string filename = "data/N200108011200-09-14.fbank";

  /* Feature */
  DenseFeature feat(filename);
  feat.DumpData();

  /* HAC */
  SegTree segtree;
  segtree.ConstructTree(feat);
  segtree.DumpData();

  /* Acoustic segments */
  vector<int> as_index;
  segtree.GetBasicSeg(&as_index, segtree.MergeMean() + 0.5 * segtree.MergeStd());
  cout << "Acoustic Segments:\n";
  for (unsigned i = 0; i < as_index.size(); ++i) {
    cout << setw(3) << as_index[i] << ": "
      << segtree.StartT(as_index[i]) << " - " << segtree.EndT(as_index[i])
      << endl;
  }

  return 0;
}
