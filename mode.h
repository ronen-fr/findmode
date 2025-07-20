#pragma once

// mode.h
// This header file defines the mode_collector class template for collecting modes from a population of data.

#include <fmt/format.h>

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <functional>  // for std::identity
#include <ranges>
#include <unordered_map>  // for std::unordered_map


// The inputs are
template <typename OBJ_ID, typename K, OBJ_ID NO_ID = OBJ_ID{0}>
class mode_collector {

 private:
  struct node_type_t {
    size_t m_count{0};
    OBJ_ID m_id{NO_ID};  ///< Store the object ID associated with this value
  };


  const size_t m_population_size;
  const size_t m_sample_size;  // i.e. the good candidates

  /// \todo: consider PMR on the stack
  /// Map to store frequency of each value
  std::unordered_map<K, node_type_t, std::identity> m_frequency_map;

  /// Actual count of elements added. Must match m_sample_size.
  size_t actual_count{0};


 public:
  enum class mode_status_t {
    no_mode_value,  ///< No clear victory for any value
    mode_value,  ///< we have a winner, but it has less than half of the sample
    authorative_value  ///< more than half of the sample is of the same value
    //tie_value ///< Multiple values are tied for mode
  };

  struct res_type_t {
    mode_status_t tag;
    K key;
    OBJ_ID id;
    size_t count;
  };

  explicit mode_collector(size_t population_size, size_t sample_size)
      : m_population_size(population_size)
      , m_sample_size(sample_size)
  {
    m_frequency_map.reserve(population_size);
  }

  // Add a value to the collector
  void add(OBJ_ID obj, K value)
  {
    auto& node = m_frequency_map[value];
    node.m_count++;
    // note: it's OK to overwrite the ID here:
    node.m_id = obj;  // Store the object ID associated with this value
    actual_count++;
  }

  /// Get the mode of the collected values
  res_type_t find_mode()
  {
    assert(actual_count == m_sample_size);
    assert(!m_frequency_map.empty());

    // use ranges algorithm to find the mode
    auto max_elem = std::ranges::max_element(
        m_frequency_map, {},
        [](const auto& pair) { return pair.second.m_count; });

    // Check for clear victory
    if (max_elem->second.m_count > m_sample_size / 2) {
      return {
          mode_status_t::authorative_value, max_elem->first,
          max_elem->second.m_id, max_elem->second.m_count};
    }

    // Check for possible ties
    const auto max_elem_cnt = max_elem->second.m_count;

    max_elem->second.m_count = 0;  // Reset the count of the max element
    const auto second_best_elem = std::ranges::max_element(
        m_frequency_map, {},
        [](const auto& pair) { return pair.second.m_count; });
    max_elem->second.m_count = max_elem_cnt;  // Restore the count

    if (second_best_elem->second.m_count == max_elem_cnt) {
      return {
          mode_status_t::no_mode_value, max_elem->first, max_elem->second.m_id,
          max_elem_cnt};
    }

    return {
        mode_status_t::mode_value, max_elem->first, max_elem->second.m_id,
        max_elem_cnt};
  }
};

//namespace fmt {

// Custom formatter for mode_collector::res_type_t

// template <>
// struct fmt::formatter<typename mode_collector<int, uint64_t, 0>::res_type_t> {
//   template <typename ParseContext>
//   constexpr auto parse(ParseContext& ctx)
//   {
//     return ctx.begin();
//   }
// 
//   using rest = mode_collector<int, uint64_t, 0>::res_type_t;
// 
//   template <typename FormatContext>
//   auto format(const rest& res, FormatContext& ctx) const -> fmt::v10::appender
//   {
//     return format_to(
//         ctx.out(), "Mode: {}, Key: {}, ID: {}, Count: {}",
//         static_cast<int>(res.tag), res.key, res.id, res.count);
//   }
// };


// template <>
// struct formatter<Scrub::ScrubJob> {
//   constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }
//
//   template <typename FormatContext>
//   auto format(const Scrub::ScrubJob& sjob, FormatContext& ctx) const
//   {
//     return fmt::format_to(
// 	ctx.out(), "pg[{}]:sh:{}/dp:{}<{}>",
// 	sjob.pgid, sjob.shallow_target, sjob.deep_target, sjob.state_desc());
//   }
// };

// template <typename OBJ_ID, typename K>
// struct fmt::formatter<typename mode_collector<OBJ_ID, K, 0>::res_type_t> {
//   template <typename ParseContext>
//   constexpr auto parse(ParseContext& ctx) {
//     return ctx.begin();
//   }
//
//   using rest = mode_collector<OBJ_ID, K, 0>::res_type_t;
//
//   template <typename FormatContext>
//   auto format(const rest& res,
//               FormatContext& ctx) const {
//     return format_to(ctx.out(), "Mode: {}, Key: {}, ID: {}, Count: {}",
//                      static_cast<int>(res.tag), res.key, res.id, res.count);
//   }
// };
//} // namespace fmt
