set(library_name filters)

# build library
add_library(${library_name} INTERFACE)
target_include_directories(
        ${library_name}
        INTERFACE

        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
        $<INSTALL_INTERFACE:${INSTALL_INCLUDE_DIRECTORY}/${INSTALL_INCLUDE_DIRECTORY}>
)
target_link_libraries(${library_name} INTERFACE cityhash libdivide fastfilter impala efficient_cuckoofilter amd_mortonfilter vacuumfilter)

if (NUMA_FOUND)
    target_link_libraries(${library_name} INTERFACE ${NUMA_LIBRARY})
    target_compile_definitions(${library_name} INTERFACE HAVE_NUMA)
endif (NUMA_FOUND)

if (ENABLE_BSD)
    target_compile_definitions(${library_name} INTERFACE ENABLE_BSD)
    target_link_libraries(${library_name} INTERFACE bsd)
endif ()

add_library(${library_name}::${library_name} ALIAS ${library_name})

include(GNUInstallDirs)
install(
    TARGETS ${library_name}
    EXPORT PartitionedFiltersTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(
    DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/" # source directory
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${INSTALL_INCLUDE_DIRECTORY} # target directory
    FILES_MATCHING PATTERN "*.hpp" # select header files
)
