# Huffman Coding
This project was done as a class project for the following class:
Data Structures and Algorithms (INFO0902-1)
It is a C implementation of the huffman coding algorithm.  

Grade: ?/20

### Running
First, compile:  
`gcc -o huffman main.c CodingTree.c BinarySequence.c CharVector.c coding.c decoding.c HeapPriorityQueue.c`  
Then, run:   
` [-e] [-d] [-f <eof_char>] [-o <outptPath>] <textPath> <csvPath>`  
* -e To encode
* -d To decode
* <eof_char> the end of sequence character (Default: 28)
* -o Output file path
* textPath: Input file path
* csvPath the file containing the frequency of each character
### Report
The release folder contains a pdf report, answering some theoretical questions
about the project. (In French)
### Assignment
[Click here to see the assignment PDF](http://www.montefiore.ulg.ac.be/~jmbegon/2019_2020/sda/p2.pdf)