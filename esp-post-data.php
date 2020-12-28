<!--
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/cloud-weather-station-esp32-esp8266/

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
-->
<?php

    $servername = "localhost";

    // REPLACE with your Database name
    $dbname = "bilateks_esp";
    // REPLACE with Database user
    $username = "bilateks_jure";
    // REPLACE with Database user password
    $password = "vt5RsgCrJA3cnHY";

  // Keep this API Key value to be compatible with the ESP code provided in the project page. If you change this value, the ESP sketch needs to match
  $api_key_value = "tPmAT5Ab3j7F9";

  $api_key= $sensor = $location = $userllc = $dht_temp = $dht_humid = $supp1 "";

  if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $api_key = test_input($_POST["api_key"]);
    if($api_key == $api_key_value) {
      $sensor = test_input($_POST["sens"]);
      $location = test_input($_POST["loc"]);
      $userllc = test_input($_POST["usex"]);
      $dht_temp = test_input($_POST["temp"]);
      $dht_humid = test_input($_POST["humid"]);
      $supp1 = test_input($_POST["sup"]);

              // Create connection
        $conn = new mysqli($servername, $username, $password, $dbname);
        // Check connection
        if ($conn->connect_error) {
            die("Connection failed: " . $conn->connect_error);
        } 
        
        $sql = "INSERT INTO SensorData (sens, loc, usex, temp, humid, sup)
        VALUES ('" . $sensor . "', '" . $location . "', '" . $userllc . "', '" . $dht_temp . "', '" . $dht_humid . "', '" . $supp1 . "')";
        
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