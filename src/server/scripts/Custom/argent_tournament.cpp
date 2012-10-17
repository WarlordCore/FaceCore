#include "ScriptPCH.h"
/*
-- SQL --
-- npc_lake_frog
UPDATE `creature_template` SET `ScriptName` = 'npc_lake_frog' WHERE `entry` IN (33211,33224);
UPDATE `creature_template` SET `gossip_menu_id` = 33220 WHERE `entry` = 33220;
REPLACE INTO `gossip_menu_option` (`menu_id`, `option_text`, `option_id`, `npc_option_npcflag`, `action_script_id`) VALUES ('33220', 'Do you know, where I can find Ashwood Brand Sword?', '1', '1', '33220');
REPLACE INTO `gossip_scripts` (`id`, `command`, `datalong`, `datalong2`) VALUES ('33220', '15', '62554', '3');
*/

#define NPC_MAIDEN 33220
#define TRANSFORM_MAIDEN 62550
#define WARTS_SPELL 62581
#define LIP_BALM_SPELL 62574
#define FROG_LOVE 62537
#define SUMMON_ASHOOD_BRAND_SPELL 62554
#define MAIDEN_SAY "Thank to you, here is you Ashwood Brand !"

struct A_BLADE_FIT_FOR_A_CHAMPION_QUEST
{
  uint32 quest_id;
};

A_BLADE_FIT_FOR_A_CHAMPION_QUEST new_quest[] = {13603, 13666, 13673, 13741, 13746, 13752, 13757, 13762, 13768, 13773, 13778, 13783};

class npc_lake_frog : public CreatureScript
{
public:
    npc_lake_frog() : CreatureScript("npc_lake_frog") { }

    struct npc_lake_frogAI : public ScriptedAI
    {
        npc_lake_frogAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
        }

        void ReceiveEmote(Player* pPlayer, uint32 emote)
        {
            switch (emote)
            {
                case TEXT_EMOTE_KISS:

                    if (me->HasAura(FROG_LOVE))
                        return;

                    if (!pPlayer->HasAura(LIP_BALM_SPELL))
                    {
                        me->CastSpell(pPlayer, WARTS_SPELL, true);
                        return;
                    }

                    if (pPlayer->GetQuestStatus(13603) == QUEST_STATUS_INCOMPLETE || pPlayer->GetQuestStatus(13666) == QUEST_STATUS_INCOMPLETE ||
                        pPlayer->GetQuestStatus(13673) == QUEST_STATUS_INCOMPLETE || pPlayer->GetQuestStatus(13741) == QUEST_STATUS_INCOMPLETE ||
                        pPlayer->GetQuestStatus(13746) == QUEST_STATUS_INCOMPLETE || pPlayer->GetQuestStatus(13752) == QUEST_STATUS_INCOMPLETE ||
                        pPlayer->GetQuestStatus(13757) == QUEST_STATUS_INCOMPLETE || pPlayer->GetQuestStatus(13762) == QUEST_STATUS_INCOMPLETE ||
                        pPlayer->GetQuestStatus(13768) == QUEST_STATUS_INCOMPLETE || pPlayer->GetQuestStatus(13773) == QUEST_STATUS_INCOMPLETE ||
                        pPlayer->GetQuestStatus(13778) == QUEST_STATUS_INCOMPLETE || pPlayer->GetQuestStatus(13783) == QUEST_STATUS_INCOMPLETE )
                    {
                        if (rand()%4 == 1)
                        {
                            me->CastSpell(me, TRANSFORM_MAIDEN, true);
                            me->MonsterSay(MAIDEN_SAY, LANG_UNIVERSAL, 0);
                            me->UpdateEntry(NPC_MAIDEN); //For gossip
                            me->DespawnOrUnsummon(30000);
                        }
                        else
                            me->CastSpell(me, FROG_LOVE, true);
                    }

                    break;
            }
        }
    };

    CreatureAI *GetAI(Creature* pCreature) const
    {
        return new npc_lake_frogAI(pCreature);
    }
};

void AddSC_ArgentTournament()
{
    new npc_lake_frog;
}
