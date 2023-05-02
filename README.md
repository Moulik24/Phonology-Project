# Phonology-Project
This is my term project for Advanced Phonology, Ling 510, at UW-Madison. 
This project implements a read-eval-print loop (REPL), where the user enters a Japanese word in its romanized, or English, spelling, and the output suggests how that word would be pronounced. Currently, we are only considering the effects of Nasal Assimilation, which is how the sound **n** changes, and High Vowel Devoicing, which is how the sounds **i** and **u** change in certain sound environments. 
To do this, I used the **foma** C-library ([see documentation](https://code.google.com/archive/p/foma/)). foma allowed me to create [Finite State Transducers](https://en.wikipedia.org/wiki/Finite-state_transducer) just by supplying Regular Expressions. An input Japanese word is then run through a Finite State Transducer to get the output suggesting its pronunciation. 
Let's try a simple word. 散歩 (さんぽ) 'walk; stroll' has romanized spelling *sanpo.* We put this into the REPL:
```
Please enter your word: 
sanpo
Resulting word is: 
sampo
```
The **n** in the input word is an **m** in the output word. This sound-change process is known as Nasal Assimilation where **n** in this case is pronounced like **m**. This sound is made with the lips, assimilating to the following sound **p**, which is also made with the lips.

Let's try a harder example. 言語学, (げんごがく) 'linguistics' has romanized spelling *gengogaku* Let's enter this word into the REPL:
```
Please enter your word: 
gengogaku
Resulting word is: 
geŋgogaku̥
```
We see that the **n** in the input word is a **ŋ** in the output word. The **u** in the input word becomes a **u̥** in the output word. See the Background section for more information on these sounds. 

## Background
Phonology is a branch of linguistics that studies the sound systems of different languages (https://en.wikipedia.org/wiki/Phonology). It is widely accepted that the words that speakers store in their brain, their Lexicon, is in an 'underlying representation,' which is different from the 'surface representation,' which is how the word is actually pronounced. In English,  

### The Lexicon
Currently, my program allows any word as long as it is made up of the following letters:
```
a i u e o k g s z t d ts ch j dz n h f b p m y r w
```
These are the allowed letters used in the romanized spelling of Japanese.

### High Vowel Devoicing
This rule was proposed by 

### Nasal Assimilation

### Output Words
For the most part, 
Need to say what the spread output symbols as well as the eng actually mean and how they're pronounced.

## Installation
First, you will need to install foma. See the **Installation** section at https://fomafst.github.io/. 
Next, you will need a C-compiler such as **gcc**. 
**fstImage.py** will require you to install Python. However, this script is only used for viewing images of the Finite State Transducers. So, it is not necessary if you will just be using the REPL to get Japanese pronunciations.

### Mac Users
To install gcc

Python should come pre-installed. 


### Windows Users
To install gcc

To install Python, 

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
does not overwrite **.foma** files, instead reading from **.foma** files if they exist. So, if you make changes to **main.c** to modify a FST, you will not see any changes when running **./main** unless you first delete the appropriate **.foma** files.