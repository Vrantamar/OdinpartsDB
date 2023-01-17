# OdinpartsDB
This is a simulation of a fictional MySQL database for an e-commerce site that provides a platform to sell and buy car parts.
Each user has to insert personal data such as DoB, Name, Surname etc; at the end of the registration process, a timestap will be generatd and added to the user data.
Each user has to add a payment option, in the form of a credit card number, cvv, expiration date and so on that will be stored for further use.
Each user can be a buyer, a seller or both (like eBay), and if he/she is a seller, it will have a rating.
Each item has various descriptions and a list of models it is compatible with, and each order will store data like order ID, shipment id, date of shipment, date of arrival, etc.
It is strongly recommended to take a view of the database to understand it's structure, as this readme covers only the most basic overview of the situation at best.
The language used for the naming of tables and some attributes is Italian, so I would recommend any non-proficient in this language to have a dictionary by their side. An EER Diagram (in Italian) will be included to clarify the situation and give better context, although some errors may be present as the EER diagram was being updated during the numerous modifications during the DB implementation.

This project was part of a university course evaluation, but I will drop by occasionally to check if questions or issues arise.

This is intended mostly as a reference or a DB to play around with for new students.

To generate the DB, simply run the masterfile in your MySQL environment.

### Update Jan 2023
Project has been selected to be translated for use in a ORACLE PDB, coupled with development of ODPI-C applicatives to operate transactions on the DB.
A docker container running an Oracle database, is now required to run the code successfully. Bear in mind that since you may have configured your docker container to run with different networking options, the connection string `#define CONN_STRING "172.17.0.2:1521/XEPDB1"` might be different.
Also, in order to run the code, Oracle Instant Client must be installed on the Linux system. In addition, the odpi-c library must be compiled.
Furthermore, it is required to export the value of the shell variable `LD_LIBRARY_PATH` appending the location of the builded library and the instant client install location.
For example:
`export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/path_to_folder/odpi-main/lib:/opt/oracle/instantclient_21_8`

To compile the code, simply edit the following commands to suit your needs:

`cd /path_to_build/build`

`gcc -o client ../src/client.c -I/path_to_folder/odpi-main/include -L/path_to_folder/odpi-main/lib -lodpic`

#### Changelog
14-Jan-2023: client executable can successfully connect to docker instance of an Oracle21XE database.

--Akvarion
