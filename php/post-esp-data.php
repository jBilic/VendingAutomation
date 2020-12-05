<?php

/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-esp8266-mysql-database-php/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

$servername = "localhost";

// UNESI PODATKE BAZE
$dbname = "xxxxx";
// UNESI USERNAME KORISNIKA
$username = "xxxxx";
// UNESI PASSWORD KORISNIKA
$password = "xxxxx";


// Keep this API Key value to be compatible with the ESP32 code provided in the project page. 
// If you change this value, the ESP32 sketch needs to match
$api_key_value = "tPmAT5Ab3j7F9";

$api_key= $devId = $locationId = $userId = $temp = $humidity = $supp1 = $supp2 = $supp3 = $supp4 = "";

if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $api_key = test_input($_POST["api_key"]);
    if($api_key == $api_key_value) {
        $devId = test_input($_POST["devId"]);
        $locationId = test_input($_POST["locationId"]);
        $userId = test_input($_POST["userId"]);
        $temp = test_input($_POST["temp"]);
        $humidity = test_input($_POST["humidity"]);
        $supp1 = test_input($_POST["supp1"]);
        $supp2 = test_input($_POST["supp2"]);
        $supp3 = test_input($_POST["supp3"]);
        $supp4 = test_input($_POST["supp4"]);
        
        // Create connection
        $conn = new mysqli($servername, $username, $password, $dbname);
        // Check connection
        if ($conn->connect_error) {
            die("Connection failed: " . $conn->connect_error);
        } 
        
        $sql = "INSERT INTO SensorData (devId, locationId, userId, temp, humidity, supp1, supp2, supp3, supp4)
        VALUES ('" . $devId . "', '" . $locationId . "', '" . $userId . "', '" . $temp . "', '" . $humidity . "', '" . $supp1 . "', '" . $supp2 . "', '" . $supp3 . "', '" . $supp4 . "')";
        
        if ($conn->query($sql) === TRUE) {
            echo "New record created successfully";
        } 
        else {
            echo "Error: " . $sql . "<br>" . $conn->error;
        }
    
        $conn->close();
    }
    else {
        echo "Wrong API Key provided.";
    }

}
else {
    echo "No data posted with HTTP POST.";
}

function test_input($data) {
    $data = trim($data);
    $data = stripslashes($data);
    $data = htmlspecialchars($data);
    return $data;
}
