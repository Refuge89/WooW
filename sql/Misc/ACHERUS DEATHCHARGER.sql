
UPDATE `creature_proto` SET `npcflags`='16777216' WHERE (`entry`='28302');
-- Origional faction 35
UPDATE `creature_proto` SET `faction`='35' WHERE (`entry`='28782');
-- Origional faction 2082 and Origional npcflags 0
UPDATE `creature_proto` SET `faction`='35',`npcflags`='16777216' WHERE (`entry`='28901');
-- AcherusDeathcharger
UPDATE `creature_proto` SET `spell1`=52362, `spell2`=52268 WHERE `entry`=28782 LIMIT 1;
UPDATE `quests` SET `ReqKillMobOrGOId1`=28782, `ReqKillMobOrGOCount1`=1, `ReqCastSpellId1`=0 WHERE `entry`=12687 LIMIT 1;

INSERT INTO `creature_spawns` (`entry`, `map`, `position_x`, `position_y`, `position_z`, `orientation`, `movetype`, `displayid`, `faction`, `flags`, `bytes0`, `bytes1`, `bytes2`, `emote_state`, `npc_respawn_link`, `channel_spell`, `channel_target_sqlid`, `channel_target_sqlid_creature`, `standstate`, `death_state`, `mountdisplayid`, `slot1item`, `slot2item`, `slot3item`, `CanFly`, `phase`, `vehicle`) VALUES (28653, 609, 2351.83, -5701.31, 154.04, 5.52, 0, 16416, 2084, 32768, 16777472, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 38633, 0, 0, 0, 32, 0);
UPDATE `creature_spawns` SET `faction`='2082',`flags`='0' WHERE (`entry`='28782' AND `map`='609');

UPDATE `creature_proto` SET `npcflags`=2 WHERE `entry`=28653 LIMIT 1;