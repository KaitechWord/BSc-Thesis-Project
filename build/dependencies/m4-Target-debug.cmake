# Avoid multiple calls to find_package to append duplicated properties to the targets
include_guard()########### VARIABLES #######################################################################
#############################################################################################
set(m4_FRAMEWORKS_FOUND_DEBUG "") # Will be filled later
conan_find_apple_frameworks(m4_FRAMEWORKS_FOUND_DEBUG "${m4_FRAMEWORKS_DEBUG}" "${m4_FRAMEWORK_DIRS_DEBUG}")

set(m4_LIBRARIES_TARGETS "") # Will be filled later


######## Create an interface target to contain all the dependencies (frameworks, system and conan deps)
if(NOT TARGET m4_DEPS_TARGET)
    add_library(m4_DEPS_TARGET INTERFACE IMPORTED)
endif()

set_property(TARGET m4_DEPS_TARGET
             APPEND PROPERTY INTERFACE_LINK_LIBRARIES
             $<$<CONFIG:Debug>:${m4_FRAMEWORKS_FOUND_DEBUG}>
             $<$<CONFIG:Debug>:${m4_SYSTEM_LIBS_DEBUG}>
             $<$<CONFIG:Debug>:>)

####### Find the libraries declared in cpp_info.libs, create an IMPORTED target for each one and link the
####### m4_DEPS_TARGET to all of them
conan_package_library_targets("${m4_LIBS_DEBUG}"    # libraries
                              "${m4_LIB_DIRS_DEBUG}" # package_libdir
                              "${m4_BIN_DIRS_DEBUG}" # package_bindir
                              "${m4_LIBRARY_TYPE_DEBUG}"
                              "${m4_IS_HOST_WINDOWS_DEBUG}"
                              m4_DEPS_TARGET
                              m4_LIBRARIES_TARGETS  # out_libraries_targets
                              "_DEBUG"
                              "m4"    # package_name
                              "${m4_NO_SONAME_MODE_DEBUG}")  # soname

# FIXME: What is the result of this for multi-config? All configs adding themselves to path?
set(CMAKE_MODULE_PATH ${m4_BUILD_DIRS_DEBUG} ${CMAKE_MODULE_PATH})

########## GLOBAL TARGET PROPERTIES Debug ########################################
    set_property(TARGET m4::m4
                 APPEND PROPERTY INTERFACE_LINK_LIBRARIES
                 $<$<CONFIG:Debug>:${m4_OBJECTS_DEBUG}>
                 $<$<CONFIG:Debug>:${m4_LIBRARIES_TARGETS}>
                 )

    if("${m4_LIBS_DEBUG}" STREQUAL "")
        # If the package is not declaring any "cpp_info.libs" the package deps, system libs,
        # frameworks etc are not linked to the imported targets and we need to do it to the
        # global target
        set_property(TARGET m4::m4
                     APPEND PROPERTY INTERFACE_LINK_LIBRARIES
                     m4_DEPS_TARGET)
    endif()

    set_property(TARGET m4::m4
                 APPEND PROPERTY INTERFACE_LINK_OPTIONS
                 $<$<CONFIG:Debug>:${m4_LINKER_FLAGS_DEBUG}>)
    set_property(TARGET m4::m4
                 APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES
                 $<$<CONFIG:Debug>:${m4_INCLUDE_DIRS_DEBUG}>)
    # Necessary to find LINK shared libraries in Linux
    set_property(TARGET m4::m4
                 APPEND PROPERTY INTERFACE_LINK_DIRECTORIES
                 $<$<CONFIG:Debug>:${m4_LIB_DIRS_DEBUG}>)
    set_property(TARGET m4::m4
                 APPEND PROPERTY INTERFACE_COMPILE_DEFINITIONS
                 $<$<CONFIG:Debug>:${m4_COMPILE_DEFINITIONS_DEBUG}>)
    set_property(TARGET m4::m4
                 APPEND PROPERTY INTERFACE_COMPILE_OPTIONS
                 $<$<CONFIG:Debug>:${m4_COMPILE_OPTIONS_DEBUG}>)

########## For the modules (FindXXX)
set(m4_LIBRARIES_DEBUG m4::m4)
