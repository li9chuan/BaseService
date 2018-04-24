<?php

	include_once('public_func.php');

	error_reporting(E_ERROR | E_PARSE);
	set_error_handler('err_callback');

	// For error handling, buffer all output
	ob_start('ob_callback_r2login');

	include_once('config.php');

	
    $Channel        = $_GET['Channel'];
    $GameType       = $_GET['GameType'];
    $JsCode         = $_GET['JsCode'];
    $AppName        = $_GET['AppName'];
    
    $APPID          = $WX_APPID[$AppName];
    $SECRET         = $WX_SECRET[$AppName];
            
    $RequestURL     = "https://api.weixin.qq.com/sns/jscode2session?appid=$APPID&secret=$SECRET&js_code=$JsCode&grant_type=authorization_code";
    $wx_dejson      = RequestJson($RequestURL);

    
    if( !isset($wx_dejson['errcode']) )
    {
        $actIdDb = mysqli_connect($DBHost, $DBUserName, $DBPassword) or die("mysqli_connect die.");
		mysqli_select_db ($actIdDb, $DBName) or die("mysqli_select_db die.");
        
        mysqli_query($actIdDb, "SET NAMES utf8mb4");
        $query = "SELECT f_account_id,f_freeze_to FROM t_account_id WHERE f_name='$OpenID' AND f_appname='$AppName' AND f_chal='$Channel'";
        $result = mysqli_query ($actIdDb, $query) or die();
        
        if (mysqli_num_rows($result) == 0)
        {
            // no user id record, build one
            $query = "INSERT INTO t_account_id (f_name,f_chal,f_appname) VALUES ('$OpenID','$Channel','$AppName')";
            mysqli_query ($actIdDb, $query) or die();
            
            // re get
            $query = "SELECT f_account_id,f_freeze_to FROM t_account_id WHERE f_name='$OpenID' AND f_appname='$AppName' AND f_chal='$Channel'";
            $result = mysqli_query ($actIdDb, $query) or die();
        }
        
        
        if (mysqli_num_rows($result) == 1)
        {
            $row = mysqli_fetch_array($result);
            $freeze_to  = $row["f_freeze_to"];
            
            
            
            if( time() >= $freeze_to )
            {
                $UserID         = $row["f_account_id"];
                $OpenID         = $wx_dejson['openid'];
                $SessionKey     = $wx_dejson['session_key'];
                $NonceStr       = RandomStr(20);
                $Timestamp      = time();
                
                $login_sign_data    = $UserID . $Channel . $GameType . $AppName . $OpenID . $NonceStr . $Timestamp . $SIG_KEY[$AppName];
                $Token              = strtoupper(md5($login_sign_data));

                $redata = array();
                $redata['Token'] 		= $Token; 
                $redata['OpenID']	    = $OpenID;
                $redata['NonceStr']	    = $NonceStr;
                $redata['Timestamp']    = $Timestamp;
                $redata['SessionKey']   = $SessionKey;
                $redata['UID']          = $UserID;
                $redata['FES']          = "127.0.0.1:9999";

                echo json_encode($redata);
            }
            
        }
    }
    else
    {
        echo $wx_dejson['errMsg'];
    }

	// no more to do (other global statement are old garbage)
	die();

// ----------------------------------------------------------------------------------------
// Functions
// ----------------------------------------------------------------------------------------



?>
