/*
Navicat MySQL Data Transfer

Source Server         : localhost_3306
Source Server Version : 50611
Source Host           : localhost:3306
Source Database       : d_mt_account

Target Server Type    : MYSQL
Target Server Version : 50611
File Encoding         : 65001

Date: 2013-05-05 14:05:10
*/

drop database if exists d_mt_account ;

create database d_mt_account default character set utf8 ;

use d_mt_account ;

-- ----------------------------
-- Table structure for `t_account`
-- ----------------------------
CREATE TABLE `t_account` (
  `f_account_id` bigint(10) unsigned NOT NULL AUTO_INCREMENT COMMENT '帐号id',
  `f_account_name` varchar(24) UNIQUE NOT NULL DEFAULT '' COMMENT '帐号名称',
  `f_password` varchar(64) NOT NULL DEFAULT '' COMMENT '密码',
  `f_inserttime` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT '创建时间',
  PRIMARY KEY (`f_account_id`, `f_account_name`)
) ENGINE=MyISAM AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for `t_account_id`
-- ----------------------------
CREATE TABLE `t_account_id` (
  `f_account_id` bigint(10) unsigned NOT NULL AUTO_INCREMENT COMMENT '帐号id' UNIQUE,
  `f_name` varchar(24) NOT NULL DEFAULT '' COMMENT '帐号名称',
  `f_type` smallint(4) NOT NULL DEFAULT 0 COMMENT '账号类型',
  `f_frozon_time` timestamp NOT NULL DEFAULT 0 COMMENT '冻结时间',
  `f_inserttime` timestamp DEFAULT CURRENT_TIMESTAMP COMMENT '激活时间',
  PRIMARY KEY (`f_name`, `f_type`)
) ENGINE=MyISAM AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Procedure structure for `InsertTestAccount`
-- ----------------------------
DROP PROCEDURE IF EXISTS `InsertTestAccount`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `InsertTestAccount`()
BEGIN
	
	DECLARE i int default 1;
	TRUNCATE TABLE `t_account`;

  WHILE(i < 10000) DO
		INSERT INTO `t_account` (f_account_name,f_password) VALUES (concat("test",i), '123456');
		SET i = i+1;
	END WHILE;

END
;;
DELIMITER ;

