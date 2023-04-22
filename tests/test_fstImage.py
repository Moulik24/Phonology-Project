import pytest
from fstImage import FST_Image

def test_non_existent_file():
    try:
        fst_Image = FST_Image("tests/testFiles/nonexistent.foma")
        assert False
    except ValueError as e:
        assert True
    except:
        assert False

def test_bad_file_extension():
    try:
        fst_Image = FST_Image("tests/testFiles/badFileExtension.txt")
        assert False
    except ValueError as e:
        assert True
    except:
        assert False

def test_directory():
    try:
        fst_Image = FST_Image("tests/testFiles")
        assert False
    except ValueError as e:
        assert True
    except:
        assert False

def test_create_image():
    fst_Image = FST_Image("tests/testFiles/exampleFST.foma", "tests/testFiles/testFSTImages")
    fst_Image.create_image()