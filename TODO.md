* make list of nodes using a default scenario
* create tree for default scenario
* create nodes required
* load/execute scenario

scenario would be probably:
* login with username/pass
* if fail, try again (creating the account)
* if fail again, stop and error out
* otherwise, get character list
* if list is not empty, pick first character
* else create character with name and pick that one
* send message when on the map
