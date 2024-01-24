#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "wav.h"

int check_format(WAVHEADER header);
int get_block_size(WAVHEADER header);

int main(int argc, char *argv[])
{
    // Ensure proper usage
    // TODO #1
    /* In the first TODO, you should ensure the program accepts two command-line arguments:
    the name of the input WAV file and the name of the output WAV file. If the program
    does not meet these conditions, you should print an appropriate error message and return 1, ending the program.  */

    FILE *input = NULL;
    FILE *output = NULL;

    input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("error with input file\n");
        return 1;
    }
    if (argv[3] != NULL)
    {
        printf("too many arguments\n");
        return 1;
    }

    /*  Open input file for reading
        TODO #2 DONE In the second TODO, you should open your input file. We’ll need to open the input file in “read-only” mode,
        since we’ll only read data from the input file. It may be wise to check that the file has been opened successfully. Otherwise,
        you should print an appropriate error message and return 1, exiting the program. We should hold off on opening the output file,
        though, lest we create a new WAV file before knowing the input file is valid! */

    /* Read header
        TODO #3 In the third TODO, you should read the header from the input file. Recall that, in wav.h,
        we’ve already implemented a struct that can store a WAV file’s header. Since we’ve written #include
        "wav.h" at the top of reverse.c, you, too, can use the WAVHEADER struct. */
    WAVHEADER header;
    fread(header.chunkID, 1, 4, input);
    fread(&header.chunkSize, 4, 1, input);
    fread(header.format, 1, 4, input);
    fread(header.subchunk1ID, 1, 4, input);
    fread(&header.subchunk1Size, 4, 1, input);
    fread(&header.audioFormat, 2, 1, input);
    fread(&header.numChannels, 2, 1, input);
    fread(&header.sampleRate, 4, 1, input);
    fread(&header.byteRate, 4, 1, input);
    fread(&header.blockAlign, 2, 1, input);
    fread(&header.bitsPerSample, 2, 1, input);
    fread(header.subchunk2ID, 1, 4, input);
    fread(&header.subchunk2Size, 4, 1, input);

    // Use check_format to ensure WAV format
    /* In the fourth TODO,
    you should complete the check_format function. check_format takes a single argument,
    a WAVHEADER called header, representing a struct containing the input file’s header. If header indicates
    the file is indeed a WAV file, the check_format function should return true. If not, check_format should
    return false. To check if a file is of the WAV format, we can compare the elements from the input file
    header to those we would expect from a WAV file. It suffices to show the “WAVE” marker characters are found
    in the format member of the WAVHEADER struct (see Background for more detail on WAV file headers). */
    // TODO #4
    check_format(header);
    // Open output file for writing
    // TODO #5
    output = fopen(argv[2], "w");
    if (output == NULL)
    {
        printf("output not found");
        return 1;
    }

    // Write header to file
    // TODO #6
    fwrite(&header, 44, 1, output);

    // Use get_block_size to calculate size of block
    /*     In the seventh TODO, you should implement the get_block_size function. get_block_size, like check_format,
        takes a single argument: this is a WAVHEADER called header, representing the struct containing the input file’s header.
        get_block_size should return an integer representing the block size of the given WAV file, in bytes. We can think of a
        block as a unit of auditory data. For audio, we calculate the size of each block with the following calculation: number
        of channels multiplied by bytes per sample. Luckily, the header contains all the information we need to compute these
        values. Be sure to reference the Background section for a more in-depth explanation as to what these values mean and how
        they are stored. See too wav.h, to determine which members of WAVHEADER might be useful. */
    // TODO #7
    int block_size = get_block_size(header);

    // Write reversed audio to file
    // TODO #8
    /*     The eighth and final TODO is where the actual reversing of the audio takes place. To do this, we need to read in each block of
        auditory data starting from the very end of the input file and moving backwards, simultaneously writing each block to the output
        file so they are written in reverse order.
        First, we should declare an array to store each block we read in. Then, it’s up to you to iterate through the input file audio data.
        You’ll want to be sure you read through all of the audio, but don’t erroneously copy any of the data from the header!
        Additionally, for testing purposes, we would like to maintain the order of the channels for each
        audio block. For example, in a WAV file with two channels (stereophonic sound), we want to make sure that the first channel of the
        last audio block in the input becomes the first channel of the first audio block in the output. */
    int num_samples = header.subchunk2Size / block_size;
    unsigned char buffer[block_size];
    for (int i = 0; i < num_samples; i++)
    {
        fseek(input, -(i + 1) * block_size, SEEK_END);
        fread(buffer, block_size, 1, input);
        fseek(output, 44 + i * block_size, SEEK_SET);
        fwrite(buffer, block_size, 1, output);
    }

    fclose(input);
    fclose(output);
}

int check_format(WAVHEADER header)
{
    // TODO #4
    if (header.format[0] == 'W' && header.format[1] == 'A' && header.format[2] == 'V' && header.format[3] == 'E')
    {
        return 0;
    }
    else
    {
        printf("not a wave");
        return 1;
    }
}

int get_block_size(WAVHEADER header)
{
    // TODO #7
    int block_size = header.numChannels * header.bitsPerSample / 8;
    return block_size;
}