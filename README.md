This project is envisioned as a way for me to learn simple, but effective, end-to-end techniques for facilitating order matching.

The following is a diagram of the information flow:

<img width="467" alt="image" src="https://github.com/user-attachments/assets/85a3b517-8081-484b-b944-bd852941d0db" />

# Order Matching Engine

A multithreaded C++ order matching engine with real-time networking support, trade logging, and modular architecture. The engine accepts TCP connections from multiple clients, processes orders concurrently, matches them based on price-time priority, and sends trade notifications back to the clients involved.

This project demonstrates deep understanding of low-level systems, concurrency, and socket programming, with emphasis on performance and extensibility.

## Features

- Concurrent TCP server supporting multiple clients
- Custom lightweight protocol for order communication
- Thread-safe producer-consumer buffer between network and engine
- Matching engine with price-time priority (supports LIMIT and MARKET orders)
- Real-time trade notifications to clients
- Trade logging to CSV file
- Fully modular and testable architecture

## Architecture Overview

The system follows a decoupled, modular design:

Clients connect to the server and send orders. The TCP server parses incoming orders and pushes them into a shared thread-safe buffer. A separate matching engine thread consumes orders and executes trades. Executed trades are logged to CSV and sent back to the clients via the notifier.

You can refer to the architecture diagram above.

## Build Instructions

This project uses CMake.

```bash
mkdir build
cd build
cmake ..
make
```

After building, two executables are produced:
- `./order_engine` – starts the matching engine and TCP server
- `./client` – a basic client that connects to the server and allows you to send orders

## Running the System

### 1. Start the server and engine

```bash
./order_engine
```

This will launch the TCP server (port 9999) and the matching engine thread.

### 2. Start one or more clients

In separate terminal windows:

```bash
./client
```

You can run multiple client instances concurrently to simulate trading activity.

## Order Format

Orders must be sent using the following CSV format, encapsulated in a length-prefixed TCP message:

```
[NEW],[id],[BUY|SELL],[LIMIT|MARKET],[price],[quantity],[timestamp]
[CANCEL],[id]
```

Example:

```
NEW,1,BUY,LIMIT,101.5,10,1711920000
```

- `id` is a unique order ID
- `price` and `quantity` are numeric
- `timestamp` can be auto-generated on the server if needed

## Trade Notifications

When a trade is executed, both clients involved in the trade receive a notification in this format:

```
TRADE,[buyId],[sellId],[price],[quantity]
```

Example:

```
TRADE,1,2,101.5,10
```

## Trade Logging

All trades are logged to `trades.csv` automatically, with the following columns:

```
buyOrderId,sellOrderId,price,quantity
```

## Project Structure

```
src/
├── core/           # Matching engine, Order, Trade logic
├── concurrency/    # Thread-safe OrderBuffer
├── networking/     # TcpServer, serialization, client/server
├── listeners/      # TradeLogger, TradeNotifier
tests/              # Unit tests
CMakeLists.txt
README.md
```

## Performance Benchmark

### Simple Benchmark

The engine was benchmarked on a MacBook Air M4 with the following results:

| Orders     | Time     | Throughput        |
|------------|----------|-------------------|
| 100k       | 42 ms    | 2.38M orders/sec  |
| 1M         | 220 ms   | 4.54M orders/sec  |
| 10M        | 1.52 s   | 6.56M orders/sec  |
| 100M       | 19.77 s  | 5.05M orders/sec  |

Benchmark uses randomly generated LIMIT orders (BUY/SELL), single-threaded matching, and a shared thread-safe buffer between producers and the engine.

Run benchmarks with:

```bash
./benchmarkSimple
./benchmarkConcurrent
```

### Concurrent Benchmark

Threaded benchmarks show slightly lower throughput due to mutex contention in the shared order buffer. Future versions may use lock-free data structures to improve parallel ingestion.

## Benchmark (Lock-Free Queue)

After replacing the original mutex-based queue with a lock-free MPSC queue (moodycamel::ConcurrentQueue), throughput improved significantly:

| Orders     | Time     | Throughput           |
|------------|----------|----------------------|
| 100k       | 11 ms    | 9.09M orders/sec     |
| 1M         | 41 ms    | 24.39M orders/sec    |
| 10M        | 306 ms   | 32.68M orders/sec    |
| 100M       | 2.75 s   | 36.42M orders/sec    |

This demonstrates how eliminating contention on the shared order buffer enables the engine to scale efficiently across multiple producer threads.

## Performance graph

![image](https://github.com/user-attachments/assets/639d7d77-dcc7-4ae7-bed7-8a60e339145a)



## Future Improvements

- Symbol-aware order books and matching
- Order cancellation support
- REST or WebSocket client interface
- Enhanced client session handling

## Author

Bogdan Petrovic

