import sys

# Hard-coded GF256_XOR_count array constructed from the given list in [BKL16].
# Index 0 corresponds to element=1, Index 1 -> element=2, ..., Index 254 -> element=255.
GF256_XOR_count = [
0,2,3,2,3,2,3,2,2,3, # Elements 1 to 10
2,4,2,3,3,2,3,2,2,2, # Elements 11 to 20
3,3,3,2,2,3,2,2,2,2, # Elements 21 to 30
2,3,3,2,3,3,2,3,2,2, # Elements 31 to 40
3,2,3,2,3,3,3,2,2,2, # Elements 41 to 50
3,3,3,3,3,3,3,3,2,2, # Elements 51 to 60
3,3,2,2,2,3,3,3,2,2, # Elements 61 to 70
3,2,2,2,2,3,2,2,2,4, # Elements 71 to 80
2,3,3,3,3,2,2,2,3,2, # Elements 81 to 90
2,2,2,2,3,3,2,3,3,2, # Elements 91 to 100
3,3,3,2,3,3,2,3,3,3, # Elements 101 to 110
2,2,2,3,2,2,3,2,2,3, # Elements 111 to 120
2,2,2,3,2,3,2,3,3,2, # Elements 121 to 130
3,3,2,3,2,3,2,3,2,3, # Elements 131 to 140
2,3,2,3,3,2,3,3,3,3, # Elements 141 to 150
3,2,2,3,3,3,2,2,3,3, # Elements 151 to 160
3,3,3,2,2,3,2,3,2,2, # Elements 161 to 170
2,3,2,3,3,4,2,2,2,3, # Elements 171 to 180
3,3,2,2,3,2,3,4,4,3, # Elements 181 to 190
2,2,2,3,3,3,3,2,3,2, # Elements 191 to 200
2,3,2,2,3,3,2,2,3,3, # Elements 201 to 210
3,3,2,3,2,3,2,3,2,2, # Elements 211 to 220
2,3,2,2,2,3,2,2,3,2, # Elements 221 to 230
2,2,2,2,2,2,4,2,2,2, # Elements 231 to 240
3,3,3,2,2,3,3,2,3,2, # Elements 241 to 250
3,3,3,2,2          # Elements 251 to 255
]

def main():
    if len(sys.argv) < 2:
        print("Usage: python3 xorcount.py [arg]")
        sys.exit(1)

    arg = sys.argv[1]
    input_filename = f"noRelfile{arg}.txt"
    output_filename = f"xcount{arg}.txt"

    try:
        with open(input_filename, 'r') as infile, open(output_filename, 'w') as outfile:
            for line in infile:
                line = line.strip()
                if not line:
                    continue
                # Parse the four elements
                elems = line.split()
                if len(elems) < 4:
                    # Not a valid line of data
                    continue
                elem1, elem2, elem3, elem4 = map(int, elems)

                # Lookup the XOR counts
                xcvelem1 = GF256_XOR_count[elem1 - 1]
                xcvelem2 = GF256_XOR_count[elem2 - 1]
                xcvelem3 = GF256_XOR_count[elem3 - 1]
                xcvelem4 = GF256_XOR_count[elem4 - 1]

                # Compute the result
                result = 4*(xcvelem1 + xcvelem2 + xcvelem3 + xcvelem4) + 96

                # Write the result
                outfile.write(str(result) + "\n")

    except FileNotFoundError:
        print(f"Error: File {input_filename} not found.")
        sys.exit(1)

if __name__ == "__main__":
    main()

