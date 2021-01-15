#!/bin/bash

FICHERO1="fuseLib.c" #>18kb
FICHERO2="myFS.c" #>11kb
FICHERO3="myFS.h" #>7kb
MPOINT="./mount-point"
#APARTADO A
if find temp  ; then  # Si existe tmp, borra el directorio recursivamente con -r
	echo -e '\e[1;32mBorrando Directorio TEMP\e[0m'
	sleep 1
	rm -r -f temp
fi

#Creamos el directorio temp
echo -e '\e[1;32mCreando directorio temp\e[0m'
sleep 1
mkdir temp
#Copiamos fichero1 en temp y despues lo copiamos en mpoint
echo "Copying $FICHERO1"
cp ./src/$FICHERO1 ./temp/
cp ./temp/$FICHERO1 $MPOINT/
read -p "Press enter..."
#Copiamos fichero2 en temp y despues lo copiamos en mpoint
echo "Copying $FICHERO2"
cp ./src/$FICHERO2 ./temp/
cp ./temp/$FICHERO2 $MPOINT/
read -p "Press enter..."




#APARTADO B Auditamos disco
./my-fsck-static-64 virtual-disk

read -p "Press enter..."
echo "Comparando $FICHERO1"
if diff ./temp/$FICHERO1 $MPOINT/$FICHERO1 >/dev/null; then
	echo -e '\e[1;32m$FICHERO1 IGUALES \e[0m'
else
	echo -e '\e[1;31mError en $FICHERO1\e[0m'
fi
read -p "Press enter..."
echo "Comparando $FICHERO1"
if diff ./temp/$FICHERO2 $MPOINT/$FICHERO2 >/dev/null; then
	echo -e '\e[1;32m$FICHERO2 IGUALES \e[0m'
else
	echo -e '\e[1;31mError en $FICHERO2\e[0m'
fi

read -p "Press enter..."

#truncar el primer fichero en temp y en nuestro SF para que ocupe un bloque menos

echo "Truncando $FICHERO1 en TEMP y $MPOINT"
truncate ./temp/$FICHERO1 -s 16384
truncate $MPOINT/$FICHERO1 -s 16384
#ocupaba 18kb(5bloques), lo truncamos en la medida de 4bloques*4096

read -p "Press enter..."



#APARTADO C	
	#Auditar disco
./my-fsck-static-64 virtual-disk	
	#hacer un diff entre el fichero original(de src o de temp?) y el truncado
read -p "Press enter..."
echo "Comparando $FICHERO1 de src con el de $MPOINT tras truncado"
if diff ./src/$FICHERO1 $MPOINT/$FICHERO1 >/dev/null; then #temp o src
	echo -e '\e[1;32m$FICHERO1 IGUALES \e[0m'
else
	echo -e '\e[1;31mError en $FICHERO1\e[0m'
fi
#APARTADO D
	#Copiar un tercer fichero de texto a nuestro SF
echo "Copying $FICHERO3"
cp ./src/$FICHERO3 ./temp/
cp ./temp/$FICHERO3 $MPOINT/
read -p "Press enter..."
	
#APARTADO E
	#auditar disco
./my-fsck-static-64 virtual-disk	
	#diff entre el fichero original y el truncado 
echo "Comparando $FICHERO1 tras truncado y creacion del nuevo fichero3"
if diff ./temp/$FICHERO1 $MPOINT/$FICHERO1 >/dev/null; then #temp o src fichero 1 o 3
	echo -e '\e[1;32m$FICHERO1 IGUALES \e[0m'
else
	echo -e '\e[1;31mError en $FICHERO1\e[0m'
fi
#APARTADO F
	#trunque el segundo fichero en temp y en SF haciendo que ocupe un bloque mas
echo "Truncando $FICHERO2 en TEMP y $MPOINT"
truncate ./temp/$FICHERO2 -s 16384
truncate $MPOINT/$FICHERO2 -s 16384
#ocupaba 11kb(3bloques), lo truncamos en la medida de 4bloques*4096

read -p "Press enter..."

#APARTADO G
	#auditar disco
./my-fsck-static-64 virtual-disk	
	#diff entre fichero original y truncado
echo "Comparando $FICHERO1 tras truncado y creacion del nuevo fichero3 y truncado del fichero 2"
if diff ./src/$FICHERO1 $MPOINT/$FICHERO1 >/dev/null; then #temp o src fichero 1 o 3
	echo -e '\e[1;32m$FICHERO1 IGUALES \e[0m'
else
	echo -e '\e[1;31mError en $FICHERO1\e[0m'
fi




