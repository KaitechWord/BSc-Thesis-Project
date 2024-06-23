# Load the debug and release variables
file(GLOB DATA_FILES "${CMAKE_CURRENT_LIST_DIR}/vaapi-*-data.cmake")

foreach(f ${DATA_FILES})
    include(${f})
endforeach()

# Create the targets for all the components
foreach(_COMPONENT ${vaapi_COMPONENT_NAMES} )
    if(NOT TARGET ${_COMPONENT})
        add_library(${_COMPONENT} INTERFACE IMPORTED)
        message(${vaapi_MESSAGE_MODE} "Conan: Component target declared '${_COMPONENT}'")
    endif()
endforeach()

if(NOT TARGET vaapi::vaapi)
    add_library(vaapi::vaapi INTERFACE IMPORTED)
    message(${vaapi_MESSAGE_MODE} "Conan: Target declared 'vaapi::vaapi'")
endif()
# Load the debug and release library finders
file(GLOB CONFIG_FILES "${CMAKE_CURRENT_LIST_DIR}/vaapi-Target-*.cmake")

foreach(f ${CONFIG_FILES})
    include(${f})
endforeach()