cmake_minimum_required(VERSION 3.16)
project(PhotonicSort VERSION 1.6.1 LANGUAGES CXX)

# PhotonicSort hybrid residual — dual residual is the DEFAULT production path.
# Escape hatch: -DCLASSICAL_RESIDUAL=ON restores classical unconditional ska on mid-band HE.
# EXTERNAL-clean. THE BEASTIE BOYZ

option(CLASSICAL_RESIDUAL "Use classical residual (no dual-evidence talent drive)" OFF)
option(PHOTONIC_BUILD_EXAMPLES "Build hybrid_sort example" ON)

add_library(photonic_hybrid INTERFACE)
add_library(PhotonicSort::hybrid ALIAS photonic_hybrid)

target_include_directories(photonic_hybrid INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/residual>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/baselines>
  $<INSTALL_INTERFACE:include/PhotonicSort>
)

target_compile_features(photonic_hybrid INTERFACE cxx_std_17)

if(CLASSICAL_RESIDUAL)
  target_compile_definitions(photonic_hybrid INTERFACE CLASSICAL_RESIDUAL)
endif()

# Install residual headers under include/PhotonicSort/
install(DIRECTORY residual/
  DESTINATION include/PhotonicSort
  FILES_MATCHING PATTERN "*.hpp" PATTERN "*.h"
)

# Optional: install baselines if present (user can also FetchContent/curl them)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/baselines")
  install(DIRECTORY baselines/
    DESTINATION include/PhotonicSort
    FILES_MATCHING PATTERN "*.hpp" PATTERN "*.h"
  )
endif()

install(TARGETS photonic_hybrid EXPORT PhotonicSortTargets)

install(EXPORT PhotonicSortTargets
  FILE PhotonicSortTargets.cmake
  NAMESPACE PhotonicSort::
  DESTINATION lib/cmake/PhotonicSort
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/PhotonicSortConfigVersion.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion
)

file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/PhotonicSortConfig.cmake"
"include(\"\${CMAKE_CURRENT_LIST_DIR}/PhotonicSortTargets.cmake\")\n")

install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/PhotonicSortConfig.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/PhotonicSortConfigVersion.cmake"
  DESTINATION lib/cmake/PhotonicSort
)

if(PHOTONIC_BUILD_EXAMPLES)
  # Baselines required to build the example; CI/reproduce fetch them.
  if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/baselines/pdqsort.h"
     AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/baselines/ska_sort.hpp")
    add_executable(hybrid_sort_example examples/hybrid_sort.cpp)
    target_link_libraries(hybrid_sort_example PRIVATE PhotonicSort::hybrid)
  else()
    message(STATUS "baselines/ missing — skip hybrid_sort_example (curl pdqsort + ska_sort into baselines/)")
  endif()
endif()

message(STATUS "PhotonicSort hybrid residual: DEFAULT dual residual (talent drive)")
if(CLASSICAL_RESIDUAL)
  message(STATUS "  CLASSICAL_RESIDUAL=ON — escape hatch active")
endif()
