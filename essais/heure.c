#include <time.h>
#include <stdio.h>
#include <stdlib.h>

int main(){
   time_t now = time (NULL);
   struct tm tm_now = *localtime (&now);
   char s_now[sizeof "JJ/MM/AAAA HH:MM:SS"];
   strftime (s_now, sizeof s_now, "%d/%m/%Y %H:%M:%S", &tm_now);
   printf ("%s\n", s_now);
   return 0;
}
