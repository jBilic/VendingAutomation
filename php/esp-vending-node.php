<!--
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/cloud-weather-station-esp32-esp8266/

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
-->
<?php
    include_once('esp-database.php');
    if ($_GET["readingsCount"]){
      $data = $_GET["readingsCount"];
      $data = trim($data);
      $data = stripslashes($data);
      $data = htmlspecialchars($data);
      $readings_count = $_GET["readingsCount"];
    }
    // default readings count set to 20
    else {
      $readings_count = 30;
    }

    $last_reading = getLastReadings();
    $last_reading_temp = $last_reading["temp"];
    $last_reading_humi = $last_reading["humid"];
    $last_reading_time = $last_reading["reading_time"];

    // Uncomment to set timezone to - 1 hour (you can change 1 to any number)
    //$last_reading_time = date("Y-m-d H:i:s", strtotime("$last_reading_time - 1 hours"));
    // Uncomment to set timezone to + 7 hours (you can change 7 to any number)
    //$last_reading_time = date("Y-m-d H:i:s", strtotime("$last_reading_time + 7 hours"));
    
    //https://www.geeksforgeeks.org/how-to-create-toggle-switch-by-using-html-and-css/ ------> odavde je skinut kod za tipku

    $min_temp = minReading($readings_count, 'temp');
    $max_temp = maxReading($readings_count, 'temp');
    $avg_temp = avgReading($readings_count, 'temp');

    $min_humi = minReading($readings_count, 'humid');
    $max_humi = maxReading($readings_count, 'humid');
    $avg_humi = avgReading($readings_count, 'humid');
?>

<!DOCTYPE html>
<html>
    <head><meta http-equiv="Content-Type" content="text/html; charset=utf-8">
        <link rel="stylesheet" type="text/css" href="esp-style.css">
        <meta name="viewport" content="width=device-width, initial-scale=1">
        <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.4.1/jquery.min.js"></script>
    </head>
    <header class="header">
        <h1>Status samoposlužnog aparata</h1>
    </header>
<body>
    
    <p>Zadnje očitanje: <?php echo $last_reading_time; ?></p>
    
<head> 
    <title>VMNS</title> 
      
    <style> 
        h1 { 
            color: black; 
        } 
                
        /* toggle in label designing */ 
        .toggle { 
            position : relative ; 
            display : inline-block; 
            width : 100px; 
            height : 52px; 
            background-color: red; 
            border-radius: 30px; 
            border: 2px solid gray; 
        } 
                
        /* After slide changes */ 
        .toggle:after { 
            content: ''; 
            position: absolute; 
            width: 50px; 
            height: 50px; 
            border-radius: 50%; 
            background-color: gray; 
            top: 1px;  
            left: 1px; 
            transition:  all 0.5s; 
        } 
                
        /* Toggle text */ 
        p { 
            font-family: Arial, Helvetica, sans-serif; 
            font-weight: bold; 
        } 
                
        /* Checkbox cheked effect */ 
        .checkbox:checked + .toggle::after { 
            left : 49px;  
        } 
                
        /* Checkbox cheked toggle label bg color */ 
        .checkbox:checked + .toggle { 
            background-color: green; 
        } 
                
        /* Checkbox vanished */ 
        .checkbox {  
            display : none; 
        } 
    </style> 
</head> 
  
<body> 
    <center> 
        <h3>Uključivanje i isključivanje uređaja</h3> 
        <input type="checkbox" id="switch"
                    class="checkbox" /> 
        <label for="switch" class="toggle"> 
            <p>OFF      ON</p> 
        </label> 
    </center> 
</body> 
    
    <section class="content">
	    <div class="box gauge--1">
	    <h3>TEMPERATURA</h3>
              <div class="mask">
			  <div class="semi-circle"></div>
			  <div class="semi-circle--mask"></div>
			</div>
		    <p style="font-size: 30px;" id="temp">--</p>
		    <table cellspacing="5" cellpadding="5">
		        <tr>
		            <td>Min</td>
                    <td>Max</td>
                    <td>Average</td>
                </tr>
                <tr>
                    <td><?php echo $min_temp['min_amount']; ?> &deg;C</td>
                    <td><?php echo $max_temp['max_amount']; ?> &deg;C</td>
                    <td><?php echo round($avg_temp['avg_amount'], 2); ?> &deg;C</td>
                </tr>
            </table>
        </div>
        <div class="box gauge--2">
            <h3>VLAGA</h3>
            <div class="mask">
                <div class="semi-circle"></div>
                <div class="semi-circle--mask"></div>
            </div>
            <p style="font-size: 30px;" id="humi">--</p>
            <table cellspacing="5" cellpadding="5">
                <tr>
                    <td>Min</td>
                    <td>Max</td>
                    <td>Average</td>
                </tr>
                <tr>
                    <td><?php echo $min_humi['min_amount']; ?>%</td>
                    <td><?php echo $max_humi['max_amount']; ?>%</td>
                    <td><?php echo round($avg_humi['avg_amount'], 2); ?>%</td>
                </tr>
            </table>
        </div>
    </section>
<?php
    echo   '<h3> Pregled zadnjih 30 očitanja </h3>
            <table cellspacing="5" cellpadding="5" id="tableReadings">
                <tr>
                    <th>ID</th>
                    <th>Device</th>
                    <th>Lokacija</th>
                    <th>Korisnik</th>
                    <th>Temperatura</th>
                    <th>Vlaga</th>
                    <th>Kava</th>
                    <th>Čaše</th>
                    <th>Očitano</th>
                </tr>';

    $result = getAllReadings($readings_count);
        if ($result) {
        while ($row = $result->fetch_assoc()) {
            $row_id = $row["id"];
            $row_sens = $row["sens"];
            $row_loc = $row["loc"];
            $row_usex = $row["usex"];
            $row_temp = $row["temp"];
            $row_humid = $row["humid"];
            $row_coffee = $row["coffee"];
            $row_cup = $row["cup"];
            $row_reading_time = $row["reading_time"];
            // Uncomment to set timezone to - 1 hour (you can change 1 to any number)
            //$row_reading_time = date("Y-m-d H:i:s", strtotime("$row_reading_time - 1 hours"));
            // Uncomment to set timezone to + 7 hours (you can change 7 to any number)
            //$row_reading_time = date("Y-m-d H:i:s", strtotime("$row_reading_time + 7 hours"));

            echo '<tr>
                    <td>' . $row_id . '</td>
                    <td>' . $row_sens . '</td>
                    <td>' . $row_loc . '</td>
                    <td>' . $row_usex . '</td>
                    <td>' . $row_temp . '</td>
                    <td>' . $row_humid . '</td>
                    <td>' . $row_coffee . '</td>
                    <td>' . $row_cup . '</td>
                    <td>' . $row_reading_time . '</td>
                  </tr>';
        }
        echo '</table>';
        $result->free();
    }
?>

<script>
    var value1 = <?php echo $last_reading_temp; ?>;
    var value2 = <?php echo $last_reading_humi; ?>;
    setTemperature(value1);
    setHumidity(value2);

    function setTemperature(curVal){
    	//set range for Temperature in Celsius -5 Celsius to 38 Celsius
    	var minTemp = -5.0;
    	var maxTemp = 38.0;
        //set range for Temperature in Fahrenheit 23 Fahrenheit to 100 Fahrenheit
    	//var minTemp = 23;
    	//var maxTemp = 100;

    	var newVal = scaleValue(curVal, [minTemp, maxTemp], [0, 180]);
    	$('.gauge--1 .semi-circle--mask').attr({
    		style: '-webkit-transform: rotate(' + newVal + 'deg);' +
    		'-moz-transform: rotate(' + newVal + 'deg);' +
    		'transform: rotate(' + newVal + 'deg);'
    	});
    	$("#temp").text(curVal + ' ºC');
    }

    function setHumidity(curVal){
    	//set range for Humidity percentage 0 % to 100 %
    	var minHumi = 0;
    	var maxHumi = 100;

    	var newVal = scaleValue(curVal, [minHumi, maxHumi], [0, 180]);
    	$('.gauge--2 .semi-circle--mask').attr({
    		style: '-webkit-transform: rotate(' + newVal + 'deg);' +
    		'-moz-transform: rotate(' + newVal + 'deg);' +
    		'transform: rotate(' + newVal + 'deg);'
    	});
    	$("#humi").text(curVal + ' %');
    }

    function scaleValue(value, from, to) {
        var scale = (to[1] - to[0]) / (from[1] - from[0]);
        var capped = Math.min(from[1], Math.max(from[0], value)) - from[0];
        return ~~(capped * scale + to[0]);
    }
</script>
</body>
</html>
