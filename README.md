# Mercenaris of Astonia v2 Server for Windows

The original source code can be found in the archives directory and on the author's site https://brockhaus.org/merc2.html

Setup / Usage Notes
* Run ```godpwd password > godpassword.txt``` to create an encypted God password.  Alternately, you can edit the DEFAULT_GODPASSWORD in gendefs.h and recompile.
* Place the moav2srv.exe, badnames.txt, badwords.txt, and godpassword.txt file in a directory where the server will run.
* Place the .dat and .obj directories in the directory where the server will run, maintaining the files within these directories.  Create a .save and .tmp directory as well.  You may have to do this from the command line as Windows Explorer doesn't like names that start with a period.
* Make sure the user running the server has read/write access to the directory and subdirectories.
* Client version 2.14.06 may be used to connect to the server.
* Start the client with ```merc.exe -p 5555 -h localhost``` to connect to the local server.
* Type the unencrypted God password in the game chat to get access to all the server commands in game.
* Type /help in the game chat to see most of the available commands.  Look at the source code to see all commands available.
* Stop the server with CTRL+C or else the data files may become corrupted.  You may need to reset the GF_DIRTY flag if that happens.

Other Notes
* The mmap implementation needs some work, but seems to function for this use case
* The cgi programs need some changes to be useful in Windows
* The server hasn't been tested for running any significant length of time, but it seems to be stable.
* Due to the use of select for the network code, the server won't be able to handle more than FD_SETSIZE connections, which is typically 64 on Windows.  If you don't limit the maximum connections to less than this value, the server will probably crash when the limit is reached.
