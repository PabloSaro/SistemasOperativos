#!/bin/bash

MPOINT="./mount-point"

rm -R -f test
mkdir test

echo 'file ' > ./test/file1.txt
echo "Copying file 1"
cp ./test/file1.txt $MPOINT/
read -p "Press enter..."

echo "Creating file 2"
cp src/MyFileSystem.c $MPOINT/
read -p "Press enter..."

echo "Creating file 3"
cp src/myFS.c $MPOINT/
read -p "Press enter..."

ls $MPOINT -la
read -p "Press enter..."



