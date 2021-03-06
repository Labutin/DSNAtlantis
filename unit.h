// START A3HEADER
//
// This source file is part of the Atlantis PBM game program.
// Copyright (C) 1995-1999 Geoff Dunbar
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program, in the file license.txt. If not, write
// to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
// Boston, MA 02111-1307, USA.
//
// See the Atlantis Project web page for details:
// http://www.prankster.com/project
//
// END A3HEADER
#ifndef UNIT_CLASS
#define UNIT_CLASS

class Unit;
class UnitId;

#include "faction.h"
#include "alist.h"
#include "gameio.h"
#include "orders.h"
#include "fileio.h"
#include "skills.h"
#include "items.h"
#include "object.h"

enum {
    GUARD_NONE,
    GUARD_GUARD,
    GUARD_AVOID,
    GUARD_SET,
    GUARD_ADVANCE
};

enum {
  TAX_NONE,
  TAX_TAX,
  TAX_PILLAGE
};

enum {
  REVEAL_NONE,
  REVEAL_UNIT,
  REVEAL_FACTION
};

enum {
  U_NORMAL,
  U_MAGE,
  U_GUARD,
  U_WMON,
  NUNITTYPES
};

#define FLAG_BEHIND 1
#define FLAG_AUTOTAX 4
#define FLAG_HOLDING 8
#define FLAG_NOAID 16
#define FLAG_INVIS 32
#define FLAG_CONSUMING_UNIT 64
#define FLAG_CONSUMING_FACTION 128
#define FLAG_NOSPOILS			0x0100
#define FLAG_FLYSPOILS			0x0200
#define FLAG_WALKSPOILS			0x0400
#define FLAG_RIDESPOILS			0x0800

class UnitId : public AListElem {
public:
  UnitId();
  ~UnitId();
  AString Print();
  
  int unitnum; /* if 0, it is a new unit */
  int alias;
  int faction;
};

class UnitPtr : public AListElem {
public:
  Unit * ptr;
};

UnitPtr *GetUnitList(AList *, Unit *);

class Unit : public AListElem
{
public:
    Unit();
    Unit(int,Faction *,int = 0);
    ~Unit();

    void SetMonFlags();
    void MakeWMon(char *,int,int);
  
    void Writeout( Aoutfile *f );
    void Readin( Ainfile *f, AList *, ATL_VER v );
    
    void WriteReport(Areport *,int,int,int,int);
    AString MageReport();
    AString * BattleReport();
    AString TemplateReport();
  
    void ClearOrders();
    void ClearCastOrders();
    void DefaultOrders(Object *);
    void SetName(AString *);
    void SetDescribe(AString *);
    void PostTurn(ARegion *reg);
    
    int IsLeader();
    int IsNormal();
    int GetMons();
    int GetMen();
    int GetSoldiers();
    int GetMen(int);
    void SetMen(int,int);
    int GetMoney();
    void SetMoney(int);
    int IsAlive();

    int MaintCost();
    void Short(int);
	
    int GetStealth();
    int GetTactics();
    int GetObservation();
    int GetEntertainment();
    int GetAttackRiding();
    int GetDefenseRiding();

    //
    // These are rule-set specific, in extra.cpp.
    //
    int GetStealthBonus();
    int GetObservationBonus();

    int GetSkill(int);
    void SetSkill(int,int);
    void SetSkillDays(int,int);
    int GetRealSkill(int);
    void ForgetSkill(int);
    int CheckDepend(int,int,int);
    int CanStudy(int);
    int Study(int,int); /* Returns 1 if it succeeds */
    void AdjustSkills();
// dsnlab
    AString SpoilsReport(void);
    void CheckMage (void);
    int CanGetSpoil(Item *i);
    void AdjustSkills(int);

    int CanSee(ARegion *,Unit *); /* Return 1 if can see, 2 if can see
                                     faction */
    int CanCatch(ARegion *,Unit *);
    int GetAttitude(ARegion *,Unit *); /* Get this unit's attitude toward
                                          the Unit parameter */
    int Hostile();
    int Forbids(ARegion *,Unit *);
    int Weight();
    int CanFly(int);
    int CanFly();
    int CanRide(int);
    int CanWalk(int);
    int CanSwim();
    int MoveType();
    int CalcMovePoints();
    int CanMoveTo(ARegion *,ARegion *);
    int GetFlag(int);
    void SetFlag(int,int);
    void CopyFlags(Unit *);
	int GetBattleItem( int batType, int index );

    int Taxers();
    
    void MoveUnit( Object *newobj );

    void Event(const AString &);
    void Error(const AString &);
  
    Faction * faction;
    Object *object;
    AString * name;
    AString * describe;
    int num;
    int type;
    int alias;
    int guard;	/* Also, avoid- see enum above */
    int reveal;
    int flags;
    int taxing;
    int movepoints;
    int canattack;
    int nomove;
    SkillList skills;
    ItemList items;
    int combat;
    AList oldorders;
    int needed; /* For assessing maintenance */
    int losses;

// dsnlab
    int age;    
    /* Orders */
    int destroy;
    int enter;
    UnitId * promote;
    AList findorders;
    AList giveorders;
    AList buyorders;
    AList tradeorders;
    AList sellorders;
    AList forgetorders;
    CastOrder *castorders;
    TeleportOrder *teleportorders;
    Order * stealorders;
    Order * monthorders;
    AttackOrder * attackorders;
    ARegion * advancefrom;
};

#endif
