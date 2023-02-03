include_directories(${CMAKE_CURRENT_SOURCE_DIR}/shared)
set(SHARED_FILES 
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/ShaderProgram.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/ShaderProgram.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/OBJLoader.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/OBJLoader.h
)


# Seance 01: Introduction
# - imGUI example
add_subdirectory(01_imGUIExample)
add_subdirectory(01_imGUIDemo)
# - draw 2 triangles (simplest example)
add_subdirectory(01_Triangles)
# - color (uniform, vertex attribute, blending)
add_subdirectory(02_PositionAndColor)
# - example lighting cylinder with directional light
add_subdirectory(03_LightingNoCamera)
# - simple example of different transformation
add_subdirectory(04_Transformation)
# - example animation and rotations
add_subdirectory(04_Animation)

