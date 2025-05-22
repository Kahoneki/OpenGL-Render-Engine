cmake_minimum_required(VERSION 3.31)
project(NekiEngine)
set(CMAKE_CXX_STANDARD 23)

add_compile_definitions(GLM_ENABLE_EXPERIMENTAL)

file(GLOB_RECURSE SOURCES "program.cpp" "Source Files/*.cpp" "Shaders/*.cpp")
add_executable(NekiExecutable ${SOURCES})

include_directories(vendor/include)

#GLAD
add_library(glad vendor/include/GLAD/glad.c)
target_include_directories(glad PUBLIC vendor/include/GLAD)


#ImGUI
find_package(imgui CONFIG REQUIRED)

#ASSIMP
find_package(assimp CONFIG REQUIRED)

#GLM
find_package(glm CONFIG REQUIRED)

#STB
find_package(Stb REQUIRED)
target_include_directories(NekiExecutable PRIVATE ${Stb_INCLUDE_DIR})

#OpenGL
find_package(OpenGL REQUIRED)

#GLFW (vcpkg)
find_package(glfw3 CONFIG REQUIRED)




target_link_libraries(NekiExecutable PRIVATE
        glad
        glfw
        assimp::assimp
        glm::glm
        OpenGL::GL
        imgui::imgui
)

add_custom_command(TARGET NekiExecutable POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:NekiExecutable>/Shaders"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/Shaders"
        "$<TARGET_FILE_DIR:NekiExecutable>/Shaders"
    COMMENT "Copying Shaders directory to output"
)

add_custom_command(TARGET NekiExecutable POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:NekiExecutable>/Resource Files"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/Resource Files"
        "$<TARGET_FILE_DIR:NekiExecutable>/Resource Files"
    COMMENT "Copying Resource Files directory to output"
)