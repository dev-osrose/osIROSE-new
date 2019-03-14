-- MySQL dump 10.13  Distrib 8.0.11, for Win64 (x86_64)
--
-- Host: 192.168.1.80    Database: osirose
-- ------------------------------------------------------
-- Server version	8.0.13

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
 SET NAMES utf8 ;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `accounts`
--

DROP TABLE IF EXISTS `accounts`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
 SET character_set_client = utf8mb4 ;
CREATE TABLE `accounts` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `account_type` enum('system','user') CHARACTER SET utf8 COLLATE utf8_unicode_ci NOT NULL DEFAULT 'user',
  `username` varchar(64) COLLATE utf8_unicode_ci DEFAULT NULL,
  `password` varchar(256) COLLATE utf8_unicode_ci DEFAULT NULL,
  `salt` varchar(256) COLLATE utf8_unicode_ci DEFAULT NULL,
  `access` int(11) DEFAULT '1',
  `active` int(11) DEFAULT '1',
  `platinium` tinyint(1) DEFAULT '0',
  `online` tinyint(1) DEFAULT '0',
  `login_count` int(11) DEFAULT '0',
  `lastip` varchar(15) COLLATE utf8_unicode_ci DEFAULT '0.0.0.0',
  `lasttime` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  UNIQUE KEY `username_UNIQUE` (`username`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `characters`
--

DROP TABLE IF EXISTS `characters`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
 SET character_set_client = utf8mb4 ;
CREATE TABLE `characters` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `userid` int(11) unsigned NOT NULL DEFAULT '0',
  `name` varchar(24) NOT NULL,
  `race` int(10) unsigned NOT NULL DEFAULT '0',
  `level` int(10) unsigned NOT NULL DEFAULT '1',
  `job` int(11) unsigned DEFAULT '0',
  `clanid` int(11) unsigned NOT NULL DEFAULT '0',
  `clan_contribution` int(11) unsigned NOT NULL DEFAULT '0',
  `clan_rank` int(11) unsigned NOT NULL DEFAULT '0',
  `factionid` int(11) unsigned NOT NULL DEFAULT '0',
  `faction_rank` int(11) unsigned NOT NULL DEFAULT '0',
  `fame` int(11) unsigned NOT NULL DEFAULT '0',
  `faction_fame1` int(11) unsigned NOT NULL DEFAULT '0',
  `faction_fame2` int(11) unsigned NOT NULL DEFAULT '0',
  `faction_points1` int(11) unsigned NOT NULL DEFAULT '0',
  `faction_points2` int(11) unsigned NOT NULL DEFAULT '0',
  `faction_points3` int(11) unsigned NOT NULL DEFAULT '0',
  `face` int(11) unsigned NOT NULL DEFAULT '0',
  `hair` int(11) unsigned NOT NULL DEFAULT '0',
  `stone` int(11) unsigned NOT NULL DEFAULT '0',
  `exp` bigint(20) unsigned NOT NULL DEFAULT '0',
  `zuly` int(10) unsigned NOT NULL DEFAULT '0',
  `current_hp` int(10) unsigned NOT NULL DEFAULT '81',
  `current_mp` int(10) unsigned NOT NULL DEFAULT '55',
  `max_hp` int(10) unsigned NOT NULL DEFAULT '81',
  `max_mp` int(10) unsigned NOT NULL DEFAULT '55',
  `stamina` int(11) unsigned NOT NULL DEFAULT '0',
  `str` int(10) unsigned NOT NULL DEFAULT '10',
  `dex` int(10) unsigned NOT NULL DEFAULT '10',
  `int_` int(10) unsigned NOT NULL DEFAULT '10',
  `con` int(10) unsigned NOT NULL DEFAULT '10',
  `charm` int(10) unsigned NOT NULL DEFAULT '10',
  `sense` int(10) unsigned NOT NULL DEFAULT '10',
  `stat_points` int(10) unsigned NOT NULL DEFAULT '0',
  `skill_points` int(10) unsigned NOT NULL DEFAULT '0',
  `penalty_exp` bigint(20) unsigned DEFAULT '0',
  `revive_map` int(11) NOT NULL DEFAULT '1',
  `map` int(11) NOT NULL DEFAULT '20',
  `x` bigint(32) NOT NULL DEFAULT '520000',
  `y` bigint(32) NOT NULL DEFAULT '520000',
  `delete_date` datetime DEFAULT NULL COMMENT 'Time until the character gets deleted',
  `pk_flag` int(10) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  UNIQUE KEY `id_UNIQUE` (`id`),
  UNIQUE KEY `name_UNIQUE` (`name`),
  KEY `user_idx` (`userid`),
  CONSTRAINT `user` FOREIGN KEY (`userid`) REFERENCES `accounts` (`id`) ON DELETE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `friends`
--

DROP TABLE IF EXISTS `friends`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
 SET character_set_client = utf8mb4 ;
CREATE TABLE `friends` (
  `char_id` int(10) unsigned NOT NULL DEFAULT '0',
  `friend_id` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`char_id`),
  KEY `char_id_idx` (`char_id`,`friend_id`),
  CONSTRAINT `charid` FOREIGN KEY (`char_id`) REFERENCES `characters` (`id`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `inventory`
--

DROP TABLE IF EXISTS `inventory`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
 SET character_set_client = utf8mb4 ;
CREATE TABLE `inventory` (
  `uid` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `char_id` int(11) unsigned NOT NULL DEFAULT '0',
  `itemid` int(11) unsigned NOT NULL DEFAULT '0',
  `itemtype` int(11) unsigned NOT NULL DEFAULT '0',
  `amount` int(11) unsigned NOT NULL DEFAULT '0',
  `refine` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `slot` int(11) unsigned NOT NULL DEFAULT '0',
  `gem_opt` int(11) unsigned NOT NULL DEFAULT '0',
  `socket` int(10) unsigned NOT NULL DEFAULT '0',
  `price` int(11) unsigned NOT NULL DEFAULT '0',
  `storage_type` enum('inventory','wishlist','storage') NOT NULL DEFAULT 'inventory',
  PRIMARY KEY (`uid`),
  UNIQUE KEY `id_UNIQUE` (`uid`),
  KEY `char_id_idx` (`char_id`),
  CONSTRAINT `char_id` FOREIGN KEY (`char_id`) REFERENCES `characters` (`id`) ON DELETE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `item_db`
--

DROP TABLE IF EXISTS `item_db`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
 SET character_set_client = utf8mb4 ;
CREATE TABLE `item_db` (
  `type` tinyint(2) NOT NULL DEFAULT '0',
  `id` smallint(5) unsigned NOT NULL DEFAULT '0',
  `name` varchar(50) NOT NULL DEFAULT '',
  `desc` varchar(500) NOT NULL DEFAULT '',
  `subtype` smallint(3) NOT NULL DEFAULT '0',
  `price_buy` mediumint(10) unsigned DEFAULT NULL,
  `price_sell` mediumint(10) unsigned DEFAULT NULL,
  `weight` smallint(5) unsigned NOT NULL DEFAULT '0',
  `attack` smallint(3) unsigned DEFAULT NULL,
  `defense` smallint(3) unsigned DEFAULT NULL,
  `range` smallint(2) unsigned DEFAULT NULL,
  `slots` tinyint(2) unsigned DEFAULT NULL,
  `equip_jobs` int(12) unsigned DEFAULT NULL,
  `equip_genders` tinyint(2) unsigned DEFAULT NULL,
  `equip_level` tinyint(3) unsigned DEFAULT NULL,
  `refinable` tinyint(1) unsigned DEFAULT NULL,
  `view_id` smallint(3) unsigned DEFAULT NULL,
  `script` text,
  PRIMARY KEY (`type`,`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `list_clan`
--

DROP TABLE IF EXISTS `list_clan`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
 SET character_set_client = utf8mb4 ;
CREATE TABLE `list_clan` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `logo` int(11) DEFAULT NULL,
  `back` int(11) DEFAULT NULL,
  `name` varchar(50) DEFAULT NULL,
  `cp` int(11) DEFAULT NULL,
  `grade` int(11) DEFAULT NULL,
  `slogan` varchar(100) DEFAULT NULL,
  `news` varchar(200) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `party`
--

DROP TABLE IF EXISTS `party`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
 SET character_set_client = utf8mb4 ;
CREATE TABLE `party` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `name` varchar(24) NOT NULL DEFAULT '',
  `leader_id` int(11) NOT NULL DEFAULT '0' COMMENT 'Leader''s char ID',
  `options` int(11) NOT NULL DEFAULT '0',
  `level` int(11) NOT NULL DEFAULT '1',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Parties persist until they are disbanded.';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `party_members`
--

DROP TABLE IF EXISTS `party_members`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `party_members` (
  `id` int(10) unsigned NOT NULL,
  `member_id` int(11) NOT NULL,
  `rank` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT 'Sort by oldest first',
  PRIMARY KEY (`id`, `member_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;


--
-- Table structure for table `sessions`
--

DROP TABLE IF EXISTS `sessions`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
 SET character_set_client = utf8mb4 ;
CREATE TABLE `sessions` (
  `id` int(10) NOT NULL,
  `userid` int(10) NOT NULL,
  `channelid` int(11) DEFAULT NULL,
  `time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `charid` int(10) DEFAULT NULL,
  `worldip` varchar(20) DEFAULT NULL,
  `worldport` int(20) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `skill`
--

DROP TABLE IF EXISTS `skill`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
 SET character_set_client = utf8mb4 ;
CREATE TABLE `skill` (
  `char_id` int(10) unsigned NOT NULL DEFAULT '0',
  `id` smallint(5) unsigned NOT NULL DEFAULT '0',
  `level` tinyint(3) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`char_id`,`id`),
  CONSTRAINT `id` FOREIGN KEY (`char_id`) REFERENCES `characters` (`id`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `skill_db`
--

DROP TABLE IF EXISTS `skill_db`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
 SET character_set_client = utf8mb4 ;
CREATE TABLE `skill_db` (
  `id` int(10) unsigned NOT NULL,
  `name` varchar(45) NOT NULL,
  `desc` varchar(5000) DEFAULT '',
  `level` varchar(45) DEFAULT '0',
  `type` varchar(45) NOT NULL,
  `range` int(11) DEFAULT NULL,
  `class_` set('0','1','3','5') DEFAULT '0',
  `power` int(11) DEFAULT NULL,
  `script` text,
  PRIMARY KEY (`id`),
  UNIQUE KEY `id_UNIQUE` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `storage`
--

DROP TABLE IF EXISTS `storage`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
 SET character_set_client = utf8mb4 ;
CREATE TABLE `storage` (
  `owner` int(11) NOT NULL,
  `itemnum` int(11) NOT NULL,
  `itemtype` int(11) NOT NULL,
  `refine` int(11) NOT NULL,
  `durability` int(11) NOT NULL DEFAULT '40',
  `lifespan` int(11) NOT NULL DEFAULT '100',
  `slotnum` int(11) NOT NULL,
  `count` int(11) NOT NULL DEFAULT '1',
  `stats` int(11) NOT NULL DEFAULT '0',
  `socketed` int(11) NOT NULL DEFAULT '0',
  `appraised` int(11) NOT NULL DEFAULT '0',
  `gem` int(11) NOT NULL DEFAULT '0'
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `wishlist`
--

DROP TABLE IF EXISTS `wishlist`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
 SET character_set_client = utf8mb4 ;
CREATE TABLE `wishlist` (
  `uid` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `char_id` int(11) unsigned NOT NULL DEFAULT '0',
  `itemid` int(11) unsigned NOT NULL DEFAULT '0',
  `itemtype` int(11) unsigned NOT NULL DEFAULT '0',
  `amount` int(11) unsigned NOT NULL DEFAULT '0',
  `refine` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `slot` int(11) unsigned NOT NULL DEFAULT '0',
  `gem_opt` int(11) unsigned NOT NULL DEFAULT '0',
  `socket` int(10) unsigned NOT NULL DEFAULT '0',
  `price` int(11) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`uid`),
  UNIQUE KEY `id_UNIQUE` (`uid`),
  KEY `char_id_idx` (`char_id`),
  CONSTRAINT `wish_char_id` FOREIGN KEY (`char_id`) REFERENCES `characters` (`id`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping events for database 'osirose'
--

--
-- Dumping routines for database 'osirose'
--
/*!50003 DROP FUNCTION IF EXISTS `character_exists` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_unicode_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` FUNCTION `character_exists`(`_char` VARCHAR(24)) RETURNS int(11)
    READS SQL DATA
BEGIN
  
  if (select * from characters where name = _char) > 0
  then
    return 1;
  end if;
  
-- We were unable to find a character with that name
return 0;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP FUNCTION IF EXISTS `create_salt` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_unicode_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` FUNCTION `create_salt`() RETURNS varchar(256) CHARSET utf8
    READS SQL DATA
BEGIN
SET @salt = SHA2( SHA2(RAND(), 256), 256 );
RETURN @salt;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `create_account` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_unicode_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `create_account`(IN `_user` VARCHAR(24), IN `_pass` VARCHAR(64))
    MODIFIES SQL DATA
BEGIN
SET @salt = create_salt();
INSERT INTO accounts(username, password, salt) VALUES(_user, SHA2( CONCAT(_pass,@salt), 256), @salt);
SET @salt = NULL;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `create_char` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_unicode_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `create_char`(IN `_name` VARCHAR(24), IN `_userid` INT, IN `_race` INT, IN `_face` INT, IN `_hair` INT, IN `_stone` INT)
    MODIFIES SQL DATA
BEGIN
  DECLARE charid INT(10);
  DECLARE hatuid INT(10);
	insert into characters(userid, name, race, face, hair, stone) values(_userid, _name, _race, _face, _hair, _stone);
	SET charid = LAST_INSERT_ID();
	insert into inventory(char_id, itemid, itemtype, amount, slot) values(charid, 30, 3, 1, 3);
	insert into inventory(char_id, itemid, itemtype, amount, slot) values(charid, 1, 8, 1, 7);
	
	if _race = 0 then SET hatuid = 222;
	else SET hatuid = 221;
	end if;
	
	insert into inventory(char_id, itemid, itemtype, amount, slot) values(charid, hatuid, 2, 1, 12);
	
	insert into skill(char_id, id, level) values(charid, 11, 1);
	insert into skill(char_id, id, level) values(charid, 12, 1);
	insert into skill(char_id, id, level) values(charid, 16, 1);
	insert into skill(char_id, id, level) values(charid, 19, 1);
	insert into skill(char_id, id, level) values(charid, 20, 1);
	insert into skill(char_id, id, level) values(charid, 21, 1);
	insert into skill(char_id, id, level) values(charid, 26, 1);
	insert into skill(char_id, id, level) values(charid, 41, 1);
	insert into skill(char_id, id, level) values(charid, 42, 1);
	insert into skill(char_id, id, level) values(charid, 43, 1);
	insert into skill(char_id, id, level) values(charid, 46, 1);
	insert into skill(char_id, id, level) values(charid, 47, 1);
	insert into skill(char_id, id, level) values(charid, 48, 1);
	insert into skill(char_id, id, level) values(charid, 49, 1);
	insert into skill(char_id, id, level) values(charid, 50, 1);
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `create_session` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_unicode_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `create_session`(IN `_sessionid` INT, IN `_userid` INT, IN `_channelid` INT)
    MODIFIES SQL DATA
BEGIN
  INSERT into sessions(id, userid, channelid, time) values(_sessionid, _userid, _channelid, now());
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `delete_character` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_unicode_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `delete_character`(IN `_charid` INT, IN `_name` VARCHAR(20) CHARSET utf8, IN `_delete` INT)
    MODIFIES SQL DATA
BEGIN
  DECLARE _delete_date datetime;
  set _delete_date=NULL;
  set time_zone = '+00:00';
  
  if _delete > 1 then SET _delete_date = NOW();
  elseif _delete = 1 then SET _delete_date = NOW() + INTERVAL 1 DAY;
  end if;
	
  UPDATE `characters` 
  SET delete_date = _delete_date
  WHERE characters.id = _charid AND characters.name = _name;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `get_character` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_unicode_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `get_character`(IN `_charid` INT)
    READS SQL DATA
BEGIN
  SELECT * FROM `characters` WHERE characters.id = _charid;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `get_character_list` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_unicode_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `get_character_list`(IN `_userid` INT)
    READS SQL DATA
BEGIN
  set time_zone = '+00:00';
  delete from characters where characters.delete_date != 0 AND characters.delete_date <= NOW() AND userid = _userid;
  SELECT * FROM characters WHERE userid = _userid ;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `get_equipped` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_unicode_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `get_equipped`(IN `_charid` INT(11))
    READS SQL DATA
BEGIN
  SELECT * FROM inventory WHERE char_id = _charid AND slot < 10;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `get_inventory` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_unicode_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `get_inventory`(IN `_charid` INT(11))
    READS SQL DATA
BEGIN
  SELECT * FROM inventory WHERE char_id = _charid;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `get_session` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_unicode_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `get_session`(IN `_sessionid` INT, IN `_pass` VARCHAR(32) CHARSET utf8)
    READS SQL DATA
BEGIN
  SELECT sessions.userid, sessions.channelid, sessions.charid, accounts.platinium
  FROM `sessions`
  LEFT JOIN `accounts` on sessions.userid = accounts.id 
  WHERE sessions.id = _sessionid AND password = SHA2( CONCAT(_pass,accounts.salt), 256);
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `get_skills` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_unicode_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `get_skills`(IN `_charid` INT(11))
    READS SQL DATA
BEGIN
  SELECT * FROM skill WHERE char_id = _charid;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `update_session_with_character` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_unicode_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `update_session_with_character`(IN `_sessionid` INT, IN `_charid` INT)
    MODIFIES SQL DATA
BEGIN
  UPDATE `sessions` 
  SET charid = _charid
  WHERE sessions.id = _sessionid;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `user_exists` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_unicode_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `user_exists`(IN `_user` VARCHAR(24))
    READS SQL DATA
BEGIN
  SELECT id from accounts where username = _user;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `user_login` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_unicode_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `user_login`(IN `_user` VARCHAR(16) CHARSET utf8, IN `_pass` VARCHAR(64) CHARSET utf8)
    READS SQL DATA
BEGIN
  SELECT id, password, access, active, `online` FROM accounts WHERE username = _user COLLATE utf8_unicode_ci AND password = SHA2( CONCAT(_pass,salt), 256) COLLATE utf8_unicode_ci;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2019-05-11  0:16:28
