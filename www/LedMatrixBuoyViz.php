<!--
// Copyright (c) 2013 Hyperceptive, LLC
// Use of this source code is governed by The MIT License.
// that can be found in the LICENSE file.
-->
<?php

function put_ini_file($file, $array, $i = 0)
{
  $str="";

  foreach ($array as $k => $v)
  {
    if (is_array($v))
    {
      $str .= str_repeat(" ",$i*2)."[$k]".PHP_EOL; 
      $str .= put_ini_file("", $v);  //mateo: , $i+1);
    }
    else
    {
      $str .= str_repeat(" ",$i*2)."$k = $v".PHP_EOL; 
    }
  }

  if($file)
    return file_put_contents($file, $str);
  else
    return $str;
}

$ConfigFile = "/home/pi/led-matrix-buoy-viz/config.ini";

$iniArray = parse_ini_file($ConfigFile, TRUE, INI_SCANNER_RAW);

//var_dump($iniArray);

$globalArray = $iniArray['GLOBALS'];
$vizArray = $iniArray['VISUALIZATIONS'];


// TODO: Create a funciton for writing it back out.
//put_ini_file($ConfigFile, $iniArray);



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
          <form>
            <table>
              <tr>
                <th>Option</th>
                <th>Value</th>
              </tr>
              <? while ($configValue = current($globalArray)) { ?>
              <tr>
                <td><?= key($globalArray) ?></td>
                <td>
                <? if ($configValue == "true") { ?>
                  <input type="radio" name=<?=key($globalArray)?> value="true" checked="checked">Show</input>
                  <input type="radio" name=<?=key($globalArray)?> value="false">Hide</input>
                <? } else if ($configValue == "false") { ?>
                  <input type="radio" name=<?=key($globalArray)?> value="true">Show</input>
                  <input type="radio" name=<?=key($globalArray)?> value="false" checked="checked">Hide</input>                
                <? } else { ?>
                  <input type="text" name=<?=key($globalArray)?> value=<?=$configValue?> />
                <? } ?>
                </td>

              </tr><?
                next($globalArray);
               } ?>
              <tr><td colspan="2"><div align="center"><input type="submit" /><div></td></tr>
            </table>            
  
            <h2>Visualization Options</h2>
            <table>
              <tr>
                <th>Option</th>
                <th>Value</th>
              </tr>
              <? while ($configValue = current($vizArray)) { ?>
              <tr>
                <td><?= key($vizArray) ?></td>
                <td>
                <? if ($configValue == "true") { ?>
                  <input type="radio" name=<?=key($vizArray)?> value="true" checked="checked">Show</input>
                  <input type="radio" name=<?=key($vizArray)?> value="false">Hide</input>
                <? } else if ($configValue == "false") { ?>
                  <input type="radio" name=<?=key($vizArray)?> value="true">Show</input>
                  <input type="radio" name=<?=key($vizArray)?> value="false" checked="checked">Hide</input>                
                <? } else { ?>
                  <input type="text" name=<?=key($vizArray)?> value=<?=$configValue?> />
                <? } ?>
                </td>
              </tr><?
                next($vizArray);
               } ?>
              <tr><td colspan="2"><div align="center"><input type="submit" /><div></td></tr>
            </table>

          </form>
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
