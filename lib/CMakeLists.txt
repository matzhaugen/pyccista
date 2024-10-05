add_library(pybind11_numpy_example pybind11_numpy_example.cpp)
find_package(Eigen3 CONFIG REQUIRED)
find_package(pybind11 CONFIG REQUIRED)
target_link_libraries(pybind11_numpy_example PUBLIC Eigen3::Eigen)
target_include_directories(
  pybind11_numpy_example
  PUBLIC $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include/>
         $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
