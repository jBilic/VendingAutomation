v<?php
include_once('esp-database.php');
  
$servername = "localhost";

// REPLACE with your Database name
$dbname = "pursahos_esp32";
// REPLACE with Database user
$username = "pursahos_jure";
// REPLACE with Database user password
$password = "tvzmiuzeozivot";

// Keep this API Key value to be compatible with the ESP32 code provided in the project page. 
// If you change this value, the ESP32 sketch needs to match
$api_key_value = "tPmAT5Ab3j7F9";

$api_key= $sens = $loc = $usex = $temp = $humid = $coffee = $cup = "";

  if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $api_key = test_input($_POST["api_key"]);
    if($api_key == $api_key_value) {
      $sens = test_input($_POST["sens"]);
      $loc = test_input($_POST["loc"]);
      $usex = test_input($_POST["usex"]);
      $temp = test_input($_POST["temp"]);
      $humid = test_input($_POST["humid"]);
      $coffee = test_input($_POST["coffee"]);
      $cup = test_input($_POST["cup"]);
      
    $result = insertReading($sens, $loc, $usex, $temp, $humid, $coffee, $cup);
    echo $result;
        
    // Create connection
    $conn = new mysqli($servername, $username, $password, $dbname);
    // Check connection
    if ($conn->connect_error) {
        die("Connection failed: " . $conn->connect_error);
        } 
        
    $sql = "INSERT INTO SensorData (sens, loc, usex, temp, humid, coffee, cup) VALUES ('" . $sens . "', '" . $loc . "', '" . $usex . "', '" . $temp . "', '" . $humid . "', '" . $coffee . "','" . $cup . "')";
        
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
