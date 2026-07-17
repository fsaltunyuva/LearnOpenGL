
set(BUILD_SHARED_LIBS OFF)
# ========================== #
#           GLFW             #
# ========================== #
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_DOCS OFF)
set(GLFW_INSTALL OFF)
set(GLFW_INSTALL OFF)
set(GLFW_USE_HYBRID_HPG ON)

add_subdirectory(glfw)

set_target_properties(update_mappings PROPERTIES FOLDER "Ext")
set_target_properties(glfw PROPERTIES FOLDER Ext)

# ========================== #
#            GLM             #
# ========================== #
set(GLM_ENABLE_CXX_20)

add_subdirectory(glm)

set_target_properties(glm PROPERTIES FOLDER Ext)

# ========================== #
#           GLAD             #
# ========================== #
add_subdirectory(glad)
set_target_properties(glad PROPERTIES FOLDER Ext)

# ========================== #
#            STB             #
# ========================== #
add_subdirectory(stb_image)
set_target_properties(stb_image PROPERTIES FOLDER Ext)

