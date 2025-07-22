#pragma once

// mode_pmr.h
// A PMR-enabled version of mode.h

// V2: collect the bad-peers/good-peers sets

#include <fmt/format.h>

#include <algorithm>
#include <array>
#include <cassert>
#include <cstddef>
#include <functional>  // for std::identity
#include <memory_resource>
#include <ranges>
#include <unordered_map>  // for std::unordered_map

//#define DUMP_SIZE

// note the use of std::identity: it's a pretty fast hash function,
// but we are restricted to size_t sized keys (per stdlib implementation
// of the unrdered map).

// The inputs are
template <typename OBJ_ID, typename K, OBJ_ID NO_ID = OBJ_ID{0}>
// K must fit in size_t due to us using std::identity
  requires(sizeof(K) <= sizeof(size_t))
class mode_collector_pmr {

 private:
  struct node_type_t {
    size_t m_count{0};
    OBJ_ID m_id{NO_ID};  ///< Store the object ID associated with this value
  };

  std::array<std::byte, 256> m_buffer;  // PMR buffer
  std::pmr::monotonic_buffer_resource m_mbr{
      m_buffer.data(), m_buffer.size(), nullptr};

  const size_t m_population_size;
  const size_t m_sample_size;  // i.e. the good candidates

  /// \todo: consider PMR on the stack
  /// Map to store frequency of each value
  std::pmr::unordered_map<K, node_type_t, std::identity> m_frequency_map;

  /// Actual count of elements added. Must match m_sample_size.
  size_t actual_count{0};


 public:
  enum class mode_status_t {
    no_mode_value,  ///< No clear victory for any value
    mode_value,  ///< we have a winner, but it has less than half of the sample
    authorative_value  ///< more than half of the sample is of the same value
    //tie_value ///< Multiple values are tied for mode
  };

  struct results_t {
    mode_status_t tag;
    K key;
    OBJ_ID id;
    size_t count;
  };

  explicit mode_collector_pmr(
      size_t population_size,
      size_t sample_size) noexcept
      : m_population_size(population_size)
      , m_sample_size(sample_size)
      , m_frequency_map(&m_mbr)
  {
    m_frequency_map.reserve(population_size);
  }

  // Add a value to the collector
  void add(OBJ_ID obj, K value) noexcept
  {
    auto& node = m_frequency_map[value];
    node.m_count++;
    // note: it's OK to overwrite the ID here:
    node.m_id = obj;  // Store the object ID associated with this value
    actual_count++;

#ifdef DUMP_SIZE
    cout << fmt::format(
        "add: container size: {}, count: {}, value: {}, id: {}\n",
        m_frequency_map.size(), actual_count, value, obj);
#endif
  }

  /// Get the mode of the collected values
  results_t find_mode() noexcept
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
