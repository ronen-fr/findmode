#include <cstdint>
#include <vector>
#include <unordered_map>
#include <cassert>
#include <iostream>
#include <string>
#include <string_view>
#include "./mode.h"

using namespace std;
using namespace std::string_literals;

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


void list_res(const mclct_t::res_type_t& res, string nm)
{
   const string_view nm_tag = [](mclct_t::mode_status_t t) {
       switch (t) {
           case mclct_t::mode_status_t::no_mode_value:
               return "..no mode.....";
           case mclct_t::mode_status_t::mode_value:
               return "..mode_value..";
           case mclct_t::mode_status_t::authorative_value:
               return "..autho.......";
           default:
               return "unknown";
       }
   }(res.tag);
   cout << fmt::format("{}: mode result: tag:{},\tkey:{}, id:{}, count:{}\n",
                       nm, nm_tag, res.key, res.id, res.count);
}

void basic_1st_test()
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
}

struct test_instance_t {
  string nm_;
  src_t dt_;
  mclct_t::res_type_t expected_;
};

void run_test(const test_instance_t& test)
{
  mclct_t mc{test.dt_.size(), test.dt_.size()};
  collect(mc, test.dt_);

  auto result = mc.find_mode();
  list_res(result, test.nm_);
  assert(result.tag == test.expected_.tag);
  if (result.tag != mclct_t::mode_status_t::no_mode_value) {
    assert(result.key == test.expected_.key);
    assert(result.id == test.expected_.id);
    assert(result.count == test.expected_.count);
  }
}

test_instance_t test_cases[] = {
  {"Test 1", {{10, 100}, {11, 200}, {12, 150}, {13, 100}, {14, 100}},
   {mclct_t::mode_status_t::authorative_value, 100, 14, 3}},
  {"Test 1", {{10, 100}, {31, 200}, {32, 232}, {33, 233}, {34, 100}},
   {mclct_t::mode_status_t::mode_value, 100, 34, 2}},
  {"Test 2", {{20, 300}, {21, 300}, {22, 400}, {23, 500}, {24, 500}},
   {mclct_t::mode_status_t::no_mode_value, 300, 20, 2}}
};

int main()
{
   basic_1st_test();
   for (const auto& test : test_cases) {
       run_test(test);
   }
}

