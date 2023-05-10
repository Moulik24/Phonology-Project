# Phonology-Project
This is my term project for Advanced Phonology, Ling 510, at UW-Madison. <br>
This project implements a read-eval-print loop (REPL), where the user enters a Japanese word in its romanized, or English, spelling, and the output suggests how that word would be pronounced. Currently, we are only considering the effects of Nasal Assimilation, which is how the sound **n** changes, and High Vowel Devoicing, which is how the sounds **i** and **u** change in certain sound environments. <br>
To do this, I used the **foma** C-library ([see documentation](https://code.google.com/archive/p/foma/)). foma allowed me to create [Finite State Transducers](https://en.wikipedia.org/wiki/Finite-state_transducer) just by supplying Regular Expressions. An input Japanese word is then run through a Finite State Transducer to get the output suggesting its pronunciation. See the [foma github page](https://fomafst.github.io/) for more information.<br><br>

Let's try a simple word. 散歩 (さんぽ) 'walk; stroll' has romanized spelling *sanpo.* We put this into the REPL:
```
Please enter your word: 
sanpo
Resulting word is: 
sampo
```
The **n** in the input word is an **m** in the output word. This sound-change process is known as Nasal Assimilation where **n** in this case is pronounced like **m**. This sound is made with the lips, assimilating to the following sound **p**, which is also made with the lips. <br>

Let's try a harder example. 言語学, (げんごがく) 'linguistics' has romanized spelling *gengogaku.* Let's enter this word into the REPL:
```
Please enter your word: 
gengogaku
Resulting word is: 
geŋgogaku̥
```
We see that the **n** in the input word is a **ŋ** in the output word. The **u** in the input word becomes a **u̥** in the output word. See the Background section for more information on these sounds. 

## Background
[Phonology](https://en.wikipedia.org/wiki/Phonology) is a branch of linguistics that studies the sound systems of human language. It is widely accepted that the words that speakers store in their brain, their Lexicon, is in an 'underlying representation,' which is different from the 'surface representation,' which is how the word is actually pronounced. This project considers the underlying sounds **i**, **u**, and **n**, which are pronounced differently in their surface forms.

### The Lexicon
Currently, the REPL allows any word as long as it is made up of the following letters:
```
a i u e o k g s z t d ts ch j dz n h f b p m y r w
```
These are the allowed letters used in the romanized spelling of Japanese. Note that for long vowel sounds, some romanized spelling systems use a macron, such as for *kyū* 'nine'. This can also be written as *kyuu* which this REPL will accept, where we type the same vowel twice for a long vowel. 

### High Vowel Devoicing
There are two parts to High Vowel Devoicing in Japanese (source: Japanese: Revised Edition by Shoichi Iwasaki). <br>
A short high vowel is devoiced: <br>
* between two voiceless consonants <br>
* between a voiceless consonant and a pause <br>

In the code, this is implemented as:
* i -> i̥ between any pairs of sounds from k, s, t, ts, ch, h, f, p. The pause in this case is the end of a word. <br>
* u -> u̥ in the same environment as above. <br>

Here is a [Youtube video](https://www.youtube.com/watch?v=ulhd9fl2Gug&ab_channel=CampanasdeJapanese) with more information on High Vowel Devoicing and its pronunciation.

### Nasal Assimilation
There are three parts to Nasal Assimilation in Japanese (source: Introductory Phonology by Bruce Hayes, Ch. 3, Exercise 3). <br>
* n assimilates to the Place of Articulation of the following consonant. <br>
* n becomes ŋ word finally. <br>
* n becomes a glide before a glide. <br>

In the code, this is implemented as:
* n -> m before p,m,b. <br>
* n -> ŋ before k,g,.#. (end of word symbol) <br>
* n -> w̃ before w. <br>
* n -> ȷ̃ before j. <br>

Here's a quick [Youtube video](https://www.youtube.com/watch?v=xpzpbuFHVVU&ab_channel=Dogen) on just the pronunciation of **ŋ**. <br>
The ~ symbol represents nasalization, so **w̃** is pronounced like **w** with some nasal quality. 

### Output Words
The output words have all the same letters as the Lexicon, with the following additions, which we explained in the above two sections:
```
i̥ u̥ w̃ ȷ̃ ŋ
```

## Installation
Note that **fstImage.py** will require you to install Python. However, this script is only used for viewing images of the Finite State Transducers. So, it is not necessary if you will just be using the REPL to get Japanese pronunciations. <br>
Additionally, **fstImage.py** only currently uses modules from the standard library, so you shouldn't have to install any further modules in order to run it. For development, I used **pytest**, which does need to be installed in order to run [tests/test_fstImage.py](tests/test_fstImage.py). 

### The Minimum
If you don't want to have all the code on your local computer, or are finding certain things tricky to install, you can just download [JapaneseFST.foma](JapaneseFST.foma) from this github page. You will still need to install **foma**, see the below sections on how to do so, or look at [foma's github page](https://fomafst.github.io/) if you want to try following their instructions. <br>
Once you have foma installed, open your Terminal, and type in:
```
foma
```
to run foma. At the prompt, **foma[0]:**, you can load in **JapaneseFST.foma** with: 
```
foma[0]: load stack JapaneseFST.foma
```
This will put the Finite State Transducer on the stack, which means that you can run:
```
foma[1]: down
```
to open a prompt to give the FST an input and get a corresponding output. This is pretty much the same functionality as the REPL that I implemented. The differences are that my REPL will write out all the phonological rules files so that you can run **fstImage.py** on any one of them. Additionally, running **./main** to execute my REPL involves fewer steps compared to launching foma and loading in your FST. Lastly, one thing to be aware of is that if foma outputs 
```
???
```
for an input word, this means that the input word is not in the [Lexicon](#the-lexicon).

### Mac Users
First, install [Command Line Tools](https://developer.apple.com/forums/thread/13781#:~:text=They%20allow%20programmers%20to%20compile,prompt%20you%20to%20install%20them.) which will allow to do compile programs among other tasks. You can do so by typing: 
```
xcode-select --install
```
Next, install the package manager Homebrew. To do so, open your [Terminal](https://support.apple.com/guide/terminal/open-or-quit-terminal-apd5265185d-f365-44cb-8b09-71a064a42125/mac#:~:text=Terminal%20for%20me-,Open%20Terminal,%2C%20then%20double%2Dclick%20Terminal.). At the prompt, type in:
```
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```
Once it finishes, we'll install **gcc**, a compiler to compile C-code. Type in:
```
brew install gcc
```
To install foma, type in: 
```
brew install foma
```
Finally, you can get this repository's code onto your local computer by using [Git](https://git-scm.com/). Most Macs come with git pre-installed. Type:
```
git version
```
to verify that it is installed. If it is not, type:
```
brew install git
```
Python should also come pre-installed on your Mac. <br>
Now, see [Downloading and Running the Code](#downloading-and-running-the-code).

Here are some other articles I used: <br>
* [What does Homebrew do and how do you install it?](https://brew.sh/) <br>
* [How to install gcc on Mac](https://osxdaily.com/2023/05/02/how-install-gcc-mac/) <br>
* [install foma with brew](https://formulae.brew.sh/formula/foma) <br>
 
### Windows Users
I would recommend using WSL, Windows Subsystem for Linux, see the [installation notes](https://learn.microsoft.com/en-us/windows/wsl/install). <br>
After installation, open wsl from your search bar. To install gcc, type: 
```
sudo apt update & sudo apt upgrade
```
followed by 
```
sudo apt-get install gcc
```
See [here](https://stackoverflow.com/questions/62215963/how-to-install-gcc-and-gdb-for-wslwindows-subsytem-for-linux) and [here](https://paperbun.org/how-to-install-linux-with-c-compiler-in-windows-sd3ktpoltpbo/) for more information or if this does not work. <br>

To install Python, type:
```
sudo apt install python3 python3-pip
```
To install Git, type:
```
sudo apt-get install git
```
Now, see [Downloading and Running the Code](#downloading-and-running-the-code).

## Downloading and Running the code
To download the code on this repository, for both Mac and WSL for Windows, open your terminal and navigate to a folder of your choice. Type 
```
git clone https://github.com/Moulik24/Phonology-Project.git
```
to create a new sub-folder where all the code from this repository will be downloaded.
Go into the newly created folder in your terminal, and type: 
```
make
```
to create a file called **main**. This is a file that when run, opens up the REPL. Type:
```
./main
```
to run the file! <br>
See below if you are curious about getting images of the created FSTs.

## Getting an Image of the FST
First, your FST must be in a binary file with file extension **.foma**. Running the Makefile will automatically generate some FSTs. To get a png image of a certain FST, for example **JapaneseFST.foma**, you can run:
```
python fstImage.py JapaneseFST.foma
```
This will run some commands in the **foma** interpreter to generate a png image of the FST. A directory called **FSTImages** in your current directory is automatically created, and **JapaneseFST.png** is put into this directory. You can optionally specify a different directory that you want the image to be put into, for example **myDirectory**, like so:
```
python fstImage.py JapaneseFST.foma myDirectory
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