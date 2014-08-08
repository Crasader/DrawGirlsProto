<?php
/*
-- 07021535 : 정보변경히스토리에 카테고리추가

ALTER TABLE `dg001`.`ModifyHistory` 
ADD COLUMN `category` VARCHAR(50) NOT NULL AFTER `regDate`;

ALTER TABLE `dg002`.`ModifyHistory` 
ADD COLUMN `category` VARCHAR(50) NOT NULL AFTER `regDate`;


*/
?>