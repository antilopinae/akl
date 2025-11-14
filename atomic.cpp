#include "akl/atomic.hpp"

void TestAtomic() {
    akl::atomic<int> atom{1};
    atom.exchange(1);
    atom.dec();
    atom.dec_ret_last();
    atom.inc();
    atom.inc_ret_last();
    atom.operator++();
    atom.operator--();
    atom.operator+=(5);
    atom.operator-=(5);
}
