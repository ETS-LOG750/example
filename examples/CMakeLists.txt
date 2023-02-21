include_directories(${CMAKE_CURRENT_SOURCE_DIR}/shared)
set(SHARED_FILES 
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/ShaderProgram.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/ShaderProgram.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/OBJLoader.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/OBJLoader.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/Camera.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/Camera.h
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
# - example (simple) de differentes facon de dessiner un rectangle
add_subdirectory(05_DrawSquares)
# - example geometry shaders (normal computation at each vertices)
add_subdirectory(05_GeometryShader)
# - example tesselation (Bezier surfaces)
add_subdirectory(05_TesselationTeapot)
# - example utilisation unproject
add_subdirectory(06_Unproject)
# - lab 2 obj loading example
add_subdirectory(Lab_2_ObjLoader)
# - lab 2 picking
add_subdirectory(Lab_2_Picking)

