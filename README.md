QuickPoll
Easy and lightweight polling system through wifi and qt client
============================================================================================

by Alessandro Mauro - www.MaETech.it - alez@maetech.it (c) 2014

Released under GPLv3 license (see file 'COPYING')

============================================================================================

What is this:

Allow many users (as many as the access point let connect, usually from 20 to 250) 
to score/rate something/someone through their smartphones, and simple web page.
A QT desktop application is used to manage the whole system.
The web server must be installed in addition to the app to get the system work.

============================================================================================

Requirements:

- Php script that makes the game (not included in this repository - contact me)

- Web server with PHP support

- Wifi local lan with IPv4 class-C addresses dhcp (ip is used to identify each user) -> max 253 users

- Qt to compile project

- Qt application must have r/w access to web server folder

============================================================================================

DISCLAIMER: this system is intended for NON PROFESSIONAL, DOMESTIC use.
Since it has "game" purposes, it does not have security shields.

Qt desktop application manages the entire polling process.
- creates the index.php file in the web server directory
- the php script allows users to vote: their score is stored in a folder, one file for each user
   the filename is created from the latter number of the ipv4 address.
- the qt app polls the folder and reads the files
- when the polling is closed the qt app removes all vote files and changes the index.php

============================================================================================
Many thanks to:
http://p.yusukekamiyamane.com/
http://www.patricktaylor.com
