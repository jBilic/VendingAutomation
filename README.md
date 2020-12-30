# VendingAutomation

**php folder** contains everything required for setting up the database and server as well as how to show data in web browser.

Entire project is hosted on A2Hosting.com. All tasks are to be done through cPanel interface.

## SQL database stuff

Create SQL database. Create a user and assign it to the databse. Database name, username and password have to be updated in esp-post-data.php and esp-database.php
Creata a table in this database with commands from txt file (SQL setup.txt)


## Server side stuff

These **four** files:

* _esp-post-data.php
* _esp-database.php
* _esp-vending-node.php
* _esp-style.css

need to be placed in public_html folder of server.

Their roles are:

* esp-post-data.php -> collects data from ESP32 and passes it to esp-database.php
* esp-database.php -> handles the filling of SQL base with values from esp-post-data.php. Handles all additional function (averages etc.)
* esp-vending-node.php -> displays all data in the browser
* esp-style.css -> defines styling of esp-vending-node.php


After visiting **your-website-url.com/esp-vending-node.php** you should see the sensor readings.
