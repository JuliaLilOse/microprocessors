include("${CMAKE_CURRENT_LIST_DIR}/rule.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/file.cmake")

set(lab3_pt2_default_library_list )

# Handle files with suffix (s|as|asm|AS|ASM|As|aS|Asm), for group default-XC8
if(lab3_pt2_default_default_XC8_FILE_TYPE_assemble)
add_library(lab3_pt2_default_default_XC8_assemble OBJECT ${lab3_pt2_default_default_XC8_FILE_TYPE_assemble})
    lab3_pt2_default_default_XC8_assemble_rule(lab3_pt2_default_default_XC8_assemble)
    list(APPEND lab3_pt2_default_library_list "$<TARGET_OBJECTS:lab3_pt2_default_default_XC8_assemble>")

endif()

# Handle files with suffix S, for group default-XC8
if(lab3_pt2_default_default_XC8_FILE_TYPE_assemblePreprocess)
add_library(lab3_pt2_default_default_XC8_assemblePreprocess OBJECT ${lab3_pt2_default_default_XC8_FILE_TYPE_assemblePreprocess})
    lab3_pt2_default_default_XC8_assemblePreprocess_rule(lab3_pt2_default_default_XC8_assemblePreprocess)
    list(APPEND lab3_pt2_default_library_list "$<TARGET_OBJECTS:lab3_pt2_default_default_XC8_assemblePreprocess>")

endif()

# Handle files with suffix [cC], for group default-XC8
if(lab3_pt2_default_default_XC8_FILE_TYPE_compile)
add_library(lab3_pt2_default_default_XC8_compile OBJECT ${lab3_pt2_default_default_XC8_FILE_TYPE_compile})
    lab3_pt2_default_default_XC8_compile_rule(lab3_pt2_default_default_XC8_compile)
    list(APPEND lab3_pt2_default_library_list "$<TARGET_OBJECTS:lab3_pt2_default_default_XC8_compile>")

endif()

# Handle files with suffix elf, for group default-XC8
if(lab3_pt2_default_default_XC8_FILE_TYPE_objcopy_avr)
add_library(lab3_pt2_default_default_XC8_objcopy_avr OBJECT ${lab3_pt2_default_default_XC8_FILE_TYPE_objcopy_avr})
    lab3_pt2_default_default_XC8_objcopy_avr_rule(lab3_pt2_default_default_XC8_objcopy_avr)
    list(APPEND lab3_pt2_default_library_list "$<TARGET_OBJECTS:lab3_pt2_default_default_XC8_objcopy_avr>")

endif()

# Handle files with suffix elf, for group default-XC8
if(lab3_pt2_default_default_XC8_FILE_TYPE_objcopy_lss)
add_library(lab3_pt2_default_default_XC8_objcopy_lss OBJECT ${lab3_pt2_default_default_XC8_FILE_TYPE_objcopy_lss})
    lab3_pt2_default_default_XC8_objcopy_lss_rule(lab3_pt2_default_default_XC8_objcopy_lss)
    list(APPEND lab3_pt2_default_library_list "$<TARGET_OBJECTS:lab3_pt2_default_default_XC8_objcopy_lss>")

endif()


# Main target for this project
add_executable(lab3_pt2_default_image_x2XvrH_B ${lab3_pt2_default_library_list})

set_target_properties(lab3_pt2_default_image_x2XvrH_B PROPERTIES
    OUTPUT_NAME "default"
    SUFFIX ".elf"
    ADDITIONAL_CLEAN_FILES "${output_extensions}"
    RUNTIME_OUTPUT_DIRECTORY "${lab3_pt2_default_output_dir}")
target_link_libraries(lab3_pt2_default_image_x2XvrH_B PRIVATE ${lab3_pt2_default_default_XC8_FILE_TYPE_link})
# Add the link options from the rule file.
lab3_pt2_default_link_rule( lab3_pt2_default_image_x2XvrH_B)


#Add objcopy steps
lab3_pt2_default_objcopy_avr_rule(lab3_pt2_default_image_x2XvrH_B)
lab3_pt2_default_objcopy_lss_rule(lab3_pt2_default_image_x2XvrH_B)
