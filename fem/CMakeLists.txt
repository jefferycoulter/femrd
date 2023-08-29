set(FEM_HDRS
    fem.h
)

target_sources(${PROJECT_NAME} PUBLIC ${FEM_HDRS})

add_subdirectory(base)
add_subdirectory(elements)
add_subdirectory(logger)
add_subdirectory(math)