
cmake_minimum_required(VERSION 2.8)

project(stend)

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Sql REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(OpenCV REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(OpenMP REQUIRED)

pkg_check_modules(PC_LUA QUIET lua)
pkg_check_modules(PC_COMBINE QUIET combine)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CXX_FLAGS "${CXX_FLAGS} ${OpenMP_CXX_FLAGS} -Wall -pipe -std=c++11")

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${Qt5Widgets_INCLUDE_DIRS} ${Qt5Sql_INCLUDE_DIRS} ${PC_LUA_INCLUDE_DIRS} ${PC_COMBINE_INCLUDE_DIRS})

############################################################################

qt5_wrap_ui(dialogs ui/gui/resources/dialogs/main_window.ui ui/gui/resources/dialogs/about_dialog.ui ui/gui/resources/dialogs/setup_dialog.ui)
qt5_add_resources(base_resources resources/base_resources.qrc)
qt5_add_resources(gui_resources ui/gui/resources/gui_resources.qrc)

set(sources ${base_resources}
	third_party/qtftp/qftp.cpp third_party/qtftp/qurlinfo.cpp
	algo/algo.cpp algo/deblurring.cpp algo/skeleton.cpp algo/combine_quality.cpp algo/vector.cpp
	db/db.cpp
	script_engine/script_engine.cpp script_engine/opencv.cpp script_engine/auxiliary.cpp script_engine/context.cpp script_engine/map.cpp
	map/map.cpp
	matrix/matrix.cpp
	module/module.cpp
	context/context.cpp
	stat/stat.cpp
	various/various.cpp
	video/video.cpp
	main_loop/main_loop.cpp)
set(sources_console ui/console/main.cpp ui/console/core.cpp ui/console/display.cpp)
set(sources_gui ${dialogs} ${gui_resources} ui/gui/main.cpp ui/gui/ui/display.cpp ui/gui/ui/main_window.cpp ui/gui/ui/menu.cpp ui/gui/ui/about_dialog.cpp ui/gui/ui/script_highlighter.cpp ui/gui/ui/setup_dialog.cpp)

############################################################################ 

if(${UNIX})

	include_directories(/usr/lib/R/library/Rcpp/include/ /usr/lib/R/library/RInside/include/)
	link_directories(/usr/lib/R/library/Rcpp/lib/ /usr/lib/R/library/RInside/lib/)

endif()

############################################################################ 

string(REPLACE ";" " " CXX_FLAGS " ${CXX_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_FLAGS}" CACHE string "" FORCE)

if(${UNIX})

	add_library(stend SHARED ${sources})
	add_executable(console ${sources_console})
	add_executable(gui ${sources_gui})

	qt5_use_modules(stend Core Test Widgets Sql Network)
	qt5_use_modules(console Core Test Sql Network)
	qt5_use_modules(gui Core Test Widgets Sql Network)

	target_link_libraries(stend ${OpenCV_LIBS} ${PC_LUA_LIBRARIES} ${PC_COMBINE_LIBRARIES} glapi)
	target_link_libraries(console stend)
	target_link_libraries(gui stend)

else()

	add_executable(gui ${sources_gui} ${sources})
	qt5_use_modules(gui Core Test Widgets Sql Network)
	target_link_libraries(gui ${OpenCV_LIBS} ${PC_LUA_LIBRARIES} ${PC_COMBINE_LIBRARIES})

endif()

