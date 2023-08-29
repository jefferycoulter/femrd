# specify cmake version
cmake_minimum_required(VERSION 3.20)

# project name
project(fem)

# cmake settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/fem)

# compiler settings
add_compile_options(-g)
if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux") 
    if(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
        message("Adding vectorization flags for x86-64 architecture")
        add_compile_options(-mavx -mfma) # for vectorization in eigen
    endif()
endif()

# this is just for me
if(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
    if(${CMAKE_C_COMPILER_ID} STREQUAL "AppleClang")
        message(STATUS "Adding OpenMP_C_FLAGS compiler flags for AppleClang")
        set(OpenMP_C_FLAGS "-fopenmp=libomp -Wno-unused-command-line-argument")
    endif()
    if(${CMAKE_CXX_COMPILER_ID} STREQUAL "AppleClang")
        message(STATUS "Adding OpenMP_CXX_FLAGS compiler flags for AppleClang")
        set(OpenMP_CXX_FLAGS "-fopenmp=libomp -Wno-unused-command-line-argument")
    endif()
endif(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin") 

# grab openmp
find_package(OpenMP REQUIRED)
if (OPENMP_FOUND)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif(OPENMP_FOUND)

# grab blas for eigen
if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    set(BLA_VENDOR Intel10_64lp)
endif()
find_package(BLAS REQUIRED)
IF (BLAS_FOUND)
    include_directories(${BLAS_INCLUDE_DIR})
    if(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
        add_definitions(-DEIGEN_USE_BLAS)
    elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
        add_definitions(-DEIGEN_USE_MKL_ALL)
    endif()
endif(BLAS_FOUND)

# grab lapack for eigen
find_package(LAPACK REQUIRED)
IF (LAPACK_FOUND)
    include_directories(${LAPACK_INCLUDE_DIR})
    if(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
        add_definitions(-DEIGEN_USE_LAPACK)
    elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
        add_definitions(-DMKL_LP64)
    endif()
endif(LAPACK_FOUND)

# specify executable files
add_library(${PROJECT_NAME} STATIC fem/fem.h)

# add subdirectories with cmake files
add_subdirectory(fem)
add_subdirectory(tests)
#add_subdirectory(eigen)

# directories to include and libraries to link
target_include_directories(${PROJECT_NAME} 
    PUBLIC 
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/fem
        ${CMAKE_SOURCE_DIR}/eigen
)
target_link_libraries(${PROJECT_NAME} ${BLAS_LIBRARIES} ${LAPACK_LIBRARIES})
