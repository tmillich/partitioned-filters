get_filename_component(library_name fastfilter NAME)
add_library(${library_name} INTERFACE)
target_include_directories(${library_name} INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>)

add_library(filters::${library_name} ALIAS ${library_name})
include(GNUInstallDirs)
install(
        TARGETS ${library_name}
        EXPORT PartitionedFiltersTargets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(
        DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/" # source directory
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} # target directory
        FILES_MATCHING PATTERN "*.h" # select header files
)