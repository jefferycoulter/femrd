set(BASE_SRCS
    mesh.cpp
    model.cpp
    solver.cpp
)

set(BASE_HDRS
    mesh.h
    model.h
    solver.h
)

target_sources(${PROJECT_NAME} PRIVATE ${BASE_SRCS})
target_sources(${PROJECT_NAME} PUBLIC ${BASE_HDRS})