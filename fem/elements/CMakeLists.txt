set(ELEMENT_SRCS
    # 1D elements
    linline.cpp
    # 2D elements
    lintri.cpp
    # 3D elements
    lintet.cpp
)

set(ELEMENT_HDRS
    element.h
    nullelem.h
    # 1D elements
    linline.h
    # 2D elements
    lintri.h
    # 3D elements
    lintet.h
)

target_sources(${PROJECT_NAME} PRIVATE ${ELEMENT_SRCS})
target_sources(${PROJECT_NAME} PUBLIC ${ELEMENT_HDRS})