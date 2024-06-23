# Avoid multiple calls to find_package to append duplicated properties to the targets
include_guard()########### VARIABLES #######################################################################
#############################################################################################
set(libtool_FRAMEWORKS_FOUND_DEBUG "") # Will be filled later
conan_find_apple_frameworks(libtool_FRAMEWORKS_FOUND_DEBUG "${libtool_FRAMEWORKS_DEBUG}" "${libtool_FRAMEWORK_DIRS_DEBUG}")

set(libtool_LIBRARIES_TARGETS "") # Will be filled later


######## Create an interface target to contain all the dependencies (frameworks, system and conan deps)
if(NOT TARGET libtool_DEPS_TARGET)
    add_library(libtool_DEPS_TARGET INTERFACE IMPORTED)
endif()

set_property(TARGET libtool_DEPS_TARGET
             APPEND PROPERTY INTERFACE_LINK_LIBRARIES
             $<$<CONFIG:Debug>:${libtool_FRAMEWORKS_FOUND_DEBUG}>
             $<$<CONFIG:Debug>:${libtool_SYSTEM_LIBS_DEBUG}>
             $<$<CONFIG:Debug>:automake::automake>)

####### Find the libraries declared in cpp_info.libs, create an IMPORTED target for each one and link the
####### libtool_DEPS_TARGET to all of them
conan_package_library_targets("${libtool_LIBS_DEBUG}"    # libraries
                              "${libtool_LIB_DIRS_DEBUG}" # package_libdir
                              "${libtool_BIN_DIRS_DEBUG}" # package_bindir
                              "${libtool_LIBRARY_TYPE_DEBUG}"
                              "${libtool_IS_HOST_WINDOWS_DEBUG}"
                              libtool_DEPS_TARGET
                              libtool_LIBRARIES_TARGETS  # out_libraries_targets
                              "_DEBUG"
                              "libtool"    # package_name
                              "${libtool_NO_SONAME_MODE_DEBUG}")  # soname

# FIXME: What is the result of this for multi-config? All configs adding themselves to path?
set(CMAKE_MODULE_PATH ${libtool_BUILD_DIRS_DEBUG} ${CMAKE_MODULE_PATH})

########## GLOBAL TARGET PROPERTIES Debug ########################################
    set_property(TARGET libtool::libtool
                 APPEND PROPERTY INTERFACE_LINK_LIBRARIES
                 $<$<CONFIG:Debug>:${libtool_OBJECTS_DEBUG}>
                 $<$<CONFIG:Debug>:${libtool_LIBRARIES_TARGETS}>
                 )

    if("${libtool_LIBS_DEBUG}" STREQUAL "")
        # If the package is not declaring any "cpp_info.libs" the package deps, system libs,
        # frameworks etc are not linked to the imported targets and we need to do it to the
        # global target
        set_property(TARGET libtool::libtool
                     APPEND PROPERTY INTERFACE_LINK_LIBRARIES
                     libtool_DEPS_TARGET)
    endif()

    set_property(TARGET libtool::libtool
                 APPEND PROPERTY INTERFACE_LINK_OPTIONS
                 $<$<CONFIG:Debug>:${libtool_LINKER_FLAGS_DEBUG}>)
    set_property(TARGET libtool::libtool
                 APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES
                 $<$<CONFIG:Debug>:${libtool_INCLUDE_DIRS_DEBUG}>)
    # Necessary to find LINK shared libraries in Linux
    set_property(TARGET libtool::libtool
                 APPEND PROPERTY INTERFACE_LINK_DIRECTORIES
                 $<$<CONFIG:Debug>:${libtool_LIB_DIRS_DEBUG}>)
    set_property(TARGET libtool::libtool
                 APPEND PROPERTY INTERFACE_COMPILE_DEFINITIONS
                 $<$<CONFIG:Debug>:${libtool_COMPILE_DEFINITIONS_DEBUG}>)
    set_property(TARGET libtool::libtool
                 APPEND PROPERTY INTERFACE_COMPILE_OPTIONS
                 $<$<CONFIG:Debug>:${libtool_COMPILE_OPTIONS_DEBUG}>)

########## For the modules (FindXXX)
set(libtool_LIBRARIES_DEBUG libtool::libtool)
