# The following variables contains the files used by the different stages of the build process.
set(Nano_XC8_C_Template_default_default_XC8_FILE_TYPE_assemble)
set_source_files_properties(${Nano_XC8_C_Template_default_default_XC8_FILE_TYPE_assemble} PROPERTIES LANGUAGE ASM)

# For assembly files, add "." to the include path for each file so that .include with a relative path works
foreach(source_file ${Nano_XC8_C_Template_default_default_XC8_FILE_TYPE_assemble})
        set_source_files_properties(${source_file} PROPERTIES INCLUDE_DIRECTORIES "$<PATH:NORMAL_PATH,$<PATH:REMOVE_FILENAME,${source_file}>>")
endforeach()

set(Nano_XC8_C_Template_default_default_XC8_FILE_TYPE_assemblePreprocess "${CMAKE_CURRENT_SOURCE_DIR}/../../../main.S")
set_source_files_properties(${Nano_XC8_C_Template_default_default_XC8_FILE_TYPE_assemblePreprocess} PROPERTIES LANGUAGE ASM)

# For assembly files, add "." to the include path for each file so that .include with a relative path works
foreach(source_file ${Nano_XC8_C_Template_default_default_XC8_FILE_TYPE_assemblePreprocess})
        set_source_files_properties(${source_file} PROPERTIES INCLUDE_DIRECTORIES "$<PATH:NORMAL_PATH,$<PATH:REMOVE_FILENAME,${source_file}>>")
endforeach()

set(Nano_XC8_C_Template_default_default_XC8_FILE_TYPE_compile)
set_source_files_properties(${Nano_XC8_C_Template_default_default_XC8_FILE_TYPE_compile} PROPERTIES LANGUAGE C)
set(Nano_XC8_C_Template_default_default_XC8_FILE_TYPE_link)
set(Nano_XC8_C_Template_default_default_XC8_FILE_TYPE_objcopy_avr)
set(Nano_XC8_C_Template_default_image_name "default.elf")
set(Nano_XC8_C_Template_default_image_base_name "default")

# The output directory of the final image.
set(Nano_XC8_C_Template_default_output_dir "${CMAKE_CURRENT_SOURCE_DIR}/../../../out/Nano_XC8_C_Template")

# The full path to the final image.
set(Nano_XC8_C_Template_default_full_path_to_image ${Nano_XC8_C_Template_default_output_dir}/${Nano_XC8_C_Template_default_image_name})

# Potential output file extensions
set(output_extensions
    .hex
    .hxl
    .mum
    .o
    .sdb
    .sym
    .cmf)
list(TRANSFORM output_extensions PREPEND "${Nano_XC8_C_Template_default_output_dir}/${Nano_XC8_C_Template_default_image_base_name}")
