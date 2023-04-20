import subprocess
import os
import argparse
from pathlib import Path

class FST_Image:
    def __init__(self, FST_path, output_image_directory):
        self.FST_path = FST_path
        self.FST_name = self.__get_fst_name()
        self.output_image_directory = output_image_directory
        self.png_path = ""

    def __FST_file_exists(self):
        """
        Check if the FST file given exists or not.
        """
        if not os.path.exists(self.FST_path):
            return False        
        return True
    
    def __get_fst_name(self):
        """
        Returns the name of the FST, which is just the stem of its file path. For example, an input of FSTs/myfst.foma return myfst.
        """
        return Path(self.FST_path).stem
    
    def __create_dot_file(self):
        """
        Makes the dot file corresponding to the FST. Puts the dot file in the current directory. 
        The dot file is in a format that graphviz can read. 
        """
        load = "load stack {}".format(self.FST_path)
        dot_filename = "{}.txt".format(self.FST_name)
        printDot = "print dot > {}".format(dot_filename)

        foma_commands = [load, printDot]
        argument_string = ""
        for cmd in foma_commands:
            argument_string += '-e "{}" '.format(cmd)
        output = subprocess.check_output("foma {} -s".format(argument_string),
                                stderr=subprocess.STDOUT,
                                shell=True
        ).decode('utf-8') 
        print(output)
        return dot_filename
    
    def __create_png(self, dot_filepath):
        """
        :param dot_filename: The full filename of the dot file that defines the Finite State Transducer.

        Makes the png of the Finite State Transducer from the dot file. Puts it into the given output_directory.
        """
        if not os.path.exists(self.output_image_directory):
            print("Making directory {}...".format(self.output_image_directory))
            os.mkdir(self.output_image_directory)

        png_filename = "{}.png".format(self.FST_name)
        output_png_path = os.path.join(self.output_image_directory, png_filename)

        print("Putting image of {} into {} directory...".format(self.FST_name, self.output_image_directory))
        convertToPng = "dot -Tpng -o {} {}".format(output_png_path, dot_filepath)

        output = subprocess.check_output(convertToPng, 
                            shell=True).decode('utf-8')
        print(output)
        print("Done!")

    def __clean_up(self, file_to_delete):
        """
        :param file_to_delete: Path to file to delete.

        Used to delete extra files made, specifically a dot file.
        """
        if(os.path.exists(file_to_delete)):
            os.remove(file_to_delete)
    
    def create_image(self):
        """
        :param fst_filepath: Path to foma file that describes a Finite State Transducer.

        Creates a png of the Finite State Transducer and puts it in the given output directory. Creates the directory if it does not exist.
        """
        if not self.__FST_file_exists():
            print("Sorry, couldn't find {}. Please try again.".format(self.FST_path))
            return 
        dot_filepath = self.__create_dot_file()
        self.__create_png(dot_filepath)
        self.__clean_up(dot_filepath)

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('FST_path',type=str,default=None,help="The path to the .foma file binary of your Finite State Transducer.")
    parser.add_argument('output_image_directory',type=str,nargs='?',default="FSTImages",
                        help="The folder you want the png image of the Finite State Transducer to be put into. By default, the folder is called FSTImages, and is created if it doesn't exist.")
    args = parser.parse_args()

    fst_Image = FST_Image(args.FST_path, args.output_image_directory)
    fst_Image.create_image()
