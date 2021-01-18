<?php

    $servername = "localhost";
// REPLACE with your Database name
    $dbname = "pursahos_esp32";
// REPLACE with Database user
    $username = "pursahos_jure";
// REPLACE with Database user password
    $password = "tvzmiuzeozivot";

    $data = $_POST["value"];

// Create connection
    $conn = new mysqli($servername, $username, $password, $dbname);
// Check connection
    if ($conn->connect_error) {
      die("Connection failed: " . $conn->connect_error);
    }

    $sql = "INSERT INTO RelayData (Relay_Status) VALUES ($data)";
    $conn->query($sql);
    
    $conn->close();

?>