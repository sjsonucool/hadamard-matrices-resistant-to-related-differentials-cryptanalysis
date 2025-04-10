# Hadamard-Based MixColumns Matrices Resistant to Related-Differential Attacks

This repository contains three key pieces of code (and a data file) that implement the construction and analysis of 4×4 Hadamard-based MixColumns matrices over $\mathrm{GF}(2^8)$. These matrices are analyzed for MDS (maximum distance separable) properties and tested for specific differential-attack-related relations. See the paper [Construction of Hadamard-based MixColumns Matrices Resistant to Related-Differential Cryptanalysis](https://cic.iacr.org/p/2/1/37) for further details.

---

## Overview of Files and Folders

- **filterNoRelMatrices.c**  
  - Exhaustively generates 4-element tuples $\{h_0, h_1, h_2, h_3\}$ over $\mathrm{GF}(2^8)$ to form a 4×4 Hadamard matrix.
  - Checks if the matrix is MDS by verifying non-zero determinants of all submatrices.
  - Checks if the elements satisfy certain relations that would make them vulnerable to related differential attack.
  - Saves only those 4-element tuples (in a text file) that pass the MDS check and do **not** satisfy any of the known relations, i.e., the “resistant” tuples.

- **binarymatrixlistoflists.c**  
  - Reads the 4-element tuples produced by `filterNoRelMatrices.c` from a text file.
  - Converts each 4×4 matrix into a 32×32 binary matrix block-by-block.  
    - Each $\mathrm{GF}(2^8)$ element $x$ is converted into an 8×8 binary matrix via exponentiating a base multiplication matrix (for generator 3 in $\mathrm{GF}(2^8)$ ).
  - Outputs a Python code file (`.py`) that contains a list of these 32×32 binary matrices.
    
  **Note**: The papers's primary focus/contribution is **not** on lightweight matrix construction but the construction of matrices resistant to related-differential cryptanalysis. However, we have provided this code to facilitate those who want to perform further lightweight calculations/analysis on the binary forms of the resistant matrices. The code contains function for forming Hadamard and Circulant matrices from a given tuple, but can also be easily modified with minimum overhead to get binary forms of other types of matrices over $\mathrm{GF}(2^8)$. The only changes required are correctly specifying the respective input file(s), and creating a function for matrix formation if the matrix structure differs from Hadamard or Circulant.

- **xorcount.py**  
  - Reads the 4-element tuples output by `filterNoRelMatrices.c`.
  - Uses a pre-computed XOR cost table (`GF256_XOR_count`) from the paper "[Lightweight multiplication in $\mathrm{GF}(2^n)$ with applications to MDS matrices](https://doi.org/10.1007/978-3-662-53018-4_23)".
  - Computes the total XOR cost of implementing each 4×4 matrix in hardware (or in a chosen cost model).
  - Outputs one cost value per line in a file.

- **boxes-ref.dat**  
  - Contains lookup tables (`Alogtable` and `Logtable`) used for multiplication, exponentiation, and inverse in $\mathrm{GF}(2^8)$ (irreducible modulo polynomial $X^8 + X^4 + X^3 + X + 1$). Additionally, it also contains lookup tables for $\mathrm{GF}(2^4)$ (modulo $X^4 + X^3 + 1$ and $X^4 + X + 1$), $\mathrm{GF}(2^5)$ (modulo $X^5 + X^2 + 1$), $\mathrm{GF}(2^6)$ (modulo $X^6 + X + 1$) which can be used by specifying the correct table in the function `gmul`, correct exponent in function `modinverse` (i.e. $a^{{2^n}-2}$), and setting values in for loops repctive to the field sizes. The codes will be updated soon with macros to make the task of switching fields a bit more easier.
  - Required by both `filterNoRelMatrices.c` and `binarymatrixlistoflists.c`.
 
- **cost_from_[XZL+20]**
  - Contains files with XOR cost, calculated from the method described in the paper [Optimizing Implementations of Linear Layers](https://doi.org/10.13154/tosc.v2020.i2.120-145), of the first 5 resistant matrices with $M[0][0]=1$. The codes can be found in this [repository](https://github.com/xiangzejun/Optimizing_Implementations_of_Linear_Layers.git).
---

## Compilation and Usage

### filterNoRelMatrices.c

1. **Compile** (example):
    
    `cc -O3 filterNoRelMatrices.c`

2. **Run** (using `arg = 3` as example):
    
    `./a.out 3`

   This will create an output file `noRelfile3.txt` containing all 4-element tuples that:
   1. Pass the MDS property.
   2. Do not satisfy the specified relations.

**Note**: The command-line argument (`argv[1]`) sets `h[0]`, the first element of the 4-element tuple. The code then exhaustively searches for `h[1]`, `h[2]`, and `h[3]`.

---

### binarymatrixlistoflists.c

1. **Compile**:
    
    `cc -O3 binarymatrixlistoflists.c`

2. **Run** (with `arg = 3`):
    
    `./a.out 3`

   - Reads `noRelfile3.txt`.
   - Produces a file `noRelfile3.txt_binmatrices.py` containing a Python list of the 32×32 binary matrices.

---

### xorcount.py

1. **Run** directly with Python (using `arg = 3`):

   `python3 xorcount.py 3`

   - Reads `noRelfile3.txt`.
   - Produces a file `xcount3.txt` with the XOR cost of each matrix.

---

## boxes-ref.dat

This file is used as an `#include` by the C programs. Ensure it resides in the same directory as the `.c` source files so that they can compile and run correctly.

---

## Parallelization

The codes can be parallelized easily by creating shell script containing lines `./a.out i` or `python3 xorcount.py i` (i=1,2,3 and so on), compiling the codes as instructed and running the script with [GNU Parallel](https://doi.org/10.5281/zenodo.1146014) as 

    parallel -j [num_cores] < [SH filename]

---

## Contributing

Feel free to open issues or pull requests to suggest improvements or ask questions.
