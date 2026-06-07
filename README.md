# AES MixColumns Implementation (C++)

## Overview

This project implements the AES (Advanced Encryption Standard) MixColumns transformation using finite field arithmetic in GF(2⁸).

The program accepts a 128-bit hexadecimal input, converts it into a 4×4 AES state matrix, performs the MixColumns transformation, displays the state before and after processing, and saves the results to an output file.

---

## Features

* AES MixColumns implementation
* Supports 128-bit hexadecimal input
* Keyboard input support
* File input support
* 4×4 state matrix visualization
* Output file generation
* Input validation

---

## Technologies Used

* C++
* AES Cryptography
* Finite Field Arithmetic (GF(2⁸))
* Visual Studio Code

---

## Project Structure

AES-MixColumns-Implementation/

├── Q11_MixColumns.cpp

├── README.md

├── MixColumns_INPUT_sample.txt

└── MixColumns_OUTPUT.txt

---

## Compilation

Compile the program on AES-MixColumns-Implementation terminal using g++:

bash
g++ MixColumns.cpp -o mixcolumns


## Execution

Run the executable:

bash
./mixcolumns


or on Windows:

bash
.\mixcolumns


---

## Example Input

text
D4BF5D30E0B452AEB84111F11E2798E5


---

## Learning Outcomes

* Understanding AES encryption architecture
* Implementing MixColumns transformation
* Finite field multiplication in GF(2⁸)
* Input validation and file handling in C++
* Cryptographic programming concepts

---

## Author

Nurairin Syamimi

Cybersecurity Student
