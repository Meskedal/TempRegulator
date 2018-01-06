#!/usr/bin/python

import sys, os

def main():
	file_path = '/var/www/html/wp-content/themes/twentyseventeen'
	complete_name = os.path.join(file_path, "arguments.php")
	f = open(complete_name,'w+')
	argument_list = sys.argv[1:len(sys.argv)]
	arguments = ""
	for i in range(0, len(argument_list)):
		arguments += argument_list[i]
		if(i != len(argument_list)-1):
			arguments+= ' '
	template = "<?php /* Template Name: Custom-Page */ ?>"
	#header = "\n<html> <head><p>"+arguments+"</p></head></html>"
	header = arguments
	f.write(template)
	f.write(header)
	f.close()


if __name__ == "__main__":
    main()
	
## add path to file, use a smart setup of arguments