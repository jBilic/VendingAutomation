# VendingAutomation

PHP folder contains everything required for setting up the database and server as well as how to show data in web browser.
Create SQL database. Create a user and assign it to the databse. Database name, username and password have to be updated in esp-post-data.php and esp-database.php
Creata a table in this database with commands from txt file (SQL setup.txt)



These four files:

esp-post-data.php
esp-database.php
esp-vending-node.php
esp-style.css

need to be placed in public_html folder of server.

Their roles are:

esp-post-data.php -> collects data from ESP32 and passes it to esp-database.php
esp-database.php -> handles the filling of SQL base with values from esp-post-data.php. Handles all additional function (averages etc.)
esp-vending-node.php -> displays all data in the browser
esp-style.css -> defines styling of esp-vending-node.php

After visiting your-website-url.com/esp-vending-node.php
you should see the sensor readings.
