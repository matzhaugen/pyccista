add_library(pyconcord pyconcord.cpp)
find_package(Eigen3 CONFIG REQUIRED)
find_package(pybind11 CONFIG REQUIRED)
target_link_libraries(pyconcord PUBLIC Eigen3::Eigen)
target_include_directories(
  pyconcord
  PUBLIC $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include/>
         $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
