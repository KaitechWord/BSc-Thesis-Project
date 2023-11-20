find_package( Boost 1.40 COMPONENTS program_options REQUIRED )
include_directories( ${Boost_INCLUDE_DIR} )

set(main_src
    source/main.cpp
)

include(source/FileManager/CMakeLists.txt)
include(source/Config/CMakeLists.txt)
include(source/Filter/CMakeLists.txt)
include(source/ImageFilter/CMakeLists.txt)
include(source/SignalFilter/CMakeLists.txt)

add_executable(Signal ${main_src} ${file_manager_src} ${config_src} ${filter_src} ${image_filter_src} ${${signal_filter_src}})
add_executable(Image ${main_src} ${file_manager_src} ${config_src} ${filter_src} ${image_filter_src} ${${signal_filter_src}})

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
target_link_libraries(Signal LINK_PUBLIC ${Boost_LIBRARIES})