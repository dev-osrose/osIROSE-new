-- MySQL dump 10.17  Distrib 10.3.12-MariaDB, for Linux (x86_64)
--
-- Host: localhost    Database: osirose
-- ------------------------------------------------------
-- Server version	10.3.12-MariaDB

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;
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
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `accounts` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `account_type` enum('system','user') COLLATE utf8_unicode_ci NOT NULL DEFAULT 'user',
  `username` varchar(64) COLLATE utf8_unicode_ci DEFAULT NULL,
  `password` varchar(256) COLLATE utf8_unicode_ci DEFAULT NULL,
  `salt` varchar(256) COLLATE utf8_unicode_ci DEFAULT NULL,
  `access` int(11) DEFAULT 1,
  `active` int(11) DEFAULT 1,
  `platinium` tinyint(1) DEFAULT 0,
  `online` tinyint(1) DEFAULT 0,
  `login_count` int(11) DEFAULT 0,
  `lastip` varchar(15) COLLATE utf8_unicode_ci DEFAULT '0.0.0.0',
  `lasttime` timestamp NOT NULL DEFAULT current_timestamp(),
  PRIMARY KEY (`id`),
  UNIQUE KEY `username_UNIQUE` (`username`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `accounts`
--

LOCK TABLES `accounts` WRITE;
/*!40000 ALTER TABLE `accounts` DISABLE KEYS */;
/*!40000 ALTER TABLE `accounts` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `characters`
--

DROP TABLE IF EXISTS `characters`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `characters` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `userid` int(11) unsigned NOT NULL DEFAULT 0,
  `name` varchar(24) NOT NULL,
  `race` int(10) unsigned NOT NULL DEFAULT 0,
  `level` int(10) unsigned NOT NULL DEFAULT 1,
  `job` int(11) unsigned DEFAULT 0,
  `clanid` int(11) unsigned NOT NULL DEFAULT 0,
  `clan_contribution` int(11) unsigned NOT NULL DEFAULT 0,
  `clan_rank` int(11) unsigned NOT NULL DEFAULT 0,
  `factionid` int(11) unsigned NOT NULL DEFAULT 0,
  `faction_rank` int(11) unsigned NOT NULL DEFAULT 0,
  `fame` int(11) unsigned NOT NULL DEFAULT 0,
  `faction_fame1` int(11) unsigned NOT NULL DEFAULT 0,
  `faction_fame2` int(11) unsigned NOT NULL DEFAULT 0,
  `faction_points1` int(11) unsigned NOT NULL DEFAULT 0,
  `faction_points2` int(11) unsigned NOT NULL DEFAULT 0,
  `faction_points3` int(11) unsigned NOT NULL DEFAULT 0,
  `face` int(11) unsigned NOT NULL DEFAULT 0,
  `hair` int(11) unsigned NOT NULL DEFAULT 0,
  `stone` int(11) unsigned NOT NULL DEFAULT 0,
  `exp` bigint(20) unsigned NOT NULL DEFAULT 0,
  `zuly` int(10) unsigned NOT NULL DEFAULT 0,
  `current_hp` int(10) unsigned NOT NULL DEFAULT 81,
  `current_mp` int(10) unsigned NOT NULL DEFAULT 55,
  `max_hp` int(10) unsigned NOT NULL DEFAULT 81,
  `max_mp` int(10) unsigned NOT NULL DEFAULT 55,
  `stamina` int(11) unsigned NOT NULL DEFAULT 0,
  `str` int(10) unsigned NOT NULL DEFAULT 10,
  `dex` int(10) unsigned NOT NULL DEFAULT 10,
  `int_` int(10) unsigned NOT NULL DEFAULT 10,
  `con` int(10) unsigned NOT NULL DEFAULT 10,
  `charm` int(10) unsigned NOT NULL DEFAULT 10,
  `sense` int(10) unsigned NOT NULL DEFAULT 10,
  `stat_points` int(10) unsigned NOT NULL DEFAULT 0,
  `skill_points` int(10) unsigned NOT NULL DEFAULT 0,
  `penalty_exp` bigint(20) unsigned DEFAULT 0,
  `revive_map` int(11) NOT NULL DEFAULT 1,
  `map` int(11) NOT NULL DEFAULT 20,
  `x` bigint(32) NOT NULL DEFAULT 520000,
  `y` bigint(32) NOT NULL DEFAULT 520000,
  `delete_date` datetime DEFAULT NULL COMMENT 'Time until the character gets deleted',
  `pk_flag` int(10) NOT NULL DEFAULT 0,
  PRIMARY KEY (`id`),
  UNIQUE KEY `id_UNIQUE` (`id`),
  UNIQUE KEY `name_UNIQUE` (`name`),
  KEY `user_idx` (`userid`),
  CONSTRAINT `characters_ibfk_1` FOREIGN KEY (`userid`) REFERENCES `accounts` (`id`) ON DELETE CASCADE ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `characters`
--

LOCK TABLES `characters` WRITE;
/*!40000 ALTER TABLE `characters` DISABLE KEYS */;
/*!40000 ALTER TABLE `characters` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `friends`
--

DROP TABLE IF EXISTS `friends`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `friends` (
  `char_id` int(10) unsigned NOT NULL DEFAULT 0,
  `friend_id` int(10) unsigned NOT NULL DEFAULT 0,
  PRIMARY KEY (`char_id`),
  KEY `char_id_idx` (`char_id`,`friend_id`),
  CONSTRAINT `friends_ibfk_1` FOREIGN KEY (`char_id`) REFERENCES `characters` (`id`) ON DELETE CASCADE ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `friends`
--

LOCK TABLES `friends` WRITE;
/*!40000 ALTER TABLE `friends` DISABLE KEYS */;
/*!40000 ALTER TABLE `friends` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `inventory`
--

DROP TABLE IF EXISTS `inventory`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `inventory` (
  `uid` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `char_id` int(11) unsigned NOT NULL DEFAULT 0,
  `itemid` int(11) unsigned NOT NULL DEFAULT 0,
  `itemtype` int(11) unsigned NOT NULL DEFAULT 0,
  `amount` int(11) unsigned NOT NULL DEFAULT 0,
  `refine` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `slot` int(11) unsigned NOT NULL DEFAULT 0,
  `gem_opt` int(11) unsigned NOT NULL DEFAULT 0,
  `socket` int(10) unsigned NOT NULL DEFAULT 0,
  `price` int(11) unsigned NOT NULL DEFAULT 0,
  `storage_type` enum('inventory','wishlist','storage') NOT NULL DEFAULT 'inventory',
  PRIMARY KEY (`uid`),
  UNIQUE KEY `id_UNIQUE` (`uid`),
  KEY `char_id_idx` (`char_id`),
  CONSTRAINT `inventory_ibfk_1` FOREIGN KEY (`char_id`) REFERENCES `characters` (`id`) ON DELETE CASCADE ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `inventory`
--

LOCK TABLES `inventory` WRITE;
/*!40000 ALTER TABLE `inventory` DISABLE KEYS */;
/*!40000 ALTER TABLE `inventory` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `item_db`
--

DROP TABLE IF EXISTS `item_db`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `item_db` (
  `type` tinyint(2) NOT NULL DEFAULT 0,
  `id` smallint(5) unsigned NOT NULL DEFAULT 0,
  `name` varchar(50) NOT NULL DEFAULT '',
  `desc` varchar(500) NOT NULL DEFAULT '',
  `subtype` smallint(3) NOT NULL DEFAULT 0,
  `price_buy` mediumint(10) unsigned DEFAULT NULL,
  `price_sell` mediumint(10) unsigned DEFAULT NULL,
  `weight` smallint(5) unsigned NOT NULL DEFAULT 0,
  `attack` smallint(3) unsigned DEFAULT NULL,
  `defense` smallint(3) unsigned DEFAULT NULL,
  `range` smallint(2) unsigned DEFAULT NULL,
  `slots` tinyint(2) unsigned DEFAULT NULL,
  `equip_jobs` int(12) unsigned DEFAULT NULL,
  `equip_genders` tinyint(2) unsigned DEFAULT NULL,
  `equip_level` tinyint(3) unsigned DEFAULT NULL,
  `refinable` tinyint(1) unsigned DEFAULT NULL,
  `view_id` smallint(3) unsigned DEFAULT NULL,
  `script` text DEFAULT NULL,
  PRIMARY KEY (`type`,`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `item_db`
--

LOCK TABLES `item_db` WRITE;
/*!40000 ALTER TABLE `item_db` DISABLE KEYS */;
/*!40000 ALTER TABLE `item_db` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `list_clan`
--

DROP TABLE IF EXISTS `list_clan`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
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
-- Dumping data for table `list_clan`
--

LOCK TABLES `list_clan` WRITE;
/*!40000 ALTER TABLE `list_clan` DISABLE KEYS */;
/*!40000 ALTER TABLE `list_clan` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `party`
--

DROP TABLE IF EXISTS `party`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `party` (
  `party_id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `name` varchar(24) NOT NULL DEFAULT '',
  `leader_id` int(11) NOT NULL DEFAULT 0 COMMENT 'Leader''s Account ID',
  `leader_char` int(11) NOT NULL DEFAULT 0 COMMENT 'Character ID',
  PRIMARY KEY (`party_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Parties persist until they are disbanded.';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `party`
--

LOCK TABLES `party` WRITE;
/*!40000 ALTER TABLE `party` DISABLE KEYS */;
/*!40000 ALTER TABLE `party` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `sessions`
--

DROP TABLE IF EXISTS `sessions`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `sessions` (
  `id` int(10) NOT NULL,
  `userid` int(10) NOT NULL,
  `channelid` int(11) DEFAULT NULL,
  `time` timestamp NOT NULL DEFAULT current_timestamp(),
  `charid` int(10) DEFAULT NULL,
  `worldip` varchar(20) DEFAULT NULL,
  `worldport` int(20) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `sessions`
--

LOCK TABLES `sessions` WRITE;
/*!40000 ALTER TABLE `sessions` DISABLE KEYS */;
/*!40000 ALTER TABLE `sessions` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `skill`
--

DROP TABLE IF EXISTS `skill`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `skill` (
  `char_id` int(10) unsigned NOT NULL DEFAULT 0,
  `id` smallint(5) unsigned NOT NULL DEFAULT 0,
  `level` tinyint(3) unsigned NOT NULL DEFAULT 0,
  PRIMARY KEY (`char_id`,`id`),
  CONSTRAINT `skill_ibfk_1` FOREIGN KEY (`char_id`) REFERENCES `characters` (`id`) ON DELETE CASCADE ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `skill`
--

LOCK TABLES `skill` WRITE;
/*!40000 ALTER TABLE `skill` DISABLE KEYS */;
/*!40000 ALTER TABLE `skill` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `skill_db`
--

DROP TABLE IF EXISTS `skill_db`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `skill_db` (
  `id` int(10) unsigned NOT NULL,
  `name` varchar(45) NOT NULL,
  `desc` varchar(45) DEFAULT '',
  `level` varchar(45) DEFAULT '0',
  `type` varchar(45) NOT NULL,
  `range` int(11) DEFAULT NULL,
  `class_` set('0') DEFAULT '0',
  `power` int(11) DEFAULT NULL,
  `script` text DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `id_UNIQUE` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `skill_db`
--

LOCK TABLES `skill_db` WRITE;
/*!40000 ALTER TABLE `skill_db` DISABLE KEYS */;
/*!40000 ALTER TABLE `skill_db` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2019-05-13 21:01:53
