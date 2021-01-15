make clean
echo -e '\e[1;32mCompilando...\e[0m'
make 
echo Borrando mount-point
rm -r -f mount-point
sleep 2
echo Creando mount-point
mkdir mount-point
sleep 1
echo Creando SF
./fs-fuse -t 2097152 -a virtual-disk -f "-d -s mount-point"
