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

enum Creatures
{
    NPC_ALIZABAL                      = 55869
};

enum Yells
{
    YELL_ALIZABAL_INTRO               = -1999927,
    YELL_ALIZABAL_AGGRO               = -1999928,
    YELL_ALIZABAL_BLADE_DANCE_1       = -1999929,
    YELL_ALIZABAL_BLADE_DANCE_2       = -1999930,
    YELL_ALIZABAL_SEETHING_HATE_1     = -1999931,
    YELL_ALIZABAL_SEETHING_HATE_2     = -1999932,
    YELL_ALIZABAL_SEETHING_HATE_3     = -1999933,
    YELL_ALIZABAL_SKEWER_1            = -1999934,
    YELL_ALIZABAL_SKEWER_2            = -1999935,
    YELL_ALIZABAL_KILL_PLAYER_1       = -1999936,
    YELL_ALIZABAL_KILL_PLAYER_2       = -1999937,
    YELL_ALIZABAL_KILL_PLAYER_3       = -1999938,
    YELL_ALIZABAL_KILL_PLAYER_4       = -1999939,
    YELL_ALIZABAL_WIPE                = -1999940,
    YELL_ALIZABAL_DEATH               = -1999941
};

enum Spells
{
    SPELL_SKEWER                      = 104936,
    SPELL_SEETHING_HATE               = 105067,
    SPELL_BLADE_DANCE                 = 104994,
    SPELL_BERSERK                     = 47008
};

enum GameObjects
{
    GAMEOBJECT_TOLBARAD_DOOR_01       = 207849      // The door to the room is: Doodad_TolBarad_Door_01. GUID = 207849, ID = 209849
};


// First and foremost,  Skewer and  Seething Hate are on a shared mechanic.  [>> DONE <<]
// They are both exactly a 20 second CD, there will always be 2 casts of each between each  Blade Dance. 
// Only 1 cast of each before first blade dance of fight though since it comes earlier then 1 minute.  Skewer is always cast on the current aggro target (hopefully the tank).  
// Seething Hate is cast on a random target in raid. This CAN target tanks.

// Pull:
// 1. 6-8 seconds in she will either cast  Seething Hate OR  Skewer, which one is cast is random.  [>> DONE <<]
// 2. About 8 seconds after first one, she'll cast the one she didn't cast.

// 1. After any time a  Blade Dance ends, 8 seconds later just like pull, she'll cast randomly either  Skewer or  Seething Hate. There is no way to know which one it will be definitively
// 2. 8 seconds after previous, she'll cast opposite spell.
// 3. 20 seconds after first special was used that special will be used again
// 4. 20 seconds after second special was used that special will be used again.
// 5. Next  Blade Dance will come after both specials have been cast twice. Which is 60 seconds after last.


struct boss_alizabal : public CreatureScript
{
    boss_alizabal() : CreatureScript("boss_alizabal") {}

    struct boss_alizabalAI : public ScriptedAI
    {
        boss_alizabalAI(Creature* pCreature) : ScriptedAI(pCreature) { }

        // Timers
        uint32 m_uiEnrageTimer;
        uint32 m_uiSharedTimerFirst;
        uint32 m_uiSharedTimerSecond;
        uint32 m_uiBladeDanceTimer;

        void Reset() override
        {
            DoScriptText(YELL_ALIZABAL_WIPE, m_creature);

            m_uiEnrageTimer = 5 * MINUTE * IN_MILLISECONDS;
            m_uiSharedTimerFirst = urand(6,8) * IN_MILLISECONDS;
            // m_uiSharedTimerSecond
            m_uiBladeDanceTimer = 25 * IN_MILLISECONDS;
        }

        void Aggro(Unit* /*pWho*/) override
        {
            DoScriptText(YELL_ALIZABAL_AGGRO, m_creature);
        }

        void UpdateAI(const uint32 uiDiff) override
        {
            if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            {
                return;
            }

            // Shared Timer for Seething Hate and Skewer
            if (m_uiSharedTimerFirst < uiDiff)
            {
                switch (urand(0, 1))
                {
                case 0:  // Seething Hate is cast onto random target.
                    if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1, SPELL_SEETHING_HATE, SELECT_FLAG_PLAYER))
                    {
                        if (DoCastSpellIfCan(m_creature, SPELL_SEETHING_HATE) == CAST_OK)
                        {
                            m_uiSharedTimerFirst = 8 * IN_MILLISECONDS;
                            switch (urand(0,2))
                            {
                                case 0: DoScriptText(YELL_ALIZABAL_SEETHING_HATE_1, m_creature); break;
                                case 1: DoScriptText(YELL_ALIZABAL_SEETHING_HATE_2, m_creature); break;
                                case 2: DoScriptText(YELL_ALIZABAL_SEETHING_HATE_3, m_creature); break;
                            }
                        }
                    }
                    break;

                case 1:   // Skewer is cast onto current target.
                    if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_SKEWER) == CAST_OK)
                    {
                        m_uiSharedTimerFirst = 8 * IN_MILLISECONDS;
                        switch (urand(0,1))
                        {
                            case 0: DoScriptText(YELL_ALIZABAL_SKEWER_1, m_creature); break;
                            case 1: DoScriptText(YELL_ALIZABAL_SKEWER_2, m_creature); break;
                        }
                    }
                    break;
                }
            }
            else
            {
                m_uiSharedTimerFirst -= uiDiff;
            }

            // Blade Dance Timer
            if (m_uiBladeDanceTimer < uiDiff)
            {
                if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_BLADE_DANCE) == CAST_OK)
                {
                    m_uiBladeDanceTimer = 1 * MINUTE * IN_MILLISECONDS;
                    switch (urand(0,1))
                    {
                        case 0: DoScriptText(YELL_ALIZABAL_BLADE_DANCE_1, m_creature); break;
                        case 1: DoScriptText(YELL_ALIZABAL_BLADE_DANCE_2, m_creature); break;
                    }
                }
            }
            else
            {
                m_uiBladeDanceTimer -= uiDiff;
            }

            // Berserk Timer
            if (m_uiEnrageTimer < uiDiff)
            {
                if (DoCastSpellIfCan(m_creature, SPELL_BERSERK) == CAST_OK)
                {
                    m_uiEnrageTimer = 5 * MINUTE * IN_MILLISECONDS;
                }
            }
            else
            {
                m_uiEnrageTimer -= uiDiff;
            }

            DoMeleeAttackIfReady();
        }

        void KilledUnit(Unit* pVictim) override
        {
            if (pVictim->GetTypeId() != TYPEID_PLAYER)
            {
                return;
            }

            switch (urand(0, 3))
            {
                case 0: DoScriptText(YELL_ALIZABAL_KILL_PLAYER_1, m_creature); break;
                case 1: DoScriptText(YELL_ALIZABAL_KILL_PLAYER_2, m_creature); break;
                case 2: DoScriptText(YELL_ALIZABAL_KILL_PLAYER_3, m_creature); break;
                case 3: DoScriptText(YELL_ALIZABAL_KILL_PLAYER_4, m_creature); break;
            }
        }

        void JustDied(Unit* /*pKiller*/) override
        {
            DoScriptText(YELL_ALIZABAL_DEATH, m_creature);
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



// Database Scripts to Support This Script.
// INSERT INTO `script_binding` (`type`,`ScriptName`,`bind`,`data`) VALUES (0, 'boss_alizabal', 55869, 0);
// DELETE FROM `script_texts` WHERE `entry` IN (-1999927,-1999928,-1999929,-1999930,-1999931,-1999932,-1999933,9-1999934,-1999935,-1999936,-1999937,-1999938,-1999939,-1999940,-1999941);
// INSERT INTO `script_texts` (`entry`,`content_default`,`type`,`comment`) VALUES 
// (-1999927,'How I HATE this place. My captors may be long-dead, but don\'t think I wo>
// (-1999928,'I hate adventurers.',1,''),
// (-1999929,'I hate standing still!',1,''),
// (-1999930,'I hate you all!',1,''),
// (-1999931,'Feel my hatred!',1,''),
// (-1999932,'My hatred burns!',1,''),
// (-1999933,'My hate will consume you!',1,''),
// (-1999934,'I hate armor.',1,''),
// (-1999935,'I hate martyrs.',1,''),
// (-1999936,'I still hate you.',1,''),
// (-1999937,'Do you hate me? Good.',1,''),
// (-1999938,'I hate mercy.',1,''),
// (-1999939,'I didn\'t hate that.',1,''),
// (-1999940,'I hate incompetent raiders.',1,''),
// (-1999941,'I hate... every one of you...',1,'');
