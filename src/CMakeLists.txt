aux_source_directory(. DIRSRCS)
add_library(posix_commons ${DIRSRCS})
target_link_libraries(posix_commons pthread)