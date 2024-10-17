# Parallel File Compression Using OpenMP and zlib

# file-compressing-using-threads
This project implements a multithreaded file compression tool using C++ with OpenMP and zlib. It reads files in chunks, compresses them in parallel using threads, and writes the compressed data to an output file in gzip format. The program demonstrates the use of OpenMP for parallelization and ensures thread-safe file operations using mutexes.

## Overview

This project implements a multithreaded file compression tool using C++ with the **zlib** compression library and **OpenMP** for parallelization. The program reads a file, compresses its content in parallel chunks, and writes the compressed data to an output file in `.gz` format.

## Features

- **Parallel compression**: The program uses OpenMP to parallelize the compression of file chunks.
- **zlib-based compression**: It leverages the `zlib` library for high-performance file compression.
- **Thread safety**: A mutex ensures that multiple threads can write to the output file safely.

## Prerequisites

Make sure you have the following installed on your system:

- **g++** (with OpenMP support): Install via Homebrew on macOS or package manager on Linux.
- **zlib**: This is often pre-installed on most systems, but you can install it via your package manager.

## Build Instructions

1. Compile the program with OpenMP and zlib:
   ```bash
   g++-14 -fopenmp -o compress_file compress_file.cpp -lz
   ```

2. Run the program:
   ```bash
   ./compress_file
   ```

   The program will compress `example.txt` into `example_compressed.gz` using 4 threads by default.

## How it Works

The program performs the following steps:

1. **Read File in Chunks**: The input file is read in chunks of 1024 bytes (defined by `CHUNK_SIZE`).
2. **Parallel Compression**: Using OpenMP, the file is compressed in parallel. Each chunk is handled by a separate thread.
3. **Thread-Safe Writing**: The compressed data is written to the output file in a thread-safe manner using a mutex.
4. **Output**: The final compressed file is saved in gzip format.

## Libraries Used

- **zlib**: [zlib manual](https://www.zlib.net/manual.html)
- **OpenMP**: [OpenMP Documentation](https://www.openmp.org/wp-content/uploads/OpenMP-4.0-C.pdf)
- **std::fstream**: [cppreference: std::fstream](https://en.cppreference.com/w/cpp/io/basic_fstream)
- **std::mutex**: [cppreference: std::mutex](https://en.cppreference.com/w/cpp/thread/mutex)

## Contributing

Feel free to submit pull requests or raise issues if you find bugs or have feature suggestions. Contributions are always welcome!

