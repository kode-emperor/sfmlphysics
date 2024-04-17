cmake_minimum_required(VERSION 3.29)
project(SFMLPhysics LANGUAGES CXX)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(SRC "src/main.c++")
add_executable(SFMLPhysics ${SRC})
if(WIN32) 
    list(APPEND CMAKE_PREFIX_PATH "C:\\Program Files (x86)\\ReactPhysics3D")
elseif(APPLE)
    list(APPEND CMAKE_PREFIX_PATH "usr/local/lib/cmake/ReactPhysics3D")
endif()

if(APPLE) 
    list(APPEND CMAKE_PREFIX_PATH "usr/local/lib/cmake/SFML")
elseif(WIN32)
    list(APPEND CMAKE_PREFIX_PATH "C:\\Program Files (x86)\\SFML")
endif()
find_package(Boost 1.84.0 COMPONENTS program_options REQUIRED)
find_package(SFML 2.6.1 COMPONENTS system window graphics REQUIRED)
find_package(ReactPhysics3D REQUIRED)
target_link_libraries(SFMLPhysics PRIVATE Boost::program_options sfml-graphics ReactPhysics3D::ReactPhysics3D)
target_compile_features(SFMLPhysics PRIVATE cxx_std_20)
if(WIN32)
    add_custom_command(
        TARGET SFMLPhysics
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:SFMLPhysics>
        VERBATIM)
endif()

install(TARGETS SFMLPhysics)