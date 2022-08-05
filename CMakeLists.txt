cmake_minimum_required(VERSION 3.1.0)
project(G52CPP VERSION 1.0.0 LANGUAGES CXX)

# set standards
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

# set compile options
#set(COMPILE_OPTIONS_CWX "-Wall -Wextra -pedantic -Werror")

# include cmake/
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

# find SDL packages
find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_image REQUIRED)

# include SDL includes
include_directories(
	${SDL2_INCLUDE_DIR}
	${SDL2_TTF_INCLUDE_DIR}
	${SDL2_IMAGE_INCLUDE_DIR}
)

set(SOURCE_FILES_BASE
	"${PROJECT_SOURCE_DIR}/src/mainfunction.cpp"
	"${PROJECT_SOURCE_DIR}/src/lib/BaseEngine.cpp"
	"${PROJECT_SOURCE_DIR}/src/lib/DisplayableObject.cpp"
	"${PROJECT_SOURCE_DIR}/src/lib/DrawingSurface.cpp"
	"${PROJECT_SOURCE_DIR}/src/lib/FontManager.cpp"
	"${PROJECT_SOURCE_DIR}/src/lib/SimpleImage.cpp"
	"${PROJECT_SOURCE_DIR}/src/lib/TileManager.cpp"
)

set(SOURCE_FILES_CW2
	"${PROJECT_SOURCE_DIR}/src/Engine.cpp"
	"${PROJECT_SOURCE_DIR}/src/FontUtil.cpp"
	"${PROJECT_SOURCE_DIR}/src/Colour.cpp"
	"${PROJECT_SOURCE_DIR}/src/Surface.cpp"
	"${PROJECT_SOURCE_DIR}/src/Filter.cpp"
	"${PROJECT_SOURCE_DIR}/src/Tick.cpp"
	"${PROJECT_SOURCE_DIR}/src/Render.cpp"
    "${PROJECT_SOURCE_DIR}/src/PixelScalingFilter.cpp"
	"${PROJECT_SOURCE_DIR}/src/SurfaceBoundsFilter.cpp"
	"${PROJECT_SOURCE_DIR}/src/InputManager.cpp"
	"${PROJECT_SOURCE_DIR}/src/SpriteManager.cpp"
	"${PROJECT_SOURCE_DIR}/src/TimeManager.cpp"
	"${PROJECT_SOURCE_DIR}/src/InputListeners.cpp"
	"${PROJECT_SOURCE_DIR}/src/StateManager.cpp"
	"${PROJECT_SOURCE_DIR}/src/StateRequirements.cpp"
	"${PROJECT_SOURCE_DIR}/src/State.cpp"
	"${PROJECT_SOURCE_DIR}/src/StateExit.cpp"
	"${PROJECT_SOURCE_DIR}/src/StateMenu.cpp"
	"${PROJECT_SOURCE_DIR}/src/StateLevel.cpp"
	"${PROJECT_SOURCE_DIR}/src/StateMap.cpp"
	"${PROJECT_SOURCE_DIR}/src/TextScroll.cpp"
	"${PROJECT_SOURCE_DIR}/src/MenuButton.cpp"
	"${PROJECT_SOURCE_DIR}/src/MenuInput.cpp"
	"${PROJECT_SOURCE_DIR}/src/SaveGame.cpp"
	"${PROJECT_SOURCE_DIR}/src/MapCountry.cpp"
	"${PROJECT_SOURCE_DIR}/src/MapPlayer.cpp"
	"${PROJECT_SOURCE_DIR}/src/Level.cpp"
	"${PROJECT_SOURCE_DIR}/src/LevelPlayer.cpp"
	"${PROJECT_SOURCE_DIR}/src/MyTileManager.cpp"
)

# set compile flags for CWX source code files
#set_property(SOURCE ${SOURCE_FILES_CW2} APPEND PROPERTY COMPILE_FLAGS ${COMPILE_OPTIONS_CWX})

# add source files
add_executable(SDL2_App
	"${SOURCE_FILES_BASE}"
	"${SOURCE_FILES_CW2}"
)

# link libraries
target_link_libraries(SDL2_App
	${SDL2_LIBRARY}
	${SDL2_TTF_LIBRARY}
	${SDL2_IMAGE_LIBRARY}
)

# copy resources
add_custom_command(TARGET SDL2_App PRE_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
		${PROJECT_SOURCE_DIR}/src/resources ${PROJECT_BINARY_DIR})
