<?php
	require_once('tools/nel_message.php');

	class CPayServiceWeb extends CCallbackClient
	{
		function cb_unity_pay($userId, $oid)
		{
			$msg = new CMessage;
			$msg->setName("CBUP");
			$msg->serialUint32($userId);
			$msg->serialUint32($oid);
            
            echo $userId;
            echo $oid;
	
			return parent::sendMessage($msg);
		}

		function waitCallback()
		{
			$message = parent::waitMessage();

			if ($message == false)
				return false;

			switch($message->MsgName)
			{
			case "CBUP":
            
                $userId = 0;
                $oid = 0;
                $message->serialUint32($userId);
                $message->serialUint32($oid);
                
                echo $userId;
                echo $oid;
                
                break;

			default:
				return false;
			}

			return true;
		}
	}
    
?>
