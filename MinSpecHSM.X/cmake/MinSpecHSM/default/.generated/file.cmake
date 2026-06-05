# The following variables contains the files used by the different stages of the build process.
set(MinSpecHSM_default_default_XC32_FILE_TYPE_assemble)
set_source_files_properties(${MinSpecHSM_default_default_XC32_FILE_TYPE_assemble} PROPERTIES LANGUAGE ASM)

# For assembly files, add "." to the include path for each file so that .include with a relative path works
foreach(source_file ${MinSpecHSM_default_default_XC32_FILE_TYPE_assemble})
        set_source_files_properties(${source_file} PROPERTIES INCLUDE_DIRECTORIES "$<PATH:NORMAL_PATH,$<PATH:REMOVE_FILENAME,${source_file}>>")
endforeach()

set(MinSpecHSM_default_default_XC32_FILE_TYPE_assembleWithPreprocess)
set_source_files_properties(${MinSpecHSM_default_default_XC32_FILE_TYPE_assembleWithPreprocess} PROPERTIES LANGUAGE ASM)

# For assembly files, add "." to the include path for each file so that .include with a relative path works
foreach(source_file ${MinSpecHSM_default_default_XC32_FILE_TYPE_assembleWithPreprocess})
        set_source_files_properties(${source_file} PROPERTIES INCLUDE_DIRECTORIES "$<PATH:NORMAL_PATH,$<PATH:REMOVE_FILENAME,${source_file}>>")
endforeach()

set(MinSpecHSM_default_default_XC32_FILE_TYPE_compile
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../../src/AD.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../../src/BOARD.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../../src/ES_CheckEvents.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../../src/ES_Framework.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../../src/ES_KeyboardInput.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../../src/ES_PostList.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../../src/ES_Queue.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../../src/ES_TattleTale.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../../src/ES_Timers.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../../src/IO_Ports.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../../src/LED.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../../src/RC_Servo.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../../src/pwm.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../../src/roach.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../../src/serial.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../../src/timers.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../HSMFiles/AlignSubHSM.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../HSMFiles/AvoidSubHSM.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../HSMFiles/ES_Main.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../HSMFiles/FollowTapeSubHSM.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../HSMFiles/ISZFinderSubHSM.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../HSMFiles/LaunchSubHSM.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../HSMFiles/LocateTapeSubHSM.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../HSMFiles/MainHSM.c")
set_source_files_properties(${MinSpecHSM_default_default_XC32_FILE_TYPE_compile} PROPERTIES LANGUAGE C)
set(MinSpecHSM_default_default_XC32_FILE_TYPE_compile_cpp)
set_source_files_properties(${MinSpecHSM_default_default_XC32_FILE_TYPE_compile_cpp} PROPERTIES LANGUAGE CXX)
set(MinSpecHSM_default_default_XC32_FILE_TYPE_link)
set(MinSpecHSM_default_default_XC32_FILE_TYPE_bin2hex)
set(MinSpecHSM_default_image_name "default.elf")
set(MinSpecHSM_default_image_base_name "default")

# The output directory of the final image.
set(MinSpecHSM_default_output_dir "${CMAKE_CURRENT_SOURCE_DIR}/../../../out/MinSpecHSM")

# The full path to the final image.
set(MinSpecHSM_default_full_path_to_image ${MinSpecHSM_default_output_dir}/${MinSpecHSM_default_image_name})
