set (CMAKE_CXX_STANDARD 17)

ADD_LIBRARY( GameLibs STATIC
             src/Boats.cpp )

ADD_EXECUTABLE( boats_game
                main.cpp )

TARGET_LINK_LIBRARIES( boats_game
                       GameLibs )

include_directories(${CMAKE_SOURCE_DIR})
