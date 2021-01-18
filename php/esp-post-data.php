<?php
include_once('esp-database.php');
  
$servername = "localhost";

// REPLACE with your Database name
$dbname = "pursahos_esp32";
// REPLACE with Database user
$username = "pursahos_jure";
// REPLACE with Database user password
$password = "tvzmiuzeozivot";

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
      
    // Create connection
    $conn = new mysqli($servername, $username, $password, $dbname);
    // Check connection
    if ($conn->connect_error) {
        die("Connection failed: " . $conn->connect_error);
    } 
        
    $sql = "INSERT INTO SensorData (sens, loc, usex, temp, humid, coffee, cup) VALUES ('" . $sens . "', '" . $loc . "', '" . $usex . "', '" . $temp . "', '" . $humid . "', '" . $coffee . "','" . $cup . "')";
        
    if ($conn->query($sql) === TRUE) {
        echo "New record created successfully";
    } else {
        echo "Error: " . $sql . "<br>" . $conn->error;
    } 
    
    $conn->close();
    
   } else {
      echo "Wrong API Key provided.";
   } }
   
   

    if ($_SERVER["REQUEST_METHOD"] == "GET") {
        $action = test_input($_GET["action"]);
        if ($action == "get_last_relay_value") {
        // Create connection
        $conn = new mysqli($servername, $username, $password, $dbname);
        // Check connection
        if ($conn->connect_error) {
            die("Connection failed: " . $conn->connect_error);
        }

        $sql = "SELECT Relay_Status FROM RelayData ORDER BY id DESC LIMIT 1";
            if ($result3 = $conn->query($sql)) {
                while ($row = $result3->fetch_assoc()) {
                echo ($row["Relay_Status"]);
        }}
        else {
            return false;
        }
        $conn->close();
    } else {
            echo "Invalid HTTP request.";
    }}
    

function test_input($data) {
    $data = trim($data);
    $data = stripslashes($data);
    $data = htmlspecialchars($data);
    return $data;
}
?>