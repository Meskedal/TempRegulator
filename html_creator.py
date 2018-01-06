#!/usr/bin/python

import sys, os

def main():
	file_path = 'C:/Users/Eskedal/Desktop'
	complete_name = os.path.join(file_path, "arguments.html")
	f = open(complete_name,'w+')
	argument_list = sys.argv[1:len(sys.argv)]
	arguments = ""
	for i in range(0, len(argument_list)):
		arguments += argument_list[i]
		if(i != len(argument_list)-1):
			arguments += ' '
	header = "<html> <head><p>"+arguments+"</p></head></html>"

	f.write(header)
	f.close()


if __name__ == "__main__":
    main()
	
## add path to file, use a smart setup of arguments