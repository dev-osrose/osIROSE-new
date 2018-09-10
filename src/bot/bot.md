# Integration testing
To be able to test complex interactions between all servers and the client, it was necessary to build an automated tool.
This tool is a basic tcp client that just connects to the local server and tries to identificates with the following credentials: test/test.
If there is an account with such username/password, it then tries to select the first character and connect to the map server if there is one.

All of the code was poorly written in a single cpp file as a proof of concept mostly (also because I wanted to test that workflow without firing up the client all the time).
Following is a list of ideas to improve the tool:
* make a user inteface so we can inspect packets, and all of the stored information.
* clean up the codebase
* improve the tool to support more than just login into the map server (for example, displaying the received packets about monsters, npcs and users)
* improve the tool to be able to send packets from the user interface (for example sending a chat packet)
