# Rope

The rope data structure is used to efficiently store and manipulate strings or texts of arbitrary length.
It is a type of binary tree where each leave holds a part of the text.
This allows for faster insertion and erasure compared to normal strings when the length is growing.

# Performance

| Operation | Rope | String |
|-|-|-|
| Index | O(log n) | O(1) |
| Append | O(log n) | O(n) |
| Split | O(log n) | O(1) |
| Insert | O(log n) | O(n) |
| Erase | O(log n) | O(n) |

# TODO

- [ ] Basic operations:
  - [x] Index
  - [x] Append
  - [x] Split
  - [x] Insert
  - [x] Erase
  - [ ] Iterator
- [ ] Tree rebalancing 
- [ ] Fixed size leaf nodes
- [ ] Own allocator for leaf nodes
- [ ] Benchmarks