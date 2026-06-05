set(DEPENDENT_MP_BIN2HEXMinSpecHSM_default_mUN63ejm "/Applications/microchip/xc32/v4.35/bin/xc32-bin2hex")
set(DEPENDENT_DEPENDENT_TARGET_ELFMinSpecHSM_default_mUN63ejm "${CMAKE_CURRENT_LIST_DIR}/../../../../out/MinSpecHSM/default.elf")
set(DEPENDENT_TARGET_DIRMinSpecHSM_default_mUN63ejm "${CMAKE_CURRENT_LIST_DIR}/../../../../out/MinSpecHSM")
set(DEPENDENT_BYPRODUCTSMinSpecHSM_default_mUN63ejm ${DEPENDENT_TARGET_DIRMinSpecHSM_default_mUN63ejm}/${sourceFileNameMinSpecHSM_default_mUN63ejm}.c)
add_custom_command(
    OUTPUT ${DEPENDENT_TARGET_DIRMinSpecHSM_default_mUN63ejm}/${sourceFileNameMinSpecHSM_default_mUN63ejm}.c
    COMMAND ${DEPENDENT_MP_BIN2HEXMinSpecHSM_default_mUN63ejm} --image ${DEPENDENT_DEPENDENT_TARGET_ELFMinSpecHSM_default_mUN63ejm} --image-generated-c ${sourceFileNameMinSpecHSM_default_mUN63ejm}.c --image-generated-h ${sourceFileNameMinSpecHSM_default_mUN63ejm}.h --image-copy-mode ${modeMinSpecHSM_default_mUN63ejm} --image-offset ${addressMinSpecHSM_default_mUN63ejm} 
    WORKING_DIRECTORY ${DEPENDENT_TARGET_DIRMinSpecHSM_default_mUN63ejm}
    DEPENDS ${DEPENDENT_DEPENDENT_TARGET_ELFMinSpecHSM_default_mUN63ejm})
add_custom_target(
    dependent_produced_source_artifactMinSpecHSM_default_mUN63ejm 
    DEPENDS ${DEPENDENT_TARGET_DIRMinSpecHSM_default_mUN63ejm}/${sourceFileNameMinSpecHSM_default_mUN63ejm}.c
    )
