##Copyright (C) 2022, Advanced Micro Devices, Inc. All rights reserved.##

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

add_executable(test_libblis "")

add_subdirectory(src)

target_link_libraries(test_libblis debug "${LIB_NAME}.lib")
if(ENABLE_OPENMP AND BUILD_SHARED_LIBS)
    target_link_libraries(test_libblis "${OMP_LIB}")
endif()
target_link_libraries(test_libblis optimized  "${LIB_NAME}.lib")



