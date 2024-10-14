cmake_minimum_required(VERSION 3.16...3.30)

# Set a name and a version number for your project:
project(
  pyconcord
  VERSION 1.0.1
  LANGUAGES CXX)

# Initialize some default paths
include(GNUInstallDirs)

# Define the minimum C++ standard that is required
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Configuration options
option(BUILD_CPP "Enable building of C++ interface" ON)
option(BUILD_PYTHON "Enable building of Python interface" ON)
option(BUILD_DOCS "Enable building of documentation" ON)

# Build the core c++ library
add_subdirectory(lib)

# Build the c++ tests
include(CTest)
if(BUILD_TESTING)
  add_subdirectory(ext/Catch2)
  include(./ext/Catch2/extras/Catch.cmake)
  add_subdirectory(tests/cpp)
endif()

# Build the documentation
if(BUILD_DOCS)
  add_subdirectory(doc)
endif()

# Build the python interface
if(BUILD_PYTHON)
  add_subdirectory(src)
endif()

# Install c++ interface
if(BUILD_CPP)
  # Add an alias target for use if this project is included as a subproject in
  # another project
  add_library(pyconcord::pyconcord ALIAS
              pyconcord)

  # Install targets and configuration
  install(
    TARGETS pyconcord
    EXPORT pyconcord_config
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})

  install(
    EXPORT pyconcord_config
    NAMESPACE pyconcord::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/pyconcord)

  install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/include/
          DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
endif()

# This prints a summary of found dependencies
include(FeatureSummary)
feature_summary(WHAT ALL)
