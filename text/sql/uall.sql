-- MySQL dump 10.13  Distrib 5.1.51, for unknown-linux-gnu (x86_64)
--
-- Host: localhost    Database: Url
-- ------------------------------------------------------
-- Server version	5.1.51

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
-- Table structure for table `UALL`
--

DROP TABLE IF EXISTS `UALL`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `UALL` (
  `Url` char(128) NOT NULL,
  `Latest` char(64) DEFAULT NULL,
  `Blockmask` tinyint(1) DEFAULT '1',
  PRIMARY KEY (`Url`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `UALL`
--

LOCK TABLES `UALL` WRITE;
/*!40000 ALTER TABLE `UALL` DISABLE KEYS */;
INSERT INTO `UALL` VALUES ('http://money.163.com/',NULL,1),('http://finance.youth.cn/',NULL,1),('http://www.p5w.net/',NULL,1),('http://www.ccstock.cn/',NULL,1),('http://www.eeo.com.cn/',NULL,1),('http://www.nbd.com.cn/',NULL,1),('http://www.time-weekly.com/',NULL,1),('http://www.mofcom.gov.cn/',NULL,1),('http://www.guancha.cn/',NULL,1),('http://www.cneo.com.cn/',NULL,1),('http://www.caixin.com/',NULL,1),('http://www.21jingji.com/channel/finance/jingrongjigou/',NULL,1),('http://www.stcn.com/',NULL,1),('http://finance.cnr.cn/',NULL,1),('http://finance.people.com.cn/',NULL,1),('http://www.cnfol.com/',NULL,1),('http://finance.qq.com/',NULL,1),('http://www.xinhuanet.com/fortune/',NULL,1),('http://finance.sina.com.cn/',NULL,1),('http://jingji.cntv.cn/',NULL,1),('http://www.hexun.com/',NULL,1),('http://www.eastmoney.com/',NULL,1),('http://finance.chinanews.com/',NULL,1),('http://finance.bjnews.com.cn/',NULL,1),('http://www.yangtse.com/caijing/',NULL,1),('http://finance.ynet.com/2.1.0/84859.html',NULL,1),('http://caijing.chinadaily.com.cn/',NULL,1),('http://www.cfi.cn/',NULL,1),('http://economy.gmw.cn/',NULL,1),('http://economy.southcn.com/',NULL,1),('http://finance.ifeng.com/',NULL,1),('http://finance.china.com.cn/',NULL,1),('http://finance.qq.com/hk/',NULL,1),('http://www.p5w.net/stock/',NULL,1),('http://www.ccstock.cn/finance/hangyedongtai/',NULL,1),('http://www.eeo.com.cn/industry/',NULL,1),('http://www.nbd.com.cn/columns/44',NULL,1),('http://www.time-weekly.com/html/gongsi/',NULL,1),('http://www.guancha.cn/KeJi',NULL,1),('http://www.cneo.com.cn/company',NULL,1),('http://culture.caixin.com/',NULL,1),('http://www.21jingji.com/channel/finance/yazhoushichang/',NULL,1),('http://finance.youth.cn/finance_insurance/',NULL,1),('http://www.p5w.net/fund/index.htm',NULL,1),('http://www.ccstock.cn/finance/',NULL,1),('http://www.eeo.com.cn/finance/',NULL,1),('http://www.guancha.cn/economy',NULL,1),('http://www.p5w.net/news/index.htm',NULL,1),('http://www.ccstock.cn/fund/',NULL,1),('http://www.eeo.com.cn/nation/',NULL,1),('http://www.time-weekly.com/html/jinrong/',NULL,1),('http://www.guancha.cn/military-affairs',NULL,1),('http://finance.caixin.com/',NULL,1),('http://www.p5w.net/kuaixun/',NULL,1),('http://www.ccstock.cn/jrjg/',NULL,1),('http://www.eeo.com.cn/politics/',NULL,1),('http://auto.nbd.com.cn/',NULL,1),('http://finance.stcn.com/',NULL,1),('http://www.time-weekly.com/html/quanqiu/',NULL,1),('http://www.guancha.cn/internation',NULL,1),('http://stock.eastmoney.com/',NULL,1),('http://www.cneo.com.cn/business',NULL,1),('http://international.caixin.com/',NULL,1),('http://finance.youth.cn/finance_fund/',NULL,1),('http://news.stcn.com/',NULL,1),('http://xg.stock.cnfol.com/',NULL,1),('http://www.ccstock.cn/stock/chuangyeban/',NULL,1),('http://news.nbd.com.cn/',NULL,1),('http://economy.caixin.com/',NULL,1),('http://www.ccstock.cn/ipo/',NULL,1),('http://company.nbd.com.cn/',NULL,1),('http://www.time-weekly.com/html/caijing/',NULL,1),('http://www.ccstock.cn/stock/',NULL,1),('http://www.cneo.com.cn/financial',NULL,1),('http://china.caixin.com/',NULL,1),('http://news.cnr.cn/',NULL,1),('http://finance.qq.com/hgjj.htm',NULL,1),('http://www.ccstock.cn/gscy/',NULL,1),('http://www.time-weekly.com/html/keji/',NULL,1),('http://www.guancha.cn/politics',NULL,1),('http://finance.qq.com/jrsc.htm',NULL,1),('http://stock.stcn.com/',NULL,1),('http://finance.qq.com/usstock/',NULL,1),('http://economy.southcn.com/node_165838.htm',NULL,1),('http://roll.cnr.cn/ygkx/',NULL,1),('http://finance.sina.com.cn/fund/',NULL,1),('http://www.cneo.com.cn/macro',NULL,1),('http://companies.caixin.com/',NULL,1),('http://www.21jingji.com/channel/jingji/quyu/',NULL,1),('http://www.21jingji.com/channel/jingji/shijun/',NULL,1),('http://finance.youth.cn/finance_bank/',NULL,1),('http://www.21jingji.com/channel/jingji/guoji/',NULL,1),('http://finance.youth.cn/finance_house/',NULL,1),('http://www.21jingji.com/channel/jingji/hongguan/',NULL,1),('http://finance.sina.com.cn/bond/',NULL,1),('http://finance.youth.cn/finance_Ecommerce/',NULL,1),('http://www.21jingji.com/channel/jingji/gonggong/',NULL,1),('http://energy.people.com.cn/',NULL,1),('http://stock.cfi.cn/',NULL,1),('http://finance.china.com.cn/live.shtml',NULL,1),('http://gold.cnfol.com/',NULL,1),('http://finance.youth.cn/finance_IT/',NULL,1),('http://economy.southcn.com/node_165832.htm',NULL,1),('http://company.stcn.com/',NULL,1),('http://finance.gmw.cn/',NULL,1),('http://finance.sina.com.cn/futuremarket/',NULL,1),('http://finance.qq.com/gjcj.htm',NULL,1),('http://economy.southcn.com/node_165813.htm',NULL,1),('http://politics.people.com.cn/',NULL,1),('http://www.21jingji.com/',NULL,1),('http://finance.ynet.com/2.1.0/84049.html',NULL,1),('http://finance.youth.cn/IPO/',NULL,1),('http://finance.qq.com/gsbd.htm',NULL,1),('http://finance.sina.com.cn/stock/usstock/',NULL,1),('http://tech.cnr.cn/',NULL,1),('http://gold.hexun.com/',NULL,1),('http://hkstock.cnfol.com/',NULL,1),('http://forex.cnfol.com/',NULL,1),('http://fund.cfi.cn/',NULL,1),('http://finance.ynet.com/2.1.0/84054.html',NULL,1),('http://finance.youth.cn/finance_stock/',NULL,1),('http://roll.cnr.cn/china/',NULL,1),('http://world.people.com.cn/',NULL,1),('http://finance.ynet.com/2.1.0/84168.html',NULL,1),('http://finance.people.com.cn/stock/',NULL,1),('http://finance.ifeng.com/business/',NULL,1),('http://scitech.people.com.cn/',NULL,1),('http://finance.ifeng.com/fund/',NULL,1),('http://fund.cnfol.com/',NULL,1),('http://futures.cnfol.com/',NULL,1),('http://futures.cfi.cn/',NULL,1),('http://sc.stock.cnfol.com/',NULL,1),('http://stock.cnfol.com/',NULL,1),('http://news.cnfol.com/',NULL,1),('http://industry.cfi.cn/',NULL,1),('http://roll.finance.qq.com/',NULL,1),('http://cnews.chinadaily.com.cn/',NULL,1),('http://finance.qq.com/stock/',NULL,1),('http://finance.sina.com.cn/stock/newstock/',NULL,1),('http://finance.sina.com.cn/stock/',NULL,1),('http://finance.qq.com/xfwt.htm',NULL,1),('http://www.xinhuanet.com/fortune/dujia.htm',NULL,1),('http://finance.sina.com.cn/nmetal/',NULL,1),('http://tech.hexun.com/',NULL,1),('http://finance.sina.com.cn/forex/',NULL,1),('http://stock.eastmoney.com/shichang.html',NULL,1),('http://finance.sina.com.cn/stock/hkstock/',NULL,1),('http://channel.chinanews.com/u/finance/yw.shtml',NULL,1),('http://news.hexun.com/',NULL,1),('http://finance.ifeng.com/hk/',NULL,1),('http://finance.ifeng.com/stock/',NULL,1),('http://money.163.com/stock/',NULL,1),('http://finance.ifeng.com/itfinance/',NULL,1),('http://stock.hexun.com/',NULL,1),('http://finance.ifeng.com/macro/',NULL,1),('http://finance.china.com.cn/stock/',NULL,1),('http://www.time-weekly.com/html/shishi/',NULL,1),('http://tech.china.com.cn/',NULL,1),('http://finance.china.com.cn/consume/index.shtml',NULL,1),('http://money.163.com/finance/',NULL,1),('http://finance.china.com.cn/industry/index.shtml',NULL,1),('http://jingji.cntv.cn/gongsi/index.shtml',NULL,1),('http://stock.eastmoney.com/gegu.html',NULL,1),('http://jingji.cntv.cn/stock/index.shtml',NULL,1),('http://money.163.com/special/00252G50/macro.html',NULL,1),('http://stock.hexun.com/quanshang/',NULL,1),('http://bank.hexun.com/',NULL,1),('http://world.chinadaily.com.cn/',NULL,1),('http://finance.eastmoney.com/',NULL,1),('http://stock.eastmoney.com/newstock.html',NULL,1),('http://biz.eastmoney.com/',NULL,1),('http://channel.chinanews.com/cns/cl/cj-gjcj.shtml',NULL,1),('http://www.yangtse.com/guonei/',NULL,1),('http://finance.ynet.com/2.1.0/84065.html',NULL,1),('http://china.chinadaily.com.cn/',NULL,1),('http://forex.cfi.cn/',NULL,1),('http://economy.gmw.cn/node_12467.htm',NULL,1),('http://www.21jingji.com/channel/finance/Investment/',NULL,1),('http://www.21jingji.com/channel/finance/zichanguangli/',NULL,1),('http://www.21jingji.com/channel/money/shangshigongsi/',NULL,1),('http://www.21jingji.com/channel/money/jigou/',NULL,1),('http://www.21jingji.com/channel/money/zibenshichang/',NULL,1),('http://www.21jingji.com/channel/money/changwaishichang/',NULL,1),('http://www.bbtnews.com.cn/news/The international/',NULL,1),('http://www.bbtnews.com.cn/news/Culture/',NULL,1),('http://www.bbtnews.com.cn/news/Tourism/',NULL,1),('http://www.bbtnews.com.cn/news/Business/',NULL,1),('http://www.bbtnews.com.cn/news/The financial market/',NULL,1),('http://www.bbtnews.com.cn/news/The listed company/',NULL,1),('http://www.bbtnews.com.cn/news/Sankei shimbun/',NULL,1),('http://www.bbtnews.com.cn/news/Political and econom/',NULL,1),('http://www.bbtnews.com.cn/news/touban/',NULL,1),('http://www.bbtnews.com.cn/',NULL,1);
/*!40000 ALTER TABLE `UALL` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2016-02-12 23:42:56
