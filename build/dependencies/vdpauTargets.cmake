# Load the debug and release variables
file(GLOB DATA_FILES "${CMAKE_CURRENT_LIST_DIR}/vdpau-*-data.cmake")

foreach(f ${DATA_FILES})
    include(${f})
endforeach()

# Create the targets for all the components
foreach(_COMPONENT ${vdpau_COMPONENT_NAMES} )
    if(NOT TARGET ${_COMPONENT})
        add_library(${_COMPONENT} INTERFACE IMPORTED)
        message(${vdpau_MESSAGE_MODE} "Conan: Component target declared '${_COMPONENT}'")
    endif()
endforeach()

if(NOT TARGET vdpau::vdpau)
    add_library(vdpau::vdpau INTERFACE IMPORTED)
    message(${vdpau_MESSAGE_MODE} "Conan: Target declared 'vdpau::vdpau'")
endif()
# Load the debug and release library finders
file(GLOB CONFIG_FILES "${CMAKE_CURRENT_LIST_DIR}/vdpau-Target-*.cmake")

foreach(f ${CONFIG_FILES})
    include(${f})
endforeach()