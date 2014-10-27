Modified version of the program seen in lesson, using hash tables instead of arrays in order
to reduce memory footprint.
For implementing hash tables the UThash library (http://troydhanson.github.io/uthash/) is used,
the reasons for this choice were:
- Efficiency (hash tables generally have O(1) average time cost);
- Ease of use of the library (since every operation is done through an opaque macro).\n
In order to increase modularity a new source file and related header have been created.

