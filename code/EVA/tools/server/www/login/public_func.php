<?php

	include_once('config.php');

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

	function random_nums($length)
	{
		$output='';
		for ($a = 0; $a < $length; $a++) {
			$output .= chr(mt_rand(48, 57));
		}
		return $output;
	}
	
	/**
	 * 生成指定长度的随机字符串(包含大写英文字母, 小写英文字母, 数字)
	 * @param $length int 需要生成的字符串的长度
	 * @return string 包含 大小写英文字母 和 数字 的随机字符串
	 */
	function random_str($length){    
		//生成一个包含 大写英文字母, 小写英文字母, 数字 的数组    
		$arr = array_merge(range(0, 9), range('a', 'z'), range('A', 'Z'));    
		$str = '';    
		$arr_len = count($arr);    
		for ($i = 0; $i < $length; $i++)    {        
			$rand = mt_rand(0, $arr_len-1);        
			$str.=$arr[$rand];    
		}    
		return $str;
	}
	
	function RequestJson($RequestURL)
	{
		$ctx  = stream_context_create( array( 'http' => array( 'timeout' => 7 ) ) );
		$html = file_get_contents($RequestURL, 0, $ctx);
		return json_decode($html, TRUE);
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
	
	function getIP()
	{
		static $realip;
		if (isset($_SERVER)){
			if (isset($_SERVER["HTTP_X_FORWARDED_FOR"])){
				$realip = $_SERVER["HTTP_X_FORWARDED_FOR"];
			} else if (isset($_SERVER["HTTP_CLIENT_IP"])) {
				$realip = $_SERVER["HTTP_CLIENT_IP"];
			} else {
				$realip = $_SERVER["REMOTE_ADDR"];
			}
		} else {
			if (getenv("HTTP_X_FORWARDED_FOR")){
				$realip = getenv("HTTP_X_FORWARDED_FOR");
			} else if (getenv("HTTP_CLIENT_IP")) {
				$realip = getenv("HTTP_CLIENT_IP");
			} else {
				$realip = getenv("REMOTE_ADDR");
			}
		}
		return $realip;
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
	
?>
