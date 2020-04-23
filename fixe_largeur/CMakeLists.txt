cmake_minimum_required(VERSION 3.12.2)
project(huffman_coding)
set(CMAKE_C_STANDARD 99)
add_executable(huffman main.c CodingTree.c coding.c CharVector.c BinarySequence.c ListPriorityQueue.c decoding.c)