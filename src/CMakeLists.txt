include_directories(${IoCContainer_SOURCE_DIR}/src/reflection)
include_directories(${IoCContainer_SOURCE_DIR}/src/utils)
include_directories(${LUA_INCLUDEDIR})

add_subdirectory(reflection)
IF(SELFPORTRAIT_BUILD_PARSER)
    add_subdirectory(parser)
ENDIF()
add_subdirectory(lua_module)
add_subdirectory(unit_test)
add_subdirectory(utils)
add_subdirectory(benchmarks)
