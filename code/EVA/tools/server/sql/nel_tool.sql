/*
Navicat MySQL Data Transfer

Source Server         : localhost
Source Server Version : 50611
Source Host           : localhost:3306
Source Database       : nel_tool

Target Server Type    : MYSQL
Target Server Version : 50611
File Encoding         : 65001

Date: 2014-09-10 20:48:57
*/

drop database if exists nel_tool ;
create database nel_tool default character set utf8 ;
use nel_tool ;

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for `neltool_annotations`
-- ----------------------------
DROP TABLE IF EXISTS `neltool_annotations`;
CREATE TABLE `neltool_annotations` (
  `annotation_id` int(11) NOT NULL AUTO_INCREMENT,
  `annotation_domain_id` int(11) DEFAULT NULL,
  `annotation_shard_id` int(11) DEFAULT NULL,
  `annotation_data` varchar(255) NOT NULL DEFAULT '',
  `annotation_user_name` varchar(32) NOT NULL DEFAULT '',
  `annotation_date` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`annotation_id`),
  UNIQUE KEY `annotation_shard_id` (`annotation_shard_id`),
  UNIQUE KEY `annotation_domain_id` (`annotation_domain_id`)
) ENGINE=MyISAM AUTO_INCREMENT=14 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of neltool_annotations
-- ----------------------------
INSERT INTO `neltool_annotations` VALUES ('12', null, '106', 'Welcome to the Shard Admin Website!', 'admin', '1409802992');
INSERT INTO `neltool_annotations` VALUES ('13', null, '302', '宽字节注入测试(縗) -- 这里是这组服务器的注释。', 'admin', '1409909065');

-- ----------------------------
-- Table structure for `neltool_applications`
-- ----------------------------
DROP TABLE IF EXISTS `neltool_applications`;
CREATE TABLE `neltool_applications` (
  `application_id` int(11) NOT NULL AUTO_INCREMENT,
  `application_name` varchar(64) NOT NULL DEFAULT '',
  `application_uri` varchar(255) NOT NULL DEFAULT '',
  `application_restriction` varchar(64) NOT NULL DEFAULT '',
  `application_order` int(11) NOT NULL DEFAULT '0',
  `application_visible` int(11) NOT NULL DEFAULT '0',
  `application_icon` varchar(128) NOT NULL DEFAULT '',
  PRIMARY KEY (`application_id`)
) ENGINE=MyISAM AUTO_INCREMENT=41 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of neltool_applications
-- ----------------------------
INSERT INTO `neltool_applications` VALUES ('1', 'Main', 'index.php', '', '100', '1', 'imgs/icon_main.gif');
INSERT INTO `neltool_applications` VALUES ('2', 'Logout', 'index.php?mode=logout', '', '999999', '1', 'imgs/icon_logout.gif');
INSERT INTO `neltool_applications` VALUES ('3', 'Admin', 'tool_administration.php', 'tool_admin', '1500', '1', 'imgs/icon_admin.gif');
INSERT INTO `neltool_applications` VALUES ('4', 'Prefs', 'tool_preferences.php', 'tool_preferences', '1000', '1', 'imgs/icon_preferences.gif');
INSERT INTO `neltool_applications` VALUES ('5', 'Admin/Users', '', 'tool_admin_user', '1502', '0', '');
INSERT INTO `neltool_applications` VALUES ('6', 'Admin/Applications', '', 'tool_admin_application', '1501', '0', '');
INSERT INTO `neltool_applications` VALUES ('7', 'Admin/Domains', '', 'tool_admin_domain', '1504', '0', '');
INSERT INTO `neltool_applications` VALUES ('8', 'Admin/Shards', '', 'tool_admin_shard', '1505', '0', '');
INSERT INTO `neltool_applications` VALUES ('9', 'Admin/Groups', '', 'tool_admin_group', '1503', '0', '');
INSERT INTO `neltool_applications` VALUES ('10', 'Admin/Logs', '', 'tool_admin_logs', '1506', '0', '');
INSERT INTO `neltool_applications` VALUES ('11', 'Main/Start', '', 'tool_main_start', '101', '0', '');
INSERT INTO `neltool_applications` VALUES ('12', 'Main/Stop', '', 'tool_main_stop', '102', '0', '');
INSERT INTO `neltool_applications` VALUES ('13', 'Main/Restart', '', 'tool_main_restart', '103', '0', '');
INSERT INTO `neltool_applications` VALUES ('14', 'Main/Kill', '', 'tool_main_kill', '104', '0', '');
INSERT INTO `neltool_applications` VALUES ('15', 'Main/Abort', '', 'tool_main_abort', '105', '0', '');
INSERT INTO `neltool_applications` VALUES ('16', 'Main/Execute', '', 'tool_main_execute', '108', '0', '');
INSERT INTO `neltool_applications` VALUES ('18', 'Notes', 'tool_notes.php', 'tool_notes', '900', '1', 'imgs/icon_notes.gif');
INSERT INTO `neltool_applications` VALUES ('19', 'Player Locator', 'tool_player_locator.php', 'tool_player_locator', '200', '1', 'imgs/icon_player_locator.gif');
INSERT INTO `neltool_applications` VALUES ('20', 'Player Locator/Display Players', '', 'tool_player_locator_display_players', '201', '0', '');
INSERT INTO `neltool_applications` VALUES ('21', 'Player Locator/Locate', '', 'tool_player_locator_locate', '202', '0', '');
INSERT INTO `neltool_applications` VALUES ('22', 'Main/LockDomain', '', 'tool_main_lock_domain', '110', '0', '');
INSERT INTO `neltool_applications` VALUES ('23', 'Main/LockShard', '', 'tool_main_lock_shard', '111', '0', '');
INSERT INTO `neltool_applications` VALUES ('24', 'Main/WS', '', 'tool_main_ws', '112', '0', '');
INSERT INTO `neltool_applications` VALUES ('25', 'Main/ResetCounters', '', 'tool_main_reset_counters', '113', '0', '');
INSERT INTO `neltool_applications` VALUES ('26', 'Main/ServiceAutoStart', '', 'tool_main_service_autostart', '114', '0', '');
INSERT INTO `neltool_applications` VALUES ('27', 'Main/ShardAutoStart', '', 'tool_main_shard_autostart', '115', '0', '');
INSERT INTO `neltool_applications` VALUES ('28', 'Main/WS/Old', '', 'tool_main_ws_old', '112', '0', '');
INSERT INTO `neltool_applications` VALUES ('29', 'Graphs', 'tool_graphs.php', 'tool_graph', '500', '1', 'imgs/icon_graphs.gif');
INSERT INTO `neltool_applications` VALUES ('30', 'Notes/Global', '', 'tool_notes_global', '901', '0', '');
INSERT INTO `neltool_applications` VALUES ('31', 'Log Analyser', 'tool_log_analyser.php', 'tool_las', '400', '1', 'imgs/icon_log_analyser.gif');
INSERT INTO `neltool_applications` VALUES ('32', 'Guild Locator', 'tool_guild_locator.php', 'tool_guild_locator', '300', '1', 'imgs/icon_guild_locator.gif');
INSERT INTO `neltool_applications` VALUES ('33', 'Player Locator/UserID Check', '', 'tool_player_locator_userid_check', '203', '0', '');
INSERT INTO `neltool_applications` VALUES ('34', 'Player Locator/CSR Relocate', '', 'tool_player_locator_csr_relocate', '204', '0', '');
INSERT INTO `neltool_applications` VALUES ('35', 'Guild Locator/Guilds Update', '', 'tool_guild_locator_manage_guild', '301', '0', '');
INSERT INTO `neltool_applications` VALUES ('36', 'Guild Locator/Members Update', '', 'tool_guild_locator_manage_members', '302', '0', '');
INSERT INTO `neltool_applications` VALUES ('37', 'Entities', 'tool_event_entities.php', 'tool_event_entities', '350', '1', 'imgs/icon_entity.gif');
INSERT INTO `neltool_applications` VALUES ('38', 'Admin/Restarts', '', 'tool_admin_restart', '1507', '0', '');
INSERT INTO `neltool_applications` VALUES ('39', 'Main/EasyRestart', '', 'tool_main_easy_restart', '116', '0', '');
INSERT INTO `neltool_applications` VALUES ('40', '商城管理', 'tool_shop.php', 'tool_shop', '340', '1', 'imgs/shop.png');

-- ----------------------------
-- Table structure for `neltool_domains`
-- ----------------------------
DROP TABLE IF EXISTS `neltool_domains`;
CREATE TABLE `neltool_domains` (
  `domain_id` int(11) NOT NULL AUTO_INCREMENT,
  `domain_name` varchar(128) NOT NULL DEFAULT '',
  `domain_as_host` varchar(128) NOT NULL DEFAULT '',
  `domain_as_port` int(11) NOT NULL DEFAULT '0',
  `domain_rrd_path` varchar(255) NOT NULL DEFAULT '',
  `domain_las_admin_path` varchar(255) NOT NULL DEFAULT '',
  `domain_las_local_path` varchar(255) NOT NULL DEFAULT '',
  `domain_application` varchar(128) NOT NULL DEFAULT '',
  `domain_sql_string` varchar(128) NOT NULL DEFAULT '',
  `domain_hd_check` int(11) NOT NULL DEFAULT '0',
  `domain_mfs_web` text,
  `domain_cs_sql_string` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`domain_id`)
) ENGINE=MyISAM AUTO_INCREMENT=13 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of neltool_domains
-- ----------------------------
INSERT INTO `neltool_domains` VALUES ('12', '测试一区', '127.0.0.1', '46700', 'D:/MT/trunk/code/EVA/server/save_shard/rrd_graphs', '', '', 'ryzom_open', 'mysql://shard@localhost/ring_open', '0', '', 'mysql://shard@localhost/atrium_forums');

-- ----------------------------
-- Table structure for `neltool_groups`
-- ----------------------------
DROP TABLE IF EXISTS `neltool_groups`;
CREATE TABLE `neltool_groups` (
  `group_id` int(11) NOT NULL AUTO_INCREMENT,
  `group_name` varchar(32) NOT NULL DEFAULT 'NewGroup',
  `group_level` int(11) NOT NULL DEFAULT '0',
  `group_default` int(11) NOT NULL DEFAULT '0',
  `group_active` int(11) NOT NULL DEFAULT '0',
  `group_default_domain_id` tinyint(3) unsigned DEFAULT NULL,
  `group_default_shard_id` tinyint(3) unsigned DEFAULT NULL,
  PRIMARY KEY (`group_id`)
) ENGINE=MyISAM AUTO_INCREMENT=12 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of neltool_groups
-- ----------------------------
INSERT INTO `neltool_groups` VALUES ('1', 'AdminGroup', '0', '0', '1', '12', '255');
INSERT INTO `neltool_groups` VALUES ('2', 'NevraxGroup', '0', '1', '1', null, null);
INSERT INTO `neltool_groups` VALUES ('3', 'AdminDebugGroup', '10', '0', '1', '12', '255');
INSERT INTO `neltool_groups` VALUES ('4', 'SupportSGMGroup', '0', '0', '1', null, null);
INSERT INTO `neltool_groups` VALUES ('5', 'NevraxATSGroup', '0', '0', '1', null, null);
INSERT INTO `neltool_groups` VALUES ('6', 'SupportGMGroup', '0', '0', '1', null, null);
INSERT INTO `neltool_groups` VALUES ('7', 'SupportReadOnlyGroup', '0', '0', '1', null, null);
INSERT INTO `neltool_groups` VALUES ('8', 'NevraxLevelDesigners', '0', '0', '1', null, null);
INSERT INTO `neltool_groups` VALUES ('9', 'NevraxReadOnlyGroup', '0', '0', '1', '9', '56');
INSERT INTO `neltool_groups` VALUES ('10', 'YubDevGroup', '0', '0', '1', '12', '106');
INSERT INTO `neltool_groups` VALUES ('11', 'GuestGroup', '0', '0', '1', '12', '106');

-- ----------------------------
-- Table structure for `neltool_group_applications`
-- ----------------------------
DROP TABLE IF EXISTS `neltool_group_applications`;
CREATE TABLE `neltool_group_applications` (
  `group_application_id` int(11) NOT NULL AUTO_INCREMENT,
  `group_application_group_id` int(11) NOT NULL DEFAULT '0',
  `group_application_application_id` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`group_application_id`),
  KEY `group_application_group_id` (`group_application_group_id`),
  KEY `group_application_application_id` (`group_application_application_id`)
) ENGINE=MyISAM AUTO_INCREMENT=1003 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of neltool_group_applications
-- ----------------------------
INSERT INTO `neltool_group_applications` VALUES ('879', '1', '10');
INSERT INTO `neltool_group_applications` VALUES ('878', '1', '8');
INSERT INTO `neltool_group_applications` VALUES ('877', '1', '7');
INSERT INTO `neltool_group_applications` VALUES ('876', '1', '9');
INSERT INTO `neltool_group_applications` VALUES ('875', '1', '5');
INSERT INTO `neltool_group_applications` VALUES ('874', '1', '6');
INSERT INTO `neltool_group_applications` VALUES ('873', '1', '3');
INSERT INTO `neltool_group_applications` VALUES ('872', '1', '4');
INSERT INTO `neltool_group_applications` VALUES ('871', '1', '30');
INSERT INTO `neltool_group_applications` VALUES ('870', '1', '18');
INSERT INTO `neltool_group_applications` VALUES ('869', '1', '29');
INSERT INTO `neltool_group_applications` VALUES ('868', '1', '31');
INSERT INTO `neltool_group_applications` VALUES ('867', '1', '37');
INSERT INTO `neltool_group_applications` VALUES ('866', '1', '36');
INSERT INTO `neltool_group_applications` VALUES ('865', '1', '35');
INSERT INTO `neltool_group_applications` VALUES ('864', '1', '32');
INSERT INTO `neltool_group_applications` VALUES ('863', '1', '34');
INSERT INTO `neltool_group_applications` VALUES ('862', '1', '33');
INSERT INTO `neltool_group_applications` VALUES ('861', '1', '21');
INSERT INTO `neltool_group_applications` VALUES ('860', '1', '20');
INSERT INTO `neltool_group_applications` VALUES ('859', '1', '19');
INSERT INTO `neltool_group_applications` VALUES ('858', '1', '39');
INSERT INTO `neltool_group_applications` VALUES ('857', '1', '27');
INSERT INTO `neltool_group_applications` VALUES ('856', '1', '26');
INSERT INTO `neltool_group_applications` VALUES ('1001', '3', '10');
INSERT INTO `neltool_group_applications` VALUES ('1000', '3', '8');
INSERT INTO `neltool_group_applications` VALUES ('999', '3', '7');
INSERT INTO `neltool_group_applications` VALUES ('998', '3', '9');
INSERT INTO `neltool_group_applications` VALUES ('997', '3', '5');
INSERT INTO `neltool_group_applications` VALUES ('996', '3', '6');
INSERT INTO `neltool_group_applications` VALUES ('995', '3', '3');
INSERT INTO `neltool_group_applications` VALUES ('994', '3', '4');
INSERT INTO `neltool_group_applications` VALUES ('993', '3', '30');
INSERT INTO `neltool_group_applications` VALUES ('992', '3', '18');
INSERT INTO `neltool_group_applications` VALUES ('991', '3', '29');
INSERT INTO `neltool_group_applications` VALUES ('990', '3', '31');
INSERT INTO `neltool_group_applications` VALUES ('989', '3', '37');
INSERT INTO `neltool_group_applications` VALUES ('988', '3', '36');
INSERT INTO `neltool_group_applications` VALUES ('987', '3', '35');
INSERT INTO `neltool_group_applications` VALUES ('986', '3', '32');
INSERT INTO `neltool_group_applications` VALUES ('985', '3', '40');
INSERT INTO `neltool_group_applications` VALUES ('984', '3', '34');
INSERT INTO `neltool_group_applications` VALUES ('983', '3', '33');
INSERT INTO `neltool_group_applications` VALUES ('982', '3', '21');
INSERT INTO `neltool_group_applications` VALUES ('981', '3', '20');
INSERT INTO `neltool_group_applications` VALUES ('980', '3', '19');
INSERT INTO `neltool_group_applications` VALUES ('979', '3', '39');
INSERT INTO `neltool_group_applications` VALUES ('978', '3', '27');
INSERT INTO `neltool_group_applications` VALUES ('597', '4', '36');
INSERT INTO `neltool_group_applications` VALUES ('596', '4', '35');
INSERT INTO `neltool_group_applications` VALUES ('595', '4', '32');
INSERT INTO `neltool_group_applications` VALUES ('594', '4', '21');
INSERT INTO `neltool_group_applications` VALUES ('593', '4', '20');
INSERT INTO `neltool_group_applications` VALUES ('592', '4', '19');
INSERT INTO `neltool_group_applications` VALUES ('591', '4', '24');
INSERT INTO `neltool_group_applications` VALUES ('590', '4', '23');
INSERT INTO `neltool_group_applications` VALUES ('589', '4', '14');
INSERT INTO `neltool_group_applications` VALUES ('588', '4', '12');
INSERT INTO `neltool_group_applications` VALUES ('632', '2', '18');
INSERT INTO `neltool_group_applications` VALUES ('631', '2', '37');
INSERT INTO `neltool_group_applications` VALUES ('630', '2', '32');
INSERT INTO `neltool_group_applications` VALUES ('629', '2', '21');
INSERT INTO `neltool_group_applications` VALUES ('628', '2', '20');
INSERT INTO `neltool_group_applications` VALUES ('627', '2', '19');
INSERT INTO `neltool_group_applications` VALUES ('626', '2', '24');
INSERT INTO `neltool_group_applications` VALUES ('625', '2', '23');
INSERT INTO `neltool_group_applications` VALUES ('624', '2', '22');
INSERT INTO `neltool_group_applications` VALUES ('623', '2', '16');
INSERT INTO `neltool_group_applications` VALUES ('622', '2', '15');
INSERT INTO `neltool_group_applications` VALUES ('621', '2', '14');
INSERT INTO `neltool_group_applications` VALUES ('620', '2', '13');
INSERT INTO `neltool_group_applications` VALUES ('977', '3', '26');
INSERT INTO `neltool_group_applications` VALUES ('855', '1', '25');
INSERT INTO `neltool_group_applications` VALUES ('619', '2', '12');
INSERT INTO `neltool_group_applications` VALUES ('976', '3', '25');
INSERT INTO `neltool_group_applications` VALUES ('854', '1', '28');
INSERT INTO `neltool_group_applications` VALUES ('975', '3', '28');
INSERT INTO `neltool_group_applications` VALUES ('718', '5', '18');
INSERT INTO `neltool_group_applications` VALUES ('717', '5', '37');
INSERT INTO `neltool_group_applications` VALUES ('716', '5', '32');
INSERT INTO `neltool_group_applications` VALUES ('715', '5', '21');
INSERT INTO `neltool_group_applications` VALUES ('714', '5', '20');
INSERT INTO `neltool_group_applications` VALUES ('713', '5', '19');
INSERT INTO `neltool_group_applications` VALUES ('712', '5', '27');
INSERT INTO `neltool_group_applications` VALUES ('711', '5', '26');
INSERT INTO `neltool_group_applications` VALUES ('710', '5', '24');
INSERT INTO `neltool_group_applications` VALUES ('709', '5', '23');
INSERT INTO `neltool_group_applications` VALUES ('708', '5', '22');
INSERT INTO `neltool_group_applications` VALUES ('707', '5', '16');
INSERT INTO `neltool_group_applications` VALUES ('706', '5', '15');
INSERT INTO `neltool_group_applications` VALUES ('705', '5', '14');
INSERT INTO `neltool_group_applications` VALUES ('974', '3', '24');
INSERT INTO `neltool_group_applications` VALUES ('609', '6', '35');
INSERT INTO `neltool_group_applications` VALUES ('608', '6', '32');
INSERT INTO `neltool_group_applications` VALUES ('607', '6', '21');
INSERT INTO `neltool_group_applications` VALUES ('606', '6', '20');
INSERT INTO `neltool_group_applications` VALUES ('605', '6', '19');
INSERT INTO `neltool_group_applications` VALUES ('604', '6', '24');
INSERT INTO `neltool_group_applications` VALUES ('603', '6', '23');
INSERT INTO `neltool_group_applications` VALUES ('602', '6', '14');
INSERT INTO `neltool_group_applications` VALUES ('601', '6', '12');
INSERT INTO `neltool_group_applications` VALUES ('600', '6', '11');
INSERT INTO `neltool_group_applications` VALUES ('973', '3', '23');
INSERT INTO `neltool_group_applications` VALUES ('972', '3', '22');
INSERT INTO `neltool_group_applications` VALUES ('853', '1', '24');
INSERT INTO `neltool_group_applications` VALUES ('704', '5', '13');
INSERT INTO `neltool_group_applications` VALUES ('703', '5', '12');
INSERT INTO `neltool_group_applications` VALUES ('852', '1', '23');
INSERT INTO `neltool_group_applications` VALUES ('587', '4', '11');
INSERT INTO `neltool_group_applications` VALUES ('618', '2', '11');
INSERT INTO `neltool_group_applications` VALUES ('702', '5', '11');
INSERT INTO `neltool_group_applications` VALUES ('612', '7', '19');
INSERT INTO `neltool_group_applications` VALUES ('851', '1', '22');
INSERT INTO `neltool_group_applications` VALUES ('971', '3', '16');
INSERT INTO `neltool_group_applications` VALUES ('970', '3', '15');
INSERT INTO `neltool_group_applications` VALUES ('598', '4', '18');
INSERT INTO `neltool_group_applications` VALUES ('599', '4', '4');
INSERT INTO `neltool_group_applications` VALUES ('610', '6', '18');
INSERT INTO `neltool_group_applications` VALUES ('611', '6', '4');
INSERT INTO `neltool_group_applications` VALUES ('613', '7', '20');
INSERT INTO `neltool_group_applications` VALUES ('614', '7', '21');
INSERT INTO `neltool_group_applications` VALUES ('615', '7', '32');
INSERT INTO `neltool_group_applications` VALUES ('616', '7', '35');
INSERT INTO `neltool_group_applications` VALUES ('617', '7', '4');
INSERT INTO `neltool_group_applications` VALUES ('633', '2', '4');
INSERT INTO `neltool_group_applications` VALUES ('969', '3', '14');
INSERT INTO `neltool_group_applications` VALUES ('968', '3', '13');
INSERT INTO `neltool_group_applications` VALUES ('850', '1', '16');
INSERT INTO `neltool_group_applications` VALUES ('849', '1', '15');
INSERT INTO `neltool_group_applications` VALUES ('848', '1', '14');
INSERT INTO `neltool_group_applications` VALUES ('847', '1', '13');
INSERT INTO `neltool_group_applications` VALUES ('846', '1', '12');
INSERT INTO `neltool_group_applications` VALUES ('719', '5', '4');
INSERT INTO `neltool_group_applications` VALUES ('720', '8', '11');
INSERT INTO `neltool_group_applications` VALUES ('721', '8', '12');
INSERT INTO `neltool_group_applications` VALUES ('722', '8', '13');
INSERT INTO `neltool_group_applications` VALUES ('723', '8', '14');
INSERT INTO `neltool_group_applications` VALUES ('724', '8', '15');
INSERT INTO `neltool_group_applications` VALUES ('725', '8', '16');
INSERT INTO `neltool_group_applications` VALUES ('726', '8', '22');
INSERT INTO `neltool_group_applications` VALUES ('727', '8', '23');
INSERT INTO `neltool_group_applications` VALUES ('728', '8', '24');
INSERT INTO `neltool_group_applications` VALUES ('729', '8', '25');
INSERT INTO `neltool_group_applications` VALUES ('730', '8', '26');
INSERT INTO `neltool_group_applications` VALUES ('731', '8', '27');
INSERT INTO `neltool_group_applications` VALUES ('732', '8', '19');
INSERT INTO `neltool_group_applications` VALUES ('733', '8', '20');
INSERT INTO `neltool_group_applications` VALUES ('734', '8', '21');
INSERT INTO `neltool_group_applications` VALUES ('735', '8', '37');
INSERT INTO `neltool_group_applications` VALUES ('736', '8', '4');
INSERT INTO `neltool_group_applications` VALUES ('737', '9', '29');
INSERT INTO `neltool_group_applications` VALUES ('738', '9', '4');
INSERT INTO `neltool_group_applications` VALUES ('967', '3', '12');
INSERT INTO `neltool_group_applications` VALUES ('845', '1', '11');
INSERT INTO `neltool_group_applications` VALUES ('966', '3', '11');
INSERT INTO `neltool_group_applications` VALUES ('880', '1', '38');
INSERT INTO `neltool_group_applications` VALUES ('909', '10', '18');
INSERT INTO `neltool_group_applications` VALUES ('908', '10', '29');
INSERT INTO `neltool_group_applications` VALUES ('907', '10', '37');
INSERT INTO `neltool_group_applications` VALUES ('906', '10', '36');
INSERT INTO `neltool_group_applications` VALUES ('905', '10', '35');
INSERT INTO `neltool_group_applications` VALUES ('904', '10', '32');
INSERT INTO `neltool_group_applications` VALUES ('903', '10', '34');
INSERT INTO `neltool_group_applications` VALUES ('902', '10', '33');
INSERT INTO `neltool_group_applications` VALUES ('901', '10', '21');
INSERT INTO `neltool_group_applications` VALUES ('900', '10', '20');
INSERT INTO `neltool_group_applications` VALUES ('899', '10', '19');
INSERT INTO `neltool_group_applications` VALUES ('898', '10', '23');
INSERT INTO `neltool_group_applications` VALUES ('897', '10', '13');
INSERT INTO `neltool_group_applications` VALUES ('910', '10', '30');
INSERT INTO `neltool_group_applications` VALUES ('965', '11', '29');
INSERT INTO `neltool_group_applications` VALUES ('964', '11', '37');
INSERT INTO `neltool_group_applications` VALUES ('963', '11', '32');
INSERT INTO `neltool_group_applications` VALUES ('962', '11', '34');
INSERT INTO `neltool_group_applications` VALUES ('961', '11', '33');
INSERT INTO `neltool_group_applications` VALUES ('960', '11', '21');
INSERT INTO `neltool_group_applications` VALUES ('959', '11', '20');
INSERT INTO `neltool_group_applications` VALUES ('958', '11', '19');
INSERT INTO `neltool_group_applications` VALUES ('1002', '3', '38');

-- ----------------------------
-- Table structure for `neltool_group_domains`
-- ----------------------------
DROP TABLE IF EXISTS `neltool_group_domains`;
CREATE TABLE `neltool_group_domains` (
  `group_domain_id` int(11) NOT NULL AUTO_INCREMENT,
  `group_domain_group_id` int(11) NOT NULL DEFAULT '0',
  `group_domain_domain_id` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`group_domain_id`),
  KEY `group_domain_group_id` (`group_domain_group_id`)
) ENGINE=MyISAM AUTO_INCREMENT=91 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of neltool_group_domains
-- ----------------------------
INSERT INTO `neltool_group_domains` VALUES ('76', '1', '12');
INSERT INTO `neltool_group_domains` VALUES ('90', '3', '12');
INSERT INTO `neltool_group_domains` VALUES ('89', '11', '12');

-- ----------------------------
-- Table structure for `neltool_group_shards`
-- ----------------------------
DROP TABLE IF EXISTS `neltool_group_shards`;
CREATE TABLE `neltool_group_shards` (
  `group_shard_id` int(11) NOT NULL AUTO_INCREMENT,
  `group_shard_group_id` int(11) NOT NULL DEFAULT '0',
  `group_shard_shard_id` int(11) NOT NULL DEFAULT '0',
  `group_shard_domain_id` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`group_shard_id`),
  KEY `group_shard_group_id` (`group_shard_group_id`),
  KEY `group_shard_domain_id` (`group_shard_domain_id`)
) ENGINE=MyISAM AUTO_INCREMENT=1523 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of neltool_group_shards
-- ----------------------------
INSERT INTO `neltool_group_shards` VALUES ('1490', '1', '302', '12');
INSERT INTO `neltool_group_shards` VALUES ('1522', '3', '302', '12');
INSERT INTO `neltool_group_shards` VALUES ('1521', '11', '302', '12');

-- ----------------------------
-- Table structure for `neltool_locks`
-- ----------------------------
DROP TABLE IF EXISTS `neltool_locks`;
CREATE TABLE `neltool_locks` (
  `lock_id` int(11) NOT NULL AUTO_INCREMENT,
  `lock_domain_id` int(11) DEFAULT NULL,
  `lock_shard_id` int(11) DEFAULT NULL,
  `lock_user_name` varchar(32) NOT NULL DEFAULT '',
  `lock_date` int(11) NOT NULL DEFAULT '0',
  `lock_update` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`lock_id`),
  UNIQUE KEY `lock_shard_id` (`lock_shard_id`),
  UNIQUE KEY `lock_domain_id` (`lock_domain_id`)
) ENGINE=MyISAM AUTO_INCREMENT=19 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of neltool_locks
-- ----------------------------
INSERT INTO `neltool_locks` VALUES ('18', null, '302', 'admin', '1410346920', '1410346920');

-- ----------------------------
-- Table structure for `neltool_logs`
-- ----------------------------
DROP TABLE IF EXISTS `neltool_logs`;
CREATE TABLE `neltool_logs` (
  `logs_id` int(11) NOT NULL AUTO_INCREMENT,
  `logs_user_name` varchar(32) NOT NULL DEFAULT '0',
  `logs_date` int(11) NOT NULL DEFAULT '0',
  `logs_data` varchar(255) NOT NULL DEFAULT '',
  PRIMARY KEY (`logs_id`)
) ENGINE=MyISAM AUTO_INCREMENT=258 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of neltool_logs
-- ----------------------------

-- ----------------------------
-- Table structure for `neltool_notes`
-- ----------------------------
DROP TABLE IF EXISTS `neltool_notes`;
CREATE TABLE `neltool_notes` (
  `note_id` int(11) NOT NULL AUTO_INCREMENT,
  `note_user_id` int(11) NOT NULL DEFAULT '0',
  `note_title` varchar(128) NOT NULL DEFAULT '',
  `note_data` text NOT NULL,
  `note_date` int(11) NOT NULL DEFAULT '0',
  `note_active` int(11) NOT NULL DEFAULT '0',
  `note_global` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`note_id`)
) ENGINE=MyISAM AUTO_INCREMENT=11 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of neltool_notes
-- ----------------------------
INSERT INTO `neltool_notes` VALUES ('2', '27', 'Welcome', 'Welcome to the shard administration website!\r\n\r\nThis website is used to monitor and restart shards.\r\n\r\nIt also gives some player characters information.', '1272378065', '1', '1');
INSERT INTO `neltool_notes` VALUES ('3', '27', 'Shard Start', '# At the same time : NS and TS\r\n[1 min] : all MS, you can boot them all at the same time\r\n[1 min] : IOS\r\n[3 mins] : GMPS\r\n[3 mins] : EGS\r\n[5 mins] : AI Fyros\r\n[1 min 30] : AI Zorai\r\n[1 min 30] : AI Matis\r\n[1 min 30] : AI TNP\r\n[1 min 30] : AI NPE\r\n[1 min 30] : AI Tryker\r\n[1 min 30] : All FS and SBS at the same time\r\n[30 secs] : WS (atm the WS starts in OPEN mode by default, so be fast before CSR checkage, fix for that inc soon)\r\n\r\nNOTE: you can check the uptime for those timers in the right column of the admin tool: UpTime\r\n', '1158751126', '1', '0');
INSERT INTO `neltool_notes` VALUES ('5', '27', 'shutting supplementary', 'the writing wont change when lock the ws\r\n\r\nuntick previous boxes as you shut down\r\n\r\nwait 5 between the ws and the egs ie egs is 5 past rest is 10 past', '1153395380', '1', '0');
INSERT INTO `neltool_notes` VALUES ('4', '27', 'Shard Stop', '1. Broadcast to warn players\r\n\r\n2. 10 mins before shutdown, lock the WS\r\n\r\n3. At the right time shut down WS\r\n\r\n4. Shut down EGS\r\nOnly the EGS. Wait 5 reals minutes. Goal is to give enough time to egs, in order to save all the info he has to, and letting him sending those message to all services who need it.\r\n\r\n5. Shut down the rest, et voil&agrave;, you&#039;re done.', '1153314198', '1', '0');
INSERT INTO `neltool_notes` VALUES ('6', '27', 'Start (EGS to high?)', 'If [EGS] is to high on startup:\r\n\r\n[shut down egs]\r\n[5 mins]\r\n\r\n[IOS] &amp; [GPMS] (shut down at same time)\r\n\r\nAfter the services are down follow &quot;UP&quot; process with timers again.\r\n\r\nIOS\r\n[3 mins]\r\nGPMS\r\n[3 mins]\r\nEGS\r\n[5 mins]\r\nbla bla...', '1153395097', '1', '0');
INSERT INTO `neltool_notes` VALUES ('7', '27', 'opening if the egs is too high on reboot', '&lt;kadael&gt; here my note on admin about egs to high on startup\r\n&lt;kadael&gt; ---\r\n&lt;kadael&gt; If [EGS] is to high on startup:\r\n&lt;kadael&gt; [shut down egs]\r\n&lt;kadael&gt; [5 mins]\r\n&lt;kadael&gt; [IOS] &amp; [GPMS] (at same time shut down )\r\n&lt;kadael&gt; after the services are down follow &quot;UP&quot; process with timers again.\r\n&lt;kadael&gt; IOS\r\n&lt;kadael&gt; [3 mins]\r\n&lt;kadael&gt; GPMS\r\n&lt;kadael&gt; [3 mins]\r\n&lt;kadael&gt; EGS\r\n&lt;kadael&gt; [5 mins]\r\n&lt;kadael&gt; bla bla...\r\n&lt;kadael&gt; ---', '1153395362', '1', '0');
INSERT INTO `neltool_notes` VALUES ('10', '27', 'Ring points', 'Commande pour donner tout les points ring &agrave; tout le monde :\r\n\r\nDans le DSS d&#039;un Shard Ring entrer : DefaultCharRingAccess f7:j7:l6:d7:p13:g9:a9', '1155722296', '1', '0');
INSERT INTO `neltool_notes` VALUES ('9', '27', 'Start (EGS to high?)', 'If [EGS] is to high on startup: \r\n  \r\n [shut down egs] \r\n [5 mins] \r\n  \r\n [IOS] &amp; [GPMS] (shut down at same time) \r\n  \r\n After the services are down follow &quot;UP&quot; process with timers again. \r\n  \r\n IOS \r\n [3 mins] \r\n GPMS \r\n [3 mins] \r\n EGS \r\n [5 mins] \r\n bla bla...', '1153929658', '1', '0');

-- ----------------------------
-- Table structure for `neltool_restart_groups`
-- ----------------------------
DROP TABLE IF EXISTS `neltool_restart_groups`;
CREATE TABLE `neltool_restart_groups` (
  `restart_group_id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `restart_group_name` varchar(50) DEFAULT NULL,
  `restart_group_list` varchar(50) DEFAULT NULL,
  `restart_group_order` varchar(50) DEFAULT NULL,
  PRIMARY KEY (`restart_group_id`),
  UNIQUE KEY `restart_group_id` (`restart_group_id`),
  KEY `restart_group_id_2` (`restart_group_id`)
) ENGINE=MyISAM AUTO_INCREMENT=6 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of neltool_restart_groups
-- ----------------------------
INSERT INTO `neltool_restart_groups` VALUES ('1', 'Low Level', 'rns,ts,ms', '1');
INSERT INTO `neltool_restart_groups` VALUES ('3', 'Mid Level', 'ios,gpms,egs', '2');
INSERT INTO `neltool_restart_groups` VALUES ('4', 'High Level', 'ais', '3');
INSERT INTO `neltool_restart_groups` VALUES ('5', 'Front Level', 'fes,sbs,dss,rws', '4');

-- ----------------------------
-- Table structure for `neltool_restart_messages`
-- ----------------------------
DROP TABLE IF EXISTS `neltool_restart_messages`;
CREATE TABLE `neltool_restart_messages` (
  `restart_message_id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `restart_message_name` varchar(20) DEFAULT NULL,
  `restart_message_value` varchar(128) DEFAULT NULL,
  `restart_message_lang` varchar(5) DEFAULT NULL,
  PRIMARY KEY (`restart_message_id`),
  UNIQUE KEY `restart_message_id` (`restart_message_id`),
  KEY `restart_message_id_2` (`restart_message_id`)
) ENGINE=MyISAM AUTO_INCREMENT=11 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of neltool_restart_messages
-- ----------------------------
INSERT INTO `neltool_restart_messages` VALUES ('5', 'reboot', 'The shard is about to go down. Please find a safe location and log out.', 'en');
INSERT INTO `neltool_restart_messages` VALUES ('4', 'reboot', 'Le serveur va redemarrer dans $minutes$ minutes. Merci de vous deconnecter en lieu sur.', 'fr');
INSERT INTO `neltool_restart_messages` VALUES ('6', 'reboot', 'Der Server wird heruntergefahren. Findet eine sichere Stelle und logt aus.', 'de');
INSERT INTO `neltool_restart_messages` VALUES ('10', 'reboot', 'Arret du serveur dans $minutes+1$ minutes', 'fr');

-- ----------------------------
-- Table structure for `neltool_restart_sequences`
-- ----------------------------
DROP TABLE IF EXISTS `neltool_restart_sequences`;
CREATE TABLE `neltool_restart_sequences` (
  `restart_sequence_id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `restart_sequence_domain_id` int(10) unsigned NOT NULL DEFAULT '0',
  `restart_sequence_shard_id` int(10) unsigned NOT NULL DEFAULT '0',
  `restart_sequence_user_name` varchar(50) DEFAULT NULL,
  `restart_sequence_step` int(10) unsigned NOT NULL DEFAULT '0',
  `restart_sequence_date_start` int(11) DEFAULT NULL,
  `restart_sequence_date_end` int(11) DEFAULT NULL,
  `restart_sequence_timer` int(11) unsigned DEFAULT '0',
  PRIMARY KEY (`restart_sequence_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of neltool_restart_sequences
-- ----------------------------

-- ----------------------------
-- Table structure for `neltool_shards`
-- ----------------------------
DROP TABLE IF EXISTS `neltool_shards`;
CREATE TABLE `neltool_shards` (
  `shard_id` int(11) NOT NULL AUTO_INCREMENT,
  `shard_name` varchar(128) NOT NULL DEFAULT '',
  `shard_as_id` varchar(255) NOT NULL DEFAULT '0',
  `shard_domain_id` int(11) NOT NULL DEFAULT '0',
  `shard_lang` char(2) NOT NULL DEFAULT 'en',
  `shard_restart` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`shard_id`),
  KEY `shard_domain_id` (`shard_domain_id`)
) ENGINE=MyISAM AUTO_INCREMENT=303 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of neltool_shards
-- ----------------------------
INSERT INTO `neltool_shards` VALUES ('302', '测试一区', '测试一区', '12', 'en', '0');

-- ----------------------------
-- Table structure for `neltool_stats_hd_datas`
-- ----------------------------
DROP TABLE IF EXISTS `neltool_stats_hd_datas`;
CREATE TABLE `neltool_stats_hd_datas` (
  `hd_id` int(11) NOT NULL AUTO_INCREMENT,
  `hd_domain_id` int(11) NOT NULL DEFAULT '0',
  `hd_server` varchar(32) NOT NULL DEFAULT '',
  `hd_device` varchar(64) NOT NULL DEFAULT '',
  `hd_size` varchar(16) NOT NULL DEFAULT '',
  `hd_used` varchar(16) NOT NULL DEFAULT '',
  `hd_free` varchar(16) NOT NULL DEFAULT '',
  `hd_percent` int(11) NOT NULL DEFAULT '0',
  `hd_mount` varchar(128) NOT NULL DEFAULT '',
  PRIMARY KEY (`hd_id`),
  KEY `hd_domain_id` (`hd_domain_id`),
  KEY `hd_server` (`hd_server`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of neltool_stats_hd_datas
-- ----------------------------

-- ----------------------------
-- Table structure for `neltool_stats_hd_times`
-- ----------------------------
DROP TABLE IF EXISTS `neltool_stats_hd_times`;
CREATE TABLE `neltool_stats_hd_times` (
  `hd_domain_id` int(11) NOT NULL DEFAULT '0',
  `hd_last_time` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`hd_domain_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of neltool_stats_hd_times
-- ----------------------------

-- ----------------------------
-- Table structure for `neltool_users`
-- ----------------------------
DROP TABLE IF EXISTS `neltool_users`;
CREATE TABLE `neltool_users` (
  `user_id` int(11) NOT NULL AUTO_INCREMENT,
  `user_name` varchar(32) NOT NULL DEFAULT '',
  `user_password` varchar(64) NOT NULL DEFAULT '',
  `user_group_id` int(11) NOT NULL DEFAULT '0',
  `user_created` int(11) NOT NULL DEFAULT '0',
  `user_active` int(11) NOT NULL DEFAULT '0',
  `user_logged_last` int(11) NOT NULL DEFAULT '0',
  `user_logged_count` int(11) NOT NULL DEFAULT '0',
  `user_menu_style` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`user_id`),
  UNIQUE KEY `user_login` (`user_name`),
  KEY `user_group_id` (`user_group_id`),
  KEY `user_active` (`user_active`)
) ENGINE=MyISAM AUTO_INCREMENT=33 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of neltool_users
-- ----------------------------
INSERT INTO `neltool_users` VALUES ('27', 'admin', '084e0343a0486ff05530df6c705c8bb4', '3', '1213886454', '1', '1409731772', '383', '2');
INSERT INTO `neltool_users` VALUES ('32', 'guest', '084e0343a0486ff05530df6c705c8bb4', '11', '1272379014', '1', '1273335407', '273', '2');

-- ----------------------------
-- Table structure for `neltool_user_applications`
-- ----------------------------
DROP TABLE IF EXISTS `neltool_user_applications`;
CREATE TABLE `neltool_user_applications` (
  `user_application_id` int(11) NOT NULL AUTO_INCREMENT,
  `user_application_user_id` int(11) NOT NULL DEFAULT '0',
  `user_application_application_id` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`user_application_id`),
  KEY `user_application_user_id` (`user_application_user_id`),
  KEY `user_application_application_id` (`user_application_application_id`)
) ENGINE=MyISAM AUTO_INCREMENT=22 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of neltool_user_applications
-- ----------------------------
INSERT INTO `neltool_user_applications` VALUES ('8', '12', '33');
INSERT INTO `neltool_user_applications` VALUES ('20', '6', '31');
INSERT INTO `neltool_user_applications` VALUES ('19', '6', '34');
INSERT INTO `neltool_user_applications` VALUES ('9', '12', '31');
INSERT INTO `neltool_user_applications` VALUES ('21', '10', '34');

-- ----------------------------
-- Table structure for `neltool_user_domains`
-- ----------------------------
DROP TABLE IF EXISTS `neltool_user_domains`;
CREATE TABLE `neltool_user_domains` (
  `user_domain_id` int(11) NOT NULL AUTO_INCREMENT,
  `user_domain_user_id` int(11) NOT NULL DEFAULT '0',
  `user_domain_domain_id` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`user_domain_id`),
  KEY `user_domain_user_id` (`user_domain_user_id`)
) ENGINE=MyISAM AUTO_INCREMENT=20 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of neltool_user_domains
-- ----------------------------
INSERT INTO `neltool_user_domains` VALUES ('5', '6', '2');
INSERT INTO `neltool_user_domains` VALUES ('9', '22', '1');
INSERT INTO `neltool_user_domains` VALUES ('10', '23', '4');
INSERT INTO `neltool_user_domains` VALUES ('4', '12', '3');
INSERT INTO `neltool_user_domains` VALUES ('6', '6', '3');
INSERT INTO `neltool_user_domains` VALUES ('11', '23', '2');
INSERT INTO `neltool_user_domains` VALUES ('12', '23', '1');
INSERT INTO `neltool_user_domains` VALUES ('13', '23', '8');
INSERT INTO `neltool_user_domains` VALUES ('18', '15', '1');
INSERT INTO `neltool_user_domains` VALUES ('17', '15', '2');
INSERT INTO `neltool_user_domains` VALUES ('19', '31', '9');

-- ----------------------------
-- Table structure for `neltool_user_shards`
-- ----------------------------
DROP TABLE IF EXISTS `neltool_user_shards`;
CREATE TABLE `neltool_user_shards` (
  `user_shard_id` int(11) NOT NULL AUTO_INCREMENT,
  `user_shard_user_id` int(11) NOT NULL DEFAULT '0',
  `user_shard_shard_id` int(11) NOT NULL DEFAULT '0',
  `user_shard_domain_id` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`user_shard_id`),
  KEY `user_shard_user_id` (`user_shard_user_id`),
  KEY `user_shard_domain_id` (`user_shard_domain_id`)
) ENGINE=MyISAM AUTO_INCREMENT=166 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of neltool_user_shards
-- ----------------------------
INSERT INTO `neltool_user_shards` VALUES ('1', '8', '1', '1');
INSERT INTO `neltool_user_shards` VALUES ('2', '9', '2', '1');
INSERT INTO `neltool_user_shards` VALUES ('68', '7', '3', '1');
INSERT INTO `neltool_user_shards` VALUES ('143', '6', '4', '1');
INSERT INTO `neltool_user_shards` VALUES ('142', '6', '2', '1');
INSERT INTO `neltool_user_shards` VALUES ('141', '6', '45', '1');
INSERT INTO `neltool_user_shards` VALUES ('140', '6', '3', '1');
INSERT INTO `neltool_user_shards` VALUES ('90', '23', '26', '4');
INSERT INTO `neltool_user_shards` VALUES ('89', '23', '20', '4');
INSERT INTO `neltool_user_shards` VALUES ('13', '14', '1', '1');
INSERT INTO `neltool_user_shards` VALUES ('14', '14', '3', '1');
INSERT INTO `neltool_user_shards` VALUES ('15', '14', '2', '1');
INSERT INTO `neltool_user_shards` VALUES ('139', '6', '1', '1');
INSERT INTO `neltool_user_shards` VALUES ('74', '17', '2', '1');
INSERT INTO `neltool_user_shards` VALUES ('73', '17', '45', '1');
INSERT INTO `neltool_user_shards` VALUES ('72', '17', '3', '1');
INSERT INTO `neltool_user_shards` VALUES ('71', '17', '1', '1');
INSERT INTO `neltool_user_shards` VALUES ('70', '17', '18', '4');
INSERT INTO `neltool_user_shards` VALUES ('88', '23', '19', '4');
INSERT INTO `neltool_user_shards` VALUES ('87', '23', '24', '4');
INSERT INTO `neltool_user_shards` VALUES ('83', '23', '23', '4');
INSERT INTO `neltool_user_shards` VALUES ('82', '23', '22', '4');
INSERT INTO `neltool_user_shards` VALUES ('81', '23', '21', '4');
INSERT INTO `neltool_user_shards` VALUES ('34', '12', '15', '3');
INSERT INTO `neltool_user_shards` VALUES ('36', '18', '2', '1');
INSERT INTO `neltool_user_shards` VALUES ('138', '6', '7', '2');
INSERT INTO `neltool_user_shards` VALUES ('80', '23', '17', '4');
INSERT INTO `neltool_user_shards` VALUES ('79', '22', '45', '1');
INSERT INTO `neltool_user_shards` VALUES ('78', '22', '3', '1');
INSERT INTO `neltool_user_shards` VALUES ('77', '21', '45', '1');
INSERT INTO `neltool_user_shards` VALUES ('76', '21', '3', '1');
INSERT INTO `neltool_user_shards` VALUES ('75', '17', '4', '1');
INSERT INTO `neltool_user_shards` VALUES ('69', '7', '45', '1');
INSERT INTO `neltool_user_shards` VALUES ('146', '6', '54', '3');
INSERT INTO `neltool_user_shards` VALUES ('91', '23', '18', '4');
INSERT INTO `neltool_user_shards` VALUES ('92', '23', '7', '2');
INSERT INTO `neltool_user_shards` VALUES ('93', '23', '13', '2');
INSERT INTO `neltool_user_shards` VALUES ('94', '23', '8', '2');
INSERT INTO `neltool_user_shards` VALUES ('95', '23', '14', '2');
INSERT INTO `neltool_user_shards` VALUES ('145', '6', '53', '3');
INSERT INTO `neltool_user_shards` VALUES ('97', '23', '10', '2');
INSERT INTO `neltool_user_shards` VALUES ('144', '6', '15', '3');
INSERT INTO `neltool_user_shards` VALUES ('99', '23', '5', '1');
INSERT INTO `neltool_user_shards` VALUES ('100', '23', '6', '1');
INSERT INTO `neltool_user_shards` VALUES ('101', '23', '1', '1');
INSERT INTO `neltool_user_shards` VALUES ('102', '23', '3', '1');
INSERT INTO `neltool_user_shards` VALUES ('103', '23', '45', '1');
INSERT INTO `neltool_user_shards` VALUES ('104', '23', '46', '1');
INSERT INTO `neltool_user_shards` VALUES ('105', '23', '2', '1');
INSERT INTO `neltool_user_shards` VALUES ('106', '23', '42', '1');
INSERT INTO `neltool_user_shards` VALUES ('107', '23', '43', '1');
INSERT INTO `neltool_user_shards` VALUES ('108', '23', '44', '1');
INSERT INTO `neltool_user_shards` VALUES ('109', '23', '4', '1');
INSERT INTO `neltool_user_shards` VALUES ('110', '23', '41', '8');
INSERT INTO `neltool_user_shards` VALUES ('111', '23', '39', '8');
INSERT INTO `neltool_user_shards` VALUES ('112', '23', '30', '8');
INSERT INTO `neltool_user_shards` VALUES ('113', '23', '32', '8');
INSERT INTO `neltool_user_shards` VALUES ('114', '23', '47', '8');
INSERT INTO `neltool_user_shards` VALUES ('115', '23', '31', '8');
INSERT INTO `neltool_user_shards` VALUES ('116', '23', '36', '8');
INSERT INTO `neltool_user_shards` VALUES ('117', '23', '37', '8');
INSERT INTO `neltool_user_shards` VALUES ('118', '23', '40', '8');
INSERT INTO `neltool_user_shards` VALUES ('156', '15', '45', '1');
INSERT INTO `neltool_user_shards` VALUES ('155', '15', '3', '1');
INSERT INTO `neltool_user_shards` VALUES ('154', '15', '1', '1');
INSERT INTO `neltool_user_shards` VALUES ('153', '15', '6', '1');
INSERT INTO `neltool_user_shards` VALUES ('152', '15', '5', '1');
INSERT INTO `neltool_user_shards` VALUES ('151', '15', '10', '2');
INSERT INTO `neltool_user_shards` VALUES ('150', '15', '14', '2');
INSERT INTO `neltool_user_shards` VALUES ('149', '15', '8', '2');
INSERT INTO `neltool_user_shards` VALUES ('148', '15', '13', '2');
INSERT INTO `neltool_user_shards` VALUES ('147', '15', '7', '2');
INSERT INTO `neltool_user_shards` VALUES ('157', '15', '46', '1');
INSERT INTO `neltool_user_shards` VALUES ('158', '15', '2', '1');
INSERT INTO `neltool_user_shards` VALUES ('159', '15', '42', '1');
INSERT INTO `neltool_user_shards` VALUES ('160', '15', '43', '1');
INSERT INTO `neltool_user_shards` VALUES ('161', '15', '44', '1');
INSERT INTO `neltool_user_shards` VALUES ('162', '15', '4', '1');
INSERT INTO `neltool_user_shards` VALUES ('163', '31', '57', '9');
INSERT INTO `neltool_user_shards` VALUES ('164', '31', '104', '9');
INSERT INTO `neltool_user_shards` VALUES ('165', '31', '103', '9');
