-- CREATE DATABASE  IF NOT EXISTS `dg001` /*!40100 DEFAULT CHARACTER SET utf8 */;
-- USE `dg001`;
-- MySQL dump 10.13  Distrib 5.6.17, for osx10.6 (i386)
--
-- Host: 10.99.197.209    Database: dg001
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

-- CHANGE MASTER TO MASTER_LOG_FILE='mysql-bin.000208', MASTER_LOG_POS=107;

--
-- Table structure for table `ArchivementHistory`
--

DROP TABLE IF EXISTS `ArchivementHistory`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `ArchivementHistory` (
  `no` int(11) NOT NULL AUTO_INCREMENT,
  `memberID` bigint(20) NOT NULL,
  `archiveID` varchar(45) NOT NULL,
  `clearDate` bigint(20) NOT NULL,
  `rewardDate` bigint(20) NOT NULL,
  `goal` int(11) NOT NULL,
  `count` int(11) NOT NULL,
  PRIMARY KEY (`no`),
  UNIQUE KEY `uniq` (`memberID`,`archiveID`),
  KEY `memberID` (`memberID`),
  CONSTRAINT `arc_user` FOREIGN KEY (`memberID`) REFERENCES `UserDataTable` (`memberID`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `CardHistory`
--

DROP TABLE IF EXISTS `CardHistory`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `CardHistory` (
  `no` int(11) NOT NULL AUTO_INCREMENT,
  `memberID` bigint(20) NOT NULL,
  `cardNo` int(11) NOT NULL,
  `takeDate` bigint(20) NOT NULL,
  `comment` text NOT NULL,
  `isMorphing` int(11) NOT NULL,
  `count` int(11) NOT NULL DEFAULT '1',
  `level` int(11) NOT NULL DEFAULT '1',
  PRIMARY KEY (`no`),
  UNIQUE KEY `uniq` (`memberID`,`cardNo`),
  KEY `memberID` (`memberID`),
  CONSTRAINT `chs_use` FOREIGN KEY (`memberID`) REFERENCES `UserDataTable` (`memberID`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `CharacterHistory`
--

DROP TABLE IF EXISTS `CharacterHistory`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `CharacterHistory` (
  `no` int(11) NOT NULL AUTO_INCREMENT,
  `memberID` bigint(20) NOT NULL,
  `characterNo` int(11) NOT NULL,
  `level` int(11) NOT NULL,
  `regDate` bigint(20) NOT NULL,
  PRIMARY KEY (`no`),
  UNIQUE KEY `uniq` (`memberID`,`characterNo`),
  KEY `memberID_idx` (`memberID`),
  CONSTRAINT `cth_use` FOREIGN KEY (`memberID`) REFERENCES `UserDataTable` (`memberID`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `CuponHistory`
--

DROP TABLE IF EXISTS `CuponHistory`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `CuponHistory` (
  `no` int(11) NOT NULL AUTO_INCREMENT,
  `memberID` bigint(20) NOT NULL,
  `cuponNo` int(11) NOT NULL,
  `cuponCode` varchar(45) NOT NULL,
  `useDate` bigint(20) NOT NULL,
  PRIMARY KEY (`no`),
  UNIQUE KEY `uniq` (`memberID`,`cuponCode`),
  KEY `memberID` (`memberID`),
  CONSTRAINT `cph_use` FOREIGN KEY (`memberID`) REFERENCES `UserDataTable` (`memberID`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=18 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `CuponUsedInfo`
--

DROP TABLE IF EXISTS `CuponUsedInfo`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `CuponUsedInfo` (
  `no` int(11) NOT NULL AUTO_INCREMENT,
  `cuponCode` varchar(45) NOT NULL,
  `useDate` bigint(20) NOT NULL,
  `memberID` bigint(20) NOT NULL,
  PRIMARY KEY (`no`),
  KEY `cuponCode` (`cuponCode`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `GiftBoxHistory`
--

DROP TABLE IF EXISTS `GiftBoxHistory`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `GiftBoxHistory` (
  `no` int(11) NOT NULL AUTO_INCREMENT,
  `memberID` bigint(20) NOT NULL,
  `regDate` bigint(20) NOT NULL,
  `confirmDate` bigint(20) NOT NULL,
  `sender` varchar(45) NOT NULL,
  `content` varchar(255) NOT NULL,
  `reward` text NOT NULL,
  `data` text NOT NULL,
  `exchangeID` varchar(45) NOT NULL,
  `exchangeList` text NOT NULL,
  PRIMARY KEY (`no`),
  KEY `memberID` (`memberID`),
  KEY `getlist` (`memberID`,`confirmDate`,`regDate`),
  CONSTRAINT `gbh_use` FOREIGN KEY (`memberID`) REFERENCES `UserDataTable` (`memberID`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `ModifyHistory`
--

DROP TABLE IF EXISTS `ModifyHistory`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `ModifyHistory` (
  `no` int(11) NOT NULL AUTO_INCREMENT,
  `memberID` bigint(20) DEFAULT NULL,
  `oldData` text NOT NULL,
  `newData` text NOT NULL,
  `reason` text NOT NULL,
  `writer` varchar(45) NOT NULL,
  `regDate` bigint(20) NOT NULL,
  `category` varchar(50) NOT NULL,
  PRIMARY KEY (`no`),
  KEY `memberID` (`memberID`),
  CONSTRAINT `mhy_use` FOREIGN KEY (`memberID`) REFERENCES `UserDataTable` (`memberID`) ON DELETE SET NULL ON UPDATE SET NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `PieceHistory`
--

DROP TABLE IF EXISTS `PieceHistory`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `PieceHistory` (
  `no` int(11) NOT NULL AUTO_INCREMENT,
  `memberID` bigint(20) NOT NULL,
  `pieceNo` int(11) NOT NULL,
  `openDate` bigint(20) NOT NULL,
  `firstClearDate` bigint(20) NOT NULL,
  `tryCount` int(11) NOT NULL,
  `clearCount` int(11) NOT NULL,
  `clearDateList` varchar(255) NOT NULL,
  `openType` varchar(45) NOT NULL,
  PRIMARY KEY (`no`),
  UNIQUE KEY `uniq` (`memberID`,`pieceNo`),
  KEY `memberID` (`memberID`),
  CONSTRAINT `pch_use` FOREIGN KEY (`memberID`) REFERENCES `UserDataTable` (`memberID`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `PuzzleHistory`
--

DROP TABLE IF EXISTS `PuzzleHistory`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `PuzzleHistory` (
  `no` int(11) NOT NULL AUTO_INCREMENT,
  `memberID` bigint(20) NOT NULL,
  `puzzleNo` int(11) NOT NULL,
  `openDate` bigint(20) NOT NULL,
  `openType` varchar(45) NOT NULL,
  `clearDate` bigint(20) NOT NULL,
  `perfectDate` bigint(20) NOT NULL,
  PRIMARY KEY (`no`),
  KEY `memberID` (`memberID`),
  CONSTRAINT `pzh_use` FOREIGN KEY (`memberID`) REFERENCES `UserDataTable` (`memberID`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `StageScoreTable`
--

DROP TABLE IF EXISTS `StageScoreTable`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `StageScoreTable` (
  `no` int(11) NOT NULL AUTO_INCREMENT,
  `stageNo` int(11) NOT NULL,
  `memberID` bigint(20) NOT NULL,
  `score` bigint(20) NOT NULL,
  `regDate` bigint(20) NOT NULL,
  `data` text NOT NULL,
  `regTime` bigint(20) NOT NULL,
  PRIMARY KEY (`no`),
  UNIQUE KEY `uniq` (`memberID`,`stageNo`),
  KEY `stageNscore` (`stageNo`,`score`),
  KEY `memberID` (`memberID`),
  CONSTRAINT `sst_use` FOREIGN KEY (`memberID`) REFERENCES `UserDataTable` (`memberID`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `UserDataTable`
--

DROP TABLE IF EXISTS `UserDataTable`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `UserDataTable` (
  `no` int(11) NOT NULL AUTO_INCREMENT,
  `memberID` bigint(20) NOT NULL,
  `nick` varchar(100) NOT NULL,
  `country` varchar(10) NOT NULL,
  `lastDate` bigint(20) NOT NULL,
  `joinDate` bigint(20) NOT NULL,
  `friendList` text NOT NULL,
  `lastTime` bigint(20) NOT NULL,
  `lastCmdNo` int(11) NOT NULL,
  `deviceID` int(11) NOT NULL,
  `flag` varchar(45) NOT NULL DEFAULT 'kr',
  `pGuide` text NOT NULL,
  `isVIP` int(11) NOT NULL DEFAULT '0',
  `isFirstBuy` int(11) NOT NULL,
  `totalPlayCount` int(11) NOT NULL,
  `failCount` int(11) NOT NULL,
  `autoLevel` int(11) NOT NULL,
  `blockDate` bigint(20) NOT NULL DEFAULT '0',
  `blockReason` varchar(255) NOT NULL,
  `selectedCharNO` int(11) NOT NULL,
  `TMInfo` text NOT NULL,
  `TMLevel` text NOT NULL,
  `highScore` bigint(20) NOT NULL,
  `archiveData` text NOT NULL,
  `eventCheckData` text NOT NULL,
  `eventCheckDate` int(11) NOT NULL,
  `eventCheckWeek` int(11) NOT NULL,
  `eventAtdNo` int(11) NOT NULL,
  `eventAtdCount` int(11) NOT NULL,
  `endlessData` text NOT NULL,
  `highPiece` int(11) NOT NULL,
  `exp` int(11) NOT NULL,
  `data` text NOT NULL,
  `lastHeartTime` bigint(20) NOT NULL,
  PRIMARY KEY (`no`),
  KEY `memberID` (`memberID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `UserStorage`
--

DROP TABLE IF EXISTS `UserStorage`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `UserStorage` (
  `no` int(11) NOT NULL AUTO_INCREMENT,
  `memberID` bigint(20) NOT NULL,
  `g` int(11) NOT NULL DEFAULT '0',
  `pr` int(11) NOT NULL DEFAULT '0',
  `fr` int(11) NOT NULL DEFAULT '0',
  `h` int(11) NOT NULL DEFAULT '0',
  `m` int(11) NOT NULL DEFAULT '0',
  `i1` int(11) NOT NULL DEFAULT '0',
  `i2` int(11) NOT NULL DEFAULT '0',
  `i3` int(11) NOT NULL DEFAULT '0',
  `i4` int(11) NOT NULL DEFAULT '0',
  `i5` int(11) NOT NULL DEFAULT '0',
  `i6` int(11) NOT NULL DEFAULT '0',
  `i7` int(11) NOT NULL DEFAULT '0',
  `i8` int(11) NOT NULL DEFAULT '0',
  `i9` int(11) NOT NULL DEFAULT '0',
  `i10` int(11) NOT NULL DEFAULT '0',
  `i11` int(11) NOT NULL DEFAULT '0',
  `p1` int(11) NOT NULL DEFAULT '0',
  `p2` int(11) NOT NULL DEFAULT '0',
  `p3` int(11) NOT NULL DEFAULT '0',
  `p4` int(11) NOT NULL DEFAULT '0',
  `p5` int(11) NOT NULL DEFAULT '0',
  `p6` int(11) NOT NULL DEFAULT '0',
  `p7` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`no`),
  UNIQUE KEY `memberIDuq` (`memberID`),
  KEY `memberID` (`memberID`),
  CONSTRAINT `usg_use` FOREIGN KEY (`memberID`) REFERENCES `UserDataTable` (`memberID`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2014-08-21 22:09:41
