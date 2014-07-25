-- CREATE DATABASE  IF NOT EXISTS `dgLog001` /*!40100 DEFAULT CHARACTER SET utf8 */;
-- USE `dgLog001`;
-- MySQL dump 10.13  Distrib 5.6.17, for osx10.6 (i386)
--
-- Host: 10.99.197.209    Database: dgLog001
-- ------------------------------------------------------
-- Server version	5.5.28-log

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
-- Position to start replication or point-in-time recovery from
--

-- CHANGE MASTER TO MASTER_LOG_FILE='mysql-bin.000182', MASTER_LOG_POS=107;

--
-- Table structure for table `UserLog_20140719`
--

DROP TABLE IF EXISTS `UserLog_20140719`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `UserLog_20140719` (
  `no` bigint(20) NOT NULL AUTO_INCREMENT,
  `memberID` bigint(20) NOT NULL,
  `ip` varchar(30) NOT NULL,
  `header` text NOT NULL,
  `category` varchar(100) NOT NULL,
  `input` text NOT NULL,
  `output` text NOT NULL,
  `regDate` bigint(20) NOT NULL,
  `execTime` float NOT NULL,
  `regTime` bigint(20) NOT NULL,
  PRIMARY KEY (`no`),
  KEY `memberID` (`memberID`)
) ENGINE=InnoDB AUTO_INCREMENT=2664 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `UserLog_20140720`
--

DROP TABLE IF EXISTS `UserLog_20140720`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `UserLog_20140720` (
  `no` bigint(20) NOT NULL AUTO_INCREMENT,
  `memberID` bigint(20) NOT NULL,
  `ip` varchar(30) NOT NULL,
  `header` text NOT NULL,
  `category` varchar(100) NOT NULL,
  `input` text NOT NULL,
  `output` text NOT NULL,
  `regDate` bigint(20) NOT NULL,
  `execTime` float NOT NULL,
  `regTime` bigint(20) NOT NULL,
  PRIMARY KEY (`no`),
  KEY `memberID` (`memberID`)
) ENGINE=InnoDB AUTO_INCREMENT=8120 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `UserLog_20140721`
--

DROP TABLE IF EXISTS `UserLog_20140721`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `UserLog_20140721` (
  `no` bigint(20) NOT NULL AUTO_INCREMENT,
  `memberID` bigint(20) NOT NULL,
  `ip` varchar(30) NOT NULL,
  `header` text NOT NULL,
  `category` varchar(100) NOT NULL,
  `input` text NOT NULL,
  `output` text NOT NULL,
  `regDate` bigint(20) NOT NULL,
  `execTime` float NOT NULL,
  `regTime` bigint(20) NOT NULL,
  PRIMARY KEY (`no`),
  KEY `memberID` (`memberID`)
) ENGINE=InnoDB AUTO_INCREMENT=15269 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `UserLog_20140722`
--

DROP TABLE IF EXISTS `UserLog_20140722`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `UserLog_20140722` (
  `no` bigint(20) NOT NULL AUTO_INCREMENT,
  `memberID` bigint(20) NOT NULL,
  `ip` varchar(30) NOT NULL,
  `header` text NOT NULL,
  `category` varchar(100) NOT NULL,
  `input` text NOT NULL,
  `output` text NOT NULL,
  `regDate` bigint(20) NOT NULL,
  `execTime` float NOT NULL,
  `regTime` bigint(20) NOT NULL,
  PRIMARY KEY (`no`),
  KEY `memberID` (`memberID`)
) ENGINE=InnoDB AUTO_INCREMENT=3444 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2014-07-22  8:57:05
