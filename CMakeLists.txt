cmake_minimum_required(VERSION 3.5)

if (POLICY CMP0071)
  cmake_policy(SET CMP0071 NEW)
endif()

project(Cutelee5 VERSION 5.2.0 LANGUAGES CXX)

# Workaround for http://public.kitware.com/Bug/view.php?id=12301
if (MINGW)
  if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build,
  options are: None(CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release
  RelWithDebInfo MinSizeRel Coverage." FORCE)
  endif()
endif()

option( BUILD_TEMPLATES "Build the Cutelee template library" TRUE )
option( BUILD_TEXTDOCUMENT "Build the Cutelee textdocument library" TRUE )
option( BUILD_MAIN_PLUGINS "Build the Cutelee Templates plugins" TRUE )
option( BUILD_I18N_PLUGIN "Build the Cutelee Templates i18n plugin" TRUE )
option( BUILD_TESTS "Build the Cutelee tests" TRUE )

if (BUILD_TESTS)
  enable_testing()
endif()

set(Cutelee_MODULE_DIR ${CMAKE_SOURCE_DIR}/cmake/modules)
list(APPEND CMAKE_MODULE_PATH ${Cutelee_MODULE_DIR})

include(GenerateExportHeader)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)

if (CMAKE_GENERATOR STREQUAL "Unix Makefiles")
  # create coverage build type
  set(CMAKE_CONFIGURATION_TYPES ${CMAKE_CONFIGURATION_TYPES} Coverage )
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
                "Debug" "Release" "MinSizeRel" "RelWithDebInfo" "Coverage")
endif()

if (CMAKE_GENERATOR STREQUAL "Unix Makefiles")
  set(CMAKE_CXX_FLAGS_COVERAGE "${CMAKE_CXX_FLAGS_RELEASE} -fprofile-arcs -ftest-coverage")
  set(CMAKE_C_FLAGS_COVERAGE "${CMAKE_C_FLAGS_RELEASE} -fprofile-arcs -ftest-coverage")
  set(CMAKE_SHARED_LINKER_FLAGS_COVERAGE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} -fprofile-arcs -ftest-coverage")
endif()

if (CMAKE_COMPILER_IS_GNUCXX)
  set (CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -Wno-long-long -std=iso9899:1990 -Wundef -Wcast-align -Werror-implicit-function-declaration -Wchar-subscripts -Wall -W -Wpointer-arith -Wwrite-strings -Wformat-security -Wmissing-format-attribute -fno-common")
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wnon-virtual-dtor -Wno-long-long -Wundef -Wcast-align -Wchar-subscripts -Wall -W -Wpointer-arith -Wformat-security -fno-check-new -fno-common -pedantic-errors")

  if (NOT APPLE)
    set ( CMAKE_SHARED_LINKER_FLAGS "-Wl,--fatal-warnings -Wl,--no-undefined ${CMAKE_SHARED_LINKER_FLAGS}")
    set ( CMAKE_MODULE_LINKER_FLAGS "-Wl,--fatal-warnings -Wl,--no-undefined ${CMAKE_MODULE_LINKER_FLAGS}")
  endif ()
endif()

add_definitions(
    -DQT_NO_KEYWORDS
    -DQT_NO_CAST_TO_ASCII
    -DQT_NO_CAST_FROM_ASCII
    -DQT_STRICT_ITERATORS
    -DQT_NO_URL_CAST_FROM_STRING
    -DQT_NO_CAST_FROM_BYTEARRAY
    -DQT_USE_QSTRINGBUILDER
    -DQT_NO_SIGNALS_SLOTS_KEYWORDS
    -DQT_USE_FAST_OPERATOR_PLUS
    -DQT_DISABLE_DEPRECATED_BEFORE=0x050c00
)

# Always include the source and build directories in the include path
# to save doing so manually in every subdirectory.
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE ON)
set(CMAKE_AUTOMOC ON)

set(Cutelee5_MIN_PLUGIN_VERSION 0)

set(Cutelee5_MAJOR_MINOR_VERSION_STRING "${Cutelee5_VERSION_MAJOR}.${Cutelee5_VERSION_MINOR}" )

set (LIB_SUFFIX "" CACHE STRING "Define suffix of library directory name (eg. '64')")

set( LIB_INSTALL_DIR lib${LIB_SUFFIX} )
set( PLUGIN_INSTALL_DIR ${LIB_INSTALL_DIR}/cutelee/${Cutelee5_MAJOR_MINOR_VERSION_STRING} )

# set up RPATH/install_name_dir
set( CMAKE_INSTALL_NAME_DIR ${CMAKE_INSTALL_PREFIX}/${LIB_INSTALL_DIR})
set( CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

include("${Cutelee_MODULE_DIR}/CuteleeMacros.cmake")

if (BUILD_TEMPLATES)
  add_subdirectory(templates)
endif()

if (BUILD_TEXTDOCUMENT)
  add_subdirectory(textdocument)
endif()

set(CMAKECONFIG_INSTALL_DIR "${LIB_INSTALL_DIR}/cmake/Cutelee${Cutelee5_VERSION_MAJOR}" )

configure_file(
  "${Cutelee_MODULE_DIR}/Cutelee${Cutelee5_VERSION_MAJOR}Config.cmake.in"
  "${CMAKE_BINARY_DIR}/Cutelee${Cutelee5_VERSION_MAJOR}Config.cmake"
  @ONLY
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file("${CMAKE_BINARY_DIR}/Cutelee${Cutelee5_VERSION_MAJOR}ConfigVersion.cmake"
  VERSION ${Cutelee5_VERSION}
  COMPATIBILITY AnyNewerVersion
)

install(FILES
  "${CMAKE_BINARY_DIR}/Cutelee${Cutelee5_VERSION_MAJOR}Config.cmake"
  "${CMAKE_BINARY_DIR}/Cutelee${Cutelee5_VERSION_MAJOR}ConfigVersion.cmake"
  DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
  COMPONENT Common
)

install(FILES
  "${Cutelee_MODULE_DIR}/CuteleeMacros.cmake"
  DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
  COMPONENT Common
)

install(EXPORT cutelee_targets
  DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
  NAMESPACE Cutelee${Cutelee5_VERSION_MAJOR}::
  FILE CuteleeTargets.cmake
  COMPONENT Common
)

# Add support for an uninstall target
configure_file(
  "${Cutelee_MODULE_DIR}/cmake_uninstall.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
  @ONLY)

add_custom_target(uninstall
  "${CMAKE_COMMAND}" -P
  "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")

if (BUILD_DOCS)
    find_package(Doxygen REQUIRED dot)
    if (DOXYGEN_FOUND AND DOXYGEN_DOT_FOUND)
    message (STATUS "Doxygen and dot found. make docs target is available")
    # Add support for a docs target

    # Available from http://gcc.gnu.org/onlinedocs/libstdc++/latest-doxygen/libstdc++.tag
    set(STLTAGFILE "${CMAKE_SOURCE_DIR}/scripts/libstdc++.tag=http://gcc.gnu.org/onlinedocs/libstdc++/latest-doxygen")
    find_file(qtcore_tag_file
        qtcore.tags
        HINTS
        "/usr/share/qt5/doc/qtcore"
        )
    set(QTTAGFILE "${qtcore_tag_file}=https://doc.qt.io/qt-5/")

    set(TAGFILES "${QTTAGFILE} ${STLTAGFILE}")
    set(DOCS_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
    configure_file(
        "${Cutelee_MODULE_DIR}/Doxyfile.in"
        "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile"
        @ONLY)

    # Add support for a Qt assistant docs target
    # qttags.tag is too big to distribute with Cutelee. Get it
    # from http://api.kde.org/kdereview-api/kdereview.tar.gz for example.
    if(EXISTS ${qtcore_tag_file})
        set(DOCS_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/qtdocs")

        # Available from http://steveire.com/libstdc++-longnames.tag
        # Also recommended: http://www.steveire.com/libstdc++-v3.qch
        set(STLTAGFILE "${CMAKE_SOURCE_DIR}/scripts/libstdc++-longnames.tag=qthelp://org.gnu.libstdcxx/libstdc++-v3/")
        set(QTTAGFILE "${qtcore_tag_file}=qthelp://org.qt-project.qtcore/qtcore/")

        set(TAGFILES "${QTTAGFILE} ${STLTAGFILE}")
        set(GENERATE_QHP "yes")

        configure_file(
        "${Cutelee_MODULE_DIR}/Doxyfile.in"
        "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile.qt"
        @ONLY)
    endif()

    set( DOCS_ARGS ${DOXYGEN_EXECUTABLE} "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile" )
    if(EXISTS "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile.qt")
        set(DOCS_ARGS ${DOCS_ARGS} COMMAND ${DOXYGEN_EXECUTABLE} "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile.qt" )
    endif()

    add_custom_target(docs
        ${DOCS_ARGS}
    )
    endif()
endif()

find_package(LCov)

if(LCOV_FOUND)
  get_property(UNITTESTS GLOBAL PROPERTY TEST_COVERAGE)

  set(coverageDir ${CMAKE_BINARY_DIR})
  set(coverage_ARGS)

  foreach(_testpath ${UNITTESTS})
    list(APPEND coverage_ARGS COMMAND ${LCOV_EXECUTABLE} -z -d ${coverageDir})

    get_filename_component(_test ${_testpath} NAME)

    list(APPEND coverage_ARGS COMMAND ${_testpath})
    list(APPEND coverage_ARGS COMMAND ${LCOV_EXECUTABLE} -c -t ${_test} -d ${coverageDir} -o ${CMAKE_BINARY_DIR}/test_${_test}.info)
    set(infoFiles ${infoFiles} -a ${CMAKE_BINARY_DIR}/test_${_test}.info)
  endforeach()

  list(APPEND coverage_ARGS COMMAND ${LCOV_EXECUTABLE} ${infoFiles} -o test_all.info)

  # For an unknown reason lcov does not work with absolute paths such as ${CMAKE_BINARY_DIR}
  file(RELATIVE_PATH relBinaryDir / ${CMAKE_BINARY_DIR})

  find_package(Qt5Core REQUIRED)
  get_property(qmake_loc TARGET Qt5::qmake PROPERTY LOCATION)
  execute_process(COMMAND "${qmake_loc}" -query QT_INSTALL_HEADERS
    RESULT_VARIABLE return_code
    OUTPUT_VARIABLE output
    OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_STRIP_TRAILING_WHITESPACE)
  if(return_code)
    message(FATAL_ERROR "Problem")
  endif()
  file(TO_CMAKE_PATH "${output}" output)
  file(RELATIVE_PATH relQtDir / ${output})
  set(Qt4_exclude -r test_all.info *${relQtDir}*)

  list(APPEND coverage_ARGS COMMAND ${LCOV_EXECUTABLE} -r test_all.info *${relBinaryDir}*
                                                                -r test_all.info *pluginpointer*
                                                                -r test_all.info *usr/include*
                                                                ${Qt4_exclude}
                                                                -o test_all.info
  )

  list(APPEND coverage_ARGS COMMAND ${CMAKE_COMMAND} -E remove_directory coverage)
  list(APPEND coverage_ARGS COMMAND ${GENHTML_EXECUTABLE} -o coverage/ -s --demangle-cpp -f test_all.info --prefix ${CMAKE_SOURCE_DIR})

  add_custom_target(coverage
    ${coverage_ARGS}
  )
endif()

find_package(Cccc)
if (CCCC_FOUND)
  macro(append_target_sources target)
    if (TARGET ${target})
      get_target_property(_tar_files ${target} SOURCES)
      foreach(f ${_tar_files})
        if(IS_ABSOLUTE f)
          list(APPEND target_files ${f})
        else()
          list(APPEND target_files ${${target}_SOURCE_DIR}/${f})
        endif()
      endforeach()
    endif()
  endmacro()


  append_target_sources(Cutelee_Templates)
  append_target_sources(Cutelee_TextDocument)
  append_target_sources(cutelee_defaultfilters)
  append_target_sources(cutelee_defaulttags)
  append_target_sources(cutelee_loadertags)
  append_target_sources(cutelee_i18ntags)

  set(cccc_ARGS ${CCCC_EXECUTABLE} ${target_files} --outdir=${CMAKE_BINARY_DIR}/cccc --html_outfile=${CMAKE_BINARY_DIR}/cccc/index.html)

  add_custom_target(cccc
    ${cccc_ARGS}
  )
endif()

include(CPackConfig)
