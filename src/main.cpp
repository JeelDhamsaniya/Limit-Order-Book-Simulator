#include <iostream>
#include <chrono>
#include <string>
#include <vector>
#include "order_book.hpp"
#include "csv_parser.hpp"
#include "data_generator.hpp"

void print_usage(const char* program_name) {
    std::cout << "Usage: " << program_name << " <input_file> [--generate <num_orders>]\n"
              << "Options:\n"
              << "  --generate <num_orders>  Generate synthetic order data\n"
              << "  <input_file>            Input CSV file with orders\n";
}

void generate_test_data(const std::string& filename, int num_orders) {
    DataGenerator generator(100.0, 0.01, 1, 1000);
    
    auto start_time = std::chrono::nanoseconds(0);
    auto end_time = std::chrono::nanoseconds(1000000000); // 1 second
    
    auto orders = generator.generate_orders(num_orders, start_time, end_time);
    
    CSVParser parser;
    if (parser.write_orders(filename, orders)) {
        std::cout << "Generated " << num_orders << " orders in " << filename << "\n";
    } else {
        std::cerr << "Failed to write generated orders to " << filename << "\n";
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        print_usage(argv[0]);
        return 1;
    }

    std::string input_file = argv[1];
    std::vector<Order> orders;
    CSVParser parser;
    OrderBook book;

    // Handle command line arguments
    if (argc > 2 && std::string(argv[2]) == "--generate") {
        if (argc != 4) {
            print_usage(argv[0]);
            return 1;
        }
        int num_orders = std::stoi(argv[3]);
        generate_test_data(input_file, num_orders);
    }

    // Read orders from file
    if (!parser.read_orders(input_file, orders)) {
        std::cerr << "Failed to read orders from " << input_file << "\n";
        return 1;
    }

    // Process orders
    auto start_time = std::chrono::high_resolution_clock::now();
    
    for (const auto& order : orders) {
        book.add_order(order);
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

    // Print statistics
    std::cout << "\nOrder Book Statistics:\n"
              << "-------------------\n"
              << "Total orders processed: " << orders.size() << "\n"
              << "Processing time: " << duration.count() << " microseconds\n"
              << "Average latency per order: " << book.get_average_execution_latency() << " nanoseconds\n"
              << "Current spread: " << book.get_spread() << "\n"
              << "Best bid: " << book.get_best_bid() << "\n"
              << "Best ask: " << book.get_best_ask() << "\n"
              << "Total bid volume: " << book.get_bid_volume() << "\n"
              << "Total ask volume: " << book.get_ask_volume() << "\n";

    // Export final book state
    std::string output_file = "book_state.csv";
    book.export_to_csv(output_file);
    std::cout << "\nOrder book state exported to " << output_file << "\n";

    return 0;
} 