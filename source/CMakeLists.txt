find_package(Boost 1.71.0 REQUIRED )
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})

include(source/Config/CMakeLists.txt)
include(source/FileManager/CMakeLists.txt)
include(source/Filter/CMakeLists.txt)
include(source/ImageFilter/CMakeLists.txt)
include(source/NaiveImageFilter/CMakeLists.txt)
include(source/SmartImageFilter/CMakeLists.txt)
include(source/NaiveSignalFilter/CMakeLists.txt)
include(source/SmartSignalFilter/CMakeLists.txt)
include(source/Signal/CMakeLists.txt)
include(source/SignalFilter/CMakeLists.txt)
include(source/ThreadPool/CMakeLists.txt)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(ROOT_DIR ${CMAKE_SOURCE_DIR})
add_definitions(-DROOT_DIR="${ROOT_DIR}")

add_executable(Signal source/signals.cpp ${file_manager_src} ${config_src} ${filter_src} ${thread_pool_src} ${signal_src} ${signal_filter_src} ${naive_signal_filter_src} ${smart_signal_filter_src})
add_executable(Image source/images.cpp ${file_manager_src} ${config_src} ${filter_src} ${thread_pool_src} ${signal_src} ${image_filter_src} ${naive_image_filter_src} ${smart_image_filter_src})

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
target_link_libraries(Signal ${Boost_LIBRARIES} ${OpenCV_LIBS})
target_link_libraries(Image ${Boost_LIBRARIES} ${OpenCV_LIBS})