# Avoid multiple calls to find_package to append duplicated properties to the targets
include_guard()########### VARIABLES #######################################################################
#############################################################################################
set(automake_FRAMEWORKS_FOUND_DEBUG "") # Will be filled later
conan_find_apple_frameworks(automake_FRAMEWORKS_FOUND_DEBUG "${automake_FRAMEWORKS_DEBUG}" "${automake_FRAMEWORK_DIRS_DEBUG}")

set(automake_LIBRARIES_TARGETS "") # Will be filled later


######## Create an interface target to contain all the dependencies (frameworks, system and conan deps)
if(NOT TARGET automake_DEPS_TARGET)
    add_library(automake_DEPS_TARGET INTERFACE IMPORTED)
endif()

set_property(TARGET automake_DEPS_TARGET
             APPEND PROPERTY INTERFACE_LINK_LIBRARIES
             $<$<CONFIG:Debug>:${automake_FRAMEWORKS_FOUND_DEBUG}>
             $<$<CONFIG:Debug>:${automake_SYSTEM_LIBS_DEBUG}>
             $<$<CONFIG:Debug>:autoconf::autoconf>)

####### Find the libraries declared in cpp_info.libs, create an IMPORTED target for each one and link the
####### automake_DEPS_TARGET to all of them
conan_package_library_targets("${automake_LIBS_DEBUG}"    # libraries
                              "${automake_LIB_DIRS_DEBUG}" # package_libdir
                              "${automake_BIN_DIRS_DEBUG}" # package_bindir
                              "${automake_LIBRARY_TYPE_DEBUG}"
                              "${automake_IS_HOST_WINDOWS_DEBUG}"
                              automake_DEPS_TARGET
                              automake_LIBRARIES_TARGETS  # out_libraries_targets
                              "_DEBUG"
                              "automake"    # package_name
                              "${automake_NO_SONAME_MODE_DEBUG}")  # soname

# FIXME: What is the result of this for multi-config? All configs adding themselves to path?
set(CMAKE_MODULE_PATH ${automake_BUILD_DIRS_DEBUG} ${CMAKE_MODULE_PATH})

########## GLOBAL TARGET PROPERTIES Debug ########################################
    set_property(TARGET automake::automake
                 APPEND PROPERTY INTERFACE_LINK_LIBRARIES
                 $<$<CONFIG:Debug>:${automake_OBJECTS_DEBUG}>
                 $<$<CONFIG:Debug>:${automake_LIBRARIES_TARGETS}>
                 )

    if("${automake_LIBS_DEBUG}" STREQUAL "")
        # If the package is not declaring any "cpp_info.libs" the package deps, system libs,
        # frameworks etc are not linked to the imported targets and we need to do it to the
        # global target
        set_property(TARGET automake::automake
                     APPEND PROPERTY INTERFACE_LINK_LIBRARIES
                     automake_DEPS_TARGET)
    endif()

    set_property(TARGET automake::automake
                 APPEND PROPERTY INTERFACE_LINK_OPTIONS
                 $<$<CONFIG:Debug>:${automake_LINKER_FLAGS_DEBUG}>)
    set_property(TARGET automake::automake
                 APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES
                 $<$<CONFIG:Debug>:${automake_INCLUDE_DIRS_DEBUG}>)
    # Necessary to find LINK shared libraries in Linux
    set_property(TARGET automake::automake
                 APPEND PROPERTY INTERFACE_LINK_DIRECTORIES
                 $<$<CONFIG:Debug>:${automake_LIB_DIRS_DEBUG}>)
    set_property(TARGET automake::automake
                 APPEND PROPERTY INTERFACE_COMPILE_DEFINITIONS
                 $<$<CONFIG:Debug>:${automake_COMPILE_DEFINITIONS_DEBUG}>)
    set_property(TARGET automake::automake
                 APPEND PROPERTY INTERFACE_COMPILE_OPTIONS
                 $<$<CONFIG:Debug>:${automake_COMPILE_OPTIONS_DEBUG}>)

########## For the modules (FindXXX)
set(automake_LIBRARIES_DEBUG automake::automake)
