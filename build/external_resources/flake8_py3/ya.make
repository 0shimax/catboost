RESOURCES_LIBRARY()



IF (HOST_OS_DARWIN AND HOST_ARCH_ARM64 OR
    HOST_OS_DARWIN AND HOST_ARCH_X86_64 OR
    HOST_OS_LINUX AND HOST_ARCH_PPC64LE OR
    HOST_OS_LINUX AND HOST_ARCH_X86_64 OR
    HOST_OS_WINDOWS AND HOST_ARCH_X86_64)
ELSE()
    MESSAGE(FATAL_ERROR Unsupported host platform for FLAKE8_PY3)
ENDIF()

DECLARE_EXTERNAL_HOST_RESOURCES_BUNDLE(
    FLAKE8_PY3
    sbr:2489192940 FOR DARWIN-ARM64
    sbr:2489196917 FOR DARWIN
    sbr:2489194129 FOR LINUX-PPC64LE
    sbr:2489200052 FOR LINUX
    sbr:2489198675 FOR WIN32
)

END()
