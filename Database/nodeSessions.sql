--
-- Table structure for table `sessions`
--

CREATE TABLE `sessions` (
  `id` int(10) NOT NULL,
  `charip` varchar(20) DEFAULT NULL,
  `charport` int(20) DEFAULT NULL,
  `worldip` varchar(20) DEFAULT NULL,
  `worldport` int(20) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;