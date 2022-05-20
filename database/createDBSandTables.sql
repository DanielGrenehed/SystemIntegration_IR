-- create database
SELECT * FROM demoiot2022.animals;CREATE DATABASE `sensor_dbs` /*!40100 DEFAULT CHARACTER SET utf8mb3 */ /*!80016 DEFAULT ENCRYPTION='N' */;

-- create sensor table
CREATE TABLE `sensor` (
  `sensor_id` int NOT NULL AUTO_INCREMENT COMMENT 'Holds ID for each sensor.',
  `sensor_type` varchar(45) NOT NULL COMMENT 'Holds what type of sensor in use.',
  `sensor_name` varchar(45) NOT NULL COMMENT 'Holds name of the sensor in use.',
  PRIMARY KEY (`sensor_id`),
  UNIQUE KEY `sensor_name_UNIQUE` (`sensor_name`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8mb3;

-- create sensor_data table
CREATE TABLE `sensor_data` (
  `sensor_data_id` int NOT NULL AUTO_INCREMENT COMMENT 'Holds ID for each row of data.',
  `sensor_data_value` varchar(8) NOT NULL COMMENT 'Holds the measure value from a sensor.',
  `sensor_data_date` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT 'Holds the timestamp for measured values.',
  `sensor_id` int NOT NULL,
  PRIMARY KEY (`sensor_data_id`),
  KEY `fk_sensor_data_sensor_idx` (`sensor_id`),
  CONSTRAINT `fk_sensor_data_sensor` FOREIGN KEY (`sensor_id`) REFERENCES `sensor` (`sensor_id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=9 DEFAULT CHARSET=utf8mb3;
