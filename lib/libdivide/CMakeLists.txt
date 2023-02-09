get_filename_component(library_name ${CMAKE_CURRENT_SOURCE_DIR} NAME)
add_library(${library_name} INTERFACE)
target_include_directories(${library_name} INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>)

add_library(filters::${library_name} ALIAS ${library_name})

set_target_properties(${library_name} PROPERTIES PUBLIC_HEADER libdivide.h)

include(GNUInstallDirs)
install(
        TARGETS ${library_name}
        EXPORT PartitionedFiltersTargets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${library_name}
)
