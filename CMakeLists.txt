cmake_minimum_required(VERSION 3.10)
project(LOB_Simulator VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable optimizations
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

# Find GTest package
find_package(GTest REQUIRED)

# Add source files
set(SOURCES
    src/main.cpp
    src/order_book.cpp
    src/order.cpp
    src/data_generator.cpp
    src/csv_parser.cpp
)

# Add header files
set(HEADERS
    include/order_book.hpp
    include/order.hpp
    include/data_generator.hpp
    include/csv_parser.hpp
)

# Create main executable
add_executable(lob_simulator ${SOURCES} ${HEADERS})

# Add include directories
target_include_directories(lob_simulator PRIVATE include)

# Link against GTest
target_link_libraries(lob_simulator PRIVATE GTest::GTest GTest::Main)

# Add tests
add_executable(lob_tests tests/main_test.cpp ${SOURCES} ${HEADERS})
target_link_libraries(lob_tests PRIVATE GTest::GTest GTest::Main) 