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

create database d_mt_account default character set utf8mb4;

use d_mt_account ;

-- ----------------------------
-- Table structure for `t_account_id`
-- ----------------------------
CREATE TABLE `t_account_id` (
  `f_account_id`    int(10) unsigned NOT NULL AUTO_INCREMENT COMMENT '帐号id' UNIQUE,
  `f_appname`       varchar(16)  NOT NULL DEFAULT '' COMMENT '游戏类型',
  `f_name`          varchar(128) NOT NULL DEFAULT '' COMMENT '帐号名称',
  `f_chal`          varchar(16)  NOT NULL DEFAULT 0 COMMENT '渠道',
  `f_mobile`        varchar(32) NOT NULL DEFAULT '' COMMENT '手机号',
  `f_freeze_to`     int(10) unsigned NOT NULL DEFAULT 0 COMMENT '冻结到什么时间',
  `f_inserttime`    timestamp DEFAULT CURRENT_TIMESTAMP COMMENT '记录生成时间',
  PRIMARY KEY (`f_appname`, `f_name`, `f_chal`)
) ENGINE=MyISAM AUTO_INCREMENT=1002 DEFAULT CHARSET=utf8mb4;


-- ----------------------------
-- Table structure for `t_account_mobile`
-- ----------------------------
CREATE TABLE `t_account_mobile` (
  `f_mobile`        varchar(32) NOT NULL DEFAULT '' COMMENT '手机号',
  `f_pwd`           varchar(32) NOT NULL DEFAULT '' COMMENT '密码',
  `f_slat`          varchar(8) NOT NULL DEFAULT '' COMMENT 'salt',
  `f_inserttime`    timestamp DEFAULT CURRENT_TIMESTAMP COMMENT '记录生成时间',
  PRIMARY KEY (`f_mobile`)
) ENGINE=MyISAM AUTO_INCREMENT=2 DEFAULT CHARSET=utf8mb4;


-- ----------------------------
-- Table structure for `t_account_info`
-- ----------------------------
CREATE TABLE `t_account_info` (
  `f_account_id`    int(10) unsigned NOT NULL COMMENT '帐号id',
  `f_nickname`      varchar(128) NOT NULL DEFAULT '' COMMENT '普通用户昵称',
  `f_sex`           smallint(4)  NOT NULL DEFAULT 0 COMMENT '普通用户性别，1为男性，2为女性',
  `f_province`      varchar(64) NOT NULL DEFAULT '' COMMENT '普通用户个人资料填写的省份',
  `f_city`          varchar(32) NOT NULL DEFAULT '' COMMENT '普通用户个人资料填写的城市',
  `f_country`       varchar(32) NOT NULL DEFAULT '' COMMENT '国家，如中国为CN',
  `f_headimgurl`    varchar(512) NOT NULL DEFAULT '' COMMENT '头像',
  `f_inserttime`    timestamp DEFAULT CURRENT_TIMESTAMP COMMENT '记录生成时间',
  PRIMARY KEY(`f_account_id`)
) ENGINE=MyISAM AUTO_INCREMENT=2 DEFAULT CHARSET=utf8mb4;




