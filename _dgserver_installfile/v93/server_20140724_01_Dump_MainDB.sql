SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='TRADITIONAL,ALLOW_INVALID_DATES';

ALTER TABLE `aCardTable` 
ADD COLUMN `version` INT(11) NOT NULL AFTER `type`;

ALTER TABLE `aKeyIntValue` 
CHANGE COLUMN `value` `value` INT(11) NOT NULL ;

ALTER TABLE `aMonsterTable` 
ADD COLUMN `version` INT(11) NOT NULL AFTER `script`;

ALTER TABLE `aShopEventTable` 
ADD COLUMN `cc` VARCHAR(45) NOT NULL AFTER `id`;

ALTER TABLE `aShopTable` 
ADD COLUMN `cc` VARCHAR(45) NOT NULL AFTER `id`;


SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;
