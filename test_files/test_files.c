#include <stdio.h>

int main( void )
{
    FILE *txt_file, *num_file;
    char *hw = "The multiply operation creates five separate copies of the 8-bit byte pattern to fan-out into a 64-bit value. The AND operation selects the bits that are in the correct (reversed) positions, relative to each 10-bit groups of bits. The multiply and the AND operations copy the bits from the original byte so they each appear in only one of the 10-bit sets. The reversed positions of the bits from the original byte coincide with their relative positions within any 10-bit set. The last step, which involves modulus division by 2^10 - 1, has the effect of merging together each set of 10 bits (from positions 0-9, 10-19, 20-29, ...) in the 64-bit value. They do not overlap, so the addition steps underlying the modulus division behave like or operations.";
    char x[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
    16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30};

    if ((txt_file = fopen("txt_file.txt", "wb")) == NULL)
    {
        fprintf(stderr, "Error opening text file.\n");
        return -1;
    }

    if ((num_file = fopen("num_file.txt", "wb")) == NULL)
    {
        fprintf(stderr, "Error opening numeric file.\n");
        return -1;
    }

    // Write text file
    if (fwrite(hw, 1, 754, txt_file) != 754)
    {
        fprintf(stderr, "Error writing text file.\n");
        fcloseall();
        return -1;
    }

    // Write numeric file
    if (fwrite(x, 1, 30, num_file) != 30)
    {
        fprintf(stderr, "Error writing numeric file file.\n");
        fcloseall();
        return -1;
    }

    fclose(txt_file);
    fclose(num_file);
    return 0;
}
