########## MACROS ###########################################################################
#############################################################################################

# Requires CMake > 3.15
if(${CMAKE_VERSION} VERSION_LESS "3.15")
    message(FATAL_ERROR "The 'CMakeDeps' generator only works with CMake >= 3.15")
endif()

if(vdpau_FIND_QUIETLY)
    set(vdpau_MESSAGE_MODE VERBOSE)
else()
    set(vdpau_MESSAGE_MODE STATUS)
endif()

include(${CMAKE_CURRENT_LIST_DIR}/cmakedeps_macros.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/vdpauTargets.cmake)
include(CMakeFindDependencyMacro)

check_build_type_defined()

foreach(_DEPENDENCY ${vdpau_FIND_DEPENDENCY_NAMES} )
    # Check that we have not already called a find_package with the transitive dependency
    if(NOT ${_DEPENDENCY}_FOUND)
        find_dependency(${_DEPENDENCY} REQUIRED ${${_DEPENDENCY}_FIND_MODE})
    endif()
endforeach()

set(vdpau_VERSION_STRING "system")
set(vdpau_INCLUDE_DIRS ${vdpau_INCLUDE_DIRS_DEBUG} )
set(vdpau_INCLUDE_DIR ${vdpau_INCLUDE_DIRS_DEBUG} )
set(vdpau_LIBRARIES ${vdpau_LIBRARIES_DEBUG} )
set(vdpau_DEFINITIONS ${vdpau_DEFINITIONS_DEBUG} )


# Only the last installed configuration BUILD_MODULES are included to avoid the collision
foreach(_BUILD_MODULE ${vdpau_BUILD_MODULES_PATHS_DEBUG} )
    message(${vdpau_MESSAGE_MODE} "Conan: Including build module from '${_BUILD_MODULE}'")
    include(${_BUILD_MODULE})
endforeach()


