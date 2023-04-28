# CMakeList.txt : projet CMake pour rainbow_sort_2, incluez la source et définissez
# la logique spécifique au projet ici.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("rainbow_sort_2")

# Ajoutez une source à l'exécutable de ce projet.
add_executable (rainbow_sort_2 "rainbow_sort_2.cpp"  "utils.hpp" "image.cpp" "image.hpp"  )

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET rainbow_sort_2 PROPERTY CXX_STANDARD 20)
endif()

# TODO: Ajoutez des tests et installez des cibles si nécessaire.
