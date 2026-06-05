include("${CMAKE_CURRENT_LIST_DIR}/rule.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/file.cmake")

set(MinSpecHSM_default_library_list )

# Handle files with suffix s, for group default-XC32
if(MinSpecHSM_default_default_XC32_FILE_TYPE_assemble)
add_library(MinSpecHSM_default_default_XC32_assemble OBJECT ${MinSpecHSM_default_default_XC32_FILE_TYPE_assemble})
    MinSpecHSM_default_default_XC32_assemble_rule(MinSpecHSM_default_default_XC32_assemble)
    list(APPEND MinSpecHSM_default_library_list "$<TARGET_OBJECTS:MinSpecHSM_default_default_XC32_assemble>")

endif()

# Handle files with suffix S, for group default-XC32
if(MinSpecHSM_default_default_XC32_FILE_TYPE_assembleWithPreprocess)
add_library(MinSpecHSM_default_default_XC32_assembleWithPreprocess OBJECT ${MinSpecHSM_default_default_XC32_FILE_TYPE_assembleWithPreprocess})
    MinSpecHSM_default_default_XC32_assembleWithPreprocess_rule(MinSpecHSM_default_default_XC32_assembleWithPreprocess)
    list(APPEND MinSpecHSM_default_library_list "$<TARGET_OBJECTS:MinSpecHSM_default_default_XC32_assembleWithPreprocess>")

endif()

# Handle files with suffix [cC], for group default-XC32
if(MinSpecHSM_default_default_XC32_FILE_TYPE_compile)
add_library(MinSpecHSM_default_default_XC32_compile OBJECT ${MinSpecHSM_default_default_XC32_FILE_TYPE_compile})
    MinSpecHSM_default_default_XC32_compile_rule(MinSpecHSM_default_default_XC32_compile)
    list(APPEND MinSpecHSM_default_library_list "$<TARGET_OBJECTS:MinSpecHSM_default_default_XC32_compile>")

endif()

# Handle files with suffix cpp, for group default-XC32
if(MinSpecHSM_default_default_XC32_FILE_TYPE_compile_cpp)
add_library(MinSpecHSM_default_default_XC32_compile_cpp OBJECT ${MinSpecHSM_default_default_XC32_FILE_TYPE_compile_cpp})
    MinSpecHSM_default_default_XC32_compile_cpp_rule(MinSpecHSM_default_default_XC32_compile_cpp)
    list(APPEND MinSpecHSM_default_library_list "$<TARGET_OBJECTS:MinSpecHSM_default_default_XC32_compile_cpp>")

endif()

# Handle files with suffix [cC], for group default-XC32
if(MinSpecHSM_default_default_XC32_FILE_TYPE_dependentObject)
add_library(MinSpecHSM_default_default_XC32_dependentObject OBJECT ${MinSpecHSM_default_default_XC32_FILE_TYPE_dependentObject})
    MinSpecHSM_default_default_XC32_dependentObject_rule(MinSpecHSM_default_default_XC32_dependentObject)
    list(APPEND MinSpecHSM_default_library_list "$<TARGET_OBJECTS:MinSpecHSM_default_default_XC32_dependentObject>")

endif()

# Handle files with suffix elf, for group default-XC32
if(MinSpecHSM_default_default_XC32_FILE_TYPE_bin2hex)
add_library(MinSpecHSM_default_default_XC32_bin2hex OBJECT ${MinSpecHSM_default_default_XC32_FILE_TYPE_bin2hex})
    MinSpecHSM_default_default_XC32_bin2hex_rule(MinSpecHSM_default_default_XC32_bin2hex)
    list(APPEND MinSpecHSM_default_library_list "$<TARGET_OBJECTS:MinSpecHSM_default_default_XC32_bin2hex>")

endif()


# Main target for this project
add_executable(MinSpecHSM_default_image_mUN63ejm ${MinSpecHSM_default_library_list})

if(NOT CMAKE_HOST_WIN32)
    set_target_properties(MinSpecHSM_default_image_mUN63ejm PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${MinSpecHSM_default_output_dir}")
endif()
set_target_properties(MinSpecHSM_default_image_mUN63ejm PROPERTIES
    OUTPUT_NAME "default"
    SUFFIX ".elf")
target_link_libraries(MinSpecHSM_default_image_mUN63ejm PRIVATE ${MinSpecHSM_default_default_XC32_FILE_TYPE_link})

# Add the link options from the rule file.
MinSpecHSM_default_link_rule( MinSpecHSM_default_image_mUN63ejm)

# Call bin2hex function from the rule file
MinSpecHSM_default_bin2hex_rule(MinSpecHSM_default_image_mUN63ejm)
if(CMAKE_HOST_WIN32)
    add_custom_command(
        TARGET MinSpecHSM_default_image_mUN63ejm
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${MinSpecHSM_default_output_dir}
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:MinSpecHSM_default_image_mUN63ejm> ${MinSpecHSM_default_output_dir}/${MinSpecHSM_default_original_image_name}
        BYPRODUCTS ${MinSpecHSM_default_output_dir}/${MinSpecHSM_default_original_image_name}
        COMMENT "Copying elf to out location")
    set_property(
        TARGET MinSpecHSM_default_image_mUN63ejm
        APPEND PROPERTY ADDITIONAL_CLEAN_FILES
        ${MinSpecHSM_default_output_dir}/${MinSpecHSM_default_original_image_name})
endif()

