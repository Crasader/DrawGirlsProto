-- CREATE DATABASE  IF NOT EXISTS `drawgirls` /*!40100 DEFAULT CHARACTER SET utf8 */;
-- USE `drawgirls`;
-- MySQL dump 10.13  Distrib 5.6.17, for osx10.6 (i386)
--
-- Host: 10.99.197.209    Database: drawgirls
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

-- CHANGE MASTER TO MASTER_LOG_FILE='mysql-bin.000186', MASTER_LOG_POS=107;

--
-- Table structure for table `aShopTable`
--

DROP TABLE IF EXISTS `aShopTable`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `aShopTable` (
  `no` int(11) NOT NULL AUTO_INCREMENT,
  `id` varchar(45) NOT NULL,
  `cc` varchar(45) NOT NULL,
  `category` varchar(45) NOT NULL,
  `type` varchar(45) NOT NULL,
  `count` int(11) NOT NULL,
  `countName` text NOT NULL,
  `priceType` varchar(45) NOT NULL,
  `price` int(11) NOT NULL,
  `priceName` text NOT NULL,
  `sale` varchar(45) NOT NULL,
  `image` varchar(255) NOT NULL,
  `data` text NOT NULL,
  `pID` varchar(45) NOT NULL,
  `exchangeID` varchar(45) NOT NULL,
  PRIMARY KEY (`no`),
  KEY `pID` (`pID`),
  KEY `exchangeID` (`exchangeID`),
  KEY `type` (`type`,`count`)
) ENGINE=InnoDB AUTO_INCREMENT=31 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `aShopTable`
--

LOCK TABLES `aShopTable` WRITE;
/*!40000 ALTER TABLE `aShopTable` DISABLE KEYS */;
INSERT INTO `aShopTable` VALUES (1,'s_r_1','kr','r','r',10,'<font color=#372396 strokecolor=#FFFFFF strokesize=1>10개</font>','m',1000,'1,000원','','shop_coin1.png','{}','g_10316_001','s_r_1'),(2,'s_r_2','kr','r','r',50,'<font color=#372396 strokecolor=#FFFFFF strokesize=1>50개</font><font color=#F050BE strokecolor=#FFFFFF strokesize=1>+5개</font>','m',5000,'5,000원','+10%','shop_coin2.png','{}','g_10316_002','s_r_2'),(3,'s_r_3','kr','r','r',100,'<font color=#372396 strokecolor=#FFFFFF strokesize=1>100개</font><font color=#F050BE strokecolor=#FFFFFF strokesize=1>+20개</font>','m',10000,'10,000원','+20%','shop_coin3.png','{}','g_10316_003','s_r_3'),(4,'s_r_4','kr','r','r',300,'<font color=#372396 strokecolor=#FFFFFF strokesize=1>300개</font><font color=#F050BE strokecolor=#FFFFFF strokesize=1>+90개</font>','m',30000,'30,000원','+30%','','{}','g_10316_004','s_r_4'),(5,'s_r_5','kr','r','r',500,'<font color=#372396 strokecolor=#FFFFFF strokesize=1>500개</font><font color=#F050BE strokecolor=#FFFFFF strokesize=1>+200개</font>','m',50000,'50,000원','+40%','','{}','g_10316_005','s_r_5'),(6,'s_r_6','kr','r','r',1000,'<font color=#372396 strokecolor=#FFFFFF strokesize=1>1000개</font><font color=#F050BE strokecolor=#FFFFFF strokesize=1>+500개</font>','m',100000,'100,000원','+50%','','{}','g_10316_006','s_r_6'),(7,'s_g_1','kr','g','g',10000,'<font>1만골드</font>','r',10,'10개','','','{}','','s_g_1'),(8,'s_g_2','kr','g','g',20000,'<font>2만골드</font>','r',20,'20개','','','{}','','s_g_2'),(9,'s_g_3','kr','g','g',50000,'<font>5만골드</font>','r',50,'50개','','','{}','','s_g_3'),(10,'s_g_4','kr','g','g',100000,'<font>10만골드</font>','r',100,'100개','','','{}','','s_g_4'),(11,'s_g_5','kr','g','g',300000,'<font>30만골드</font>','r',300,'300개','','','{}','','s_g_5'),(12,'s_g_6','kr','g','g',500000,'<font>50만골드</font>','r',500,'500개','','','{}','','s_g_6'),(13,'s_h_1','kr','h','h',5,'<font>5개</font>','r',5,'5개','','','{}','','s_h_1'),(14,'s_h_2','kr','h','h',10,'<font>10개</font>','r',10,'10개','','','{}','','s_h_2'),(15,'s_h_3','kr','h','h',25,'<font>25개</font>','r',25,'25개','','','{}','','s_h_3'),(16,'s_h_4','kr','h','h',40,'<font>40개</font>','r',40,'40개','','','{}','','s_h_4'),(17,'s_h_5','kr','h','h',75,'<font>75개</font>','r',75,'75개','','','{}','','s_h_5'),(18,'s_h_6','kr','h','h',100,'<font>100개</font>','r',100,'100개','','','{}','','s_h_6'),(19,'pg_fp','kr','e','r',100,'루비 100개','m',4000,'4,000원','50%','','{\"beforeDiscount\":\"8000\"}','g_10316_003','pg_fp'),(20,'pg_ei','kr','e','i',1,'아이템 1팩','g',2400,'2,400골드','20%','','{\"beforeDiscount\":\"3000\"}','','pg_ei'),(21,'pg_snh','kr','e','i',1,'아이템 1팩','g',2000,'2,000골드','30%','','{\"beforeDiscount\":3000}','','pg_snh'),(22,'es_r_1','kr','es_r','r',14,'<font>14개</font>','m',1000,'1,000원','40%','','{}','g_10316_001','es_r_1'),(23,'es_r_2','kr','es_r','r',70,'<font>70개</font>','m',4500,'4,500원','40%','','{}','g_10316_002','es_r_2'),(24,'es_r_3','kr','es_r','r',140,'<font>140개</font>','m',8000,'8,000원','40%','','{}','g_10316_003','es_r_3'),(25,'es_r_4','kr','es_r','r',420,'<font>420개</font>','m',21000,'21,000원','40%','','{}','g_10316_004','es_r_4'),(26,'es_r_5','kr','es_r','r',700,'<font>700개</font>','m',30000,'30,000원','40%','','{}','g_10316_005','es_r_5'),(27,'es_r_6','kr','es_r','r',1400,'<font>1400개</font>','m',50000,'50,000원','40%','','{}','g_10316_006','es_r_6'),(28,'s_i_9','kr','i','i',1,'1개','g',1000,'1,000골드','','','{}','','s_i_9'),(29,'s_i_6','kr','i','i',1,'1개','g',1000,'1,000골드','','','{}','','s_i_6'),(30,'s_i_11','kr','i','i',1,'1개','g',1000,'1,000골드','','','{}','','s_i_11');
/*!40000 ALTER TABLE `aShopTable` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2014-07-24 17:02:42
