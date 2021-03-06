if (build_type STREQUAL "debug")
    message(STATUS "--- Debug build ---")
    add_definitions(-fsanitize=address)
    add_definitions(-fsanitize=alignment)
    add_definitions(-fsanitize=bool)
    add_definitions(-fsanitize=bounds)
    add_definitions(-fsanitize=bounds-strict)
    add_definitions(-fsanitize=builtin)
    add_definitions(-fsanitize=enum)
    add_definitions(-fsanitize=float-divide-by-zero)
    add_definitions(-fsanitize=float-cast-overflow)
    add_definitions(-fsanitize=integer-divide-by-zero)
    add_definitions(-fsanitize=leak)
    #    add_definitions(-fsanitize=kernel-address)
    add_definitions(-fsanitize=nonnull-attribute)
    add_definitions(-fsanitize=null)
    add_definitions(-fsanitize=object-size)
    add_definitions(-fsanitize=pointer-compare)
    add_definitions(-fsanitize=pointer-overflow)
    add_definitions(-fsanitize=pointer-subtract)
    add_definitions(-fsanitize=return)
    add_definitions(-fsanitize=returns-nonnull-attribute)
    add_definitions(-fsanitize=signed-integer-overflow)
    add_definitions(-fsanitize=shift)
    add_definitions(-fsanitize=shift-base)
    add_definitions(-fsanitize=shift-exponent)
    #    add_definitions(-fsanitize=thread)
    add_definitions(-fsanitize=undefined)
    add_definitions(-fsanitize=unreachable)
    add_definitions(-fsanitize=vla-bound)
    add_definitions(-fsanitize=vptr)
endif ()

add_subdirectory(libgtest)

MACRO(SUBDIRLIST result curdir)
    FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
    SET(dirlist "")
    FOREACH (child ${children})
        IF (IS_DIRECTORY ${curdir}/${child} AND EXISTS ${curdir}/${child}/CMakeLists.txt)
            LIST(APPEND dirlist ${child})
        ENDIF ()
    ENDFOREACH ()
    SET(${result} ${dirlist})
ENDMACRO()

MACRO(ADD_TEST_ALL_FILES)
    get_filename_component(test_name ${CMAKE_CURRENT_SOURCE_DIR} NAME)
    message(STATUS "Creating test ${test_name}")

    file(GLOB_RECURSE test_sources *)
    list(FILTER test_sources INCLUDE REGEX "cpp$")

    add_executable("${test_name}_test" ${test_sources})

    target_link_libraries(
        "${test_name}_test"
        gtest
        gtest_main
        ${CMAKE_THREAD_LIBS_INIT}
    )
    if (build_type STREQUAL "debug")
        target_link_libraries("${test_name}_test" asan)
    endif()
    target_link_libraries("${test_name}_test" ${lib_name})
    include_directories(${CMAKE_SOURCE_DIR}/src)

    message(STATUS "Adding test: ${test_name}_test")
    add_test(NAME "${test_name}_test_caller" COMMAND "${test_name}_test")

    target_compile_options("${test_name}_test" PRIVATE --coverage)
    target_link_libraries("${test_name}_test" --coverage)
ENDMACRO()

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/src)

SUBDIRLIST(SUBDIRS ${CMAKE_CURRENT_SOURCE_DIR})

FOREACH (subdir ${SUBDIRS})
    if (subdir STREQUAL "libgtest")
        message(STATUS "This is the ${subdir}")
    else ()
        message(STATUS "Adding ${subdir}")
        add_subdirectory(${subdir})
    endif ()
ENDFOREACH ()
