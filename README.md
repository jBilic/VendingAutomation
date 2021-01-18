# VendingAutomation

**php folder** contains everything required for setting up the database and server as well as how to show data in web browser.

Entire project is hosted on A2Hosting.com. All tasks are to be done through cPanel interface.

## SQL database stuff

Create SQL database. Create a user and assign it to the databse. Database name, username and password have to be updated in esp-post-data.php and esp-database.php
Creata a table in this database with commands from txt file (SQL setup.txt)


## Server side stuff

These **five** files:

* _esp-post-data.php
* _esp-database.php
* _esp-vending-node.php
* _esp-style.css
* -esp-ajax.php
need to be placed in _public_html_ folder of server.

Their roles are:

* esp-post-data.php -> collects data from ESP32 and passes it to esp-database.php
* esp-database.php -> handles the filling of SQL base with values from esp-post-data.php. Handles all additional function (averages etc.)
* esp-vending-node.php -> displays all data in the browser
* esp-style.css -> defines styling of esp-vending-node.php
* esp-ajax.php -> handles relay status from button to SQL and vice-versa


After visiting **http://purs.a2hosted.com/esp-vending-node.php** you should see the sensor readings.

## Current status

Server is up and running, all data is being transmitted to server and stored in database.
Everything can be accesed at: 
http://purs.a2hosted.com/esp-vending-node.php

There are two separate tasks running - one that handles GET and POST communication with server (Task_1), running on Core 0.
Second task watches button inputs and is running on Core 1 (Task_2).

## Remaninig tasks:

NONE as of January 18th 2021.
