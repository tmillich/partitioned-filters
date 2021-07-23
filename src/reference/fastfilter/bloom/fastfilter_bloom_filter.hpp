#pragma once

#include <iostream>
#include <filter_base.hpp>
#include <memory>
#include <hashutil.h>
#include <bloom/bloom.h>
#include "fastfilter_bloom_parameter.hpp"

namespace filters {

    template<typename FP, size_t k, typename OP>
    struct Filter<FilterType::FastfilterBloom, FP, k, OP> {

        static constexpr bool supports_add = true;
        static constexpr bool supports_add_partition = false;

        static_assert(OP::simd == parameter::SIMD::Scalar, "only Scalar is supported!");
        static_assert(OP::partitioning == parameter::Partitioning::Disabled, "partitioning must be disabled!");
        static_assert(OP::hashingMode == parameter::HashingMode::Murmur, "only Murmur is supported!");
        static_assert(OP::addressingMode == parameter::AddressingMode::Lemire, "only Lemire is supported!");

        static constexpr size_t bits_per_item = k * 1.44;
        static constexpr bool branchless = FP::branchless;

        using T = typename std::conditional<OP::registerSize == parameter::RegisterSize::_32bit, uint32_t,
                uint64_t>::type;
        using H = hashing::SimpleMixSplit;
        using F = bloomfilter::BloomFilter<T, bits_per_item, branchless, H>;

        size_t n_partitions = 1;
        std::unique_ptr<F> filter;
        task::TaskQueue<OP::multiThreading> queue;

        Filter(size_t, size_t, size_t n_threads, size_t n_tasks_per_level) : n_partitions(n_partitions),
                                                                             queue(n_threads, n_tasks_per_level) {
        }

        forceinline
        void init(const T *histogram) {
            filter = std::make_unique<F>(*histogram);
        }

        forceinline
        bool contains(const T &value, const size_t = 0) {
            return bloomfilter::Status::Ok == filter->Contain(value);
        }

        forceinline
        bool add(const T &value, const size_t = 0) {
            return bloomfilter::Status::Ok == filter->Add(value);
        }

        bool construct(T *values, size_t length) {
            T histogram = length;
            init(&histogram);
            auto status = filter->AddAll(values, 0, length);
            return (status == bloomfilter::Status::Ok);
        }

        size_t count(T *values, size_t length) {
            if constexpr (OP::multiThreading == parameter::MultiThreading::Disabled) {
                size_t counter = 0;
                for (size_t i = 0; i < length; i++) {
                    counter += contains(values[i]);
                }
                return counter;
            } else {
                std::atomic<size_t> counter{0};

                size_t begin = 0;
                for (size_t i = queue.get_n_tasks_per_level(); i > 0; i--) {
                    const size_t end = begin + (length - begin) / i;
                    queue.add_task([this, &counter, values, begin, end](size_t) {
                        size_t local_counter = 0;
                        for (size_t i = begin; i < end; i++) {
                            local_counter += contains(values[i]);
                        }
                        counter += local_counter;
                    });
                    begin = end;
                }
                queue.execute_tasks();

                return counter;
            }
        }

        size_t size() {
            return filter->SizeInBytes();
        }

        size_t avg_size() {
            // no partitioning available
            return size();
        }

        size_t retries() {
            // cannot get the number of retries needed for building from the implementation
            return 0;
        }

        std::string to_string() {
            std::string s = "\n{\n";
            s += "\t\"k\": " + std::to_string(k) + ",\n";
            s += "\t\"size\": " + std::to_string(size() * 8) + " bits,\n";
            s += "\t\"filter_params\": " + FP::to_string() + ",\n";
            s += "\t\"optimization_params\": " + OP::to_string() + "\n";
            s += "}\n";

            return s;
        }
    };

} // filters
