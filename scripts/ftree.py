import os
import sys
import hashlib    #-- For md5
import subprocess #-- For exec shell commands

#----------------------------------------------------------------
#-- Get mime type of filename using 'file'
#-----------------------------------------------------------------
def get_mime_type(filename):
	return subprocess.check_output(['file', '-b', '--mime-type', filename]).rstrip('\n')

#----------------------------------------------------------------
#-- Calc th md5 sum of filename
#-----------------------------------------------------------------
def md5sum(filename):
	md5 = hashlib.md5()
	f = open(filename,'rb')
	with open(filename,'rb') as f:
		for chunk in iter(lambda: f.read(128*md5.block_size), b''):
        		md5.update(chunk)

	f.close()
	return md5.hexdigest()

#-----------------------------------------------------------------
#-- Walk on directory sub-tree from 'startdir' and print:
#-- count + md5sum + file + mime type
#-----------------------------------------------------------------
def traverse(startdir):
	#-- Check if is a valid dir.
	if not os.path.isdir(startdir):
		raise Exception(startdir + " path don't exists")

	#-- Check if is an absolute path.
	if not os.path.isabs(startdir):
		raise Exception(startdir + " is not an absolute path!")

	count = 0
	for root, dirs, files in os.walk(startdir):
		for name in files:
			fn = os.path.join(root, name)
			print str(count) + ";" + md5sum(fn) + ";\"" + fn + "\";" + get_mime_type(fn)
			count += 1;

#----------------------------------------------------------------
#-- Main
#----------------------------------------------------------------
if __name__ == "__main__":

	import argparse

	parser = argparse.ArgumentParser(description='Traverse FileSystem', formatter_class=argparse.ArgumentDefaultsHelpFormatter)
	parser.add_argument('-d', '--dir', required=False, metavar='<start_dir>', default=os.getcwd(), help=u'Start directory')
#	parser.add_argument('-o', '--out', required=False, metavar='<output_file>', default=os.getcwd(), help=u'Start directory')
	args = parser.parse_args()

	traverse(args.dir)


