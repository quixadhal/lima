/* Do not remove the headers from this file! see /USAGE for more info. */

// Things which are common to all combat modules

inherit LIVING;
inherit M_DAMAGE_SOURCE;

inherit __DIR__ "base/wield";
inherit __DIR__ "base/target";
inherit __DIR__ "base/death";
inherit __DIR__ "base/results";
inherit __DIR__ "base/behaviors";
#ifdef COMBAT_USES_HEARTBEATS
inherit __DIR__ "base/heart_beat";
#else
inherit __DIR__ "base/non_heart_beat";
#endif

void remove() {
#ifndef COMBAT_USES_HEARTBEATS
    non_heart_beat::remove();
#endif
    damage_source::remove();
    living::remove();
}
