#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "mytar.h"

extern char *use;

/** Copy nBytes bytes from the origin file to the destination file.
 *
 * origin: pointer to the FILE descriptor associated with the origin file
 * destination:  pointer to the FILE descriptor associated with the destination file
 * nBytes: number of bytes to copy
 *
 * Returns the number of bytes actually copied or -1 if an error occured.
 */
int copynFile(FILE * origin, FILE * destination, int nBytes){// copiamos nbytes de origen a destino

	int c, ret, bytesLeidos = 0;

	while(((c = getc(origin))!= EOF) && bytesLeidos < nBytes){//getc devuelve un char modelado a int, por eso en putc debemos castear

		//copia caracter a caracter en el fihero destino
		ret = putc((unsigned char) c, destination);

		if(feof(destination)){
			fclose(destination);
			printf("put failed");
		}

		else bytesLeidos++;

	}

	//Devuelve el numero de bytes
	return bytesLeidos;
}

/** Loads a string from a file.
 *
 * file: pointer to the FILE descriptor 
 * 
 * The loadstr() function must allocate memory from the heap to store 
 * the contents of the string read from the FILE. 
 * Once the string has been properly built in memory, the function returns
 * the starting address of the string (pointer returned by malloc()) 
 * 
 * Returns: !=NULL if success, NULL if error
 */
char* loadstr(FILE * file){


	int c, numCaracteres; //numacaracteres, numero de caracteres leidos
	int fin = 0;

	numCaracteres = 0;


	while( ((c = getc(file))!= EOF) && fin != 1) {//leo una cadena de caracteres

		if((unsigned char)c == '\0'){//si es el fin de la cadena
			fin = 1;
		}

		else
			//si no es el fin de la cadena va incrementando el num de caracteres leidos
			numCaracteres++;
	}

	char *cadena = malloc(numCaracteres * sizeof(unsigned char));//reserva memoria para la cadena

	fseek(file, -2-numCaracteres, SEEK_CUR);//atrasamos el cursor

	int i = 0;

	//vuelve a leer con el tamaño actual
	while(( c = getc(file))!= EOF && i < numCaracteres){

		cadena[i] = (unsigned char) c;
		i++;
	}

	return cadena;
}

/** Read tarball header and store it in memory.
 *
 * tarFile: pointer to the tarball's FILE descriptor 
 * nFiles: output parameter. Used to return the number 
 * of files stored in the tarball archive (first 4 bytes of the header).
 *
 * On success it returns the starting memory address of an array that stores 
 * the (name,size) pairs read from the tar file. Upon failure, the function returns NULL.
 */


stHeaderEntry* readHeader(FILE * tarFile, int *nFiles){// LEE LA CABECERA TARFILE Y COPIA LA INFORMACION EN EL HEADER Y LO DEVUELVE


	stHeaderEntry* cabeceras = NULL;
	int nr_files = 0, i;

	fscanf(tarFile, "%i", &nr_files); // leemos el numero de archivos que contiene el tar

	cabeceras = malloc(sizeof(stHeaderEntry)*nr_files); // reservamos espacio para el array de cabeceras

	for(i = 0; i < nr_files; i++){

			char *cadena;
			cadena = loadstr(tarFile); //devuelve la cadena leida
			cabeceras[i].name = cadena; //asignamos al nombre la cadena leida

			fscanf(tarFile, "%i", &cabeceras[i].size);//leemos tamaño
			cadena = NULL;
	}

	*nFiles = nr_files;
	return cabeceras;//Devolvemos el header con los datos
}

/** Creates a tarball archive 
 *
 * nfiles: number of files to be stored in the tarball
 * filenames: array with the path names of the files to be included in the tarball
 * tarname: name of the tarball archive
 * 
 * On success, it returns EXIT_SUCCESS; upon error it returns EXIT_FAILURE. 
 * (macros defined in stdlib.h).
 *
 * HINTS: First reserve room in the file to store the tarball header.
 * Move the file's position indicator to the data section (skip the header)
 * and dump the contents of the source files (one by one) in the tarball archive. 
 * At the same time, build the representation of the tarball header in memory.
 * Finally, rewind the file's position indicator, write the number of files as well as 
 * the (file name,file size) pairs in the tar archive.
 *
 * Important reminder: to calculate the room needed for the header, a simple sizeof 
 * of stHeaderEntry will not work. Bear in mind that, on disk, file names found in (name,size) 
 * pairs occupy strlen(name)+1 bytes.
 *
 */
int createTar(int nFiles, char *fileNames[], char tarName[]){


    FILE* tarFile = NULL;
	int bytesCopiados;

	tarFile = fopen(tarName, "w");//abrimos archivo pasado por tarname

	stHeaderEntry* arrayCabeceras = NULL;

	int i, desplazamiento;
	int tamCabeceras = 0;

	printf("Comenzamos a Crear archivo MTAR\n");

	for(i = 0; i < nFiles; i++){

		tamCabeceras = tamCabeceras + strlen(fileNames[i]) + 1; //tamCabeceras es un contador que guarda cuanto ocupa la cabecera
	}

	desplazamiento  = sizeof(int) + (nFiles * sizeof(unsigned int)) + tamCabeceras;//calculamos el desplazamiento


	if(fseek(tarFile, desplazamiento, SEEK_SET) != 0){ //movemos el cursor
		exit(EXIT_FAILURE);
	}

	arrayCabeceras = malloc(sizeof(stHeaderEntry)*nFiles); //reserva memoria para el array de cabeceras
	int bytesTotales = 0;

	for(i = 0; i< nFiles; i++){

		printf("Copiando %s a MTAR . . .\n", fileNames[i]);

		FILE* fichero = NULL;
		fichero = fopen(fileNames[i], "r+");
		bytesCopiados = copynFile(fichero, tarFile, INT_MAX); //copiamos los ficheros
		bytesTotales = bytesTotales + bytesCopiados;
		arrayCabeceras[i].name = fileNames[i];
		arrayCabeceras[i].size = bytesCopiados;

		sleep(1);
		fclose(fichero);
	}


	fseek(tarFile, 0, SEEK_SET);// movemos cursor
	fprintf(tarFile, "%i", nFiles);//escribimos el numero de nfiles en el tarfile

	for(i = 0; i< nFiles; i++){

			fprintf(tarFile, "%s", arrayCabeceras[i].name);//escribinos los nombres en el tarfile
			putc('\0',tarFile);
			fprintf(tarFile, "%i", arrayCabeceras[i].size);//escribimos el tamaño de los archivos
		}

	fclose(tarFile);
	return EXIT_FAILURE;
}

/** Extract files stored in a tarball archive
 *
 * tarName: tarball's pathname
 *
 * On success, it returns EXIT_SUCCESS; upon error it returns EXIT_FAILURE. 
 * (macros defined in stdlib.h).
 *
 * HINTS: First load the tarball's header into memory.
 * After reading the header, the file position indicator will be located at the 
 * tarball's data section. By using information from the 
 * header --number of files and (file name, file size) pairs--, extract files 
 * stored in the data section of the tarball.
 *
 */
int extractTar(char tarName[]){


	int nr_files, i;

	FILE * tarFile = NULL;
	stHeaderEntry* cabeceras = NULL;

	tarFile = fopen(tarName, "r+"); //abrimos archivo pasado por tarname

	cabeceras = readHeader(tarFile, &nr_files);

	/* calculara desplazamiento ya que el mtar ha dejado 4 bytes para los enteros, y solo hemos usado un byte*/

	int tamCabeceras = 0;
	for(i = 0; i < nr_files; i++){
		tamCabeceras = tamCabeceras + strlen(cabeceras[i].name) + 1;
	}

	int desplazamiento  = sizeof(int) + (nr_files * sizeof(unsigned int)) + tamCabeceras;


	if(fseek(tarFile, desplazamiento, SEEK_SET) != 0){
		exit(EXIT_FAILURE);
	}

	/*----------------------------------------------------------------------------------------*/


	for(i = 0; i < nr_files ; i++){

		printf("[%i]: Creando archivo %s, Tamanio : %i Bytes...OK\n",i,cabeceras[i].name, cabeceras[i].size);

		FILE *file = fopen(cabeceras[i].name, "w+"); // creo el file de nombe [i].name
		copynFile(tarFile, file, cabeceras[i].size);// le paso el tamaño del archivo

		fseek(tarFile, -1, SEEK_CUR);//retrasamos un caracter ya que el copynfile deja el cursor un caracter delante del que deberia
	}

	printf("numero de archivos a cargar: %i\n", nr_files);


	fclose(tarFile);
	return EXIT_FAILURE;
}
