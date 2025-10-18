add_library(common STATIC)

target_sources(common 
    PRIVATE 
        src/vector.cpp
        src/cylinder.cpp
        src/matte.cpp
        src/metal.cpp
        src/ray.cpp
        src/refractive.cpp
        src/sphere.cpp
        src/pov.cpp
        src/image.cpp
)

target_include_directories(common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(common PUBLIC Microsoft.GSL::GSL)



