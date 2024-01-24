#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// original

int main(int argc, char *argv[])
{
    FILE *file = NULL;
    FILE *image = NULL;

    file = fopen(argv[1], "rb");
    if (file == NULL)
    {
        return 1;
    }

    int n = 0; //per vedere se ci sono tutti i pattern, verrà incrementato;
    int offset = 0; // per spostare il file del block size;
    int last_offset = 0;
    int block_size = 512; // i 512 bytes dopo;
    int copy = 0; // come true or false: se 1 sto scrivendo, sennò non ho ancora cominciato;
    int check = 1; // come true or false: se 1 copio, se 0 controllo;
    int error = 2;

    unsigned char buffer[1];
    char buffer_big[block_size + 1];

    unsigned char pattern[] = {0xFF, 0xD8, 0xFF, 0xe1}; //può essere qualsiasi


    int file_number = -1;
    char buffer_name[8];


    while (fread(buffer, 1, 1, file) != 0)
    {
        if (check == 1) //1 se sono in fase controllo
        {
            if (buffer[0] == pattern[n]) //se stringhe coincidono
            {
                n++;
                offset += 1; //mi sposto di 8 se lettere coincidono
                if (n == 3) //se trovo due pattern
                {
                    //mi sposto nel file dopo i pattern e cerco il terzo piccolo
                    error = fseek(file, offset, SEEK_SET);
                    if (error != 0)
                    {
                        return 2;
                    }
                    error = fread(buffer, 1, 1, file);
                    if (error != 1)
                    {
                        return 3;
                    }
                    if ((pattern[n] & 0xf0) == buffer[0]) //Se coincide anche quello
                    {
                        // creo file, dico di non controllare, rimetto n a 0, risposto offset a prima dei pattern,
                        // insieme a last_offset, e mi sposto nel file prima dei pattern
                        if (copy == 1)
                        {
                            fclose(image);
                        }
                        file_number++;
                        sprintf(buffer_name, "%03d.jpg", file_number);
                        image = fopen(buffer_name, "wb");
                        if (image == NULL)
                        {
                            return 4;
                        }
                        check = 0;
                        n = 0;
                        offset -= 3;
                        last_offset = offset;
                        fseek(file, last_offset, SEEK_SET);
                        if ((fseek(file, last_offset, SEEK_SET)) != 0)
                        {
                            return 5;
                        }
                    }
                    else //se non coincidono: se ho già inziato, continuo a copiare da last_offset; altrimenti mi sposto a destra di 8 e riparto a controllare;
                    {
                        if (copy == 1)
                        {
                            check = 0;
                            offset = last_offset;
                        }
                        else
                        {
                            offset += 1;
                            n = 0;
                            error = fseek(file, offset, SEEK_SET);
                            if (error != 0)
                            {
                                return 6;
                            }
                        }
                    }
                }
                else //se n < 3 controllo pattern dopo;
                {
                    continue;
                }
            }
            else //se numeri non coincidono sposto comunque di otto offset se non sto copiando, sennò copio tutto;
            {
                if (copy == 0)
                {
                    n = 0;
                    offset += 1;
                }
                else
                {
                    check = 0;
                    n = 0;
                    offset = last_offset; //quando trovo due che non combaciano, offset e last_o sono uguali a last_o;
                }
            }
        }
        else if (check == 0)
        {
            error = fseek(file, last_offset, SEEK_SET);
            if (error != 0)
            {
                return 7;
            }
            error = fread(buffer_big, block_size + 1, 1, file);
            // if (error == 0)
            // {return error;}
            error = fseek(image, 0, SEEK_END);
            if (error != 0)
            {
                return 9;
            }
            error = fwrite(buffer_big, block_size, 1, image);
            if (error != 1)
            {
                return 10;
            }
            offset += block_size;
            last_offset = offset;
            error = fseek(file, offset, SEEK_SET);
            if (error != 0)
            {
                return 11;
            }
            check = 1;
            copy = 1;
        }
    }
    printf("\n");
    fclose(file);
    fclose(image);
    return 0;
}