import subprocess
import os
import argparse

def create_dot_file(fst_foma_filename):
    """
    Makes the dot file defining the fst. Puts the dot file in the current directory.
    """
    fst_foma_file_basename = os.path.basename(fst_foma_filename)
    load = "load stack {}".format(fst_foma_filename)

    dot_file_path = "{}.txt".format(fst_foma_file_basename)
    printDot = "print dot > {}".format(dot_file_path)

    foma_commands = [load, printDot]
    argument_string = ""
    for cmd in foma_commands:
        argument_string += '-e "{}" '.format(cmd)
    print(argument_string)

    subprocess.run("foma {} -s".format(argument_string), shell=True)
    return dot_file_path

def create_png(dot_file_name):
    """
    Makes the png of the fst from the dot file. Puts it within a subdirectory of the current directory.
    """
    FSTImageDir = "FSTImages"
    if not os.path.exists(FSTImageDir):
        print("Making directory {}...".format(FSTImageDir))
        os.mkdir(FSTImageDir)
    output_png_path = os.path.join(FSTImageDir, dot_filename)
    fst_name = os.path.basename(dot_file_name)
    print("Putting {} into {}".format(fst_name, FSTImageDir))
    convertToPng = "dot -Tpng -o {} {}".format(output_png_path, dot_file_name)
    subprocess.run(convertToPng, shell=True)

def clean_up(file_to_delete):
    """
    Used to delete extra files made, specifically a dot file.
    """
    os.remove(file_to_delete)
    
parser = argparse.ArgumentParser()
parser.add_argument('fst_foma_filename',type=str,default=None)
args = parser.parse_args()
dot_filename = create_dot_file(args.fst_foma_filename)
create_png(dot_filename)
clean_up(dot_filename)