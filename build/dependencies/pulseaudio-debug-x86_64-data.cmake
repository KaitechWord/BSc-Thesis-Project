########### AGGREGATED COMPONENTS AND DEPENDENCIES FOR THE MULTI CONFIG #####################
#############################################################################################

list(APPEND pulseaudio_COMPONENT_NAMES pulseaudio::pulse pulseaudio::pulse-simple)
list(REMOVE_DUPLICATES pulseaudio_COMPONENT_NAMES)
if(DEFINED pulseaudio_FIND_DEPENDENCY_NAMES)
  list(APPEND pulseaudio_FIND_DEPENDENCY_NAMES Iconv SndFile libcap libtool ALSA xorg OpenSSL)
  list(REMOVE_DUPLICATES pulseaudio_FIND_DEPENDENCY_NAMES)
else()
  set(pulseaudio_FIND_DEPENDENCY_NAMES Iconv SndFile libcap libtool ALSA xorg OpenSSL)
endif()
set(Iconv_FIND_MODE "NO_MODULE")
set(SndFile_FIND_MODE "NO_MODULE")
set(libcap_FIND_MODE "NO_MODULE")
set(libtool_FIND_MODE "NO_MODULE")
set(ALSA_FIND_MODE "NO_MODULE")
set(xorg_FIND_MODE "NO_MODULE")
set(OpenSSL_FIND_MODE "NO_MODULE")

########### VARIABLES #######################################################################
#############################################################################################
set(pulseaudio_PACKAGE_FOLDER_DEBUG "/home/kajtekk/.conan2/p/b/pulse63823e94e9e05/p")
set(pulseaudio_BUILD_MODULES_PATHS_DEBUG )


set(pulseaudio_INCLUDE_DIRS_DEBUG )
set(pulseaudio_RES_DIRS_DEBUG )
set(pulseaudio_DEFINITIONS_DEBUG "-D_REENTRANT")
set(pulseaudio_SHARED_LINK_FLAGS_DEBUG )
set(pulseaudio_EXE_LINK_FLAGS_DEBUG )
set(pulseaudio_OBJECTS_DEBUG )
set(pulseaudio_COMPILE_DEFINITIONS_DEBUG "_REENTRANT")
set(pulseaudio_COMPILE_OPTIONS_C_DEBUG )
set(pulseaudio_COMPILE_OPTIONS_CXX_DEBUG )
set(pulseaudio_LIB_DIRS_DEBUG "${pulseaudio_PACKAGE_FOLDER_DEBUG}/lib"
			"${pulseaudio_PACKAGE_FOLDER_DEBUG}/lib/pulseaudio")
set(pulseaudio_BIN_DIRS_DEBUG )
set(pulseaudio_LIBRARY_TYPE_DEBUG STATIC)
set(pulseaudio_IS_HOST_WINDOWS_DEBUG 0)
set(pulseaudio_LIBS_DEBUG pulse-simple pulse pulsecommon-14.2)
set(pulseaudio_SYSTEM_LIBS_DEBUG )
set(pulseaudio_FRAMEWORK_DIRS_DEBUG )
set(pulseaudio_FRAMEWORKS_DEBUG )
set(pulseaudio_BUILD_DIRS_DEBUG )
set(pulseaudio_NO_SONAME_MODE_DEBUG FALSE)


# COMPOUND VARIABLES
set(pulseaudio_COMPILE_OPTIONS_DEBUG
    "$<$<COMPILE_LANGUAGE:CXX>:${pulseaudio_COMPILE_OPTIONS_CXX_DEBUG}>"
    "$<$<COMPILE_LANGUAGE:C>:${pulseaudio_COMPILE_OPTIONS_C_DEBUG}>")
set(pulseaudio_LINKER_FLAGS_DEBUG
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${pulseaudio_SHARED_LINK_FLAGS_DEBUG}>"
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${pulseaudio_SHARED_LINK_FLAGS_DEBUG}>"
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${pulseaudio_EXE_LINK_FLAGS_DEBUG}>")


set(pulseaudio_COMPONENTS_DEBUG pulseaudio::pulse pulseaudio::pulse-simple)
########### COMPONENT pulseaudio::pulse-simple VARIABLES ############################################

set(pulseaudio_pulseaudio_pulse-simple_INCLUDE_DIRS_DEBUG )
set(pulseaudio_pulseaudio_pulse-simple_LIB_DIRS_DEBUG "${pulseaudio_PACKAGE_FOLDER_DEBUG}/lib")
set(pulseaudio_pulseaudio_pulse-simple_BIN_DIRS_DEBUG )
set(pulseaudio_pulseaudio_pulse-simple_LIBRARY_TYPE_DEBUG STATIC)
set(pulseaudio_pulseaudio_pulse-simple_IS_HOST_WINDOWS_DEBUG 0)
set(pulseaudio_pulseaudio_pulse-simple_RES_DIRS_DEBUG )
set(pulseaudio_pulseaudio_pulse-simple_DEFINITIONS_DEBUG "-D_REENTRANT")
set(pulseaudio_pulseaudio_pulse-simple_OBJECTS_DEBUG )
set(pulseaudio_pulseaudio_pulse-simple_COMPILE_DEFINITIONS_DEBUG "_REENTRANT")
set(pulseaudio_pulseaudio_pulse-simple_COMPILE_OPTIONS_C_DEBUG "")
set(pulseaudio_pulseaudio_pulse-simple_COMPILE_OPTIONS_CXX_DEBUG "")
set(pulseaudio_pulseaudio_pulse-simple_LIBS_DEBUG pulse-simple)
set(pulseaudio_pulseaudio_pulse-simple_SYSTEM_LIBS_DEBUG )
set(pulseaudio_pulseaudio_pulse-simple_FRAMEWORK_DIRS_DEBUG )
set(pulseaudio_pulseaudio_pulse-simple_FRAMEWORKS_DEBUG )
set(pulseaudio_pulseaudio_pulse-simple_DEPENDENCIES_DEBUG pulseaudio::pulse)
set(pulseaudio_pulseaudio_pulse-simple_SHARED_LINK_FLAGS_DEBUG )
set(pulseaudio_pulseaudio_pulse-simple_EXE_LINK_FLAGS_DEBUG )
set(pulseaudio_pulseaudio_pulse-simple_NO_SONAME_MODE_DEBUG FALSE)

# COMPOUND VARIABLES
set(pulseaudio_pulseaudio_pulse-simple_LINKER_FLAGS_DEBUG
        $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${pulseaudio_pulseaudio_pulse-simple_SHARED_LINK_FLAGS_DEBUG}>
        $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${pulseaudio_pulseaudio_pulse-simple_SHARED_LINK_FLAGS_DEBUG}>
        $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${pulseaudio_pulseaudio_pulse-simple_EXE_LINK_FLAGS_DEBUG}>
)
set(pulseaudio_pulseaudio_pulse-simple_COMPILE_OPTIONS_DEBUG
    "$<$<COMPILE_LANGUAGE:CXX>:${pulseaudio_pulseaudio_pulse-simple_COMPILE_OPTIONS_CXX_DEBUG}>"
    "$<$<COMPILE_LANGUAGE:C>:${pulseaudio_pulseaudio_pulse-simple_COMPILE_OPTIONS_C_DEBUG}>")
########### COMPONENT pulseaudio::pulse VARIABLES ############################################

set(pulseaudio_pulseaudio_pulse_INCLUDE_DIRS_DEBUG )
set(pulseaudio_pulseaudio_pulse_LIB_DIRS_DEBUG "${pulseaudio_PACKAGE_FOLDER_DEBUG}/lib"
			"${pulseaudio_PACKAGE_FOLDER_DEBUG}/lib/pulseaudio")
set(pulseaudio_pulseaudio_pulse_BIN_DIRS_DEBUG )
set(pulseaudio_pulseaudio_pulse_LIBRARY_TYPE_DEBUG STATIC)
set(pulseaudio_pulseaudio_pulse_IS_HOST_WINDOWS_DEBUG 0)
set(pulseaudio_pulseaudio_pulse_RES_DIRS_DEBUG )
set(pulseaudio_pulseaudio_pulse_DEFINITIONS_DEBUG )
set(pulseaudio_pulseaudio_pulse_OBJECTS_DEBUG )
set(pulseaudio_pulseaudio_pulse_COMPILE_DEFINITIONS_DEBUG )
set(pulseaudio_pulseaudio_pulse_COMPILE_OPTIONS_C_DEBUG "")
set(pulseaudio_pulseaudio_pulse_COMPILE_OPTIONS_CXX_DEBUG "")
set(pulseaudio_pulseaudio_pulse_LIBS_DEBUG pulse pulsecommon-14.2)
set(pulseaudio_pulseaudio_pulse_SYSTEM_LIBS_DEBUG )
set(pulseaudio_pulseaudio_pulse_FRAMEWORK_DIRS_DEBUG )
set(pulseaudio_pulseaudio_pulse_FRAMEWORKS_DEBUG )
set(pulseaudio_pulseaudio_pulse_DEPENDENCIES_DEBUG Iconv::Iconv SndFile::sndfile libcap::libcap libtool::libtool ALSA::ALSA xorg::xorg openssl::openssl)
set(pulseaudio_pulseaudio_pulse_SHARED_LINK_FLAGS_DEBUG )
set(pulseaudio_pulseaudio_pulse_EXE_LINK_FLAGS_DEBUG )
set(pulseaudio_pulseaudio_pulse_NO_SONAME_MODE_DEBUG FALSE)

# COMPOUND VARIABLES
set(pulseaudio_pulseaudio_pulse_LINKER_FLAGS_DEBUG
        $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${pulseaudio_pulseaudio_pulse_SHARED_LINK_FLAGS_DEBUG}>
        $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${pulseaudio_pulseaudio_pulse_SHARED_LINK_FLAGS_DEBUG}>
        $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${pulseaudio_pulseaudio_pulse_EXE_LINK_FLAGS_DEBUG}>
)
set(pulseaudio_pulseaudio_pulse_COMPILE_OPTIONS_DEBUG
    "$<$<COMPILE_LANGUAGE:CXX>:${pulseaudio_pulseaudio_pulse_COMPILE_OPTIONS_CXX_DEBUG}>"
    "$<$<COMPILE_LANGUAGE:C>:${pulseaudio_pulseaudio_pulse_COMPILE_OPTIONS_C_DEBUG}>")