<!--
// Copyright (c) 2013 Hyperceptive, LLC
// Use of this source code is governed by The MIT License.
// that can be found in the LICENSE file.


; Configuration file for the LED Matrix Surf Visualization.

[GLOBAL]
explain_mode = true    ; display an explanation of each visualization
explain_count = 5      ; show the explanations every X times through the loop

wait_before = 4        ; seconds to wait before fading a visualization
wait_after = 1         ; seconds to wait in between each visualization


[VISUALIZATIONS]
ground_swell = true
wind_swell = true
ground_and_wind_swell = true
tide = false
wind_buoy = false
wind_ob = false
sunrise_sunset = false
temp_Air_Water = false
-->
<?php

function put_ini_file($file, $array, $i = 0)
{
  $str="";

  foreach ($array as $k => $v)
  {
    if (is_array($v))
    {
      $str.=str_repeat(" ",$i*2)."[$k]".PHP_EOL; 
      $str.=put_ini_file("",$v, $i+1);
    }
    else
    {
      $str.=str_repeat(" ",$i*2)."$k = $v".PHP_EOL; 
    }
  }

  if($file)
    return file_put_contents($file, $str);
  else
    return $str;
}



function write_php_ini($file, $array)
{
    $res = array();
    foreach($array as $key => $val)
    {
        if(is_array($val))
        {
            $res[] = "[$key]";
            foreach($val as $skey => $sval) $res[] = "$skey = ".(is_numeric($sval) ? $sval : '"'.$sval.'"');
        }
        else $res[] = "$key = ".(is_numeric($val) ? $val : '"'.$val.'"');
    }
    safefilerewrite($file, implode("\r\n", $res));
}

function safefilerewrite($fileName, $dataToSave)
{    if ($fp = fopen($fileName, 'w'))
    {
        $startTime = microtime();
        do
        {            $canWrite = flock($fp, LOCK_EX);
           // If lock not obtained sleep for 0 - 100 milliseconds, to avoid collision and CPU load
           if(!$canWrite) usleep(round(rand(0, 100)*1000));
        } while ((!$canWrite)and((microtime()-$startTime) < 1000));

        //file was locked so now we can store information
        if ($canWrite)
        {            fwrite($fp, $dataToSave);
            flock($fp, LOCK_UN);
        }
        fclose($fp);
    }

}


// Parse ini file (without sections)
$iniArray = parse_ini_file("LedMatrixBuoyViz.ini", TRUE, INI_SCANNER_RAW);

$globalArray = $iniArray[GLOBALS];
$vizArray = $iniArray[VISUALIZATIONS];


// Try writing it back out.
put_ini_file("/home/pi/led-matrix-buoy-viz/www/fish.ini", $iniArray);

write_php_ini("/home/pi/led-matrix-buoy-viz/www/taco.ini", $iniArray);


?>
<html>
  <head>
    <meta charset="utf-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <link rel="stylesheet" href="css/responsive.css" type="text/css" media="screen"/>
    <link rel="stylesheet" href="css/styles.css" type="text/css" media="screen"/>
    <script type="text/javascript" src="js/css3-mediaqueries.js"></script>
  </head>

  <body>

    <div class="container header">
      <div class="row">
        <div class="onecol logo">
          <a href="http://www.hyperceptive.org/" title="Surf Visualization Config" id="logo">hyperceptive.org</a>
        </div>
      </div>
    </div>

    <div class="container content">
      <div class="row">
        <div class="onecol body">
          <h1>Surf Visualization</h1>

          <h2>Global Options</h2>
          <table>
            <tr>
              <th>Option</th>
              <th>Value</th>
            </tr>
            <? while ($configName = current($globalArray)) { ?>
            <tr>
              <td><?= key($globalArray) ?></td>
              <td><?= $configName ?></td>
            </tr><?
              next($globalArray);
             } ?>
          </table>

          <h2>Visualization Options</h2>
          <table>
            <tr>
              <th>Option</th>
              <th>Value</th>
            </tr>
            <? while ($configName = current($vizArray)) { ?>
            <tr>
              <td><?= key($vizArray) ?></td>
              <td><?= $configName ?></td>
            </tr><?
              next($vizArray);
             } ?>
          </table>

        </div>
      </div>
    </div>

    <div class="container footer">
      <div class="row">
        <div class="onecol foot">
          <p><a href="http://www.hyperceptive.org/">©2013 Hyperceptive, LLC. All Rights Reserved.</a></p>
        </div>
      </div>
    </div>
</body>

</html>