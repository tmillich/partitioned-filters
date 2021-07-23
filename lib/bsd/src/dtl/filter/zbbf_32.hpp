#pragma once

#include <dtl/dtl.hpp>
#include "filter_base.hpp"

namespace dtl {

//===----------------------------------------------------------------------===//
// PImpl wrapper to reduce compilation time.
//===----------------------------------------------------------------------===//
class zbbf_32 : public dtl::filter::filter_base {
  class impl;
  std::unique_ptr<impl> pimpl;

public:

  using key_t = $u32;
  using word_t = $u64; // internally, 32-bit words are used

  //===----------------------------------------------------------------------===//
  // The API functions.
  //===----------------------------------------------------------------------===//
  $u1
  insert(word_t* __restrict filter_data, key_t key);

  $u1
  batch_insert(word_t* __restrict filter_data, const key_t* keys, u32 key_cnt);

  $u1
  contains(const word_t* __restrict filter_data, key_t key) const;

  $u64
  batch_contains(const word_t* __restrict filter_data,
                 const key_t* __restrict keys, u32 key_cnt,
                 $u32* __restrict match_positions, u32 match_offset) const;

  std::string
  name() const;

  std::size_t
  size_in_bytes() const;

  std::size_t
  size() const;

  static void
  calibrate(u64 filter_size_bits = 4ull * 1024 * 8);

  static void
  force_unroll_factor(u32 u);
  //===----------------------------------------------------------------------===//

  zbbf_32(std::size_t m, u32 k, u32 word_cnt_per_block = 4, u32 zone_cnt = 2);
  ~zbbf_32() override;
  zbbf_32(zbbf_32&&) noexcept;
  zbbf_32(const zbbf_32&) = delete;
  zbbf_32& operator=(zbbf_32&&);
  zbbf_32& operator=(const zbbf_32&) = delete;

};

} // namespace dtl
