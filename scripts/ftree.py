import os
import sys
import hashlib    #-- For md5
import subprocess #-- For exec shell commands

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
	count = 0
	for root, dirs, files in os.walk(startdir):
		for name in files:
			fn = os.path.join(root, name)
			mime = subprocess.check_output(['file', '-b', '--mime-type', fn])
			print str(count) + ";" + md5sum(fn) + ";\"" + fn + "\";" + mime.rstrip('\n')
			count += 1;

#----------------------------------------------------------------
#-- Main
#----------------------------------------------------------------
if __name__ == "__main__":

	try:
		cwd = sys.argv[1]
	except:
		cwd = os.getcwd()

	#-- Check if is a valid dir.
	if os.path.isdir(cwd):
		#-- Check if is an absolute path.
		if os.path.isabs(cwd):
			#-- Do the job.
			traverse(cwd)
		else:
			print cwd + " is not an absolute path!"
	else:
		print cwd + " path don't exists"

