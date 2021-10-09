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
#include "baradin_hold.h"

// The door to the room is: Doodad_TolBarad_Door_01. GUID = 207849, ID = 209849
static const DialogueEntry aIntroDialogue[] =
{
    {SAY_ALIZABAL_INTRO, NPC_ALIZABAL, 1000},
    {0, 0, 0},
};

struct boss_alizabal : public CreatureScript
{
    boss_alizabal() : CreatureScript("boss_alizabal") {}

    struct boss_alizabalAI : public ScriptedAI
    {
        boss_alizabalAI(Creature* pCreature) : ScriptedAI(pCreature),
        m_introDialogue(aIntroDialogue)
        {
            m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
            m_introDialogue.InitializeDialogueHelper(m_pInstance);
        }

        ScriptedInstance* m_pInstance;
        DialogueHelper m_introDialogue;

        bool m_bDidIntro;

        void Reset() override
        {
            m_bDidIntro = false;
        }

        void Aggro(Unit* /*pWho*/) override
        {
            DoScriptText(SAY_ALIZABAL_INTRO, m_creature);

            if (m_pInstance)
            {
                m_pInstance->SetData(TYPE_ALIZABAL, IN_PROGRESS);
            }
        }

        void KilledUnit(Unit* pVictim) override
        {
            if (pVictim->GetTypeId() != TYPEID_PLAYER)
            {
                return;
            }

            switch(urand(0,4)) {
            case 0:
                DoScriptText(SAY_ALIZABAL_KILL_PLAYER_1, m_creature);
                break;
            case 1:
                DoScriptText(SAY_ALIZABAL_KILL_PLAYER_2, m_creature);
                break;
            case 2:
                DoScriptText(SAY_ALIZABAL_KILL_PLAYER_3, m_creature);
                break;
            case 3:
                DoScriptText(SAY_ALIZABAL_KILL_PLAYER_4, m_creature);
                break;
            }
        }

        void JustDied(Unit* /*pKiller*/) override
        {
            if (m_pInstance)
            {
                m_pInstance->SetData(TYPE_ALIZABAL, DONE);
                DoScriptText(SAY_ALIZABAL_DEATH, m_creature);
            }
        }
        // void JustDied(Unit* /*pKiller*/) override
        // {
        //     if (m_pInstance)
        //     {
        //         if (Creature* pAlizabal = m_pInstance->GetSingleCreatureFromStorage(NPC_ALIZABAL))
        //         {
        //             if (!pAlizabal->IsAlive())
        //             {
        //                 m_pInstance->SetData(TYPE_ALIZABAL, DONE);
        //                 DoScriptText(SAY_ALIZABAL_DEATH, m_creature);
        //             }
        //             // else
        //             // {
        //             //     // Remove loot flag
        //             //     m_creature->RemoveFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_LOOTABLE); // ?? Do we need this? Left over from boss_eredar_twins.cpp example.
        //             // }
        //         }
        //     }
        // }

    };

    CreatureAI* GetAI(Creature* pCreature) override
    {
        return new boss_alizabalAI(pCreature);
    }
};
