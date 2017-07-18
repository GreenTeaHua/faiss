# specify header and cpp files
file(GLOB_RECURSE faiss_gpu_headers ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
file(GLOB_RECURSE faiss_gpu_cpp ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE faiss_gpu_cuh ${CMAKE_CURRENT_SOURCE_DIR}/*.cuh)
file(GLOB_RECURSE faiss_gpu_cu ${CMAKE_CURRENT_SOURCE_DIR}/*.cu)

set(faiss_lib_gpu gpufaiss)

list(REMOVE_ITEM faiss_gpu_cpp ${CMAKE_CURRENT_SOURCE_DIR}/test/demo_ivfpq_indexing_gpu.cpp)
list(REMOVE_ITEM faiss_gpu_cpp ${CMAKE_CURRENT_SOURCE_DIR}/test/TestGpuIndexFlat.cpp)
list(REMOVE_ITEM faiss_gpu_cpp ${CMAKE_CURRENT_SOURCE_DIR}/test/TestGpuIndexIVFFlat.cpp)
list(REMOVE_ITEM faiss_gpu_cpp ${CMAKE_CURRENT_SOURCE_DIR}/test/TestGpuIndexIVFPQ.cpp)
list(REMOVE_ITEM faiss_gpu_cpp ${CMAKE_CURRENT_SOURCE_DIR}/test/TestUtils.cpp)
list(REMOVE_ITEM faiss_gpu_cu ${CMAKE_CURRENT_SOURCE_DIR}/test/TestGpuSelect.cu)
list(REMOVE_ITEM faiss_gpu_headers ${CMAKE_CURRENT_SOURCE_DIR}/test/TestUtils.h)
list(REMOVE_ITEM faiss_gpu_headers ${CMAKE_CURRENT_SOURCE_DIR}/perf/IndexWrapper.h)
list(REMOVE_ITEM faiss_gpu_headers ${CMAKE_CURRENT_SOURCE_DIR}/perf/IndexWrapper-inl.h)
list(REMOVE_ITEM faiss_gpu_cu ${CMAKE_CURRENT_SOURCE_DIR}/perf/CompareFlat.cu)
list(REMOVE_ITEM faiss_gpu_cu ${CMAKE_CURRENT_SOURCE_DIR}/perf/CompareIVFFlat.cu)
list(REMOVE_ITEM faiss_gpu_cu ${CMAKE_CURRENT_SOURCE_DIR}/perf/CompareIVFPQ.cu)
list(REMOVE_ITEM faiss_gpu_cu ${CMAKE_CURRENT_SOURCE_DIR}/perf/CompareIVFPQGrid.cu)
list(REMOVE_ITEM faiss_gpu_cu ${CMAKE_CURRENT_SOURCE_DIR}/perf/PerfSelect.cu)
list(REMOVE_ITEM faiss_gpu_cpp ${CMAKE_CURRENT_SOURCE_DIR}/perf/PerfClustering.cpp)
list(REMOVE_ITEM faiss_gpu_cpp ${CMAKE_CURRENT_SOURCE_DIR}/perf/PerfIVFPQAdd.cpp)
list(REMOVE_ITEM faiss_gpu_cpp ${CMAKE_CURRENT_SOURCE_DIR}/perf/WriteIndex.cpp)

cuda_add_library(${faiss_lib_gpu} STATIC ${faiss_gpu_headers} ${faiss_gpu_cpp} ${faiss_gpu_cuh} ${faiss_gpu_cu})
add_subdirectory(test)