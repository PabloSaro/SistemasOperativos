#ifndef _MYTAR_H
#define _MYTAR_H

#include <limits.h>

typedef enum{
  NONE,
  ERROR,
  CREATE,
  EXTRACT
} flags;

typedef struct {
  char* name;
  unsigned int size;
} stHeaderEntry; // Nombre y tamaño de cada archivo, esta info iria ubicada justo despues de el dato de los N elementos

int createTar(int nFiles, char *fileNames[], char tarName[]);//crea fichero con nombre tarName, en el se incluyen nFiles  ficheros con los nombres fileNames
int extractTar(char tarName[]);//extrae del tar los ficheros


#endif /* _MYTAR_H */
