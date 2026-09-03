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

# C++ version
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Get all files in src folder
file(
	GLOB_RECURSE SOURCE_CXX_FILES
	"src/*.cpp"
)

# Add source
if (CMAKE_BUILD_TYPE STREQUAL "Release")
	# Don't include console in release builds
	add_executable(${PROJECT_NAME} WIN32 ${SOURCE_CXX_FILES})
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
	add_executable(${PROJECT_NAME} WIN32 ${SOURCE_CXX_FILES})
endif()

# Link libraries
set(LIBS "")
set(LIBS ${LIBS} d3d11.lib dxguid.lib D3Dcompiler.lib)

target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBS})