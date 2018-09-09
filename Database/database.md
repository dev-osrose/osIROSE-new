#Database

The osirose database is defined as such:

##Table accounts
This table holds the account information (username, password, salt, access, active, platinium...) You can create and/or delete accounts here.
The salt is used as part of the password storing. This is to avoid a rainbow attack on the stored passwords in case the table is stolen.
There are a couple of stored procedures that can be used to simplify the use of that table:
* `(int)id user_exists((char[24])user)` returns the id of the given username
* `(void) create_account((char[24])user, (char[32])pass)` creates an entry in table with the given username and password (must be in SHA2). It creates a new salt and applies it to the password before storing it in the table.

##Table characters

