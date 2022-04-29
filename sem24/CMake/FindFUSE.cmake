IF (FUSE_INCLUDE_DIR)
    SET (FUSE_FIND_QUIETLY TRUE)
ENDIF (FUSE_INCLUDE_DIR)

FIND_PATH (FUSE_INCLUDE_DIR fuse.h /usr/local/include/osxfuse /usr/local/include /usr/include)

if (APPLE)
    SET(FUSE_NAMES libosxfuse.dylib fuse)
else (APPLE)
    SET(FUSE_NAMES fuse)
endif (APPLE)
FIND_LIBRARY(FUSE_LIBRARIES NAMES ${FUSE_NAMES} PATHS /lib64 /lib /usr/lib64 /usr/lib /usr/local/lib64 /usr/local/lib /usr/lib/x86_64-linux-gnu)

include ("FindPackageHandleStandardArgs")
find_package_handle_standard_args ("FUSE" DEFAULT_MSG FUSE_INCLUDE_DIR FUSE_LIBRARIES)

mark_as_advanced (FUSE_INCLUDE_DIR FUSE_LIBRARIES)

