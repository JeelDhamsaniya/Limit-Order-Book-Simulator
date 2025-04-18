# High-Frequency Trading Limit Order Book Simulator

A high-performance C++17 Limit Order Book (LOB) simulator designed for high-frequency trading applications. This project demonstrates advanced market-making and order matching capabilities with microsecond-level latency, making it particularly relevant for roles at market-making firms like Optiver.

## Features

- Price-time priority order matching
- Microsecond-level order processing latency
- Realistic synthetic order data generation
- CSV-based order input/output
- Comprehensive order book statistics
- Unit tests using Google Test
- Optimized for performance with -O3 compiler flags

## Performance Highlights

- Processes orders with sub-microsecond latency
- Efficient memory management with minimal allocations
- Thread-safe order book operations
- Real-time order matching and book updates

## Building the Project

### Prerequisites

- C++17 compatible compiler (GCC 7+, Clang 5+, or MSVC 2017+)
- CMake 3.10 or higher
- Google Test framework

### Build Instructions

```bash
# Create build directory
mkdir build && cd build

# Configure with CMake
cmake ..

# Build the project
make

# Run tests
./lob_tests
```

## Usage

### Running the Simulator

```bash
# Process orders from a CSV file
./lob_simulator orders.csv

# Generate synthetic test data
./lob_simulator test_orders.csv --generate 1000
```

### Input CSV Format

The input CSV file should have the following columns:
```
order_id,price,quantity,is_buy,timestamp
1,100.50,10,1,0
2,100.75,5,0,1000000
```

### Output

The simulator generates:
1. Real-time order book statistics
2. Execution latency metrics
3. Final order book state in CSV format

## Project Structure

```
.
├── include/
│   ├── order.hpp
│   ├── order_book.hpp
│   ├── csv_parser.hpp
│   └── data_generator.hpp
├── src/
│   ├── main.cpp
│   ├── order.cpp
│   ├── order_book.cpp
│   ├── csv_parser.cpp
│   └── data_generator.cpp
├── tests/
│   └── main_test.cpp
├── CMakeLists.txt
└── README.md
```

## Performance Optimization

The simulator is optimized for high-frequency trading scenarios:
- Uses std::map for O(log n) order book operations
- Minimizes memory allocations
- Implements efficient price-time priority matching
- Utilizes high-resolution timestamps for latency measurement

## Relevance to Market Making

This project demonstrates key skills relevant to market-making roles:
- Understanding of order book mechanics
- High-performance C++ programming
- Real-time data processing
- Market microstructure knowledge
- Performance optimization

## License

MIT License

## Author

[Your Name] 