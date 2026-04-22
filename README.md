# Shinkansen

High throughput inter-thread messaging library written in C++. Inspired by: https://github.com/LMAX-Exchange/disruptor/

## Features

- Writing messages from a single producer thread.
- Consuming messages on a separate thread for downstream processing.

## Architecture

### Ring Buffer

The message bus is implemented using a pre-allocated ring buffer, which minimises overhead on the hot path. Event structs are acquired and published by the producer, which subsequently increments an internal sequence. The consumer polls this sequence for new events and processes them as they become available. To ensure that the producer does not write over events that have not yet been consumed, the consumer position is consulted before writes are committed to the buffer.

By adhering to the single writer principle, i.e. only one thread ever writes to the ring buffer, updating the sequences can be performed with memory barriers alone, with no locking required. Introducing multiple producers requires more expensive CAS operations, which becomes a significant bottleneck.

## Testing

The tests in this repository are written using GoogleTest: https://google.github.io/googletest/primer.html