cmake_minimum_required (VERSION 3.24)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
	cmake_policy(SET CMP0141 NEW)
	set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# Get project name from directory
get_filename_component(ProjectId ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" ProjectId ${ProjectId})
project(${ProjectId})

# Support compiling resource files
ENABLE_LANGUAGE(RC)

# C++ version
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# -- Add source --
file(
	GLOB_RECURSE SOURCE_CXX_FILES
	"src/*.cpp"
	"resource/*.rc"
)

if (CMAKE_BUILD_TYPE STREQUAL "Release")
	# Don't include console in release builds
	add_executable(${PROJECT_NAME} WIN32 ${SOURCE_CXX_FILES})
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
	add_executable(${PROJECT_NAME} WIN32 ${SOURCE_CXX_FILES})
endif()

# Platform
set(IS_PLATFORM_WINDOWS 1)
# if (CMAKE_SYSTEM_NAME MATCHES Windows)
# 	set(IS_PLATFORM_WINDOWS 1)
# endif()

target_compile_definitions(${PROJECT_NAME} PRIVATE 
    BUILD_PLATFORM_WINDOWS=${IS_PLATFORM_WINDOWS}
	WIN32_LEAN_AND_MEAN
)


# Link libraries
set(LIBS "")
set(LIBS ${LIBS} d3d11.lib dxgi.lib dxguid.lib D3Dcompiler.lib)

target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBS})