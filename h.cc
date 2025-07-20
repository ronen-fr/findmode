#include <benchmark/benchmark.h>


#include <fmt/compile.h>

#include <memory>
#include <vector>
#include <iostream>
#include <optional>
#include <version>
#include <chrono>
#include <sys/time.h>
#include <string>
#include <strstream>
#include <fmt/chrono.h>
#include <fmt/core.h>
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <algorithm>
#include <utility>
#include <map>
#include <fmt/printf.h>
#include <fmt/compile.h>
#include <random>
//#include <ranges>

#include "./mode_pmr.h"
#include "./mode.h"

using namespace std;

int main_mode_nopmr();
int main_mode_pmr();

static void using_no_pmr(benchmark::State& state)
{
  for (auto _ : state) {
    main_mode_nopmr();
    //benchmark::DoNotOptimize(r);
  }
}
// Register the function as a benchmark
BENCHMARK(using_no_pmr);

static void using_pmr(benchmark::State& state)
{
  for (auto _ : state) {
    main_mode_pmr();
  }
}
// Register the function as a benchmark
BENCHMARK(using_pmr);

BENCHMARK_MAIN();



#if 0
vector<string> create_test_instance(int cnt, size_t eachs)
{
  vector<string> res;

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> distrib(0, 255);

  for (int i = 0; i < cnt; ++i) {
    string ent;
    for (size_t a=0; a<eachs; ++a) {
      ent.push_back(distrib(gen));
    }
    res.push_back(ent);
  }
  return res;
}

std::string stringToHex(const std::string& input)
{
  std::stringstream ss;
  for (char c : input) {
    ss << std::hex << std::setw(2) << std::setfill('0')
       << static_cast<int>(static_cast<unsigned char>(c));
  }
  return ss.str();
}

static void to_hex_orig(benchmark::State& state)
{
  const auto dt1 = create_test_instance(glob_cnt, glob_len);
  for (auto _ : state) {
    for (const auto& g1 : dt1) {
      std::string r = stringToHex(g1);
      benchmark::DoNotOptimize(r);
    }
  }
}
// Register the function as a benchmark
BENCHMARK(to_hex_orig);



std::string sth2(const std::string& input)
{
    fmt::memory_buffer buf;
    buf.reserve(input.size() * 2);
    for (const unsigned char b : input) {
        fmt::format_to(std::back_inserter(buf), FMT_COMPILE("{:02X}"), b);
    }
    return string{buf.begin(), buf.end()};
}

static void to_sth2(benchmark::State& state)
{
  const auto dt1 = create_test_instance(glob_cnt, glob_len);
  for (auto _ : state) {
    for (const auto& g1 : dt1) {
      std::string r = sth2(g1);
      benchmark::DoNotOptimize(r);
    }
  }
}
// Register the function as a benchmark
BENCHMARK(to_sth2);

BENCHMARK_MAIN();

#if 0
int main()
{
  cout << f1("abc123") << "\n";
  cout << f2("abc123") << "\n";

}




#if 0
int main() {

  auto o1 = ghobject_t::generate_test_instances();
  for (auto g1 : o1) {
    std::cout << fmt::format("{} \tto_str: {}", g1->hobj, g1->hobj.to_str()) << std::endl;
    std::cout << g1->hobj << "\n";
  }

  std::string test_1 = fmt::format("{}", o1.front()->hobj.snap);

  for (auto g1 : o1) {
    auto st1 = g1->hobj.to_str();
    auto st2 = g1->hobj.to_str3();
    //std::cout << fmt::format(FMT_COMPILE("== {} <> {}\n"), st1, st2);
    std::cout << fmt::format("== {} <> {}\n", st1, st2);
    assert(false || st1 == st2);
  }

  return 0;
}
#elif 0

// testing to_str()

static void obj_tostr(benchmark::State& state)
{
  auto o1 = ghobject_t::generate_test_instances();
  for (auto _ : state) {
    for (auto g1 : o1) {
      std::string r = g1->hobj.to_str();
      benchmark::DoNotOptimize(r);
    }
  }
}
// Register the function as a benchmark
BENCHMARK(obj_tostr);

static void obj_tostr_e5(benchmark::State& state)
{
  auto o1 = ghobject_t::generate_test_instances();
  for (auto _ : state) {
    for (auto g1 : o1) {
      std::string r = g1->hobj.to_str_esc5();
      benchmark::DoNotOptimize(r);
    }
  }
}
// Register the function as a benchmark
BENCHMARK(obj_tostr_e5);

static void obj_tostr3(benchmark::State& state)
{
  auto o1 = ghobject_t::generate_test_instances();
  for (auto _ : state) {
    for (auto g1 : o1) {
      std::string r = g1->hobj.to_str3();
      benchmark::DoNotOptimize(r);
    }
  }
}
// Register the function as a benchmark
BENCHMARK(obj_tostr3);

static void obj_tostr4(benchmark::State& state)
{
  auto o1 = ghobject_t::generate_test_instances();
  for (auto _ : state) {
    for (auto g1 : o1) {
      std::string r = g1->hobj.to_str4();
      benchmark::DoNotOptimize(r);
    }
  }
}
// Register the function as a benchmark
BENCHMARK(obj_tostr4);

static void obj_tostr2(benchmark::State& state)
{
  auto o1 = ghobject_t::generate_test_instances();
  for (auto _ : state) {
    for (auto g1 : o1) {
      std::string r = g1->hobj.to_str2();
      benchmark::DoNotOptimize(r);
    }
  }
}
// Register the function as a benchmark
BENCHMARK(obj_tostr2);

BENCHMARK_MAIN();


#else
// a class that uses the old (existing) formatter

struct hobj2 : public hobject_t {

  template <typename... ARGS>
  hobj2(ARGS&&... args) : hobject_t{std::forward<ARGS>(args)...}
  {}

  hobj2(const hobj2& rhs) = default;
  hobj2(hobj2&& rhs) = default;
  hobj2& operator=(const hobj2& rhs) = default;
  hobj2& operator=(hobj2&& rhs) = default;
  hobj2(hobject_t_max &&singleton) : hobj2() {
    max = true;
  }
  hobj2 &operator=(hobject_t_max &&singleton) {
    *this = hobject_t();
    max = true;
    return *this;
  }
  bool is_max() const {
    //ceph_assert(!max || (*this == hobject_t(hobject_t::get_max())));
    return max;
  }
  bool is_min() const {
    // this needs to match how it's constructed
    return snap == 0 &&
	   hash == 0 &&
	   !max &&
	   pool == INT64_MIN;
  }

  static std::list<hobj2*> generate_test_instances();
};



namespace {
// \todo reimplement
static inline void app_hobj2(const std::string& in, std::string* out)
{
  for (auto i = in.cbegin(); i != in.cend(); ++i) {
    if (*i == '%' || *i == ':' || *i == '/' || *i < 32 || *i >= 127) {
      char buf[4];
      snprintf(buf, sizeof(buf), "%%%02x", (int)(unsigned char)*i);
      out->append(buf);
    } else {
      out->push_back(*i);
    }
  }
}
}  // namespace

namespace fmt {
template <> struct formatter<hobj2> {

  constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

  template <typename FormatContext> auto format(const hobj2& ho, FormatContext& ctx)
  {
    if (static_cast<hobject_t>(ho) == hobject_t{}) {
      return fmt::format_to(ctx.out(), "MIN");
    }

    if (static_cast<hobject_t>(ho).is_max()) {
      return fmt::format_to(ctx.out(), "MAX");
    }

    std::string v;
    app_hobj2(ho.nspace, &v);
    v.push_back(':');
    app_hobj2(ho.get_key(), &v);
    v.push_back(':');
    app_hobj2(ho.oid.name, &v);

    return fmt::format_to(ctx.out(), FMT_COMPILE("{}:{:08x}:{}:{}"), static_cast<uint64_t>(ho.pool),
			  ho.get_bitwise_key_u32(), v, ho.snap);
  }
};
}  // namespace fmt



list<hobj2*> hobj2::generate_test_instances()
{
  list<hobj2*> o;
  o.push_back(new hobj2);
  o.push_back(new hobj2);
  o.back()->max = true;
  o.push_back(new hobj2(hobject_t(object_t("oxxname"), string(), 1, 234, -1, "")));

  o.push_back(
    new hobj2(hobject_t(object_t("o%:/name2"), string("aaaa"), CEPH_NOSNAP, 67, 0, "n1")));
  o.push_back(
    new hobj2(hobject_t(object_t("on%%//ame2"), string("bbbb_bbbb_bbbb_bbbb_bbbb_"), CEPH_NOSNAP, 68, 0, "n3")));
  o.push_back(
    new hobj2(hobject_t(object_t("ona\030me2"), string("c"), CEPH_NOSNAP, 64, 0, "n5")));
  o.push_back(new hobj2(
    hobject_t(object_t("oname3"), string("oname3"), CEPH_SNAPDIR, 910, 1, "n2")));
  o.push_back(new hobj2(
    hobject_t(object_t("onasss\002sssssssssssssssssssssssssssssssssssssssssssssssssme3"), string("oname3"), CEPH_SNAPDIR, 910, 1, "n2")));
  o.push_back(new hobj2(
    hobject_t(object_t("oname3"), string("oname3"), CEPH_SNAPDIR, 910, 1, "n2")));
  o.push_back(new hobj2(
    hobject_t(object_t("o//////name3"), string("oname3"), CEPH_SNAPDIR, 910, 1, "n2")));
  o.push_back(new hobj2(
    hobject_t(object_t("oname3"), string("on::::::::::::::::ame3"), CEPH_SNAPDIR, 910, 1, "n2")));

  return o;
}

// list of possible arguments for virtual_to_str_w_prefix() (1'st form):
using strwprefix_form1 = std::tuple<std::string, std::string, snapid_t, uint32_t, int64_t>;
list<strwprefix_form1> strwprefix_form1_args = {
  {"prx1", "ke\002y", CEPH_NOSNAP, 0xffffffff, 1},
  {"prx2", "key", 0, 0, 0},
  {"prx3", "", CEPH_SNAPDIR, 0xff, 0},
  {"prx4", "key", 0x1111, 0xffff, 1},
  {"prx5", "ke%%%%y", 0x88, 0xffffffff, 1}
};

// list of possible arguments for virtual_to_str_w_prefix() (2'nd form):
using strwprefix_form2 = std::tuple<std::string, object_id_t, uint32_t, int64_t>;
list<strwprefix_form2> strwprefix_form2_args = {
  {"prx01", object_id_t{"oidN", "Nspc", "key", CEPH_NOSNAP }, 0xffffffff, 1},
  {"prx02", object_id_t{"oidN", "", "key", CEPH_NOSNAP }, 0, 0},
  {"prx03", object_id_t{"oidN", "%%%%", "key", CEPH_SNAPDIR }, 0xffffffff, 1},
  {"prx04", object_id_t{"oidN", "", "oidN", 0x1111 }, 0xffffffff, 1},
  {"prx05", object_id_t{"oidN", "", "ke%%%%y", 0x88 }, 0xffffffff, 1}
};


#if 1
int main()
{

  auto o1 = hobj2::generate_test_instances();
  for (auto g1 : o1) {

    hobject_t as_hobj = *g1;
//     // loop-back
//     auto dumped = fmt::format("{}", as_hobj);
//     hobject_t from_dumped;
//     from_dumped.parse(dumped);
    std::cout << fmt::format("\nold:\t{}\nnew:\t{}\n", *g1, as_hobj) << std::endl;

    std::cout << fmt::format("{} \tto_str: {}", *g1, g1->to_str()) << std::endl;
    std::cout << "direct ostream: " << *g1 << "\n";
  }



  // check the new body-less to_str()

  for (auto g1 : strwprefix_form1_args) {

    auto nv = hobject_t::virtual_to_str_w_prefix(std::get<0>(g1), std::get<1>(g1),
                                                 std::get<2>(g1), std::get<3>(g1), std::get<4>(g1));
    std::cout << fmt::format("form1 new: {}\n", nv);

    auto n0 = hobject_t::virtual_to_str_w_prefix_v0(std::get<0>(g1), std::get<1>(g1),
                                                 std::get<2>(g1), std::get<3>(g1), std::get<4>(g1));
    std::cout << fmt::format("form1 nv0: {}\n", n0);
    assert(nv == n0);


    //auto hoid = hobject_t(object_t(), "", CEPH_NOSNAP, 0xffffffff, pool, "");
    //hoid.build_hash_cache();
    //return "SCRUB_OBJ_" + hoid.to_str();

    hobject_t ho{object_t(), std::get<1>(g1), std::get<2>(g1), std::get<3>(g1), std::get<4>(g1), ""};
    ho.build_hash_cache();
    string ov = std::get<0>(g1) + "_"s +  ho.to_str();
    std::cout << fmt::format("form1 old: {}\n", ov);
    assert(nv == ov);
  }


  for (auto g1 : strwprefix_form2_args) {

    auto nv = hobject_t::virtual_to_str_w_prefix(std::get<0>(g1), std::get<1>(g1),
						 std::get<2>(g1), std::get<3>(g1));
    std::cout << fmt::format("form2 new: {}\n", nv);
    // the old way - create an object:
    //   auto hoid = hobject_t(object_t(oid.name),
    // 			oid.locator, // key
    // 			oid.snap,
    // 			0,		// hash
    // 			pool,
    // 			oid.nspace);
    //   hoid.build_hash_cache();
    //   return "SCRUB_OBJ_" + hoid.to_str();

    hobject_t ho{std::get<1>(g1).name, std::get<1>(g1).locator, std::get<1>(g1).snap,
		 std::get<2>(g1),      std::get<3>(g1),		std::get<1>(g1).nspace};
    ho.build_hash_cache();
    string ov = std::get<0>(g1) + "_"s +  ho.to_str();
    std::cout << fmt::format("form1 old: {}\n", ov);
    assert(nv == ov);
  }

  // std::string test_1 = fmt::format("{}", o1.front()->hobj.snap);

  //   for (auto g1 : o1) {
  //     auto st1 = g1->hobj.to_str();
  //     auto st2 = g1->hobj.to_str3();
  //     //std::cout << fmt::format(FMT_COMPILE("== {} <> {}\n"), st1, st2);
  //     std::cout << fmt::format("== {} <> {}\n", st1, st2);
  //     assert(false || st1 == st2);
  //   }

  return 0;
}


#else

// testing hobject formatter

static void hobj_orig(benchmark::State& state)
{
  auto o1 = hobj2::generate_test_instances();
  for (auto _ : state) {
    for (auto g1 : o1) {
      std::string r = fmt::format("{}", *g1);
      benchmark::DoNotOptimize(r);
    }
  }
}
// Register the function as a benchmark
BENCHMARK(hobj_orig);


static void hobj_new1(benchmark::State& state)
{
  auto o1 = hobj2::generate_test_instances();
  for (auto _ : state) {
    for (auto g1 : o1) {
      std::string r = fmt::format("{}", *static_cast<hobject_t*>(g1));
      benchmark::DoNotOptimize(r);
    }
  }
}
// Register the function as a benchmark
BENCHMARK(hobj_new1);

BENCHMARK_MAIN();

#endif
#endif
#endif
#endif