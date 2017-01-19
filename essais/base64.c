#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
 
 
#define ENCODE  1
#define DECODE  2
 
 
static const char b64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
 
 
static void encode_b64 (const unsigned char* in, unsigned char* out, size_t len)
{
    out[0] = b64[in[0] >> 2];
    out[1] = b64[((in[0] & 0x03) << 4) | (in[1] >> 4)];
    out[2] = (len > 1) ? b64[((in[1] & 0x0f) << 2) | (in[2] >> 6)] : '=';
    out[3] = (len > 2) ? b64[in[2] & 0x3f] : '=';
}
 
static unsigned char val (unsigned char c)
{
    if (c >= 'A' && c <= 'Z')
        return c - 'A';
    else if (c >= 'a' && c <= 'z')
        return 26 + c - 'a';
    else if (c >= '0' && c <= '9')
        return 52 + c - '0';
    else if (c == '+')
        return 62;
    else if (c == '/')
        return 63;
    else
    {
        fprintf(stderr, "Invalid input file\n");
        exit(EXIT_FAILURE);
    }
}
 
static size_t decode_b64 (const unsigned char* in, unsigned char* out)  // len == 4
{
    unsigned char v1 = val(in[1]);
    out[0] = (val(in[0]) << 2) | (v1 >> 4);
    if (in[2] == '=')
    {
        if (in[3] == '=')
            return 1;
        else
        {
            fprintf(stderr, "Invalid input file\n");
            exit(EXIT_FAILURE);
        }
    }
    unsigned char v2 = val(in[2]);
    out[1] = (v1 << 4) | (v2 >> 2);
    if (in[3] == '=')
        return 2;
    out[2] = (v2 << 6) | val(in[3]);
    //out[2] = ((v2 & 0x03) << 6) | val(in[3]);
    return 3;
}
 
void traitement (const char* infile, const char* outfile, int flag)
{
    if ( strcmp (infile,"-") != 0 )
        if ( freopen (infile, "rb", stdin) == NULL)
        {
            perror("");
            exit(EXIT_FAILURE);
        }
    if ( strcmp (outfile,"-") != 0 )
        if ( freopen (outfile, "wb", stdout) == NULL)
        {
            perror("");
            fclose(stdin);
            exit(EXIT_FAILURE);
        }
 
    if (flag == ENCODE)
    {
        unsigned char ibuffer[4], obuffer[4];
        size_t n;
        while ( (n = fread(ibuffer, sizeof(char), 3, stdin)) != 0 )
        {
            ibuffer[n] = 0;
            encode_b64(ibuffer, obuffer, n);
            fwrite(obuffer, sizeof(char), 4, stdout);
        }
    }
    else if (flag == DECODE)
    {
        unsigned char ibuffer[4], obuffer[4];
        size_t n, le;
        while ( (n = fread(ibuffer, sizeof(char), 4, stdin)) == 4 )
        {
            le = decode_b64(ibuffer, obuffer);
            fwrite(obuffer, sizeof(char), le, stdout);
        }
        if (n != 0)
        {
            fprintf(stderr, "Invalid input file\n");
            exit(EXIT_FAILURE);
        }
    }
 
 
    if ( strcmp (infile,"-") != 0 )
        fclose(stdin);
    if ( strcmp (outfile,"-") != 0 )
        fclose(stdout);
}
 
int main(int argc, char** argv)
{
    int flag = 0, opt;
    const char *file[2];
    file[0] = file[1] = "-"; /* valeurs par défaut */
 
    while ((opt = getopt(argc, argv, "edi:o:")) != -1)
    {
        switch (opt)
        {
        case 'e':
            if (flag != 0)
            {
                fprintf(stderr, "Usage: %s [-e : -d] [-i filename] [-o filename] name\n",
                        argv[0]);
                exit(EXIT_FAILURE);
            }
            flag = ENCODE;
            break;
        case 'd':
            if (flag != 0)
            {
                fprintf(stderr, "Usage: %s [-e : -d] [-i filename] [-o filename] name\n",
                        argv[0]);
                exit(EXIT_FAILURE);
            }
            flag = DECODE;
            break;
        case 'i':
            if (strcmp(file[0],"-") != 0)
            {
                fprintf(stderr, "Usage: %s [-e : -d] [-i filename] [-o filename] name\n",
                        argv[0]);
                exit(EXIT_FAILURE);
            }
            file[0] = optarg;
            break;
        case 'o':
            if (strcmp(file[1],"-") != 0)
            {
                fprintf(stderr, "Usage: %s [-e : -d] [-i filename] [-o filename] name\n",
                        argv[0]);
                exit(EXIT_FAILURE);
            }
            file[1] = optarg;
            break;
        default: /* '?' */
            fprintf(stderr, "Usage: %s [-i filename] [-o filename] name\n",
                    argv[0]);
            exit(EXIT_FAILURE);
        }
    }
 
    if (flag == 0)
    {
        fprintf(stderr, "Expected argument after options\n");
        exit(EXIT_FAILURE);
    }
 
    traitement (file[0], file[1], flag);
 
    return EXIT_SUCCESS;
}
