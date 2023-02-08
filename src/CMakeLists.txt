set(library_name filters)

set(
    FILTER_BASE_SOURCES

    filter.hpp
    filter_base.hpp
    filter_factory.hpp
    filter_factory_base.hpp
    filter_pointer.hpp
    compiler/compiler_hints.hpp

    task/barrier.hpp
    task/task_queue.hpp

    address/addresser.hpp
    address/addresser_base.hpp
    address/addresser_lemire.hpp
    address/addresser_magic.hpp
    address/addresser_morton.hpp
    address/addresser_power_of_two.hpp
    address/addresser_vacuum.hpp

    hash/hash.hpp
    hash/hasher.hpp
    hash/hasher_base.hpp
    hash/hasher_cityhash.hpp
    hash/hasher_fasthash.hpp
    hash/hasher_identity.hpp
    hash/hasher_mul.hpp
    hash/hasher_murmur.hpp
    hash/hasher_two_independent_multiply_shift.hpp
    hash/seed.hpp

    math/math.hpp

    parameter/parameter.hpp

    partition/partition_set.hpp
    partition/cache_line_buffer.hpp

    random/random.hpp

    simd/helper.hpp
    simd/vector.hpp
    simd/vector_div.hpp
)

set(
    REFERENCE_SOURCES

    reference/amd_mortonfilter/amd_mortonfilter_filter.hpp
    reference/amd_mortonfilter/amd_mortonfilter_parameter.hpp

    reference/bsd/bsd_filter.hpp
    reference/bsd/bloom/bsd_bloom_filter.hpp
    reference/bsd/bloom/bsd_bloom_parameter.hpp
    reference/bsd/bloom/implementation/bsd_bloom_implementation.hpp
    reference/bsd/bloom/implementation/bsd_bloom_implementation_base.hpp
    reference/bsd/bloom/implementation/bsd_bloom_implementatition_naive.hpp

    reference/efficient_cuckoofilter/efficient_cuckoofilter_filter.hpp
    reference/efficient_cuckoofilter/efficient_cuckoofilter_parameter.hpp

    reference/fastfilter/fastfilter_filter.hpp
    reference/fastfilter/bloom/fastfilter_bloom_filter.hpp
    reference/fastfilter/bloom/fastfilter_bloom_parameter.hpp
    reference/fastfilter/xor/fastfilter_xor_cimplementation.hpp
    reference/fastfilter/xor/fastfilter_xor_compressed.hpp
    reference/fastfilter/xor/fastfilter_xor_filter.hpp
    reference/fastfilter/xor/fastfilter_xor_parameter.hpp
    reference/fastfilter/xor/fastfilter_xor_standard.hpp

    reference/impala/impala_bloom_filter.hpp
    reference/impala/impala_bloom_parameter.hpp

    reference/vacuumfilter/vacuumfilter_filter.hpp
    reference/vacuumfilter/vacuumfilter_parameter.hpp
)

set(
    BLOOM_SOURCES

    bloom/bloom_filter.hpp
    bloom/bloom_filter_factory.hpp
    bloom/bloom_parameter.hpp
    bloom/construct/bloom_construct.hpp
    bloom/construct/bloom_construct_base.hpp
    bloom/construct/bloom_construct_horizontal.hpp
    bloom/construct/bloom_construct_naive.hpp
    bloom/construct/bloom_construct_vertical.hpp
    bloom/container/bloom_container.hpp
    bloom/count/bloom_count.hpp
    bloom/count/bloom_count_base.hpp
    bloom/count/bloom_count_horizontal.hpp
    bloom/count/bloom_count_naive.hpp
    bloom/count/bloom_count_vertical.hpp
    bloom/mask/bloom_mask.hpp
    bloom/mask/bloom_mask_base.hpp
    bloom/mask/bloom_mask_blocked.hpp
    bloom/mask/bloom_mask_grouped.hpp
    bloom/mask/bloom_mask_helper.hpp
    bloom/mask/bloom_mask_sectorized.hpp
    bloom/mask/bloom_mask_sectorized_vertical.hpp
)

set(
    CUCKOO_SOURCES

    cuckoo/container/cuckoo_container.hpp
    cuckoo/container/cuckoo_container_base.hpp
    cuckoo/container/cuckoo_container_compressed.hpp
    cuckoo/container/cuckoo_container_retry.hpp
    cuckoo/container/cuckoo_container_standard.hpp
    cuckoo/container/cuckoo_helper.hpp
    cuckoo/cuckoo_filter.hpp
    cuckoo/cuckoo_filter_factory.hpp
    cuckoo/cuckoo_parameter.hpp
)

set(
    XOR_SOURCES

    xor/container/xor_container.hpp
    xor/container/xor_helper.hpp
    xor/container/xor_queue.hpp
    xor/container/xor_set.hpp
    xor/container/xor_stack.hpp
    xor/xor_filter.hpp
    xor/xor_filter_factory.hpp
    xor/xor_parameter.hpp
)

set(
    SHARED_FILTER_SOURCES

    ${FILTER_BASE_SOURCES}
    ${REFERENCE_SOURCES}
    ${BLOOM_SOURCES}
    ${CUCKOO_SOURCES}
    ${XOR_SOURCES}
)

# build library
add_library(${library_name} STATIC ${SHARED_FILTER_SOURCES})

set(
   FILTERS_LIBRARIES

    cityhash
    libdivide
    fastfilter
    impala
    efficient_cuckoofilter
    amd_mortonfilter
    vacuumfilter
)


if (NUMA_FOUND)
    list(APPEND FILTERS_LIBRARIES ${NUMA_LIBRARY})
    target_link_libraries(${library_name} PUBLIC )
    target_compile_definitions(${library_name} PUBLIC HAVE_NUMA)
endif (NUMA_FOUND)

if (ENABLE_BSD)
    list(APPEND FILTERS_LIBRARIES boost bsd)
    target_compile_definitions(${library_name} PUBLIC ENABLE_BSD)
endif ()

target_link_libraries(${library_name} PUBLIC ${FILTERS_LIBRARIES})

add_library(${library_name}::${library_name} ALIAS ${library_name})
