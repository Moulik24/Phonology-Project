import subprocess
import os
import argparse
from pathlib import Path

def get_fst_name(fst_filename):
    """
    :param fst_filename: Path to the .foma file that defines a Finite State Transducer.

    Returns the name of the FST, which is just the stem of its file path. For example, an input of FSTs/myfst.foma return myfst.
    """
    return Path(fst_filename).stem

def create_dot_file(fst_name, fst_filepath):
    """
    :param fst_name: The name of the Finite State Transducer.
    :param fst_filepath: Path to the .foma file that defines a Finite State Transducer.

    Makes the dot file corresponding to the FST. Puts the dot file in the current directory. 
    The dot file is in a format that graphviz can read. 
    """
    load = "load stack {}".format(fst_filepath)

    dot_filename = "{}.txt".format(fst_name)
    printDot = "print dot > {}".format(dot_filename)

    foma_commands = [load, printDot]
    argument_string = ""
    for cmd in foma_commands:
        argument_string += '-e "{}" '.format(cmd)
    print(argument_string)

    subprocess.run("foma {} -s".format(argument_string), shell=True)
    return dot_filename

def create_png(fst_name, dot_filename):
    """
    :param fst_name: The name of the Finite State Transducer.
    :param dot_filename: The full filename of the dot file that defines the Finite State Transducer.

    Makes the png of the fst from the dot file. Puts it within a subdirectory of the current directory.
    """
    FSTImageDir = "FSTImages"
    if not os.path.exists(FSTImageDir):
        print("Making directory {}...".format(FSTImageDir))
        os.mkdir(FSTImageDir)

    png_filename = "{}.png".format(fst_name)
    output_png_path = os.path.join(FSTImageDir, png_filename)

    print("Putting image of {} into {} folder...".format(fst_name, FSTImageDir))
    convertToPng = "dot -Tpng -o {} {}".format(output_png_path, dot_filename)
    subprocess.run(convertToPng, shell=True)
    print("Done!")

def clean_up(file_to_delete):
    """
    :param file_to_delete: Path to file to delete.

    Used to delete extra files made, specifically a dot file.
    """
    os.remove(file_to_delete)
    
parser = argparse.ArgumentParser()
parser.add_argument('fst_filepath',type=str,default=None)
args = parser.parse_args()

fst_filepath = args.fst_filepath
fst_name = get_fst_name(fst_filepath)
dot_filepath = create_dot_file(fst_name, fst_filepath)
create_png(fst_name, dot_filepath)
clean_up(dot_filepath)