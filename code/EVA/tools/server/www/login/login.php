<?php

	include_once('public_func.php');

	error_reporting(E_ERROR | E_PARSE);
	set_error_handler('err_callback');

	// For error handling, buffer all output
	ob_start('ob_callback_r2login');

	include_once('config.php');

	
	if (!isset($_POST['cmd']))
	{
		die (errorMsgBlock(3002));
	}

	switch($_POST['cmd'])
	{
	case 'login':

    
		$AuthVersion    = $_POST['Version'];
		$AuthType       = $_POST['AuthType'];
        $AuthAPPName    = $_POST['APPID'];
        $AuthUser       = $_POST['User'];
        $AuthGameType   = $_POST['GameType'];
        $AuthNonceStr   = randomStr(20);
        $AuthTimestamp  = time();
        
        
        
        $AuthUID        = $_POST['UID'];
        
        $login_sign_data    = $AuthVersion . $AuthType . $AuthAPPName . $AuthUser . $AuthGameType . $AuthNonceStr . $AuthTimestamp . $SIG_KEY[$AuthAPPName];
        $Token              = strtoupper(md5(login_sign_data));
        

		$redata = array();
		$redata['token'] 		= $Token; 
		$redata['UID']	        = $AuthUID;

        

		echo json_encode($redata);

	}

	// no more to do (other global statement are old garbage)
	die();

// ----------------------------------------------------------------------------------------
// Functions
// ----------------------------------------------------------------------------------------



?>
