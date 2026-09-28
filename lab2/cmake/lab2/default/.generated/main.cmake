include("${CMAKE_CURRENT_LIST_DIR}/rule.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/file.cmake")

set(lab2_default_library_list )

# Handle files with suffix (s|as|asm|AS|ASM|As|aS|Asm), for group default-XC8
if(lab2_default_default_XC8_FILE_TYPE_assemble)
add_library(lab2_default_default_XC8_assemble OBJECT ${lab2_default_default_XC8_FILE_TYPE_assemble})
    lab2_default_default_XC8_assemble_rule(lab2_default_default_XC8_assemble)
    list(APPEND lab2_default_library_list "$<TARGET_OBJECTS:lab2_default_default_XC8_assemble>")

endif()

# Handle files with suffix S, for group default-XC8
if(lab2_default_default_XC8_FILE_TYPE_assemblePreprocess)
add_library(lab2_default_default_XC8_assemblePreprocess OBJECT ${lab2_default_default_XC8_FILE_TYPE_assemblePreprocess})
    lab2_default_default_XC8_assemblePreprocess_rule(lab2_default_default_XC8_assemblePreprocess)
    list(APPEND lab2_default_library_list "$<TARGET_OBJECTS:lab2_default_default_XC8_assemblePreprocess>")

endif()

# Handle files with suffix [cC], for group default-XC8
if(lab2_default_default_XC8_FILE_TYPE_compile)
add_library(lab2_default_default_XC8_compile OBJECT ${lab2_default_default_XC8_FILE_TYPE_compile})
    lab2_default_default_XC8_compile_rule(lab2_default_default_XC8_compile)
    list(APPEND lab2_default_library_list "$<TARGET_OBJECTS:lab2_default_default_XC8_compile>")

endif()

# Handle files with suffix elf, for group default-XC8
if(lab2_default_default_XC8_FILE_TYPE_objcopy_avr)
add_library(lab2_default_default_XC8_objcopy_avr OBJECT ${lab2_default_default_XC8_FILE_TYPE_objcopy_avr})
    lab2_default_default_XC8_objcopy_avr_rule(lab2_default_default_XC8_objcopy_avr)
    list(APPEND lab2_default_library_list "$<TARGET_OBJECTS:lab2_default_default_XC8_objcopy_avr>")

endif()

# Handle files with suffix elf, for group default-XC8
if(lab2_default_default_XC8_FILE_TYPE_objcopy_lss)
add_library(lab2_default_default_XC8_objcopy_lss OBJECT ${lab2_default_default_XC8_FILE_TYPE_objcopy_lss})
    lab2_default_default_XC8_objcopy_lss_rule(lab2_default_default_XC8_objcopy_lss)
    list(APPEND lab2_default_library_list "$<TARGET_OBJECTS:lab2_default_default_XC8_objcopy_lss>")

endif()


# Main target for this project
add_executable(lab2_default_image_Ho_0zq47 ${lab2_default_library_list})

set_target_properties(lab2_default_image_Ho_0zq47 PROPERTIES
    OUTPUT_NAME "default"
    SUFFIX ".elf"
    ADDITIONAL_CLEAN_FILES "${output_extensions}"
    RUNTIME_OUTPUT_DIRECTORY "${lab2_default_output_dir}")
target_link_libraries(lab2_default_image_Ho_0zq47 PRIVATE ${lab2_default_default_XC8_FILE_TYPE_link})
# Add the link options from the rule file.
lab2_default_link_rule( lab2_default_image_Ho_0zq47)


#Add objcopy steps
lab2_default_objcopy_avr_rule(lab2_default_image_Ho_0zq47)
lab2_default_objcopy_lss_rule(lab2_default_image_Ho_0zq47)

