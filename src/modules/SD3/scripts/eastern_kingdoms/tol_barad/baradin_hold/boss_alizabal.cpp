/**
 * ScriptDev3 is an extension for mangos providing enhanced features for
 * area triggers, creatures, game objects, instances, items, and spells beyond
 * the default database scripting in mangos.
 *
 * Copyright (C) 2006-2013  ScriptDev2 <http://www.scriptdev2.com/>
 * Copyright (C) 2014-2021 MaNGOS <https://getmangos.eu>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * World of Warcraft, and all World of Warcraft or Warcraft art, images,
 * and lore are copyrighted by Blizzard Entertainment, Inc.
 */

/* ScriptData
SDName: boss_alizabal
SD%Complete: 0%
SDComment:
SDCategory: Baradin Hold
EndScriptData */

#include "precompiled.h"
// #include "baradin_hold.h"

enum 
{
    // THESE VALUES ARE NOT CORRECT - THE CORRECT TEXT STILL NEEDS TO BE ADDED TO THE DATABASE. 
    // USING THESE VALUES AS PLACEHOLDERS. BUT THEY DO POINT TO ACTUAL TEXT.
    SAY_ALIZABAL_INTRO               = -1580044,
    SAY_ALIZABAL_KILL_PLAYER_1       = -1580043,
    SAY_ALIZABAL_KILL_PLAYER_2       = -1580064,
    SAY_ALIZABAL_KILL_PLAYER_3       = -1580065,
    SAY_ALIZABAL_KILL_PLAYER_4       = -1580066,
    SAY_ALIZABAL_DEATH               = -1580067,

    // NPCs
    NPC_ALIZABAL            = 55869

};

// The door to the room is: Doodad_TolBarad_Door_01. GUID = 207849, ID = 209849

struct boss_alizabal : public CreatureScript
{
    boss_alizabal() : CreatureScript("boss_alizabal") {}

    struct boss_alizabalAI : public ScriptedAI
    {
        boss_alizabalAI(Creature* pCreature) : ScriptedAI(pCreature) { }

        void Aggro(Unit* /*pWho*/) override
        {
            DoScriptText(SAY_ALIZABAL_INTRO, m_creature);
        }

        void KilledUnit(Unit* pVictim) override
        {
            if (pVictim->GetTypeId() != TYPEID_PLAYER)
            {
                return;
            }

            // if (urand(0, 4))
            // {
            //     return;
            // }

            switch (urand(0, 3))
            {
            case 0: DoScriptText(SAY_ALIZABAL_KILL_PLAYER_1, m_creature); break;
            case 1: DoScriptText(SAY_ALIZABAL_KILL_PLAYER_2, m_creature); break;
            case 2: DoScriptText(SAY_ALIZABAL_KILL_PLAYER_3, m_creature); break;
            case 3: DoScriptText(SAY_ALIZABAL_KILL_PLAYER_4, m_creature); break;
            }
        }

        void JustDied(Unit* /*pKiller*/) override
        {
            DoScriptText(SAY_ALIZABAL_DEATH, m_creature);
        }
    };

    CreatureAI* GetAI(Creature* pCreature) override
    {
        return new boss_alizabalAI(pCreature);
    }
};

void AddSC_boss_alizabal()
{
    Script* s;

    s = new boss_alizabal();
    s->RegisterSelf();

    //pNewScript = new Script;
    //pNewScript->Name = "boss_alizabal";
    //pNewScript->GetAI = &GetAI_boss_alizabal;
    //pNewScript->RegisterSelf();
}