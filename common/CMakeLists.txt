add_library(common STATIC)

target_sources(common 
    PRIVATE 
        src/cylinder.cpp
        src/image.cpp
        src/matte.cpp
        src/metal.cpp
        src/parse_config.cpp
        src/parse_scene.cpp
        src/pov.cpp
        src/ray.cpp
        src/refractive.cpp
        src/sphere.cpp
        src/vector.cpp
)

target_include_directories(common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(common PUBLIC Microsoft.GSL::GSL)



