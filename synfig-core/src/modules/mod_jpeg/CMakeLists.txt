add_library(mod_jpeg MODULE "")

target_sources(mod_jpeg
    PUBLIC
        "${CMAKE_CURRENT_LIST_DIR}/mptr_jpeg.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/main.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/trgt_jpeg.cpp"
)

install (
    TARGETS mod_jpeg
    DESTINATION lib/synfig/modules
)
