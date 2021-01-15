#!/bin/bash
 
# Menu 
MPOINT=""
ARCHIVO=""
echo "Indica nombre de directorio fuse"
read MPOINT
cd ./$MPOINT
function _menu()
{
    echo ""
    echo "Selecciona una opcion:"
    echo
   echo -e '\e[1;32m1.-Listar Contenido\e[0m'
   echo -e '\e[1;32m2.-Crear enlace rigido\e[0m'
   echo -e '\e[1;32m3.-Mostrar contenido de un fichero\e[0m'
   echo -e '\e[1;32m4.-Crear nuevo fichero\e[0m'
   echo -e '\e[1;32m5.-Borrar fichero\e[0m'
    echo -e '\e[1;32mPulsa 9 para finalizar\e[0m'
    echo ""
}
 
opc=0
# bucle que no se detiene hasta pulsar el numero nueve
until [ $opc -eq 9 ]
do
    case $opc in
        1)
            echo " Listado de contenido"
            ls
            _menu
            ;;
        2)
            echo " Escriba nombre para generar enlace rigido"
            _menu
            ;;
        3)
            echo " Escriba nombre archivo para mostrar información"
            read ARCHIVO
            stat $ARCHIVO
            
            _menu
            ;;
        4)
            echo "Escribe nombre de fichero nuevo"
            read ARCHIVO
            > $ARCHIVO
            _menu
            ;;
         5)
            echo " Selecciona archivo para borrar"
            read ARCHIVO
            rm $ARCHIVO
            _menu
            ;;
        *)
            # si hemos puslado cualquier valor que no sea correcto
            _menu
            ;;
    esac
    read opc
done
