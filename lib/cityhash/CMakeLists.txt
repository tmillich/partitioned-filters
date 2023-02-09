get_filename_component(library_name ${CMAKE_CURRENT_SOURCE_DIR} NAME)
add_library(${library_name} STATIC city.cc)
target_include_directories(${library_name} INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
        $<INSTALL_INTERFACE:..>
)

add_library(filters::${library_name} ALIAS ${library_name})
include(GNUInstallDirs)
install(
        TARGETS ${library_name}
        EXPORT PartitionedFiltersTargets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)