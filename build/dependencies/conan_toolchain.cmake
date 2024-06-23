

# Conan automatically generated toolchain file
# DO NOT EDIT MANUALLY, it will be overwritten

# Avoid including toolchain file several times (bad if appending to variables like
#   CMAKE_CXX_FLAGS. See https://github.com/android/ndk/issues/323
include_guard()

message(STATUS "Using Conan toolchain: ${CMAKE_CURRENT_LIST_FILE}")

if(${CMAKE_VERSION} VERSION_LESS "3.15")
    message(FATAL_ERROR "The 'CMakeToolchain' generator only works with CMake >= 3.15")
endif()




########## generic_system block #############
# Definition of system, platform and toolset
#############################################







string(APPEND CONAN_CXX_FLAGS " -m64")
string(APPEND CONAN_C_FLAGS " -m64")
string(APPEND CONAN_SHARED_LINKER_FLAGS " -m64")
string(APPEND CONAN_EXE_LINKER_FLAGS " -m64")



message(STATUS "Conan toolchain: C++ Standard 17 with extensions ON")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Conan conf flags start: 
# Conan conf flags end

foreach(config IN LISTS CMAKE_CONFIGURATION_TYPES)
    string(TOUPPER ${config} config)
    if(DEFINED CONAN_CXX_FLAGS_${config})
      string(APPEND CMAKE_CXX_FLAGS_${config}_INIT " ${CONAN_CXX_FLAGS_${config}}")
    endif()
    if(DEFINED CONAN_C_FLAGS_${config})
      string(APPEND CMAKE_C_FLAGS_${config}_INIT " ${CONAN_C_FLAGS_${config}}")
    endif()
    if(DEFINED CONAN_SHARED_LINKER_FLAGS_${config})
      string(APPEND CMAKE_SHARED_LINKER_FLAGS_${config}_INIT " ${CONAN_SHARED_LINKER_FLAGS_${config}}")
    endif()
    if(DEFINED CONAN_EXE_LINKER_FLAGS_${config})
      string(APPEND CMAKE_EXE_LINKER_FLAGS_${config}_INIT " ${CONAN_EXE_LINKER_FLAGS_${config}}")
    endif()
endforeach()

if(DEFINED CONAN_CXX_FLAGS)
  string(APPEND CMAKE_CXX_FLAGS_INIT " ${CONAN_CXX_FLAGS}")
endif()
if(DEFINED CONAN_C_FLAGS)
  string(APPEND CMAKE_C_FLAGS_INIT " ${CONAN_C_FLAGS}")
endif()
if(DEFINED CONAN_SHARED_LINKER_FLAGS)
  string(APPEND CMAKE_SHARED_LINKER_FLAGS_INIT " ${CONAN_SHARED_LINKER_FLAGS}")
endif()
if(DEFINED CONAN_EXE_LINKER_FLAGS)
  string(APPEND CMAKE_EXE_LINKER_FLAGS_INIT " ${CONAN_EXE_LINKER_FLAGS}")
endif()




get_property( _CMAKE_IN_TRY_COMPILE GLOBAL PROPERTY IN_TRY_COMPILE )
if(_CMAKE_IN_TRY_COMPILE)
    message(STATUS "Running toolchain IN_TRY_COMPILE")
    return()
endif()

set(CMAKE_FIND_PACKAGE_PREFER_CONFIG ON)

# Definition of CMAKE_MODULE_PATH
list(PREPEND CMAKE_MODULE_PATH "/home/kajtekk/.conan2/p/b/proto6fafaadab0438/p/lib/cmake/protobuf" "/home/kajtekk/.conan2/p/b/openj2ca8b1aafa170/p/lib/cmake" "/home/kajtekk/.conan2/p/b/opens24dcde72b5eee/p/lib/cmake")
# the generators folder (where conan generates files, like this toolchain)
list(PREPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR})

# Definition of CMAKE_PREFIX_PATH, CMAKE_XXXXX_PATH
# The explicitly defined "builddirs" of "host" context dependencies must be in PREFIX_PATH
list(PREPEND CMAKE_PREFIX_PATH "/home/kajtekk/.conan2/p/b/proto6fafaadab0438/p/lib/cmake/protobuf" "/home/kajtekk/.conan2/p/b/openj2ca8b1aafa170/p/lib/cmake" "/home/kajtekk/.conan2/p/b/opens24dcde72b5eee/p/lib/cmake")
# The Conan local "generators" folder, where this toolchain is saved.
list(PREPEND CMAKE_PREFIX_PATH ${CMAKE_CURRENT_LIST_DIR} )
list(PREPEND CMAKE_LIBRARY_PATH "/home/kajtekk/.conan2/p/b/boost4fec724204a8d/p/lib" "/home/kajtekk/.conan2/p/b/libba10901ab442afa/p/lib" "/home/kajtekk/.conan2/p/b/opencf42a2daa4d43e/p/lib" "lib" "/home/kajtekk/.conan2/p/b/proto6fafaadab0438/p/lib" "/home/kajtekk/.conan2/p/b/ade1b56be87b68c5/p/lib" "/home/kajtekk/.conan2/p/b/xkbco82ebf4d50669b/p/lib" "/home/kajtekk/.conan2/p/b/wayla1540b2f56caa3/p/lib" "/home/kajtekk/.conan2/p/b/libff602c648d52b56/p/lib" "/home/kajtekk/.conan2/p/b/libxm8c0cee7ae2820/p/lib" "/home/kajtekk/.conan2/p/b/expat376ae912d4bab/p/lib" "/home/kajtekk/.conan2/p/b/jaspea8ba52672b1d1/p/lib" "/home/kajtekk/.conan2/p/b/opene51f17798d01bb/p/lib" "/home/kajtekk/.conan2/p/b/imath32f001d6589b2/p/lib" "/home/kajtekk/.conan2/p/b/libti7c378e29e3c7c/p/lib" "/home/kajtekk/.conan2/p/b/libded690f2568c152/p/lib" "/home/kajtekk/.conan2/p/b/libjp0810ce939e870/p/lib" "/home/kajtekk/.conan2/p/b/jbigc66f80a3deba7/p/lib" "/home/kajtekk/.conan2/p/b/zstd85ec161e2dbdf/p/lib" "/home/kajtekk/.conan2/p/b/quirc463a0bdbf0449/p/lib" "/home/kajtekk/.conan2/p/b/ffmpeb5774809463b9/p/lib" "/home/kajtekk/.conan2/p/b/xz_utce29ac95b72a9/p/lib" "/home/kajtekk/.conan2/p/b/freet3b2872fc75356/p/lib" "/home/kajtekk/.conan2/p/b/libpn88ca2dc9857cd/p/lib" "/home/kajtekk/.conan2/p/b/bzip2ea144566b91c2/p/lib" "/home/kajtekk/.conan2/p/b/brotl17ec507b558f1/p/lib" "/home/kajtekk/.conan2/p/b/openj2ca8b1aafa170/p/lib" "/home/kajtekk/.conan2/p/b/openh56e1a48ed12b3/p/lib" "/home/kajtekk/.conan2/p/b/libx213cd63480dbb8/p/lib" "/home/kajtekk/.conan2/p/b/libx26c562c20c3bf6/p/lib" "/home/kajtekk/.conan2/p/b/libvp707958cceaa84/p/lib" "/home/kajtekk/.conan2/p/b/libfd09904261a0d71/p/lib" "/home/kajtekk/.conan2/p/b/libwe5b6f490ad182f/p/lib" "/home/kajtekk/.conan2/p/b/pulse63823e94e9e05/p/lib" "/home/kajtekk/.conan2/p/b/pulse63823e94e9e05/p/lib/pulseaudio" "/home/kajtekk/.conan2/p/b/libic338b515268c7d/p/lib" "/home/kajtekk/.conan2/p/b/libsn2840507ebd251/p/lib" "/home/kajtekk/.conan2/p/b/vorbiaf4619e36c815/p/lib" "/home/kajtekk/.conan2/p/b/flac50a55013ccbf6/p/lib" "/home/kajtekk/.conan2/p/b/oggc3905aa39104c/p/lib" "/home/kajtekk/.conan2/p/b/opusa3dd93890c285/p/lib" "/home/kajtekk/.conan2/p/b/mpg12803f3c54a60a0/p/lib" "/home/kajtekk/.conan2/p/b/libmpcee6b3d6ac425/p/lib" "/home/kajtekk/.conan2/p/b/libca025171a8669ee/p/lib" "/home/kajtekk/.conan2/p/b/libto7c8b19ede4173/p/lib" "/home/kajtekk/.conan2/p/b/libalf1fc6e1c6bca4/p/lib" "/home/kajtekk/.conan2/p/b/opens24dcde72b5eee/p/lib" "/home/kajtekk/.conan2/p/b/zlib1707f0edc7d0a/p/lib" "/home/kajtekk/.conan2/p/b/libaoee8a8b5831a95/p/lib" "/home/kajtekk/.conan2/p/b/dav1dc6977c4812cc3/p/lib")
list(PREPEND CMAKE_INCLUDE_PATH "/home/kajtekk/.conan2/p/b/boost4fec724204a8d/p/include" "/home/kajtekk/.conan2/p/b/libba10901ab442afa/p/include" "/home/kajtekk/.conan2/p/b/opencf42a2daa4d43e/p/include" "/home/kajtekk/.conan2/p/b/opencf42a2daa4d43e/p/include/opencv4" "include" "/home/kajtekk/.conan2/p/b/proto6fafaadab0438/p/include" "/home/kajtekk/.conan2/p/b/ade1b56be87b68c5/p/include" "/home/kajtekk/.conan2/p/b/xkbco82ebf4d50669b/p/include" "/home/kajtekk/.conan2/p/b/wayla1540b2f56caa3/p/include" "/home/kajtekk/.conan2/p/b/libff602c648d52b56/p/include" "/home/kajtekk/.conan2/p/b/libxm8c0cee7ae2820/p/include" "/home/kajtekk/.conan2/p/b/libxm8c0cee7ae2820/p/include/libxml2" "/home/kajtekk/.conan2/p/b/expat376ae912d4bab/p/include" "/home/kajtekk/.conan2/p/b/jaspea8ba52672b1d1/p/include" "/home/kajtekk/.conan2/p/b/opene51f17798d01bb/p/include" "/home/kajtekk/.conan2/p/b/opene51f17798d01bb/p/include/OpenEXR" "/home/kajtekk/.conan2/p/b/imath32f001d6589b2/p/include" "/home/kajtekk/.conan2/p/b/imath32f001d6589b2/p/include/Imath" "/home/kajtekk/.conan2/p/b/libti7c378e29e3c7c/p/include" "/home/kajtekk/.conan2/p/b/libded690f2568c152/p/include" "/home/kajtekk/.conan2/p/b/libjp0810ce939e870/p/include" "/home/kajtekk/.conan2/p/b/jbigc66f80a3deba7/p/include" "/home/kajtekk/.conan2/p/b/zstd85ec161e2dbdf/p/include" "/home/kajtekk/.conan2/p/b/quirc463a0bdbf0449/p/include" "/home/kajtekk/.conan2/p/b/ffmpeb5774809463b9/p/include" "/home/kajtekk/.conan2/p/b/xz_utce29ac95b72a9/p/include" "/home/kajtekk/.conan2/p/b/freet3b2872fc75356/p/include" "/home/kajtekk/.conan2/p/b/freet3b2872fc75356/p/include/freetype2" "/home/kajtekk/.conan2/p/b/libpn88ca2dc9857cd/p/include" "/home/kajtekk/.conan2/p/b/bzip2ea144566b91c2/p/include" "/home/kajtekk/.conan2/p/b/brotl17ec507b558f1/p/include" "/home/kajtekk/.conan2/p/b/brotl17ec507b558f1/p/include/brotli" "/home/kajtekk/.conan2/p/b/openj2ca8b1aafa170/p/include" "/home/kajtekk/.conan2/p/b/openj2ca8b1aafa170/p/include/openjpeg-2.5" "/home/kajtekk/.conan2/p/b/openh56e1a48ed12b3/p/include" "/home/kajtekk/.conan2/p/b/libx213cd63480dbb8/p/include" "/home/kajtekk/.conan2/p/b/libx26c562c20c3bf6/p/include" "/home/kajtekk/.conan2/p/b/libvp707958cceaa84/p/include" "/home/kajtekk/.conan2/p/b/libfd09904261a0d71/p/include" "/home/kajtekk/.conan2/p/b/libwe5b6f490ad182f/p/include" "/home/kajtekk/.conan2/p/b/pulse63823e94e9e05/p/include" "/home/kajtekk/.conan2/p/b/libic338b515268c7d/p/include" "/home/kajtekk/.conan2/p/b/libsn2840507ebd251/p/include" "/home/kajtekk/.conan2/p/b/vorbiaf4619e36c815/p/include" "/home/kajtekk/.conan2/p/b/flac50a55013ccbf6/p/include" "/home/kajtekk/.conan2/p/b/oggc3905aa39104c/p/include" "/home/kajtekk/.conan2/p/b/opusa3dd93890c285/p/include" "/home/kajtekk/.conan2/p/b/opusa3dd93890c285/p/include/opus" "/home/kajtekk/.conan2/p/b/mpg12803f3c54a60a0/p/include" "/home/kajtekk/.conan2/p/b/libmpcee6b3d6ac425/p/include" "/home/kajtekk/.conan2/p/b/libca025171a8669ee/p/include" "/home/kajtekk/.conan2/p/b/libto7c8b19ede4173/p/include" "/home/kajtekk/.conan2/p/b/libalf1fc6e1c6bca4/p/include" "/home/kajtekk/.conan2/p/b/opens24dcde72b5eee/p/include" "/home/kajtekk/.conan2/p/b/zlib1707f0edc7d0a/p/include" "/home/kajtekk/.conan2/p/b/libaoee8a8b5831a95/p/include" "/home/kajtekk/.conan2/p/b/dav1dc6977c4812cc3/p/include")
set(CONAN_RUNTIME_LIB_DIRS "/home/kajtekk/.conan2/p/b/boost4fec724204a8d/p/lib" "/home/kajtekk/.conan2/p/b/libba10901ab442afa/p/lib" "/home/kajtekk/.conan2/p/b/opencf42a2daa4d43e/p/lib" "lib" "/home/kajtekk/.conan2/p/b/proto6fafaadab0438/p/lib" "/home/kajtekk/.conan2/p/b/ade1b56be87b68c5/p/lib" "/home/kajtekk/.conan2/p/b/xkbco82ebf4d50669b/p/lib" "/home/kajtekk/.conan2/p/b/wayla1540b2f56caa3/p/lib" "/home/kajtekk/.conan2/p/b/libff602c648d52b56/p/lib" "/home/kajtekk/.conan2/p/b/libxm8c0cee7ae2820/p/lib" "/home/kajtekk/.conan2/p/b/expat376ae912d4bab/p/lib" "/home/kajtekk/.conan2/p/b/jaspea8ba52672b1d1/p/lib" "/home/kajtekk/.conan2/p/b/opene51f17798d01bb/p/lib" "/home/kajtekk/.conan2/p/b/imath32f001d6589b2/p/lib" "/home/kajtekk/.conan2/p/b/libti7c378e29e3c7c/p/lib" "/home/kajtekk/.conan2/p/b/libded690f2568c152/p/lib" "/home/kajtekk/.conan2/p/b/libjp0810ce939e870/p/lib" "/home/kajtekk/.conan2/p/b/jbigc66f80a3deba7/p/lib" "/home/kajtekk/.conan2/p/b/zstd85ec161e2dbdf/p/lib" "/home/kajtekk/.conan2/p/b/quirc463a0bdbf0449/p/lib" "/home/kajtekk/.conan2/p/b/ffmpeb5774809463b9/p/lib" "/home/kajtekk/.conan2/p/b/xz_utce29ac95b72a9/p/lib" "/home/kajtekk/.conan2/p/b/freet3b2872fc75356/p/lib" "/home/kajtekk/.conan2/p/b/libpn88ca2dc9857cd/p/lib" "/home/kajtekk/.conan2/p/b/bzip2ea144566b91c2/p/lib" "/home/kajtekk/.conan2/p/b/brotl17ec507b558f1/p/lib" "/home/kajtekk/.conan2/p/b/openj2ca8b1aafa170/p/lib" "/home/kajtekk/.conan2/p/b/openh56e1a48ed12b3/p/lib" "/home/kajtekk/.conan2/p/b/libx213cd63480dbb8/p/lib" "/home/kajtekk/.conan2/p/b/libx26c562c20c3bf6/p/lib" "/home/kajtekk/.conan2/p/b/libvp707958cceaa84/p/lib" "/home/kajtekk/.conan2/p/b/libfd09904261a0d71/p/lib" "/home/kajtekk/.conan2/p/b/libwe5b6f490ad182f/p/lib" "/home/kajtekk/.conan2/p/b/pulse63823e94e9e05/p/lib" "/home/kajtekk/.conan2/p/b/pulse63823e94e9e05/p/lib/pulseaudio" "/home/kajtekk/.conan2/p/b/libic338b515268c7d/p/lib" "/home/kajtekk/.conan2/p/b/libsn2840507ebd251/p/lib" "/home/kajtekk/.conan2/p/b/vorbiaf4619e36c815/p/lib" "/home/kajtekk/.conan2/p/b/flac50a55013ccbf6/p/lib" "/home/kajtekk/.conan2/p/b/oggc3905aa39104c/p/lib" "/home/kajtekk/.conan2/p/b/opusa3dd93890c285/p/lib" "/home/kajtekk/.conan2/p/b/mpg12803f3c54a60a0/p/lib" "/home/kajtekk/.conan2/p/b/libmpcee6b3d6ac425/p/lib" "/home/kajtekk/.conan2/p/b/libca025171a8669ee/p/lib" "/home/kajtekk/.conan2/p/b/libto7c8b19ede4173/p/lib" "/home/kajtekk/.conan2/p/b/libalf1fc6e1c6bca4/p/lib" "/home/kajtekk/.conan2/p/b/opens24dcde72b5eee/p/lib" "/home/kajtekk/.conan2/p/b/zlib1707f0edc7d0a/p/lib" "/home/kajtekk/.conan2/p/b/libaoee8a8b5831a95/p/lib" "/home/kajtekk/.conan2/p/b/dav1dc6977c4812cc3/p/lib" )



if (DEFINED ENV{PKG_CONFIG_PATH})
set(ENV{PKG_CONFIG_PATH} "${CMAKE_CURRENT_LIST_DIR}:$ENV{PKG_CONFIG_PATH}")
else()
set(ENV{PKG_CONFIG_PATH} "${CMAKE_CURRENT_LIST_DIR}:")
endif()




set(CMAKE_INSTALL_BINDIR "bin")
set(CMAKE_INSTALL_SBINDIR "bin")
set(CMAKE_INSTALL_LIBEXECDIR "bin")
set(CMAKE_INSTALL_LIBDIR "lib")
set(CMAKE_INSTALL_INCLUDEDIR "include")
set(CMAKE_INSTALL_OLDINCLUDEDIR "include")


# Variables
# Variables  per configuration


# Preprocessor definitions
# Preprocessor definitions per configuration


if(CMAKE_POLICY_DEFAULT_CMP0091)  # Avoid unused and not-initialized warnings
endif()
