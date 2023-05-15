cmake_minimum_required(VERSION 3.10)
project(StarBear VERSION 0.0.1)

# Exports compile commands as a .json file; some text editors use
# this file for syntax highlighting.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find the required external dependencies.
find_package(Freetype REQUIRED)
find_package(assimp REQUIRED)
find_package(glfw3 REQUIRED)

# Gather up the source and header files.
list(APPEND SOURCE_FILES
            3rd_party/glad/glad.c
            src/core/Game.cpp
            src/graphics/Font.cpp
            src/graphics/Mesh.cpp
            src/graphics/Model.cpp
            src/graphics/Shader.cpp
            src/graphics/Texture.cpp
            src/systems/CollisionSystem.cpp
            src/systems/EnemySystem.cpp
            src/systems/LaserSystem.cpp
            src/systems/PhysicsSystem.cpp
            src/systems/ShipControllerSystem.cpp
            src/systems/ShipRenderSystem.cpp
            src/systems/ParticleEmitterSystem.cpp
            src/main.cpp)

# Create the executable.
add_executable(starBear ${SOURCE_FILES})
target_link_libraries(starBear PUBLIC glfw
                                      ${FREETYPE_LIBRARIES}
                                      ${ASSIMP_LIBRARIES})
target_include_directories(starBear PUBLIC 3rd_party
                                           src/components
                                           src/core
                                           src/graphics
                                           src/math
                                           src/systems
                                           src/util
                                           ${FREETYPE_INCLUDE_DIRS}
                                           ${ASSIMP_INCLUDE_DIRS})

# Install the executable.
install(TARGETS starBear DESTINATION ${CMAKE_SOURCE_DIR}/install)
install(DIRECTORY resources DESTINATION ${CMAKE_SOURCE_DIR}/install)
