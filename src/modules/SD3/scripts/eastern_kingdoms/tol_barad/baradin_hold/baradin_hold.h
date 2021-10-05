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

#ifndef DEF_BARADIN_HOLD_H
#define DEF_BARADIN_HOLD_H

enum 
{
    MAX_ENCOUNTER           = 3,                            // ? [Fyre] There are 3 bosses... max_encounter = 3?

    // Alizabal Text (Source: https://wowpedia.fandom.com/wiki/Alizabal)
    //// These were not in script_texts yet. Need to add them and then use the `entry` value for each.
    SAY_INTRO               = "How I HATE this place. My captors may be long-dead, but don't think I won't take it all out on you miserable treasure-hunters.",
    SAY_AGGRO               = "I hate adventurers.",
    SAY_BLADE_DANCE_1       = "I hote standing still!",
    SAY_BLADE_DANCE_2       = "I hate you all!",
    SAY_SEETHING_HATE_1     = "Feel my hatred!",
    SAY_SEETHING_HATE_2     = "My hatred burns!",
    SAY_SEETHING_HATE_3     = "My hate will consume you!",
    SAY_SKEWER_1            = "I hate armor.",
    SAY_SKEWER_2            = "I hate martyrs.",
    SAY_KILL_PLAYER_1       = "I still hate you.",
    SAY_KILL_PLAYER_2       = "Do you hate me? Good.",
    SAY_KILL_PLAYER_3       = "I hate mercy.",
    SAY_KILL_PLAYER_4       = "I didn't hate that.",
    SAY_WIPE                = "I hate incompetent raiders.",
    SAY_DEATH               = "I hate... every one of you...",

    // NPCs
    NPC_ARGALOTH            = 47120,
    NPC_OCCUTHAR            = 52363,
    NPC_ALIZABAL            = 55869

};

#endif
