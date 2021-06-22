#!/usr/bin/env python
#-------------------------------------------------------------------------------
# Copywrite Luke Titley 2015
#-------------------------------------------------------------------------------
# Runs clang-format over all the code in trace, to ensure it all conforms to the
# same coding standard.
# Coding standard is specified in .clang_format file in root trace directory.
# This should be run from the root trace directory.
# The coding standard is very similar to that of google.
#
# Below are the settings which are different to google coding standards.
#    AccessModifierOffset: -4 
#    AllowShortLoopsOnASingleLine: false  
#    DerivePointerBinding: false  
#    PointerBindsToType: false  
#    IndentWidth:     4  
#    TabWidth:        4  
#    BreakBeforeBraces: Allman

import os
import subprocess
import sys
import time

directories=["include/trace", "src/test", "src"]
render_command = ["clang-format",
                  "-style=file"]

files_to_format = [ os.path.join(i,j) for i in directories for j in os.listdir(i) if j.endswith(".cpp") or j.endswith(".h") ]

command = subprocess.Popen(render_command,
                           shell=True,
                           stdout=subprocess.PIPE,
                           stdin=subprocess.PIPE)

for f in files_to_format:
    command = subprocess.Popen(render_command,
                               stdout=subprocess.PIPE,
                               stdin=subprocess.PIPE)
    with open(f) as input_file: 
        command.stdin.write(input_file.read())
        command.stdin.close()

    with open(f, "w") as output_file:
        output_file.write(command.stdout.read())

