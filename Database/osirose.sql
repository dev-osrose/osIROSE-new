-- MySQL dump 10.13  Distrib 5.7.9, for Win64 (x86_64)
--
-- Host: localhost    Database: osirose
-- ------------------------------------------------------
-- Server version	5.7.9-log

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
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
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `username` varchar(64) DEFAULT NULL,
  `password` varchar(32) DEFAULT NULL,
  `access` int(11) DEFAULT '100',
  `lastip` varchar(15) DEFAULT '0.0.0.0',
  `lasttime` int(11) DEFAULT '0',
  `lastsvr` int(11) DEFAULT '0',
  `lastchar` varchar(64) DEFAULT NULL,
  `email` varchar(100) DEFAULT NULL,
  `nb_donation` int(11) DEFAULT '0',
  `donation` varchar(255) DEFAULT '0',
  `active` int(11) DEFAULT '1',
  `active_key` varchar(255) DEFAULT NULL,
  `zulystorage` int(11) DEFAULT '0',
  `platinum` tinyint(1) DEFAULT '0',
  `online` tinyint(1) DEFAULT '0',
  `login_count` int(11) DEFAULT '0',
  `isSiteLogged` tinyint(1) DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=ascii;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `characters`
--

DROP TABLE IF EXISTS `characters`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `characters` (
  `account` varchar(64) CHARACTER SET ascii NOT NULL,
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(64) CHARACTER SET ascii NOT NULL,
  `basicData` blob NOT NULL COMMENT 'Look, stats, money',
  `clanid` decimal(11,0) NOT NULL DEFAULT '0',
  `unionid` decimal(11,0) NOT NULL DEFAULT '0',
  `UnionPoints` int(11) NOT NULL DEFAULT '0',
  `classid` decimal(11,0) NOT NULL DEFAULT '0',
  `deletetime` decimal(20,0) NOT NULL DEFAULT '0',
  `clan_rank` int(11) NOT NULL DEFAULT '0',
  `quests` blob,
  `questflags` blob,
  `quickbar` blob NOT NULL,
  `bskills` blob NOT NULL COMMENT 'Basic Skills',
  `askill` blob NOT NULL COMMENT 'Active Skills',
  `pskill` blob NOT NULL COMMENT 'Passive Skills',
  `respawnid` int(11) NOT NULL DEFAULT '51',
  `townid` int(11) NOT NULL DEFAULT '20',
  `cskills` blob NOT NULL COMMENT 'Class Skills',
  `inv` blob NOT NULL COMMENT 'Inventory',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

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
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `list_friend`
--

DROP TABLE IF EXISTS `list_friend`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `list_friend` (
  `id` int(11) NOT NULL,
  `idfriend` int(11) NOT NULL,
  `namefriend` varchar(50) NOT NULL,
  KEY `id` (`id`),
  KEY `idfriend` (`idfriend`),
  CONSTRAINT `list_friend_ibfk_3` FOREIGN KEY (`id`) REFERENCES `characters` (`id`) ON DELETE CASCADE,
  CONSTRAINT `list_friend_ibfk_4` FOREIGN KEY (`idfriend`) REFERENCES `characters` (`id`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `sessions`
--

DROP TABLE IF EXISTS `sessions`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `sessions` (
  `userid` int(10) NOT NULL,
  `time` time NOT NULL,
  `charid` int(10) NOT NULL,
  `worldip` int(20) NOT NULL,
  `worldport` varchar(20) NOT NULL,
  `id` int(10) NOT NULL AUTO_INCREMENT,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `storage`
--

DROP TABLE IF EXISTS `storage`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
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
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping events for database 'osirose'
--

--
-- Dumping routines for database 'osirose'
--
/*!50003 DROP PROCEDURE IF EXISTS `GetChar` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'NO_AUTO_VALUE_ON_ZERO' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `GetChar`(IN `_user` VARCHAR(16))
    NO SQL
SELECT * FROM characters WHERE account = _user ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `UserLogin` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'NO_AUTO_VALUE_ON_ZERO' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `UserLogin`(IN `_user` VARCHAR(16), IN `_pass` VARCHAR(32))
    READS SQL DATA
SELECT access, active, `online` FROM accounts WHERE username = _user AND password = _pass ;;
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

-- Dump completed on 2016-03-02 23:49:39
