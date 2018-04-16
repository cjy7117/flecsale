#!/usr/bin/python
import getpass
print(getpass.getuser())


import glob
output_file = open('bee_scalability_test_parsed.output','w')
output_file.write("Number of nodes, Process per node, Execution time(s)\n")
path = "bee_scalability_test_*_.output"

for filename in sorted(glob.glob(path)):
    parsed_filename = filename.split("_")
    num_of_node = parsed_filename[len(parsed_filename)-3]
    proc_per_node = parsed_filename[len(parsed_filename)-2]
    with open(filename) as f:
        lines = f.readlines()
        for line in lines:
            if "Elapsed wall time" in line:
                words = line.split(" ")
                word = words[len(words) - 1]
                output_file.write("%13d,%15d,%15f\n" % (int(num_of_node), int(proc_per_node), float(word[0:len(word) - 3])))
output_file.close()
