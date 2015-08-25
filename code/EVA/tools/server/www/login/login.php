<?php

	error_reporting(E_ERROR | E_PARSE);
	set_error_handler('err_callback');

	// For error handling, buffer all output
	ob_start('ob_callback_r2login');

	include_once('config.php');
	include_once('login_translations.php');
	include_once('../tools/nel_message.php');
	include_once('login_service_itf.php');


	// see errorMsg
	function errorMsgBlock($errNum=GENERIC_ERROR_NUM) // $mixedArgs
	{
		$args = func_get_args();
		return '0:'.call_user_func_array('errorMsg', $args);
	}
	
	function randomkeys($length)
	{
		$output='';
		for ($a = 0; $a < $length; $a++) {
			$output .= chr(mt_rand(33, 126));
		}
		return $output;
	}

	class CWwwLog
	{
		//function CWwwLog() {}

		/*
		 * Return the log directory. Create it if it does not exist, or return false if creation failed.
		 */
		function getSafeLogDir()
		{
			// Examples:
			// __FILE__ = r:\code\ryzom\www\login\config.php
			// $_SERVER['PATH_TRANSLATED'] = 'r:/code/ryzom/www/login//r2_login.php'
			// $_SERVER['SCRIPT_FILENAME'] = 'r:/code/ryzom/www/login//r2_login.php'
			global $LogRelativePath;
			$pathInfo = pathinfo(__FILE__);
			$logPath = $pathInfo['dirname'].'/'.$LogRelativePath;
			if (!is_dir($logPath))
			{
				$res = mkdir($LogPath, 0700);
				return $res ? $logPath : false;
			}
			return $logPath;
		}

		function logStr($str)
		{
			$logPath = $this->getSafeLogDir();
			if ($logPath !== false)
			{
				$fp = fopen($logPath.'/r2_login_'.date('Y-m-d').'.log', 'a');
				fwrite($fp, date('Y-m-d H:i:s').' ('.$_SERVER['REMOTE_ADDR'].':'.$_SERVER['REQUEST_URI']."): $str\n");
				fclose($fp);
			}
		}
	}

	// Callback called on end of output buffering
	function ob_callback_r2login($buffer)
	{
		// Log only in case of error or malformed result string
		$blockHd = substr($buffer, 0, 2);
		if ($blockHd != '1:')
		{
			$logFile = new CWwwLog();
			$logFile->logStr(str_replace("\n",'\n',$buffer));
		}
		return $buffer; // sent to output
	}

	// Callback called on error
	function err_callback($errno, $errmsg, $filename, $linenum, $vars)
	{
		$logFile = new CWwwLog();
		$logFile->logStr("PHP ERROR/$errno $errmsg ($filename:$linenum)");
		// Never die after an error
	}
	
	if (!isset($_POST['cmd']))
	{
		die (errorMsgBlock(3002));
	}

	switch($_POST['cmd'])
	{
	case 'login':
		$domainId = -1;
		$username = $_POST['usr'];
		$password = $_POST['pwd'];
		
		// client sent is login info
		$actIdDb = mysqli_connect($DBHost, $DBUserName, $DBPassword) or die(errorMsgBlock(3004, 'Login', $DBHost, $DBUserName));
		mysqli_select_db ($actIdDb, $DBName) or die(errorMsgBlock(3005, 'Login', $DBName, $DBHost, $DBUserName));
		$login_usr = mysqli_real_escape_string($actIdDb, $username);
		$login_pwd = mysqli_real_escape_string($actIdDb, $password);
	
		$query = "SELECT f_account_id FROM t_account WHERE f_account_name='$login_usr' AND f_password='$login_pwd'";
		$result = mysqli_query ($actIdDb, $query) or die(errorMsgBlock(3006, $query, 'Login', $DBName, $DBHost, $DBUserName, mysqli_error($actIdDb)));
		
		if (mysqli_num_rows($result) == 1)
		{
			///  get user info
			$query = "SELECT f_account_id FROM t_account_id WHERE f_name='$login_usr' AND f_type=1";
			$result = mysqli_query ($actIdDb, $query) or die(errorMsgBlock(3006, $query, 'Login', $DBName, $DBHost, $DBUserName, mysqli_error($actIdDb)));
			
			if (mysqli_num_rows($result) == 0)
			{
				// no user id record, build one
				$query = "INSERT INTO t_account_id (f_name,f_type) VALUES ('$login_usr',1)";
				mysqli_query ($actIdDb, $query) or die(errorMsgBlock(3006, $query, 'Login', $DBName, $DBHost, $DBUserName, mysqli_error($actIdDb)));
				
				// re get
				$query = "SELECT f_account_id FROM t_account_id WHERE f_name='$login_usr' AND f_type=1";
				$result = mysqli_query ($actIdDb, $query) or die(errorMsgBlock(3006, $query, 'Login', $DBName, $DBHost, $DBUserName, mysqli_error($actIdDb)));
			}
			
			$row = mysqli_fetch_array($result);
			$uid = $row["f_account_id"];
			$token = randomkeys(16);
			
			$login = new CLoginServiceWeb;
			$res = "";
			$login->connect($LoginModuleIP, $LoginModulePort, $res);
			$login->login($uid, $token);

			if (!$login->waitCallback())
			{
				echo 'timeout';
				die();
			}
		
			echo 'token:';
			echo $token;

	


			//the rest of the process is done in the callback function

		}

	}

	// no more to do (other global statement are old garbage)
	die();

// ----------------------------------------------------------------------------------------
// Functions
// ----------------------------------------------------------------------------------------

	// return true if the check is ok
	function checkUserValidity ($login, $password)
	{
		$actIdDb = mysqli_connect($DBHost, $DBUserName, $DBPassword) or die(errorMsgBlock(3004, 'Login', $DBHost, $DBUserName));
		mysqli_select_db ($actIdDb, $DBName) or die(errorMsgBlock(3005, 'Login', $DBName, $DBHost, $DBUserName));
		$login_usr = mysqli_real_escape_string($actIdDb, $username);
		$login_pwd = mysqli_real_escape_string($actIdDb, $password);
		
		echo $login_usr;
		echo $login_pwd;
	
		$query = "SELECT f_account_id FROM t_account WHERE f_account_name='$login_usr' AND f_password='$login_pwd'";
		$result = mysqli_query ($actIdDb, $query) or die(errorMsgBlock(3006, $query, 'Login', $DBName, $DBHost, $DBUserName, mysqli_error($actIdDb)));
		
		if (mysqli_num_rows($result) == 1)
		{
			return true;
		}
		
		return false;
	}

?>
