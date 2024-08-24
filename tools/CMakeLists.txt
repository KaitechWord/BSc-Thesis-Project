find_package(nlohmann_json REQUIRED)

include(source/Config/CMakeLists.txt)

add_executable(Statistics tools/statistics.cpp ${config_src})

target_compile_features(Statistics PRIVATE cxx_std_17)
target_include_directories(Statistics PRIVATE ${CMAKE_SOURCE_DIR}/source/Config)
target_link_libraries(Statistics PRIVATE nlohmann_json::nlohmann_json)