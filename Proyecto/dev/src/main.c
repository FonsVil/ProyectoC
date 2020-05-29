#include <stdio.h>
#include <regex.h>
#include <string.h>
int main ()
{
    char linea[1024];
    FILE *text;
    text = fopen("", "r"); //Lee línea la línea y escribe en pantalla el contenido del archivo

    while(fgets(linea, 1024, (FILE*) text)) {
        char *source = linea;
        char *foursplit = "(\\d{4}-)(\\d{4})";
        char *wholenum = "(\\d{8})";
        char *twosplit = "(\\d{2}-)(\\d{2}-)(\\d{2}-)(\\d{2})";
        size_t maxM = 20; //Establece la cantidad de coincidencias máximas
        size_t maxG = 1; //Establece cuantos grupos pueden haber

        regex_t regexCompiled; //Genera la cabecera de lo compilado
        regmatch_t groupArray[maxG]; //Permite el uso de unidades de byte
        unsigned int m;
        char *cursor;

        if (regcomp(&regexCompiled, foursplit, REG_EXTENDED) || regcomp(&regexCompiled, twosplit, REG_EXTENDED) || regcomp(&regexCompiled, wholenum, REG_EXTENDED)) {
            printf("Could not compile regular expression.\n");
            return 1;
        };

        cursor = source;
        for (m = 0; m < maxM; m++) {
            if (regexec(&regexCompiled, cursor, maxG, groupArray, 0))
                break;  // No more matches

            unsigned int x = 0;
            unsigned int offset = 0;
            for (x = 0; x < maxG; x++) {
                if (groupArray[x].rm_so == (size_t) -1)
                    break;  // No more groups

                if (x == 0)
                    offset = groupArray[x].rm_eo;

                char cursorCopy[strlen(cursor) + 1];
                strcpy(cursorCopy, cursor);
                cursorCopy[groupArray[x].rm_eo] = 0;
                printf("Match %u, Group %u: [%2u-%2u]: %s\n", m, x, groupArray[x].rm_so, groupArray[x].rm_eo, cursorCopy + groupArray[x].rm_so);
            }
            cursor += offset;
        }

        regfree(&regexCompiled);
    }
    return 0;
}