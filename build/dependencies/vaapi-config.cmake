########## MACROS ###########################################################################
#############################################################################################

# Requires CMake > 3.15
if(${CMAKE_VERSION} VERSION_LESS "3.15")
    message(FATAL_ERROR "The 'CMakeDeps' generator only works with CMake >= 3.15")
endif()

if(vaapi_FIND_QUIETLY)
    set(vaapi_MESSAGE_MODE VERBOSE)
else()
    set(vaapi_MESSAGE_MODE STATUS)
endif()

include(${CMAKE_CURRENT_LIST_DIR}/cmakedeps_macros.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/vaapiTargets.cmake)
include(CMakeFindDependencyMacro)

check_build_type_defined()

foreach(_DEPENDENCY ${vaapi_FIND_DEPENDENCY_NAMES} )
    # Check that we have not already called a find_package with the transitive dependency
    if(NOT ${_DEPENDENCY}_FOUND)
        find_dependency(${_DEPENDENCY} REQUIRED ${${_DEPENDENCY}_FIND_MODE})
    endif()
endforeach()

set(vaapi_VERSION_STRING "system")
set(vaapi_INCLUDE_DIRS ${vaapi_INCLUDE_DIRS_DEBUG} )
set(vaapi_INCLUDE_DIR ${vaapi_INCLUDE_DIRS_DEBUG} )
set(vaapi_LIBRARIES ${vaapi_LIBRARIES_DEBUG} )
set(vaapi_DEFINITIONS ${vaapi_DEFINITIONS_DEBUG} )


# Only the last installed configuration BUILD_MODULES are included to avoid the collision
foreach(_BUILD_MODULE ${vaapi_BUILD_MODULES_PATHS_DEBUG} )
    message(${vaapi_MESSAGE_MODE} "Conan: Including build module from '${_BUILD_MODULE}'")
    include(${_BUILD_MODULE})
endforeach()


