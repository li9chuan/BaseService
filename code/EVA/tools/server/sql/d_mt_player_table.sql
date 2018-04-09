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

create database d_mt_player default character set utf8mb4 ;

use d_mt_player ;

/*==============================================================*/
/* Table: t_playerinfo                                          */
/*==============================================================*/
drop table if exists t_playerinfo;
create table t_playerinfo
(
   f_uid                bigint(20) unsigned NOT NULL DEFAULT 0 comment '' ,
   f_nickname           varchar(64) NOT NULL DEFAULT "" comment '昵称',
   f_portrait           int(10) unsigned NOT NULL DEFAULT 0 comment '头像',
   f_money              bigint(20) unsigned NOT NULL DEFAULT 0 comment '',
   f_rmb                bigint(20) unsigned NOT NULL DEFAULT 0 comment '',
   f_main               int(10) unsigned NOT NULL DEFAULT 0 comment '',
   f_flag_bit           bigint(20) unsigned NOT NULL DEFAULT 0 comment '各种占一位的标识',
   f_insert_time        timestamp DEFAULT CURRENT_TIMESTAMP COMMENT '记录生成时间',
   
  primary key (f_uid)
) ENGINE=MyISAM DEFAULT CHARSET=utf8mb4;
alter table t_playerinfo comment '角色基础信息';

