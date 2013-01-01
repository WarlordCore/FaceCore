/*
 * Copyright (C) 2008-2013 TrinityCore <http://www.trinitycore.org/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "halls_of_reflection.h"

enum Yells
{
    SAY_AGGRO                                     = 0,
    SAY_SLAY                                      = 1,
    SAY_DEATH                                     = 2,
    SAY_CORRUPTED_FLESH                           = 3
};

enum Spells
{
    SPELL_OBLITERATE                              = 72360,
    H_SPELL_OBLITERATE                            = 72434,
    SPELL_WELL_OF_CORRUPTION                      = 72362,
    SPELL_CORRUPTED_FLESH                         = 72363,
    H_SPELL_CORRUPTED_FLESH                       = 72436,
    SPELL_SHARED_SUFFERING                        = 72368,
    H_SPELL_SHARED_SUFFERING                      = 72369
};

enum Events
{
    EVENT_NONE,
    EVENT_OBLITERATE,
    EVENT_WELL_OF_CORRUPTION,
    EVENT_CORRUPTED_FLESH,
    EVENT_SHARED_SUFFERING,
};

class boss_marwyn : public CreatureScript
{
public:
    boss_marwyn() : CreatureScript("boss_marwyn") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_marwynAI(creature);
    }

    struct boss_marwynAI : public boss_horAI
    {
        boss_marwynAI(Creature* creature) : boss_horAI(creature) {}

        void Reset()
        {
            boss_horAI::Reset();

            if (instance)
                instance->SetData(DATA_MARWYN_EVENT, NOT_STARTED);
        }

        void EnterCombat(Unit* /*who*/)
        {
            Talk(SAY_AGGRO);
            if (instance)
                instance->SetData(DATA_MARWYN_EVENT, IN_PROGRESS);

            events.ScheduleEvent(EVENT_OBLITERATE, 10000);
            events.ScheduleEvent(EVENT_WELL_OF_CORRUPTION, DUNGEON_MODE(13000, 8000));
            events.ScheduleEvent(EVENT_CORRUPTED_FLESH, 20000);
            events.ScheduleEvent(EVENT_SHARED_SUFFERING, 15000);
        }

        void JustDied(Unit* /*killer*/)
        {
            Talk(SAY_DEATH);

            if (instance)
            {
                instance->SetData(DATA_MARWYN_EVENT, DONE);
                instance->SetData(DATA_WAVES_STATE, DONE);
            }
        }

        void KilledUnit(Unit* /*victim*/)
        {
            Talk(SAY_SLAY);
        }

        void UpdateAI(const uint32 diff)
        {
            // Return since we have no target
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
                case EVENT_OBLITERATE:
                    DoCast(me->getVictim(), DUNGEON_MODE(SPELL_OBLITERATE, H_SPELL_OBLITERATE));
                    events.ScheduleEvent(EVENT_OBLITERATE, 10000);
                    break;
                case EVENT_WELL_OF_CORRUPTION:
                    DoCast(SPELL_WELL_OF_CORRUPTION);
                    events.ScheduleEvent(EVENT_WELL_OF_CORRUPTION, DUNGEON_MODE(13000, 8000));
                    break;
                case EVENT_CORRUPTED_FLESH:
                    Talk(SAY_CORRUPTED_FLESH);
                    DoCast(DUNGEON_MODE(SPELL_CORRUPTED_FLESH, H_SPELL_CORRUPTED_FLESH));
                    events.ScheduleEvent(EVENT_CORRUPTED_FLESH, 20000);
                    break;
                case EVENT_SHARED_SUFFERING:
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                        DoCast(target, DUNGEON_MODE(SPELL_SHARED_SUFFERING, H_SPELL_SHARED_SUFFERING));
                    events.ScheduleEvent(EVENT_SHARED_SUFFERING, 15000);
                    break;
            }

            DoMeleeAttackIfReady();
        }
    };

};

void AddSC_boss_marwyn()
{
    new boss_marwyn();
}
