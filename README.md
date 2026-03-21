# 🗜️ Huffman File Compression

A file compression and decompression program built in C++ using the Huffman Coding algorithm.

## 📌 What it does
- Compresses any text file into a smaller binary file
- Decompresses the binary file back to its original content
- Uses Huffman Coding — a lossless data compression algorithm

## 🧠 How it works
1. Reads the input file and counts character frequencies
2. Builds a Huffman Tree using a min-heap priority queue
3. Generates binary codes for each character
4. Encodes the file using those codes and writes to output
5. Stores the frequency table for later decompression

## 🛠️ Built With
- C++
- Standard Template Library (STL)
  - `priority_queue` — for building the Huffman tree
  - `unordered_map` — for frequency and code storage
  - `bitset` — for binary conversion
  - `shared_ptr` — for smart memory management

## 🚀 How to Run

### Compile
```bash
g++ huffman.cpp -o huffman
```

### Run
```bash
./huffman
```

### Usage
```
=== Huffman File Compression ===
1. Compress File
2. Decompress File
3. Exit
Enter your choice: 1
Enter input filename: sample.txt
Enter output filename: compressed.bin
```

| Name |
|------|
| *Rash Ian Sinag* |
| *Hannah Maeryl Perraro* | 

**Course:** Data Structures and Algorithms  
**School:** Technological University of the Philippines - Manila
