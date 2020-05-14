# OPTIMISATION FLAGS : FIXME

set(OPT_FLAGS
    -Ofast)

# END OPTIMIZATION FLAGS
# SOURCES :

project(chessengine)

set(SRC_ENGINE
    src/pgn-parser.cc
    src/uci.cc
    src/pgn-move.cc
    src/pgn-exception.cc
    src/main.cc
    src/parser/option-parser.cc
    src/bitboard/bitboard.cc
    src/board/board.cc
    src/move.cc
    src/rule.cc
    src/chessboard/chessboard.cc
    src/perft_parser/fen_object.cc
    src/perft_parser/fen_rank.cc
    src/perft_parser/perft_parser.cc
    src/perft_parser/perft_object.cc
    src/utils/utils.cc
    src/generate-chessboard.cc
    )

include_directories(src src/parser)


# END SOURCES


# SETUP
cmake_minimum_required(VERSION 3.12)
project(Chess)

# BUILD OPTIONS
set(CMAKE_CXX_STANDARD 17)

if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "No build type selected, default to Release")
    set(CMAKE_BUILD_TYPE "Release")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra -pedantic")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${OPT_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Og -g -fsanitize=address -fno-inline")

# LIBRARIES
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost REQUIRED COMPONENTS system program_options)
set(LIBRARIES Boost::system Boost::program_options ${CMAKE_DL_LIBS})

# CHESSENGINE
add_executable(chessengine)
target_sources(chessengine PRIVATE ${SRC_ENGINE})
target_link_libraries(chessengine PRIVATE ${LIBRARIES})
