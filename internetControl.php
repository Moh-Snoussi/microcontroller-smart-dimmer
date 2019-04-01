<?php

$jsonLocation = 'dimmerInstruction.json'; // the name and the path of the json file that will be created
// the above path need to be associated with link defined in the python program (web_serial.py)

if (isset($_POST['modeSelector'])) { //check if form was submitted
    $modeSelector = $_POST['modeSelector'];// get the selected mode
    $signalStrength = $_POST['signalStrength']; // get the signal strength
    
    $response = array(); // assigning the array
    $response["My_home"] = array("Mode" => $modeSelector, "Times" => $signalStrength); // filling the array with data
    $fp = fopen($jsonLocation, 'w');// open the json file and overwrite it or create new if it doesn't exist,

    fwrite($fp, json_encode($response, JSON_UNESCAPED_UNICODE | JSON_PRETTY_PRINT)); // writing our new created array in the json location
    fclose($fp); // close the json file
    echo '<script type="text/javascript">alert("Successful");</script>'; // Alert success 
}

?>

<!DOCTYPE html>
<html>
<head>
    <title>Internet Control</title>
</head>
<body>
    <div class="content-wrapper">
        <section class="content">
            <div class="row">
                <div class="col-md-6">
                    <!-- general form elements -->
                    <div class="box box-primary">
                        <div class="box-header with-border">
                            <h3 class="box-title"></h3>
                        </div>
                        <div class="box-body">
                            Select signal strength
                            <form name='f' method='POST' action="<?php echo htmlspecialchars($_SERVER["PHP_SELF"]); ?>">
                                <div class='form-group'>
                                    <label>
                                        Signal strength
                                    </label>
                                <input type="range" class='form-control' name="signalStrength" min="1" max="7">
                                    <div class='form-group'><select name='modeSelector' class='form-control' onchange="this.form.submit()">
                                            <option disabled selected>click to chose</option>
                                            <option value="a">Slow dimming</option>
                                            <option value="0">No light</option>
                                            <option value="1">Low level 1</option>
                                            <option value="2">Low level 2</option>
                                            <option value="3">Low level 3</option>
                                            <option value="4">Mid level 1</option>
                                            <option value="5">Mid level 2</option>
                                            <option value="6">Mid level 3</option>
                                            <option value="7">High level 1</option>
                                            <option value="8">High level 2</option>
                                            <option value="9">Open full light</option>
                                        </select>
                                    </div>
                                    <input type='submit' value='submit' name='B1' class="btn btn-info pull-right">
                                </div>
                            </form>
                        </div>
                    </div>
                </div>
            </div>
        </section>
    </div>
</body> 