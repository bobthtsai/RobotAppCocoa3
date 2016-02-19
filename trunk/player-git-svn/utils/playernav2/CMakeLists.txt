OPTION (BUILD_UTILS_PLAYERNAV2 "Build the playernav utility" ON)
IF (BUILD_UTILS_PLAYERNAV2)
    IF (WITH_GNOMECANVAS)
        SET (playernav2Srcs playernav.c gui.c player.c parse.c queue.c)

        INCLUDE_DIRECTORIES (${PROJECT_SOURCE_DIR}/client_libs
                             ${GNOMECANVAS_PKG_INCLUDE_DIRS})
        LINK_DIRECTORIES (${GNOMECANVAS_PKG_LIBRARY_DIRS})
        PLAYER_ADD_EXECUTABLE (playernav2 ${playernav2Srcs})
        TARGET_LINK_LIBRARIES (playernav2 playerc playerinterface playercommon playerutil
            ${PLAYERC_EXTRA_LINK_LIBRARIES} ${GNOMECANVAS_PKG_LIBRARIES})
        SET_SOURCE_FILES_PROPERTIES (${playernav2Srcs} PROPERTIES
            COMPILE_FLAGS "${GNOMECANVAS_CFLAGS}")
        SET_TARGET_PROPERTIES (playernav2 PROPERTIES
            LINK_FLAGS "${GNOMECANVAS_LINKFLAGS}")
    ELSE (WITH_GNOMECANVAS)
        MESSAGE (STATUS "playernav will not be built - Gnome Canvas not found")
    ENDIF (WITH_GNOMECANVAS)
ENDIF (BUILD_UTILS_PLAYERNAV2)
