# VS-Bibliotheken in CLion nutzen
cmake_minimum_required(VERSION 3.16)

# Projektnamen anpassen
project(CGPrakt6_Clion)
set(CMAKE_CXX_STANDARD 20)

#Inhalt des source-Ordners auflisten
add_executable(${CMAKE_PROJECT_NAME}
        source/utils/Lights/ShadowMapGenerator.h
        source/utils/Lights/ShadowMapGenerator.cpp
        source/utils/Lights/ShaderLightMapper.h
        source/utils/Lights/ShaderLightMapper.cpp
        source/utils/Shader/PhongShader.h
        source/utils/Shader/PhongShader.cpp
        include/Framebuffer.h
        source/Framebuffer.cpp
        source/utils/Models/Aabb.h
        source/utils/Models/Aabb.cpp
        include/Application.h
        source/Application.cpp
        source/utils/Models/BaseModel.h
        source/utils/Models/BaseModel.cpp
        source/utils/Shader/BaseShader.h
        source/utils/Shader/BaseShader.cpp
        include/Camera.h
        source/Camera.cpp
        source/utils/YourClasses/color.h
        source/utils/YourClasses/color.cpp
        source/utils/Shader/ConstantShader.h
        source/utils/Shader/ConstantShader.cpp
        include/IndexBuffer.h
        source/IndexBuffer.cpp
        source/utils/Models/LineBoxModel.h
        source/utils/Models/LineBoxModel.cpp
        source/utils/Models/LinePlaneModel.h
        source/utils/Models/LinePlaneModel.cpp
        include/Matrix.h
        source/Matrix.cpp
        source/utils/Models/Model.h
        source/utils/Models/Model.cpp
        source/utils/YourClasses/rgbimage.h
        source/utils/YourClasses/rgbimage.cpp
        include/Texture.h
        source/Texture.cpp
        source/utils/Models/TriangleBoxModel.h
        source/utils/Models/TriangleBoxModel.cpp
        source/utils/Models/TrianglePlaneModel.h
        source/utils/Models/TrianglePlaneModel.cpp
        source/utils/Models/TriangleSphereModel.h
        source/utils/Models/TriangleSphereModel.cpp
        source/utils/YourClasses/vector.h
        source/utils/YourClasses/vector.cpp
        include/VertexBuffer.h
        source/VertexBuffer.cpp
        source/main.cpp

        source/Game/Spieler.cpp source/Game/Spieler.h source/Game/Verfolger.cpp source/Game/Verfolger.h source/Background/Terrain.cpp source/Background/Terrain.h source/Background/TerrainShader.cpp source/Background/TerrainShader.h source/utils/Shader/PostProcessingShader.cpp source/utils/Shader/PostProcessingShader.h source/utils/Shader/BloomShader.cpp source/utils/Shader/BloomShader.h source/utils/Shader/PostProcessor.cpp source/utils/Shader/PostProcessor.h source/utils/Shader/FogShader.cpp source/utils/Shader/FogShader.h source/utils/Shader/Godray.cpp source/utils/Shader/Godray.h)

include_directories(
        source
        libraries/glfw/include
        libraries/glew/include
        libraries/freeimage/include
        libraries/assimp/include)

find_package(OpenGL REQUIRED)

target_link_libraries(${CMAKE_PROJECT_NAME}
        OpenGL::GL
        OpenGL::GLU
        ../libraries/glfw/lib/glfw3
        ../libraries/glew/lib/glew32
        ../libraries/freeimage/lib/FreeImage
        ../libraries/assimp/lib/assimp-vc140-mt)
