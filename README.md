# ghost
our ghost project involving stuff

CUPID is the c++ daemon running on the server that does the matching and issues the orders.
FARMER is the chrome extension that farms for data.


HOW TO BUILD & RUN:
* use Eclipse CDT
* add these libraries:
	mysqlcppconn/sqlapi 
	curl
	dl
* add these library paths:
	"${workspace_loc:/${ProjName}}/api/sqlapi/lib/amd64"
* install 
	oracle-instant-client-11.2
	oracle-instant-client-devel-11.2
* add this to Run/Debug Settings:
	Environment: LD_LIBRARY_PATH = /usr/lib64/oracle/11.2.0.3.0/client/lib (the path where oracle-instant-client-devel installed the libs)
	
