# Phonology-Project
Term project for Advanced Phonology, Ling 510, at UW-Madison.

## Getting an Image of the FST
First, your FST must be in a binary file with file extension **.foma**. Running the Makefile will automatically generate some FSTs. To get a png image of a certain FST, you can run:
```
python fstImage.py myFST.foma
```
This will run some commands in the **foma** interpreter to generate a png image of the FST. A directory called **FSTImages** in your current directory is automatically created, and **myFST.png** is put into this directory. You can optionally specify a different directory that you want the image to be put into, like so:
```
python fstImage.py myFST.foma myDirectory
```
If **myDirectory** does not already exist, it is created. 
Also, 
```
python fstImage.py -h
```
provides more information about the arguments that **fstImage.py** accepts.

## Some Development Details
### Makefile
Currently,
```
make clean
```
will remove a hardcoded set of **.foma** files. Running:
```
./main
```
does not overwrite **.foma** files, instead reading from **.foma** files if they exists. So, if you make changes to **main.c** to modify a FST, you will not see any changes when running **./main** unless you first delete the appropriate **.foma** files.