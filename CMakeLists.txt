set(PROJNAME "panda")

cmake_minimum_required(VERSION 2.6)

project(${PROJNAME})

if (CMAKE_BUILD_TYPE STREQUAL "")
  set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "Choose the type of build, options are: None (CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif ()

set(CMAKE_DEBUG_POSTFIX "_d")

set(CMAKE_CXX_FLAGS "-std=gnu++11")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -mtune=generic")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g -DNDEBUG -mtune=generic")
set(CMAKE_CXX_FLAGS_DEBUG "-g -mtune=generic")

set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/bin")

option(PANDA_HAS_EIGEN "Enable if Panda3d was built with Eigen." TRUE)
option(PANDA_HAS_PYTHON "Enable if Panda3d was built with python support." FALSE)

if (PANDA_HAS_PYTHON)
	set(PYTHON_INCLUDE "/usr/include/python2.7" CACHE FILEPATH "Location of python sources." CACHE FILEPATH "Path to python includes.")
endif ()

if (PANDA_HAS_EIGEN)
	set(EIGEN_INCLUDE "/usr/include/eigen3" CACHE FILEPATH "Location of Eigen sources." CACHE FILEPATH "Path to eigen source.")
endif ()

set(PANDA_INCLUDE "/usr/local/include/panda3d" CACHE FILEPATH "Location of Panda3d sources.")
set(PANDA_PREFIX "/usr/local/lib/panda3d/" CACHE FILEPATH "The path prefix to Panda3d libraries.")

#link_directories(${PANDA_LIBS})

set(HDRS
	src/AppState.hpp
	src/AppStateManager.hpp
	src/MainApp.hpp
	src/GameState.hpp
	src/MenuState.hpp
	src/TerrainManager.hpp
	src/SkyManager.hpp
	src/Controls.hpp
	src/GraphicalInterface.hpp
	src/CharacterGen.hpp
	src/PlayerCharacter.hpp
	src/UtilFunctions.hpp
	src/AccumulationBuffer.hpp
	src/LispSystem.hpp
)

set(SRCS
	src/main.cpp
	src/AppStateManager.cpp
	src/MainApp.cpp
	src/GameState.cpp
	src/MenuState.cpp
	src/TerrainManager.cpp
	src/SkyManager.cpp
	src/Controls.cpp
	src/GraphicalInterface.cpp
	src/CharacterGen.cpp
	src/PlayerCharacter.cpp
	src/UtilFunctions.cpp
	src/AccumulationBuffer.cpp
	src/LispSystem.cpp
)

add_executable(${PROJNAME} WIN32 ${HDRS} ${SRCS})

set_target_properties(${PROJNAME} PROPERTIES DEBUG_POSTFIX _d)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	target_compile_definitions(${PROJNAME} PRIVATE VVV=1)
endif ()

include_directories(
	${PANDA_INCLUDE}
	if (PANDA_HAS_PYTHON)
		${PYTHON_INCLUDE}
	endif ()
	
	if (PANDA_HAS_EIGEN)
		${EIGEN_INCLUDE}
	endif ()
	)

if (UNIX)
	set(SUFFIX_DLL "so")
	set(SUFFIX_STATIC "a")
endif ()
if (MSVC)
	set(SUFFIX_DLL "lib")
	set(SUFFIX_STATIC "lib")
endif ()

set(PANDA_LIBS
	${PANDA_PREFIX}/libp3framework.${SUFFIX_DLL}
	${PANDA_PREFIX}/libpanda.${SUFFIX_DLL}
	${PANDA_PREFIX}/libpandaexpress.${SUFFIX_DLL}
	${PANDA_PREFIX}/libp3dtool.${SUFFIX_DLL}
	${PANDA_PREFIX}/libp3dtoolconfig.${SUFFIX_DLL})

if (PANDA_HAS_PYTHON)
	list(APPEND PANDA_LIBS ${PANDA_PREFIX}/libp3pystub.${SUFFIX_STATIC})
endif ()

list(APPEND PANDA_LIBS
	${PANDA_PREFIX}/libp3direct.${SUFFIX_DLL}
	${PANDA_PREFIX}/libp3eggbase.${SUFFIX_STATIC})

target_link_libraries(${PROJNAME}
	-lpthread
	-lecl
	-lgc
	${PANDA_LIBS}
) 

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/media)


if(MSVC OR UNIX)
        set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
endif(MSVC OR UNIX)
