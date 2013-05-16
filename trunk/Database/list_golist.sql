/*
MySQL Data Transfer
Source Host: localhost
Source Database: ktrose
Target Host: localhost
Target Database: ktrose
Date: 12/10/2012 4:35:10 PM
*/

SET FOREIGN_KEY_CHECKS=0;
-- ----------------------------
-- Table structure for list_golist
-- ----------------------------
CREATE TABLE `list_golist` (
  `id` int(11) NOT NULL auto_increment,
  `loc` int(11) NOT NULL,
  `mapname` varchar(50) character set utf8 NOT NULL,
  `lvlmin` int(11) NOT NULL default '0',
  `lvlmax` int(11) NOT NULL default '250',
  `map` int(11) NOT NULL,
  `locx` int(11) NOT NULL,
  `locy` int(11) NOT NULL,
  `isactive` int(11) NOT NULL,
  `description` varchar(50) character set utf8 default NULL,
  PRIMARY KEY  (`id`,`loc`)
) ENGINE=InnoDB AUTO_INCREMENT=23 DEFAULT CHARSET=latin1;

-- ----------------------------
-- Records 
-- ----------------------------
INSERT INTO `list_golist` VALUES ('1', '1', 'Adventure Plains', '0', '250', '22', '5731', '5194', '1', null);
INSERT INTO `list_golist` VALUES ('2', '2', 'Canyon City of Zant', '0', '250', '1', '5240', '5192', '1', null);
INSERT INTO `list_golist` VALUES ('3', '3', 'Junon Polis', '0', '250', '2', '5654', '5218', '1', null);
INSERT INTO `list_golist` VALUES ('4', '4', 'Magic City of Eucar - Luna', '50', '250', '51', '5357', '5013', '1', null);
INSERT INTO `list_golist` VALUES ('5', '5', 'Xita Refuge - Eldeon', '120', '250', '61', '5434', '4569', '1', null);
INSERT INTO `list_golist` VALUES ('6', '6', 'Junon Clan Field', '0', '250', '11', '5512', '5019', '1', null);
INSERT INTO `list_golist` VALUES ('7', '7', 'Training Grounds', '0', '250', '6', '5199', '5280', '1', null);
INSERT INTO `list_golist` VALUES ('8', '8', 'Lions Plains', '0', '250', '8', '5160', '5080', '1', null);
INSERT INTO `list_golist` VALUES ('9', '9', 'Luna Clan Field', '120', '250', '59', '5095', '5128', '1', null);
INSERT INTO `list_golist` VALUES ('10', '10', 'Grand Ballroom', '0', '250', '40', '5184', '5211', '1', null);
INSERT INTO `list_golist` VALUES ('11', '11', 'Santa Planetoid', '0', '250', '38', '5074', '5342', '1', null);
INSERT INTO `list_golist` VALUES ('12', '12', 'Sunshine Coast', '0', '250', '37', '5117', '5303', '1', null);
INSERT INTO `list_golist` VALUES ('13', '13', 'Birth Island', '0', '250', '20', '5685', '5210', '1', null);
INSERT INTO `list_golist` VALUES ('14', '14', 'Junon Polis - Island', '0', '250', '2', '4879', '5073', '1', null);
INSERT INTO `list_golist` VALUES ('15', '15', 'Title Map', '0', '250', '4', '5200', '5217', '1', null);
INSERT INTO `list_golist` VALUES ('20', '20', 'Crystal Defenders', '0', '250', '131', '5197', '5184', '0', null);
INSERT INTO `list_golist` VALUES ('21', '21', 'Cave of Ulverick', '0', '250', '138', '5084', '5040', '0', null);
INSERT INTO `list_golist` VALUES ('22', '22', 'Halls of Oblivion', '0', '250', '143', '5020', '5250', '0', null);
INSERT INTO `list_golist` VALUES ('23', '23', 'Sea of Dawn', '0', '250', '148', '5200', '4772', '0', null);
INSERT INTO `list_golist` VALUES ('24', '24', 'Sunshine Coast - Event', '0', '250', '37', '5098', '5233', '0', null);
INSERT INTO `list_golist` VALUES ('25', '25', 'Sikuku Underground Prison', '160', '250', '65', '5485', '5285', '0', null);
INSERT INTO `list_golist` VALUES ('26', '26', 'Desert City of Muris - Oro', '180', '250', '71', '5199', '5236', '0', null);
INSERT INTO `list_golist` VALUES ('27', '27', 'Desert of the Dead', '0', '250', '29', '5093', '5144', '0', null);
