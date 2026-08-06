# Using the same minimum as the godot-cpp project
cmake_minimum_required(VERSION 3.17)

# Silence unused variable warning when specified from toolchain
if(CMAKE_C_COMPILER)
endif()

set(LIBNAME "faust2godot") # "The name of the library"
set(GODOT_PROJECT_DIR "project" CACHE STRING "The directory of a Godot project folder")
set(CMAKE_CXX_STANDARD 20) #std c++ 20
set(CMAKE_CXX_STANDARD_REQUIRED ON) #std c++ 20
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(BUILD_TESTS "Build test programs" OFF)

# Make sure all the dependencies are satisfied
find_package(Python3 3.4 REQUIRED)
find_program(GIT git REQUIRED)

# Ensure godot-cpp submodule has been updated
if(NOT EXISTS "${CMAKE_CURRENT_LIST_DIR}/godot-cpp/src")
  message(NOTICE "godot-cpp bindings source not found")
  message(NOTICE "initializing/updating the godot-cpp submodule...")

  # update the c++ bindings submodule to populate it with the necessary source for the library
  execute_process(
        COMMAND git submodule update --init godot-cpp
        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
        COMMAND_ERROR_IS_FATAL ANY
    )
endif()

add_subdirectory(godot-cpp SYSTEM)

if (BUILD_TESTS)
  enable_testing()

  add_subdirectory(googletest EXCLUDE_FROM_ALL)
  add_subdirectory(tests) # unit tests subdirectory
endif(BUILD_TESTS)

# Create the gen directory at configure time so IDEs can see generated headers immediately after generate_bindings
file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/godot-cpp/gen/include")

# Add godot-cpp's module path and include the exported functions.
# This is made available for documentation generation
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${godot-cpp_SOURCE_DIR}/cmake")

include(GodotCPPModule)

# The godot-cpp target has some of useful properties attached that can be retrieved like so.
get_target_property(GODOTCPP_SUFFIX godot::cpp GODOTCPP_SUFFIX)
get_target_property(GODOTCPP_PLATFORM godot::cpp GODOTCPP_PLATFORM)

#  uncomment for llvm support
#  include(cmake_modules/LLVM.cmake)
include(cmake_modules/FaustLib.cmake)

# Now we can specify our own project which will inherit any global cmake properties or variables that have been defined.
project(faust2godot
    VERSION 1.0
    DESCRIPTION "This repository serves as a quickstart template for GDExtension development with Godot 4.0+."
    HOMEPAGE_URL "https://github.com/enetheru/godot-cpp-template/tree/main"
    LANGUAGES CXX
)

## On Linux "force" the link with stdc++
if(UNIX AND NOT APPLE)
  set(FAUST_LIBS  "stdc++"  CACHE STRING  "FAUST LIBRARIES" FORCE)
endif()

add_library(${LIBNAME} SHARED)

target_include_directories(${LIBNAME} PUBLIC "include/")
target_include_directories(${LIBNAME} PUBLIC "faust/architecture")

#if (WIN32)
#  set(FAUSTPATH "$ENV{ProgramFiles}/faust/include" CACHE STRING "Faust include directory")
#  target_include_directories(${LIBNAME} PUBLIC ${FAUSTPATH})
#endif ()

target_sources(${LIBNAME}
    PRIVATE
    src/FaustGUIGlue.cpp
    src/faust2godot.cpp
    src/inspector/EditorPluginFaust.cpp
    src/inspector/InspectorPluginFaust.cpp
    src/inspector/VSliderEditorProperty.cpp
    src/inspector/ButtonEditorProperty.cpp
    src/script/FaustScript.cpp
    src/script/FaustScriptLanguage.cpp
    src/script/FaustScriptResourceFormatLoader.cpp
    src/script/FaustScriptResourceFormatSaver.cpp
    src/script/FaustEditorSyntaxHighLighter.cpp
    src/AudioEffectFaust.cpp
    src/AudioStreamFaust.cpp
    src/MidiHandlerFaust.cpp
    src/GodotMapUI.cpp
    src/GodotMidi.cpp
)

# Fetch a list of the xml files to use for documentation and add to our target
file(GLOB_RECURSE DOC_XML LIST_DIRECTORIES NO CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/doc_classes/*.xml")
# conditionally add doc data to compile output
if(DOC_XML)
  if(GODOTCPP_TARGET MATCHES "editor|template_debug")
    target_doc_sources(${LIBNAME} ${DOC_XML})
  endif()
endif()

target_link_libraries(${LIBNAME} PRIVATE godot-cpp)
#  uncomment for llvm support
#  target_link_libraries(${PROJECT_NAME} PRIVATE LLVM)

target_link_libraries(${LIBNAME} PRIVATE staticlib)

# Require at least C++20 for this target
set_property(TARGET ${LIBNAME} PROPERTY CXX_STANDARD 20)

set_target_properties(${LIBNAME}
    PROPERTIES
    # The generator expression here prevents msvc from adding a Debug or Release subdir.
    RUNTIME_OUTPUT_DIRECTORY "$<1:${PROJECT_SOURCE_DIR}/bin/${GODOTCPP_PLATFORM}>"

    PREFIX ""
    OUTPUT_NAME "${LIBNAME}${GODOTCPP_SUFFIX}"
)

set(GODOT_PROJECT_BINARY_DIR "${PROJECT_SOURCE_DIR}/${GODOT_PROJECT_DIR}/bin/${GODOTCPP_PLATFORM}")

add_custom_command(TARGET ${LIBNAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:${LIBNAME}>" "${GODOT_PROJECT_BINARY_DIR}/$<TARGET_FILE_NAME:${LIBNAME}>"
)

if (PROJECT_IS_TOP_LEVEL AND UNIX)
  # Create symlink to compile_commands.json for IDE to pick it up
  execute_process(
        COMMAND ${CMAKE_COMMAND} -E create_symlink
            ${CMAKE_BINARY_DIR}/compile_commands.json
            ${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json
    )
endif()
