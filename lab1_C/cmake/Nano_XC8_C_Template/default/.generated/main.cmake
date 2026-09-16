include("${CMAKE_CURRENT_LIST_DIR}/rule.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/file.cmake")

set(Nano_XC8_C_Template_default_library_list )

# Handle files with suffix (s|as|asm|AS|ASM|As|aS|Asm), for group default-XC8
if(Nano_XC8_C_Template_default_default_XC8_FILE_TYPE_assemble)
add_library(Nano_XC8_C_Template_default_default_XC8_assemble OBJECT ${Nano_XC8_C_Template_default_default_XC8_FILE_TYPE_assemble})
    Nano_XC8_C_Template_default_default_XC8_assemble_rule(Nano_XC8_C_Template_default_default_XC8_assemble)
    list(APPEND Nano_XC8_C_Template_default_library_list "$<TARGET_OBJECTS:Nano_XC8_C_Template_default_default_XC8_assemble>")

endif()

# Handle files with suffix S, for group default-XC8
if(Nano_XC8_C_Template_default_default_XC8_FILE_TYPE_assemblePreprocess)
add_library(Nano_XC8_C_Template_default_default_XC8_assemblePreprocess OBJECT ${Nano_XC8_C_Template_default_default_XC8_FILE_TYPE_assemblePreprocess})
    Nano_XC8_C_Template_default_default_XC8_assemblePreprocess_rule(Nano_XC8_C_Template_default_default_XC8_assemblePreprocess)
    list(APPEND Nano_XC8_C_Template_default_library_list "$<TARGET_OBJECTS:Nano_XC8_C_Template_default_default_XC8_assemblePreprocess>")

endif()

# Handle files with suffix [cC], for group default-XC8
if(Nano_XC8_C_Template_default_default_XC8_FILE_TYPE_compile)
add_library(Nano_XC8_C_Template_default_default_XC8_compile OBJECT ${Nano_XC8_C_Template_default_default_XC8_FILE_TYPE_compile})
    Nano_XC8_C_Template_default_default_XC8_compile_rule(Nano_XC8_C_Template_default_default_XC8_compile)
    list(APPEND Nano_XC8_C_Template_default_library_list "$<TARGET_OBJECTS:Nano_XC8_C_Template_default_default_XC8_compile>")

endif()

# Handle files with suffix elf, for group default-XC8
if(Nano_XC8_C_Template_default_default_XC8_FILE_TYPE_objcopy_avr)
add_library(Nano_XC8_C_Template_default_default_XC8_objcopy_avr OBJECT ${Nano_XC8_C_Template_default_default_XC8_FILE_TYPE_objcopy_avr})
    Nano_XC8_C_Template_default_default_XC8_objcopy_avr_rule(Nano_XC8_C_Template_default_default_XC8_objcopy_avr)
    list(APPEND Nano_XC8_C_Template_default_library_list "$<TARGET_OBJECTS:Nano_XC8_C_Template_default_default_XC8_objcopy_avr>")

endif()

# Handle files with suffix elf, for group default-XC8
if(Nano_XC8_C_Template_default_default_XC8_FILE_TYPE_objcopy_lss)
add_library(Nano_XC8_C_Template_default_default_XC8_objcopy_lss OBJECT ${Nano_XC8_C_Template_default_default_XC8_FILE_TYPE_objcopy_lss})
    Nano_XC8_C_Template_default_default_XC8_objcopy_lss_rule(Nano_XC8_C_Template_default_default_XC8_objcopy_lss)
    list(APPEND Nano_XC8_C_Template_default_library_list "$<TARGET_OBJECTS:Nano_XC8_C_Template_default_default_XC8_objcopy_lss>")

endif()


# Main target for this project
add_executable(Nano_XC8_C_Template_default_image_Wcrv7C_D ${Nano_XC8_C_Template_default_library_list})

set_target_properties(Nano_XC8_C_Template_default_image_Wcrv7C_D PROPERTIES
    OUTPUT_NAME "default"
    SUFFIX ".elf"
    ADDITIONAL_CLEAN_FILES "${output_extensions}"
    RUNTIME_OUTPUT_DIRECTORY "${Nano_XC8_C_Template_default_output_dir}")
target_link_libraries(Nano_XC8_C_Template_default_image_Wcrv7C_D PRIVATE ${Nano_XC8_C_Template_default_default_XC8_FILE_TYPE_link})
# Add the link options from the rule file.
Nano_XC8_C_Template_default_link_rule( Nano_XC8_C_Template_default_image_Wcrv7C_D)


#Add objcopy steps
Nano_XC8_C_Template_default_objcopy_avr_rule(Nano_XC8_C_Template_default_image_Wcrv7C_D)
Nano_XC8_C_Template_default_objcopy_lss_rule(Nano_XC8_C_Template_default_image_Wcrv7C_D)

