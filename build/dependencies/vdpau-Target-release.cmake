# Avoid multiple calls to find_package to append duplicated properties to the targets
include_guard()########### VARIABLES #######################################################################
#############################################################################################
set(vdpau_FRAMEWORKS_FOUND_RELEASE "") # Will be filled later
conan_find_apple_frameworks(vdpau_FRAMEWORKS_FOUND_RELEASE "${vdpau_FRAMEWORKS_RELEASE}" "${vdpau_FRAMEWORK_DIRS_RELEASE}")

set(vdpau_LIBRARIES_TARGETS "") # Will be filled later


######## Create an interface target to contain all the dependencies (frameworks, system and conan deps)
if(NOT TARGET vdpau_DEPS_TARGET)
    add_library(vdpau_DEPS_TARGET INTERFACE IMPORTED)
endif()

set_property(TARGET vdpau_DEPS_TARGET
             APPEND PROPERTY INTERFACE_LINK_LIBRARIES
             $<$<CONFIG:Release>:${vdpau_FRAMEWORKS_FOUND_RELEASE}>
             $<$<CONFIG:Release>:${vdpau_SYSTEM_LIBS_RELEASE}>
             $<$<CONFIG:Release>:>)

####### Find the libraries declared in cpp_info.libs, create an IMPORTED target for each one and link the
####### vdpau_DEPS_TARGET to all of them
conan_package_library_targets("${vdpau_LIBS_RELEASE}"    # libraries
                              "${vdpau_LIB_DIRS_RELEASE}" # package_libdir
                              "${vdpau_BIN_DIRS_RELEASE}" # package_bindir
                              "${vdpau_LIBRARY_TYPE_RELEASE}"
                              "${vdpau_IS_HOST_WINDOWS_RELEASE}"
                              vdpau_DEPS_TARGET
                              vdpau_LIBRARIES_TARGETS  # out_libraries_targets
                              "_RELEASE"
                              "vdpau"    # package_name
                              "${vdpau_NO_SONAME_MODE_RELEASE}")  # soname

# FIXME: What is the result of this for multi-config? All configs adding themselves to path?
set(CMAKE_MODULE_PATH ${vdpau_BUILD_DIRS_RELEASE} ${CMAKE_MODULE_PATH})

########## GLOBAL TARGET PROPERTIES Release ########################################
    set_property(TARGET vdpau::vdpau
                 APPEND PROPERTY INTERFACE_LINK_LIBRARIES
                 $<$<CONFIG:Release>:${vdpau_OBJECTS_RELEASE}>
                 $<$<CONFIG:Release>:${vdpau_LIBRARIES_TARGETS}>
                 )

    if("${vdpau_LIBS_RELEASE}" STREQUAL "")
        # If the package is not declaring any "cpp_info.libs" the package deps, system libs,
        # frameworks etc are not linked to the imported targets and we need to do it to the
        # global target
        set_property(TARGET vdpau::vdpau
                     APPEND PROPERTY INTERFACE_LINK_LIBRARIES
                     vdpau_DEPS_TARGET)
    endif()

    set_property(TARGET vdpau::vdpau
                 APPEND PROPERTY INTERFACE_LINK_OPTIONS
                 $<$<CONFIG:Release>:${vdpau_LINKER_FLAGS_RELEASE}>)
    set_property(TARGET vdpau::vdpau
                 APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES
                 $<$<CONFIG:Release>:${vdpau_INCLUDE_DIRS_RELEASE}>)
    # Necessary to find LINK shared libraries in Linux
    set_property(TARGET vdpau::vdpau
                 APPEND PROPERTY INTERFACE_LINK_DIRECTORIES
                 $<$<CONFIG:Release>:${vdpau_LIB_DIRS_RELEASE}>)
    set_property(TARGET vdpau::vdpau
                 APPEND PROPERTY INTERFACE_COMPILE_DEFINITIONS
                 $<$<CONFIG:Release>:${vdpau_COMPILE_DEFINITIONS_RELEASE}>)
    set_property(TARGET vdpau::vdpau
                 APPEND PROPERTY INTERFACE_COMPILE_OPTIONS
                 $<$<CONFIG:Release>:${vdpau_COMPILE_OPTIONS_RELEASE}>)

########## For the modules (FindXXX)
set(vdpau_LIBRARIES_RELEASE vdpau::vdpau)
