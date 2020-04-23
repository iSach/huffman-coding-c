cmake_minimum_required(VERSION 3.13)
project(huffman_coding)
set(CMAKE_C_STANDARD 99)
add_executable(huffman main.c coding.c CodingTree.c CharVector.c BinarySequence.c HeapPriorityQueue.c)