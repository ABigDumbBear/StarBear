cmake_minimum_required(VERSION 3.10)
project(StarBear VERSION 0.0.1)

# Exports compile commands as a .json file; some text editors use
# this file for syntax highlighting.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find the required external dependencies.
find_package(KumaGL REQUIRED)

# Gather up the source and header files.
list(APPEND SOURCE_FILES
  src/core/Game.cpp
  src/core/ResourceMap.cpp
  src/systems/CollisionSystem.cpp
  src/systems/ParentSystem.cpp
  src/systems/PhysicsSystem.cpp
  src/systems/RailMoverSystem.cpp
  src/systems/ShipControllerSystem.cpp
  src/systems/ParticleEmitterSystem.cpp
  src/main.cpp
)

# Create the executable.
add_executable(starBear ${SOURCE_FILES})
target_link_libraries(starBear PUBLIC glfw KumaGL)
target_include_directories(starBear PUBLIC
  3rd_party/include
  src/components
  src/core
  src/systems
)

install(TARGETS starBear DESTINATION ${CMAKE_SOURCE_DIR}/install)
install(DIRECTORY resources DESTINATION ${CMAKE_SOURCE_DIR}/install)
