/*
Navicat MySQL Data Transfer

Source Server         : localhost_3306
Source Server Version : 50611
Source Host           : localhost:3306
Source Database       : d_mt_player

Target Server Type    : MYSQL
Target Server Version : 50611
File Encoding         : 65001

Date: 2013-05-05 14:05:24
*/


drop database if exists d_mt_player ;

create database d_mt_player default character set utf8 ;

use d_mt_player ;

/*==============================================================*/
/* Table: t_playerinfo                                          */
/*==============================================================*/
drop table if exists t_playerinfo;
create table t_playerinfo
(
   f_player_id          bigint(20) unsigned NOT NULL DEFAULT 0 comment 'PID' ,
   f_account_id         bigint(20) unsigned NOT NULL DEFAULT 0 comment '账号ID' ,
   f_nickname           char(24) NOT NULL DEFAULT "" comment '昵称' ,
   f_sex                tinyint(3) unsigned NOT NULL DEFAULT 0 comment '性别' ,
   f_level              smallint(5) unsigned NOT NULL DEFAULT 0 comment '等级' ,
   f_exp                int(10) unsigned NOT NULL DEFAULT 0,
   f_money              bigint(20) unsigned NOT NULL DEFAULT 0,
   
  primary key (f_player_id)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
alter table t_playerinfo comment '角色基础信息';

