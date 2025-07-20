#include <cstdint>
#include <vector>
#include <unordered_map>
#include <cassert>
#include <iostream>
#include "./mode.h"

using namespace std;


// basic 1st test

struct e_t {
   int id;
   uint64_t k;
};

using src_t = std::vector<e_t>;
using mclct_t = mode_collector<int, uint64_t>;

void collect(mclct_t& mc, const src_t& dt_in)
{
   for (const auto& e : dt_in) {
       mc.add(e.id, e.k);
   }
}

int main()
{
   mclct_t mc{7, 5};
   src_t data = {
       {10, 100},
       {11, 200},
       {12, 150},
       {13, 100},
       {14, 100}
   };

   collect(mc, data);

   // Find the mode
   const auto mode = mc.find_mode();
   assert(mode.tag == mclct_t::mode_status_t::authorative_value);
        assert(mode.key == 100);
   cout << "Mode found: key = " << mode.key
        << ", id = " << mode.id
        << ", count = " << mode.count << endl;

   return 0;


}


