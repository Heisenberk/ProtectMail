#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
#define ENCODE 1
#define DECODE 0
 
static char b64_e[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrs"
                      "tuvwxyz0123456789+/=";
 
static int b64_d[] = {
    ['A'] =  0, ['B'] =  1, ['C'] =  2, ['D'] =  3, ['E'] =  4,
    ['F'] =  5, ['G'] =  6, ['H'] =  7, ['I'] =  8, ['J'] =  9,
    ['K'] = 10, ['L'] = 11, ['M'] = 12, ['N'] = 13, ['O'] = 14,
    ['P'] = 15, ['Q'] = 16, ['R'] = 17, ['S'] = 18, ['T'] = 19,
    ['U'] = 20, ['V'] = 21, ['W'] = 22, ['X'] = 23, ['Y'] = 24,
    ['Z'] = 25, ['a'] = 26, ['b'] = 27, ['c'] = 28, ['d'] = 29,
    ['e'] = 30, ['f'] = 31, ['g'] = 32, ['h'] = 33, ['i'] = 34,
    ['j'] = 35, ['k'] = 36, ['l'] = 37, ['m'] = 38, ['n'] = 39,
    ['o'] = 40, ['p'] = 41, ['q'] = 42, ['r'] = 43, ['s'] = 44,
    ['t'] = 45, ['u'] = 46, ['v'] = 47, ['w'] = 48, ['x'] = 49,
    ['y'] = 50, ['z'] = 51, ['0'] = 52, ['1'] = 53, ['2'] = 54,
    ['3'] = 55, ['4'] = 56, ['5'] = 57, ['6'] = 58, ['7'] = 59,
    ['8'] = 60, ['9'] = 61, ['+'] = 62, ['/'] = 63, ['='] = 64
};
 
 
static int
isbase64(unsigned char const *s, int n)
{
    while (n-- > 0)
        if (strchr(b64_e, s[n]) == NULL)
            return 0;
    return 1;
}
 
 
static void
delete_newline(char *s)
{
    char *p;
 
    if ((p = strchr(s, '\n')) != NULL)
        *p = '\0';
}
 
 
static void
delete_b64(char *s)
{
    char *p;
 
    if ((p = strstr(s, ".b64")) != NULL)
        *p = '\0';
}
 
 
static void
drain_stdin(void)
{
    while (getchar() != '\n')
        ;
}
 
 
static int
get_nbytes(unsigned char *byte, int n, FILE *fp)
{
    int i;
    int end = (fp == stdin) ? '\n' : EOF;
 
    memset(byte, 0, n);
    for (i = 0; i < n; i++) {
        int c;
 
        if ((c = getc(fp)) != EOF && c != end)
            byte[i] = c;
        else
            break;
    }
 
    return (ferror(fp)) ? -1 : i;
}
 
 
static FILE *
base64_encode(FILE *fp)
{
    unsigned char byte[3];
    FILE *tmp;
    int n;
 
    if ((tmp = tmpfile()) == NULL)
        return NULL;
 
    do {
        unsigned char pkg[4];
 
        if ((n = get_nbytes(byte, 3, fp)) <= 0)
            break;
 
        pkg[0] = (byte[0] >> 2);
        pkg[1] = (byte[0] << 4 | byte[1] >> 4) & 0x3f;
        pkg[2] = (byte[1] << 2 | byte[2] >> 6) & 0x3f;
        pkg[3] = (byte[2] & 0x3f);
 
        putc(b64_e[pkg[0]], tmp);
        putc(b64_e[pkg[1]], tmp);
        putc((n >= 2) ? b64_e[pkg[2]] : '=', tmp);
        putc((n >= 3) ? b64_e[pkg[3]] : '=', tmp);
    } while (n == 3);
 
    rewind(tmp);
    return (n >= 0) ? tmp : NULL;
}
 
 
static FILE *
base64_decode(FILE *fp)
{
    unsigned char pkg[4];
    FILE *tmp;
    int n;
 
    if ((tmp = tmpfile()) == NULL)
        return NULL;
 
    while ((n = get_nbytes(pkg, 4, fp)) == 4) {
        unsigned char byte[4];
 
        if (!isbase64(pkg, 4))
            return NULL;
 
        byte[0] = (b64_d[pkg[0]] << 2 | b64_d[pkg[1]] >> 4);
        byte[1] = (b64_d[pkg[1]] << 4 | b64_d[pkg[2]] >> 2);
        byte[2] = (b64_d[pkg[2]] << 6 | b64_d[pkg[3]]);
 
        putc(byte[0], tmp);
        (pkg[2] != '=') ? putc(byte[1], tmp) : 0;
        (pkg[3] != '=') ? putc(byte[2], tmp) : 0;
    }
 
    rewind(tmp);
    return (n >= 0) ? tmp : NULL;
}
 
 
static void
copy(FILE *src, FILE *dst, int end)
{
    int c;
 
    while ((c = getc(src)) != EOF)
        putc(c, dst);
 
    if (end)
        putc(end, dst);
}
 
 
static int
menu(int *action, FILE **in, FILE **out)
{
    static char tmp[FILENAME_MAX];
 
    printf("Do you want to encode(1) or decode(0)? ");
    fflush(stdout);
 
    if (scanf("%d", action) != 1)
        return 0;
    drain_stdin();
 
    printf("Which file do you want to %scode('-' for stdin)?\n",
           (*action) ? "en" : "de");
 
    fgets(tmp, FILENAME_MAX, stdin);
    delete_newline(tmp);
 
    if (strcmp(tmp, "-") == 0) {
        *in = stdin;
        *out = stdout;
        return 1;
    }
 
    if ((*in = fopen(tmp, "r")) == NULL)
        return 0;
 
    if (*action == DECODE)
        delete_b64(tmp);
 
    strncat(tmp, (*action) ? ".b64" : ".orig", FILENAME_MAX);
    tmp[FILENAME_MAX - 1] = '\0';
 
    if ((*out = fopen(tmp, "w")) == NULL) {
        fclose(*in);
        return 0;
    }
 
    printf("Result saved in %s", tmp);
    return 1;
}
 
 
int
main(void)
{
    int action;
    FILE *in, *out, *tmp;
 
    if (!menu(&action, &in, &out)) {
        fputs("Invalid entry!\n", stderr);
        return EXIT_FAILURE;
    }
 
    if (action == ENCODE)
        tmp = base64_encode(in);
    else
        tmp = base64_decode(in);
 
    if (tmp == NULL) {
        fprintf(stderr, "Error when %scoding\n",
                (action) ? "en" : "de");
        fclose(in);
        fclose(out);
        return EXIT_FAILURE;
    }
 
    copy(tmp, out, (out == stdout) ? '\n' : 0);
    fclose(in);
    fclose(out);
    return EXIT_SUCCESS;
}
