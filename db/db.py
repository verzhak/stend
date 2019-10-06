
from PyQt4 import Qt, QtSql

def connect():
	
	cn = QtSql.QSqlDatabase.addDatabase("QPSQL")
	cn.setHostName("localhost")
	cn.setDatabaseName("tapsi")
	cn.setUserName("tapsi")
	cn.setPassword("liverpool")
	cn.open()
	
	return cn

def query(cn):

	return QtSql.QSqlQuery(cn)

def error(cn):

    return cn.lastError()

