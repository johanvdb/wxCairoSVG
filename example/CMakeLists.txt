cmake_minimum_required(VERSION 2.6)
project("example")

set(ADDITIONAL_LIBRARIES "" CACHE STRING "Additional libraries required for linking")

if (APPLE)
	add_definitions(-D__WXMAC__)
endif (APPLE)

if (WIN32)
	add_definitions(-D__WXMSW__)
endif (WIN32)

include (FindCairo.cmake)
if (CAIRO_FOUND)
	add_definitions(-DCAIRO ${CAIRO_DEFINITIONS})
	include_directories(${CAIRO_INCLUDE_DIR})
	link_directories(${CAIRO_LIBRARY_DIRS})	
else (CAIRO_FOUND) 
	message(FATAL_ERROR "Cairo not found.")
endif (CAIRO_FOUND)
# Section on Cairo ends here

IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	add_definitions(-D__WXGTK__)
	find_package(PkgConfig)
	pkg_check_modules(GTK2 "gtk+-2.0")
	if (NOT GTK2_FOUND)
		message(FATAL_ERROR "GTK2 not found. Please ensure that gtk2-devel is installed")
	endif (NOT GTK2_FOUND)
	include_directories(${GTK2_INCLUDE_DIRS})
	set(ADDITIONAL_LIBRARIES ${ADDITIONAL_LIBRARIES} ${GTK2_LIBRARIES})
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

if (APPLE)
	set(MACOSX_BUNDLE_BUNDLE_NAME ${VARIANT_LONG})
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/build)
	set(RESOURCE_DIRECTORY ${PROJECT_BINARY_DIR}/build/Example.app/Contents/Resources)
else (APPLE)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/build)
	set(RESOURCE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endif (APPLE)

find_package(wxWidgets COMPONENTS adv xml core base)

if (NOT wxWidgets_FOUND) 
	message(FATAL_ERROR "wxWidgets not found.")
endif (NOT wxWidgets_FOUND)

include(${wxWidgets_USE_FILE})

add_custom_target(postbuild ALL)

if (APPLE)

#	add_custom_command(TARGET postbuild POST_BUILD COMMAND rm -rf ${PROJECT_BINARY_DIR}/build/Example.app/Contents/Resources)
#	add_custom_command(TARGET postbuild POST_BUILD COMMAND mkdir -p ${PROJECT_BINARY_DIR}/build/Example.app/Contents/Resources)
	
endif (APPLE)

IF(${CMAKE_HOST_SYSTEM_NAME} MATCHES "Linux")
#	add_custom_command(TARGET postbuild POST_BUILD COMMAND cp -Rf ${PROJECT_SOURCE_DIR}/docs/sites/${VARIANT}/icons ${PROJECT_BINARY_DIR}/build/${VARIANT}/)
	IF (MINGW)
		set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} --static-libgcc)
	ENDIF (MINGW)
ENDIF(${CMAKE_HOST_SYSTEM_NAME} MATCHES "Linux")

if (CMAKE_HOST_WIN32)
#	add_custom_command(TARGET postbuild POST_BUILD COMMAND if not exist \"${RESOURCE_DIRECTORY}\" (mkdir \"${RESOURCE_DIRECTORY}\"))
#	add_custom_command(TARGET postbuild POST_BUILD COMMAND xcopy /E /Y /F /D \"${PROJECT_SOURCE_DIR}/sites/${VARIANT}/resources\" \"${RESOURCE_DIRECTORY}\")
endif (CMAKE_HOST_WIN32)

add_executable(example WIN32 MACOSX_BUNDLE example.cpp)
target_link_libraries(example ${wxWidgets_LIBRARIES} ${CAIRO_LIBRARIES} ${ADDITIONAL_LIBRARIES})