import os
import platform
import shutil
import re
import sys

from distutils.core import setup
from distutils.extension import Extension

# The preprocessor macros to define. This macro below is required for MSVC building on version 1.67 and below, but it does no harm for other systems
macros = [("BOOST_ALL_NO_LIB", None)]

# Example: libboost_python37-vc141-mt-gd-x64-1_67.lib - This would be python 3.7, built with MSVC 141, multithreading enabled, debugging, x86 arch 64-bit, boost version 1.67, static linkage.
# Dynamically linked versions do not start with lib
# Need argparse for 32- vs 64-bit as well as debugging or not and other flags. Currently selects the python version TODO
# Also shared vs static (in this case, you need the  macro) needed (dll)
WINDOWS_PREFERRED_LIB_REGEX = re.compile(r"(lib)?boost_python{}{}-[^-]+-(mt-)?(s?g?y?d?p?-)?(x|a|i|s|m|p){}-[_|0-9]+\.lib".format(sys.version_info.major, sys.version_info.minor, 64 if sys.maxsize == 0x7fffffffffffffff else 32))

if platform.system() == "Darwin":
    library_name = f"boost_python{sys.version_info.major}{sys.version_info.minor}"
elif platform.system() == "Windows":
    # Tries to find bjam (which would exist if boost has been built)
    bjam_exe_location = shutil.which("bjam")

    boost_install_dir = None
    if bjam_exe_location is not None:
        boost_install_dir = os.path.dirname(bjam_exe_location)
    else:
        try:
            # Looks for the most likely install location otherwise TODO add custom root
            boost_versions = os.listdir(os.path.expandvars("$SYSTEM_DRIVE\\Program Files\\boost"))
            # Iterates backwards so latest is selected first
            for i in range(len(boost_versions)-1, -1, -1):
                if os.path.exists(os.path.join(os.path.expandvars("$SYSTEM_DRIVE\\Program Files\\boost"), boost_versions[i], "bjam.exe")):
                    boost_install_dir = os.path.join(os.path.expandvars("$SYSTEM_DRIVE\\Program Files\\boost"), boost_versions[i])
                    break

            if boost_install_dir is None:
                raise Exception(os.path.expandvars("Could not determine boost installation location. Add the boost root path (with bjam.exe) to PATH or move it to $SYSTEM_DRIVE\\Program Files\\boost"))
        except FileNotFoundError:
            raise Exception(os.path.expandvars("Could not determine boost installation location. Add the boost root path (with bjam.exe) to PATH or move it to $SYSTEM_DRIVE\\Program Files\\boost"))

    # Adds to the necessary environment variables (just in case they were not already set) so boost's libraries and headers are included
    os.environ["LIB"] = f"{boost_install_dir}\\stage\\lib;" + str(os.environ.get("LIB"))
    os.environ["INCLUDE"] = boost_install_dir + ";" + str(os.environ.get("INCLUDE"))

    possible_libraries = list(filter(WINDOWS_PREFERRED_LIB_REGEX.match, os.listdir(f"{boost_install_dir}\\stage\\lib")))
    if not possible_libraries:
        raise Exception(f"Could not find a matching libboost_python library. Expected libboost_python{sys.version_info.major}{sys.version_info.minor} {64 if sys.maxsize == 0x7fffffffffffffff else 32}-bit library")

    # Chooses the last one, as this is probably the most recent TODO
    nl = "\n"
    print(f"Libraries found:\n{nl.join(possible_libraries)}\nChoosing \"{possible_libraries[-1]}\"")

    # Have to get rid of the .lib extension. Also chooses the last (latest) one
    library_name = possible_libraries[-1][:-4]

    if library_name.startswith("lib"):
        # If it's statically linked, this preprocessor flag is needed
        macros.append(("BOOST_PYTHON_STATIC_LIB", None))
elif platform.system() == "Linux":
    # Tested on Ubuntu Bionic
    library_name = f"boost_python-py{sys.version_info.major}{sys.version_info.minor}"

setup(name="AncientGrammar",
      ext_modules=[
          Extension("AncientGrammar",
                    ["../src/verb.cpp", "../src/utils.cpp", "wrapper.cpp", "../src/utf8proc/utf8proc.c"],
                    libraries=[library_name], define_macros=macros)
      ])
