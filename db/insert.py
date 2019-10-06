#!/usr/bin/python

import sys, hashlib, shutil
import db

def get_hash(fnames):

	md5 = hashlib.md5()

	for fname in fnames:

		with open(fname, "rb") as fl:
			
			md5.update(fl.read())
	
	return md5.hexdigest()

def get_ftp_fname(hs, ftp_dname, fnames):

	ftp_real_fnames = list(map(
		lambda fname : "%s/%s.%s" % (ftp_dname, hs, fname.split(".")[-1]),
		fnames))

	ftp_fnames = list(map(
		lambda fname : "%s.%s" % (hs, fname.split(".")[-1]),
		fnames))

	return (ftp_real_fnames, ftp_fnames)

def get_links(host, ftp_link_prefix, ftp_fnames):

	return list(map(
		lambda fname : "ftp://anonymous:@%s/%s/%s" % (host, ftp_link_prefix, fname),
		ftp_fnames))

def copy_files(fnames, ftp_real_fnames):

	for fname, ftp_real_fname in zip(fnames, ftp_real_fnames):
		
		shutil.copyfile(fname, ftp_real_fname)

cn = db.connect()
query = db.query(cn)

table_name = sys.argv[1]
name = sys.argv[2]
host = sys.argv[3]
ftp_dname = sys.argv[4]
ftp_link_prefix = sys.argv[5]
fnames = [ sys.argv[6], sys.argv[7] ]

hs = get_hash(fnames)
ftp_real_fnames, ftp_fnames = get_ftp_fname(hs, ftp_dname, fnames)
links = get_links(host, ftp_link_prefix, ftp_fnames)
copy_files(fnames, ftp_real_fnames)

try:

	if not query.exec("insert into %s values ('%s', '%s', '%s', '%s')" % (table_name, hs, name, links[0], links[1])):
		
		raise Exception

except:
		
	error = db.error(cn)
	print("[ Error ] %u %s %s %s" % (error.number(), error.databaseText(), error.driverText(), error.text()))

