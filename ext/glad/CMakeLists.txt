
add_library(glad)
target_sources(glad PRIVATE  glad.c "include/glad/glad.h")
set_target_properties(glad PROPERTIES FOLDER Ext)
target_include_directories(glad SYSTEM PUBLIC include)
