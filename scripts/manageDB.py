# --create       Create the DB
# --recreate     Drop tables ad recreate the DB
# --insert_csv   Insert data from csv
# --backup_sql   backup data (sql)
# --backup_csv   backup data (sql)
# http://zetcode.com/db/sqlitepythontutorial/

import sqlite3
import csv
import os
import sys

tbl_name = "media"
tbl_def  = '''
(
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	md5 TEXT,
	filepath TEXT UNIQUE,
	mimetype TEXT
);
'''

def table_insert_query(table_name):
	return str("INSERT INTO " + table_name + " VALUES (?,?,?,?)")

#----------------------------------------------------------------
#-- Drop table
#----------------------------------------------------------------
def database_drop_table(connection, table_name):

	print "DROP TABLE '" + table_name + "'"
	try:
		c = connection.cursor()
		c.execute("DROP TABLE IF EXISTS " + table_name + ";")
		connection.commit()
		return True

	except sqlite3.Error, e:
		print "SQLite Error %s:" % e.args[0]
		print "SQL was: " + tbl_drop.strip('\n')
		return False

#----------------------------------------------------------------
#-- Create table
#----------------------------------------------------------------
def database_create_table(connection, table_name):

	print "CREATE TABLE '" + table_name + "'"

	query = "CREATE TABLE " + table_name + tbl_def
	try:
		c = connection.cursor()
		c.execute(query)
		connection.commit()
		return True

	except sqlite3.Error, e:
		print "SQLite Error %s:" % e.args[0]
		print "SQL was: " + query.strip('\n')
		return False

#----------------------------------------------------------------
#-- Import .csv file
#----------------------------------------------------------------
def database_import_csv(connection, table_name, csvfile):

	if not os.path.isfile(csvfile):
		print "Import Error No such file or directory: '%s'" % csvfile
		return False

	print "Import data from '" + csvfile + "'"

	table_copy = table_name + "_copy"
	database_drop_table(connection, table_copy)
	if not database_create_table(connection, table_copy):
		return False

	try:
		c = connection.cursor()

		#-- Load the CSV file into CSV reader
		fd = open(csvfile, 'rb')
		reader = csv.reader(fd, delimiter=';', quotechar='"')

		#-- Iterate through the CSV reader, inserting values into the database
		for t in reader:
			c.execute(table_insert_query(table_copy), t)

		#-- Close the csv file, commit changes, and close the connection
		fd.close()

		connection.commit()

		query = "INSERT OR IGNORE INTO " + table_name + " SELECT * FROM " + table_copy
		connection.execute(query)
		database_drop_table(connection, table_copy)
		connection.commit()

		return True

	except sqlite3.Error, e:
		database_drop_table(connection, table_copy)
		print "Import Error %s:" % e.args[0]
		return False


#----------------------------------------------------------------
#-- Check if 'filename' is a SQLite database
#--
#-- returns True if filename is a SQLite database
#--
#-- File is database if:
#--	(1) file exists,
#-- (2) length is non-zero,
#-- (3) can connect,
#-- (4) has sqlite_master table
#--
#----------------------------------------------------------------
def isSQLite(filename):
	#-- Validate file exists
	if not os.path.isfile(filename):
		return False

	#-- Is not an empty file
	if not os.stat(filename).st_size:
		return False

	#-- Can open a connection
	try:
		conn = sqlite3.connect(filename)
	except:
		return False

	#-- Has sqlite_master table
	try:
		result = conn.execute('pragma table_info(sqlite_master)').fetchall()
		if len(result) == 0:
			conn.close()
			return False
	except:
		conn.close()
		return False

	#-- Looks like a good database
	conn.close()
	return True


# Convert file existing_db.db to SQL dump file dump.sql
def database_backup_to_file(dbfile):
	import time

	bak_file = os.path.join(os.getcwd(),
			   os.path.basename(dbfile) + time.strftime(".%Y%m%d-%H%M") + ".sql")

	if os.path.exists(bak_file):
		print "DB Backup:" + bak_file + "Already exists"
		return -1

	conn = sqlite3.connect(dbfile)
	full_dump = os.linesep.join(conn.iterdump())
	f = open(bak_file, 'w')
	f.writelines(full_dump)
	f.close()

	conn.close()
	return 0

#----------------------------------------------------------------
#-- Main
#----------------------------------------------------------------
if __name__ == "__main__":
	import argparse

	parser = argparse.ArgumentParser(description='Traverse FileSystem', formatter_class=argparse.ArgumentDefaultsHelpFormatter)
	parser.add_argument('--backup', action='store_true', default=False, help=u'Export DB data')
	parser.add_argument('--drop',   action='store_true', default=False, help=u'Drop table if exists')
	parser.add_argument('--create', action='store_true', default=False, help=u'Create table')
	parser.add_argument('--insert', metavar='<csv_file>', help=u'Import CSV data')
	parser.add_argument('dbfile', metavar='<db_file>')
	args = parser.parse_args()

	if not os.path.exists(args.dbfile):
		if not args.create:
			print "Database \'" + args.dbfile + "\' not exists and --create flag is not set!";
			sys.exit(-1)
	else:
		if not isSQLite(args.dbfile):
			print args.dbfile + " is not a valid SQLite database";
			sys.exit(-1)

	#-- backup and exit
	if args.backup:
		sys.exit(database_backup_to_file(args.dbfile))


	#--
	conn = sqlite3.connect(args.dbfile)

	if args.drop:
		if not database_drop_table(conn, tbl_name):
			conn.close()
			sys.exit(-1)

	if args.create:
		if not database_create_table(conn, tbl_name):
			conn.close()
			sys.exit(-1)

	if args.insert != None:
		database_import_csv(conn, tbl_name, args.insert)

	conn.close()
