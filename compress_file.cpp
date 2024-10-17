#include <iostream>
#include <fstream>
#include <zlib.h>
#include <vector>
#include <omp.h>
#include <mutex>

const int CHUNK_SIZE = 1024;
std::mutex outfile_mutex; // To protect writing to the file

// Function to compress a chunk of data
void compressChunk(std::vector<char> inputBuffer, std::ofstream* outfile) {
    z_stream deflateStream;
    deflateStream.zalloc = Z_NULL;
    deflateStream.zfree = Z_NULL;
    deflateStream.opaque = Z_NULL;

    if (deflateInit2(&deflateStream, Z_BEST_COMPRESSION, Z_DEFLATED, 15 + 16, 8, Z_DEFAULT_STRATEGY) != Z_OK) {
        std::cerr << "Failed to initialize compression!" << std::endl;
        return;
    }

    deflateStream.avail_in = inputBuffer.size();
    deflateStream.next_in = reinterpret_cast<Bytef*>(inputBuffer.data());

    char outBuffer[CHUNK_SIZE];
    do {
        deflateStream.avail_out = CHUNK_SIZE;
        deflateStream.next_out = reinterpret_cast<Bytef*>(outBuffer);

        deflate(&deflateStream, Z_NO_FLUSH);  // Compress the data

        std::streamsize bytesCompressed = CHUNK_SIZE - deflateStream.avail_out;

        // Thread-safe writing to the output file
        outfile_mutex.lock();  // Lock the file to prevent multiple threads writing at once
        outfile->write(outBuffer, bytesCompressed);  // Write compressed data to the file
        outfile_mutex.unlock();  // Unlock after writing

    } while (deflateStream.avail_out == 0);

    // Final flush of compressed data
    do {
        deflateStream.avail_out = CHUNK_SIZE;
        deflateStream.next_out = reinterpret_cast<Bytef*>(outBuffer);
        deflate(&deflateStream, Z_FINISH);  // Finish compression
        std::streamsize bytesCompressed = CHUNK_SIZE - deflateStream.avail_out;
        outfile_mutex.lock();  // Lock the file for writing
        outfile->write(outBuffer, bytesCompressed);  // Write final data
        outfile_mutex.unlock();  // Unlock the file
    } while (deflateStream.avail_out == 0);

    deflateEnd(&deflateStream);
}

// Function to manage the compression of the file using OpenMP
void compressFile(const std::string& sourceFile, const std::string& destinationFile, int numThreads) {
    std::ifstream infile(sourceFile, std::ios::binary);  // Open the input file
    std::ofstream outfile(destinationFile, std::ios::binary);  // Open the output file

    if (!infile.is_open() || !outfile.is_open()) {
        std::cerr << "Error opening file." << std::endl;
        return;
    }

    std::vector<std::vector<char>> chunks;  // To store all chunks of data
    std::vector<char> buffer(CHUNK_SIZE);

    // Read the file in chunks
    while (infile.read(buffer.data(), buffer.size()) || infile.gcount() > 0) {
        chunks.push_back(std::vector<char>(buffer.begin(), buffer.begin() + infile.gcount()));
    }

    // Use OpenMP to parallelize the compression of each chunk
    #pragma omp parallel for num_threads(numThreads) schedule(dynamic)
    for (int i = 0; i < chunks.size(); ++i) {
        compressChunk(chunks[i], &outfile);  // Compress each chunk
    }

    infile.close();
    outfile.close();
    std::cout << "File compressed successfully using OpenMP with " << numThreads << " threads!" << std::endl;
}

int main() {
    std::string sourceFile = "example.txt";  // The file you want to compress
    std::string compressedFile = "example_compressed.gz";  // The output compressed file

    int numThreads = 4;  // Number of threads to use
    compressFile(sourceFile, compressedFile, numThreads);

    return 0;
}



// g++-14 -fopenmp -o compress_file compress_file.cpp -lz

// ./compress_file

// https://www.zlib.net/manual.html
// https://en.cppreference.com/w/cpp/io/basic_fstream

// https://en.cppreference.com/w/cpp/thread/mutex