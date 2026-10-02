cmake_minimum_required(VERSION 3.10)

# SaltyNX-Tool is a Nintendo Switch only homebrew rendered with deko3d.
set(PLATFORM_SWITCH ON CACHE BOOL "build for switch" FORCE)
set(USE_DEKO3D ON CACHE BOOL "Using deko3d instead of OpenGL." FORCE)

set(BOREALIS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/borealis)
set(BOREALIS_LIBRARY ${BOREALIS_DIR}/library)

# borealis build options and toolchain (must be included before project())
include(${BOREALIS_LIBRARY}/cmake/commonOption.cmake)
include(${BOREALIS_LIBRARY}/cmake/toolchain.cmake)

project(SaltyNX-Tool)
set(APP_TITLE   "SaltyNX-Tool")
set(APP_AUTHOR  "MasaGratoR")
set(APP_VERSION "1.1.1")
set(APP_ICON    ${CMAKE_CURRENT_SOURCE_DIR}/res/icon.jpg)
set(APP_RES     ${CMAKE_CURRENT_SOURCE_DIR}/res)
set(APP_ROMFS   ${CMAKE_BINARY_DIR}/romfs)

add_subdirectory(${BOREALIS_LIBRARY})

# --- RomFS -------------------------------------------------------------------
# borealis runtime resources (translations for built-in hints, icon font,
# system images) + compiled deko3d shaders. The app's own res/ folder is
# copied on top of it at build time.
file(REMOVE_RECURSE ${APP_ROMFS})
file(GLOB BRLS_HINTS RELATIVE ${BOREALIS_DIR}/resources/i18n ${BOREALIS_DIR}/resources/i18n/*/hints.json)
foreach (hints ${BRLS_HINTS})
    get_filename_component(lang ${hints} DIRECTORY)
    file(COPY ${BOREALIS_DIR}/resources/i18n/${hints} DESTINATION ${APP_ROMFS}/i18n/${lang})
endforeach ()
file(COPY ${BOREALIS_DIR}/resources/material DESTINATION ${APP_ROMFS})
file(COPY ${BOREALIS_DIR}/resources/img/sys DESTINATION ${APP_ROMFS}/img)
gen_dksh(${APP_ROMFS}/shaders)

# --- Application -------------------------------------------------------------
file(GLOB APP_SRC ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp)
list(APPEND APP_SRC ${BOREALIS_LIBRARY}/lib/platforms/switch/switch_wrapper.c)

program_target(${PROJECT_NAME} "${APP_SRC}")
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/source)
target_compile_definitions(${PROJECT_NAME} PRIVATE
        APP_VERSION="${APP_VERSION}"
        APP_TITLE="${APP_TITLE}")
target_compile_options(${PROJECT_NAME} PRIVATE -ffunction-sections -fdata-sections ${APP_PLATFORM_OPTION})
target_link_options(${PROJECT_NAME} PRIVATE ${APP_PLATFORM_LINK_OPTION})
target_link_libraries(${PROJECT_NAME} PRIVATE borealis ${APP_PLATFORM_LIB})

add_custom_target(${PROJECT_NAME}.nro ALL DEPENDS ${PROJECT_NAME}
        COMMAND ${NX_NACPTOOL_EXE} --create "${APP_TITLE}" "${APP_AUTHOR}" "${APP_VERSION}" ${PROJECT_NAME}.nacp
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${APP_RES} ${APP_ROMFS}
        COMMAND ${NX_ELF2NRO_EXE} ${PROJECT_NAME}.elf ${PROJECT_NAME}.nro --icon=${APP_ICON} --nacp=${PROJECT_NAME}.nacp --romfsdir=${APP_ROMFS}
)
