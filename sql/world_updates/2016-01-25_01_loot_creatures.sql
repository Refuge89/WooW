--
-- Delete/Insert loot for 49426 (Emblem of Frost)
--
DELETE FROM `loot_creatures` WHERE `itemid` = 49426;

INSERT INTO `loot_creatures` (`entryid`, `itemid`, `normal10percentchance`, `normal25percentchance`, `heroic10percentchance`, `heroic25percentchance`, `mincount`, `maxcount`)
VALUES
	(36626, 49426, 100.00, 100.00, 100.00, 100.00, 2, 2),
	(38297, 49426, 100.00, 100.00, 100.00, 100.00, 2, 2),
	(38296, 49426, 100.00, 100.00, 100.00, 100.00, 2, 2),
	(36855, 49426, 100.00, 100.00, 100.00, 100.00, 2, 2),
	(37958, 49426, 100.00, 100.00, 100.00, 100.00, 2, 2),
	(38106, 49426, 100.00, 100.00, 100.00, 100.00, 2, 2),
	(37959, 49426, 100.00, 100.00, 100.00, 100.00, 2, 2),
	(36612, 49426, 100.00, 100.00, 100.00, 100.00, 2, 2),
	(37957, 49426, 100.00, 100.00, 100.00, 100.00, 2, 2),
	(38433, 49426, 100.00, 100.00, 100.00, 100.00, 2, 2),
	(38462, 49426, 100.00, 100.00, 100.00, 100.00, 2, 2),
	(37504, 49426, 100.00, 100.00, 100.00, 100.00, 2, 2),
	(37505, 49426, 100.00, 100.00, 100.00, 100.00, 2, 2),
	(37506, 49426, 100.00, 100.00, 100.00, 100.00, 2, 2),
	(36627, 49426, 100.00, 100.00, 100.00, 100.00, 2, 2),
	(38390, 49426, 100.00, 100.00, 100.00, 100.00, 2, 2),
	(38549, 49426, 100.00, 100.00, 100.00, 100.00, 2, 2),
	(38550, 49426, 100.00, 100.00, 100.00, 100.00, 2, 2),
	(36678, 49426, 100.00, 100.00, 100.00, 100.00, 2, 2),
	(38431, 49426, 100.00, 100.00, 100.00, 100.00, 2, 2),
	(38585, 49426, 100.00, 100.00, 100.00, 100.00, 2, 2),
	(38586, 49426, 100.00, 100.00, 100.00, 100.00, 2, 2),
	(37970, 49426, 100.00, 100.00, 100.00, 100.00, 2, 2),
	(38401, 49426, 100.00, 100.00, 100.00, 100.00, 2, 2),
	(38784, 49426, 100.00, 100.00, 100.00, 100.00, 2, 2),
	(38785, 49426, 100.00, 100.00, 100.00, 100.00, 2, 2),
	(37955, 49426, 100.00, 100.00, 100.00, 100.00, 2, 2),
	(38434, 49426, 100.00, 100.00, 100.00, 100.00, 2, 2),
	(38435, 49426, 100.00, 100.00, 100.00, 100.00, 2, 2),
	(38436, 49426, 100.00, 100.00, 100.00, 100.00, 2, 2),
	(36853, 49426, 100.00, 100.00, 100.00, 100.00, 2, 2),
	(38265, 49426, 100.00, 100.00, 100.00, 100.00, 2, 2),
	(38266, 49426, 100.00, 100.00, 100.00, 100.00, 2, 2),
	(38267, 49426, 100.00, 100.00, 100.00, 100.00, 2, 2),
	(36597, 49426, 100.00, 100.00, 100.00, 100.00, 2, 2),
	(39166, 49426, 100.00, 100.00, 100.00, 100.00, 2, 2),
	(39167, 49426, 100.00, 100.00, 100.00, 100.00, 2, 2),
	(39168, 49426, 100.00, 100.00, 100.00, 100.00, 2, 2),
	(39863, 49426, 100.00, 100.00, 100.00, 100.00, 2, 2),
	(39944, 49426, 100.00, 100.00, 100.00, 100.00, 2, 2),
	(39864, 49426, 100.00, 100.00, 100.00, 100.00, 3, 3),
	(39945, 49426, 100.00, 100.00, 100.00, 100.00, 3, 3),
	(39946, 49426, 100.00, 100.00, 100.00, 100.00, 1, 1),
	(39947, 49426, 100.00, 100.00, 100.00, 100.00, 1, 1),
	(39948, 49426, 100.00, 100.00, 100.00, 100.00, 1, 1),
	(37126, 49426, 100.00, 100.00, 100.00, 100.00, 1, 1);
   
--
-- Update world db version
--   
UPDATE `world_db_version` SET `LastUpdate` = '2016-01-25_01_loot_creatures' WHERE `LastUpdate` = '2016-01-21_01_creature_spawns';
