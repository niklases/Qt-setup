# FindNVML.cmake - Locate NVIDIA Management Library (NVML)
# Works on Linux and Windows

if (WIN32)
    find_path(NVML_INCLUDE_DIR
        NAMES nvml.h
        PATHS
            "$ENV{CUDA_PATH}/include"
            "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v12.0/include"
    )

    find_library(NVML_LIBRARY
        NAMES nvml
        PATHS
            "$ENV{CUDA_PATH}/lib/x64"
            "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v12.0/lib/x64"
    )
else()
    find_path(NVML_INCLUDE_DIR
        NAMES nvml.h
        PATHS
            /usr/include
            /usr/include/nvidia-ml
            /usr/local/cuda/include
    )

    find_library(NVML_LIBRARY
        NAMES nvidia-ml
        PATHS
            /usr/lib
            /usr/lib64
            /usr/lib/x86_64-linux-gnu
            /lib/x86_64-linux-gnu
            /usr/local/cuda/lib64
    )
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(NVML DEFAULT_MSG NVML_INCLUDE_DIR NVML_LIBRARY)

if (NVML_FOUND)
    set(NVML_LIBRARIES ${NVML_LIBRARY} CACHE INTERNAL "NVML Libraries")
    set(NVML_INCLUDE_DIRS ${NVML_INCLUDE_DIR} CACHE INTERNAL "NVML Include Dirs")
endif()
