
add_library(stb_image)
target_sources(stb_image PRIVATE stb_image.c)
target_include_directories(stb_image PUBLIC include)
set_target_properties(stb_image PROPERTIES FOLDER Ext)