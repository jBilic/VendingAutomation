<!DOCTYPE html>
<html><body>
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

// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);
// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
} 

$sql = "SELECT id, devId, locationId, userId, temp, humidity, supp1, supp2, supp3, supp4, reading_time FROM SensorData ORDER BY id DESC";

echo '<table cellspacing="20" cellpadding="5">
      <tr> 
        <td>RBr</td> 
        <td>DeviceID</td> 
        <td>Lokacija</td> 
        <td>Korisnik</td> 
        <td>Temperatura</td>
        <td>Vlaga</td> 
        <td>Zaliha - Twix</td> 
        <td>Zaliha - Snickers</td> 
        <td>Zaliha - Kava</td> 
        <td>Zaliha - Čaše</td> 
        <td>Zadnje očitano</td> 
      </tr>';
 
if ($result = $conn->query($sql)) {
    while ($row = $result->fetch_assoc()) {
        $row_id = $row["id"];
        $row_devId = $row["devId"];
        $row_locationId = $row["locationId"];
        $row_userId = $row["userId"];
        $row_temp = $row["temp"]; 
        $row_humidity = $row["humidity"];
        $row_supp1 = $row["supp1"];
        $row_supp2 = $row["supp2"];
        $row_supp3 = $row["supp3"];
        $row_supp4 = $row["supp4"];
        $row_reading_time = $row["reading_time"];
        // Uncomment to set timezone to - 1 hour (you can change 1 to any number)
        //$row_reading_time = date("Y-m-d H:i:s", strtotime("$row_reading_time - 1 hours"));
      
        // Uncomment to set timezone to + 4 hours (you can change 4 to any number)
        //$row_reading_time = date("Y-m-d H:i:s", strtotime("$row_reading_time + 4 hours"));
      
        echo '<tr> 
                <td>' . $row_id . '</td> 
                <td>' . $row_devId . '</td> 
                <td>' . $row_locationId . '</td> 
                <td>' . $row_userId . '</td> 
                <td>' . $row_temp . '</td>
                <td>' . $row_humidity . '</td>
                <td>' . $row_supp1 . '</td>
                <td>' . $row_supp2 . '</td>
                <td>' . $row_supp3 . '</td>
                <td>' . $row_supp4 . '</td>
                <td>' . $row_reading_time . '</td> 
              </tr>';
    }
    $result->free();
}

$conn->close();
?> 
</table>
</body>
</html>
