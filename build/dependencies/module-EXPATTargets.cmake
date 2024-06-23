# Load the debug and release variables
file(GLOB DATA_FILES "${CMAKE_CURRENT_LIST_DIR}/module-EXPAT-*-data.cmake")

foreach(f ${DATA_FILES})
    include(${f})
endforeach()

# Create the targets for all the components
foreach(_COMPONENT ${expat_COMPONENT_NAMES} )
    if(NOT TARGET ${_COMPONENT})
        add_library(${_COMPONENT} INTERFACE IMPORTED)
        message(${EXPAT_MESSAGE_MODE} "Conan: Component target declared '${_COMPONENT}'")
    endif()
endforeach()

if(NOT TARGET EXPAT::EXPAT)
    add_library(EXPAT::EXPAT INTERFACE IMPORTED)
    message(${EXPAT_MESSAGE_MODE} "Conan: Target declared 'EXPAT::EXPAT'")
endif()
# Load the debug and release library finders
file(GLOB CONFIG_FILES "${CMAKE_CURRENT_LIST_DIR}/module-EXPAT-Target-*.cmake")

foreach(f ${CONFIG_FILES})
    include(${f})
endforeach()