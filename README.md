# Huffman-coding
Coding is based on the frequency of the byte data.

Step1 : Build the frequency table with the symbols in the input data.

Step2 : Sort the frequencies from smallest to largest.

Step3 : Build the binary tree by 
        a) Summing 2 smallest nodes and combining it to one node.
        b) Resort by frequency with the new node.
        c) End when only one node is left.

Step 4 : Assign 0s and 1s to left and right side of links respectively
         Varying length codes are generated for each symbol.

Step5 : Encode the input using the codes.

Decoding can be done the same way by building huffman tree using the frequency table and comparing the code with the input.
