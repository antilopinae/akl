#include "akl/atomic.hpp"

void f() {
    akl::atomic<int> atom;
    atom.exchange(1);

}


