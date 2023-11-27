find_package(Boost 1.71.0 COMPONENTS REQUIRED )
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

set(main_src
    source/main.cpp
)

include(source/FileManager/CMakeLists.txt)
include(source/Config/CMakeLists.txt)
include(source/Filter/CMakeLists.txt)
include(source/ImageFilter/CMakeLists.txt)
include(source/SignalFilter/CMakeLists.txt)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Signal ${main_src} ${file_manager_src} ${config_src} ${filter_src} ${thread_pool_src} ${signal_src} ${signal_filter_src})
add_executable(Image ${main_src} ${file_manager_src} ${config_src} ${filter_src} ${thread_pool_src} ${image_filter_src})

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
target_link_libraries(Signal LINK_PUBLIC ${Boost_LIBRARIES})