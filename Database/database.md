# Database

Two files are of importance in this folder: osirose.sql and item_db.sql. The first one contains all of the schema tables to make the project working. The second file contains the data for items, extracted from the client we use.
The osirose database is defined as such:

## Table accounts
This table holds the account information (username, password, salt, access, active, platinium...) You can create and/or delete accounts here. This table cascade delete every linked character.
The salt is used as part of the password storing. This is to avoid a rainbow attack on the stored passwords in case the table is stolen.
There are a couple of stored procedures that can be used to simplify the use of that table:
* `(int)id user_exists((char[24])user)` returns the id of the given username
* `(void) create_account((char[24])user, (char[32])pass)` creates an entry in table with the given username and password (must be in SHA2). It creates a new salt and applies it to the password before storing it in the table.

## Table characters
The table characters contains all of the existing characters. It holds the stats, the name, race, level, job and everything that a character needs, except items. This table cascade delete every item, whishlist, friends.
There are a couple of stored procedures to simplify the table usage:
* `(bool) character_exists((char[24])name)` returns 1 of the character exists, else 0
* `(void) create_char((char[24])name, (int)userId, (int)race, (int)face, (int)hair, (int)stone)` creates a character, assigning it a charId and a basic set of equipment (the adventurer's weapon and the adventurer's clothes)
* `(void) delete_character((int)charId, (char[24])name)` deletes a character
* `(character row) get_character((int)charId)` gets a character (it's basically a `SELECT * FROM characters WHERE id = charId`)
* `(character rows) get_character_list((int) userId)` gets the list of characters for a given user (account)

## Table friends
The table friends holds the friend list of each character. One row is equivalent to one 1-1 relationship between two characters. That table is not yet used in the project. This table is 

## Table inventory
The inventory table holds all of the inventory for all characters. Each entry is based on an entry from the `item_db` table, but can contain changes specific to the item. This allow us to compress as much as possible the item data. For example, the refine level of an item is stored in the inventory table, but the name is not.
There are a number of stored procedures:
* `(inventory rows) get_equipped((int)charId)` returns the list of equipped items
* `(inventory rows) get_inventory((int)charId)` returns all of the inventory of the character

## Table wishlist
The wishlist table holds the wishlist information for each character. The table is similar to the inventory table.

## Table item_db
This table is populated with the data extracted from the client. The script column contains a lua script that will be executed on different occasions. Please refer to [RoseCommon](../src/rosecommon/rosecommon.md) for further information, including the lua API.

## Tabke list_clan
This table holds all of the created clans and their information.

## Table party
This table holds all of the information about current parties, in the even that the server goes down, the party can be restored (not currently implemented).

## Table sessions
This table holds the unique session token per logged-in account. This token exists to make sure that the account isn't logged-in more than once. It also contains some metadata for the node server. This token is deleted when the client disconnects or in the event of an outage.

## Table skill
This table holds all of the skills for each character and their level.

## Table skill_db
This table holds all of the information regarding each skill.

## Table storage
This table holds the storage items for each character.
