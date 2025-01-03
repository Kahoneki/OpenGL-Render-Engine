cmake_minimum_required(VERSION 3.10)
project(NekiEngine)

file(GLOB_RECURSE SOURCES "Source Files/*.cpp" "Source Files/*.c" "Shaders/*.cpp" "program.cpp")
add_executable(NekiExecutable ${SOURCES})

include_directories(vendor/include)

set_target_properties(NekiExecutable PROPERTIES
    LINK_DIRECTORIES "${CMAKE_SOURCE_DIR}/vendor/lib"
)
target_link_libraries(NekiExecutable glfw3 assimp opengl32)

add_custom_command(TARGET NekiExecutable POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:NekiExecutable>/Shaders"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/Shaders"
        "$<TARGET_FILE_DIR:NekiExecutable>/Shaders"
    DEPENDS ${CMAKE_SOURCE_DIR}/Shaders
    COMMENT "Copying Shaders directory to output"
)

add_custom_command(TARGET NekiExecutable POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:NekiExecutable>/Resource Files"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/Resource Files"
        "$<TARGET_FILE_DIR:NekiExecutable>/Resource Files"
    DEPENDS ${CMAKE_SOURCE_DIR}/Resource Files
    COMMENT "Copying Resource Files directory to output"
)

add_custom_command(TARGET NekiExecutable POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/vendor/lib/assimp-vc143-mtd.dll"
        "$<TARGET_FILE_DIR:NekiExecutable>"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/vendor/lib/glfw3.dll"
        "$<TARGET_FILE_DIR:NekiExecutable>"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/vendor/lib/glfw3.dll"
        "$<TARGET_FILE_DIR:NekiExecutable>"
    COMMENT "Copying Assimp and GLFW DLLs to output directory"
)