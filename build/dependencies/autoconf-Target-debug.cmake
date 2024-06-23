# Avoid multiple calls to find_package to append duplicated properties to the targets
include_guard()########### VARIABLES #######################################################################
#############################################################################################
set(autoconf_FRAMEWORKS_FOUND_DEBUG "") # Will be filled later
conan_find_apple_frameworks(autoconf_FRAMEWORKS_FOUND_DEBUG "${autoconf_FRAMEWORKS_DEBUG}" "${autoconf_FRAMEWORK_DIRS_DEBUG}")

set(autoconf_LIBRARIES_TARGETS "") # Will be filled later


######## Create an interface target to contain all the dependencies (frameworks, system and conan deps)
if(NOT TARGET autoconf_DEPS_TARGET)
    add_library(autoconf_DEPS_TARGET INTERFACE IMPORTED)
endif()

set_property(TARGET autoconf_DEPS_TARGET
             APPEND PROPERTY INTERFACE_LINK_LIBRARIES
             $<$<CONFIG:Debug>:${autoconf_FRAMEWORKS_FOUND_DEBUG}>
             $<$<CONFIG:Debug>:${autoconf_SYSTEM_LIBS_DEBUG}>
             $<$<CONFIG:Debug>:m4::m4>)

####### Find the libraries declared in cpp_info.libs, create an IMPORTED target for each one and link the
####### autoconf_DEPS_TARGET to all of them
conan_package_library_targets("${autoconf_LIBS_DEBUG}"    # libraries
                              "${autoconf_LIB_DIRS_DEBUG}" # package_libdir
                              "${autoconf_BIN_DIRS_DEBUG}" # package_bindir
                              "${autoconf_LIBRARY_TYPE_DEBUG}"
                              "${autoconf_IS_HOST_WINDOWS_DEBUG}"
                              autoconf_DEPS_TARGET
                              autoconf_LIBRARIES_TARGETS  # out_libraries_targets
                              "_DEBUG"
                              "autoconf"    # package_name
                              "${autoconf_NO_SONAME_MODE_DEBUG}")  # soname

# FIXME: What is the result of this for multi-config? All configs adding themselves to path?
set(CMAKE_MODULE_PATH ${autoconf_BUILD_DIRS_DEBUG} ${CMAKE_MODULE_PATH})

########## GLOBAL TARGET PROPERTIES Debug ########################################
    set_property(TARGET autoconf::autoconf
                 APPEND PROPERTY INTERFACE_LINK_LIBRARIES
                 $<$<CONFIG:Debug>:${autoconf_OBJECTS_DEBUG}>
                 $<$<CONFIG:Debug>:${autoconf_LIBRARIES_TARGETS}>
                 )

    if("${autoconf_LIBS_DEBUG}" STREQUAL "")
        # If the package is not declaring any "cpp_info.libs" the package deps, system libs,
        # frameworks etc are not linked to the imported targets and we need to do it to the
        # global target
        set_property(TARGET autoconf::autoconf
                     APPEND PROPERTY INTERFACE_LINK_LIBRARIES
                     autoconf_DEPS_TARGET)
    endif()

    set_property(TARGET autoconf::autoconf
                 APPEND PROPERTY INTERFACE_LINK_OPTIONS
                 $<$<CONFIG:Debug>:${autoconf_LINKER_FLAGS_DEBUG}>)
    set_property(TARGET autoconf::autoconf
                 APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES
                 $<$<CONFIG:Debug>:${autoconf_INCLUDE_DIRS_DEBUG}>)
    # Necessary to find LINK shared libraries in Linux
    set_property(TARGET autoconf::autoconf
                 APPEND PROPERTY INTERFACE_LINK_DIRECTORIES
                 $<$<CONFIG:Debug>:${autoconf_LIB_DIRS_DEBUG}>)
    set_property(TARGET autoconf::autoconf
                 APPEND PROPERTY INTERFACE_COMPILE_DEFINITIONS
                 $<$<CONFIG:Debug>:${autoconf_COMPILE_DEFINITIONS_DEBUG}>)
    set_property(TARGET autoconf::autoconf
                 APPEND PROPERTY INTERFACE_COMPILE_OPTIONS
                 $<$<CONFIG:Debug>:${autoconf_COMPILE_OPTIONS_DEBUG}>)

########## For the modules (FindXXX)
set(autoconf_LIBRARIES_DEBUG autoconf::autoconf)
