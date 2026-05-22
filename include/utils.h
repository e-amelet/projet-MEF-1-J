#ifndef UTILIS_H
#define UTILIS_H
/*évite les bugs possible avec le scanf en utilisant fgets et strtol*/
int lire_entier(const char *prompt, int min, int max);
void read_text(const char *prompt, char *buffer, int size);

#endif