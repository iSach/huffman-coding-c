cmake_minimum_required(VERSION 3.13)
project(huffman_coding)
set(CMAKE_C_STANDARD 99)

add_executable(main.c CodingTree.c coding.c CharVector.c BinarySequence.c ListPriorityQueue.c decoding.c)