get_filename_component(library_name ${CMAKE_CURRENT_SOURCE_DIR} NAME)
add_library(${library_name} STATIC block_bloom_filter_avx.cc)
target_include_directories(${library_name} INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
        $<INSTALL_INTERFACE:..>
)

set_target_properties(${library_name} PROPERTIES PUBLIC_HEADER block_bloom_filter.h)

add_library(filters::${library_name} ALIAS ${library_name})
include(GNUInstallDirs)
install(
        TARGETS ${library_name}
        EXPORT PartitionedFiltersTargets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${INSTALL_INCLUDE_DIRECTORY}/${library_name}
)
