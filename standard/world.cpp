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
#include "game.h"
#include "rules.h"

static char *regionnames[] =
{
    "Aathon",
    "Aberaeron",
    "Aberdaron",
    "Aberdovey",
    "Abernecht",
    "Abernethy",
    "Abersoch",
    "Abrantes",
    "Abrenton",
    "Adrano",
    "AeBrey",
    "Aghleam",
    "Akbou",
    "Aldan",
    "Alfaro",
    "Alghero",
    "Almeria",
    "Altnaharra",
    "Ancroft",
    "Anshun",
    "Anstruther",
    "Antor",
    "Arbroath",
    "Arcila",
    "Ardfert",
    "Ardvale",
    "Arezzo",
    "Ariano",
    "Arlon",
    "Avanos",
    "Aveiro",
    "Badalona",
    "Baechahoela",
    "Ballindine",
    "Balta",
    "Banlar",
    "Barika",
    "Bastak",
    "Bayonne",
    "Bejaia",
    "Benlech",
    "Beragh",
    "Bergland",
    "Berneray",
    "Berriedale",
    "Binhai",
    "Birde",
    "Bocholt",
    "Bogmadie",
    "Braga",
    "Brechlin",
    "Brodick",
    "Burscough",
    "Calpio",
    "Canna",
    "Capperwe",
    "Caprera",
    "Carahue",
    "Carbost",
    "Carnforth",
    "Carrigaline",
    "Caserta",
    "Catrianchi",
    "Clatter",
    "Coilaco",
    "Corinth",
    "Corofin",
    "Corran",
    "Corwen",
    "Crail",
    "Cremona",
    "Crieff",
    "Cromarty",
    "Cumbraes",
    "Daingean",
    "Darm",
    "Decca",
    "Derron",
    "Derwent",
    "Deveron",
    "Dezhou",
    "Doedbygd",
    "Doramed",
    "Dornoch",
    "Drammes",
    "Dremmer",
    "Drense",
    "Drimnin",
    "Drumcollogher",
    "Drummore",
    "Dryck",
    "Drymen",
    "Dunbeath",
    "Duncansby",
    "Dunfanaghy",
    "Dunkeld",
    "Dunmanus",
    "Dunster",
    "Durness",
    "Duucshire",
    "Elgomaar",
    "Ellesmere",
    "Ellon",
    "Enfar",
    "Erisort",
    "Eskerfan",
    "Ettrick",
    "Fanders",
    "Farafra",
    "Ferbane",
    "Fetlar",
    "Flock",
    "Florina",
    "Formby",
    "Frainberg",
    "Galloway",
    "Ganzhou",
    "Geal Charn",
    "Gerr",
    "Gifford",
    "Girvan",
    "Glenagallagh",
    "Glenanane",
    "Glin",
    "Glomera",
    "Glormandia",
    "Gluggby",
    "Gnackstein",
    "Gnoelhaala",
    "Golconda",
    "Gourock",
    "Graevbygd",
    "Grandola",
    "Gresberg",
    "Gresir",
    "Greverre",
    "Griminish",
    "Grisbygd",
    "Groddland",
    "Grue",
    "Gurkacre",
    "Haikou",
    "Halkirk",
    "Handan",
    "Hasmerr",
    "Helmsdale",
    "Helmsley",
    "Helsicke",
    "Helvete",
    "Hoersalsveg",
    "Hullevala",
    "Ickellund",
    "Inber",
    "Inverie",
    "Jaca",
    "Jahrom",
    "Jeormel",
    "Jervbygd",
    "Jining",
    "Jotel",
    "Kaddervar",
    "Karand",
    "Karothea",
    "Kashmar",
    "Keswick",
    "Kielder",
    "Killorglin",
    "Kinbrace",
    "Kintore",
    "Kirriemuir",
    "Klen",
    "Knesekt",
    "Kobbe",
    "Komarken",
    "Kovel",
    "Krod",
    "Kursk",
    "Lagos",
    "Lamlash",
    "Langholm",
    "Larache",
    "Larkanth",
    "Larmet",
    "Lautaro",
    "Leighlin",
    "Lervir",
    "Leven",
    "Licata",
    "Limavady",
    "Lingen",
    "Lintan",
    "Liscannor",
    "Locarno",
    "Lochalsh",
    "Lochcarron",
    "Lochinver",
    "Lochmaben",
    "Lom",
    "Lorthalm",
    "Louer",
    "Lurkabo",
    "Luthiir",
    "Lybster",
    "Lynton",
    "Mallaig",
    "Mataro",
    "Melfi",
    "Melvaig",
    "Menter",
    "Methven",
    "Moffat",
    "Monamolin",
    "Monzon",
    "Morella",
    "Morgel",
    "Mortenford",
    "Mullaghcarn",
    "Mulle",
    "Murom",
    "Nairn",
    "Navenby",
    "Nephin Beg",
    "Niskby",
    "Nolle",
    "Nork",
    "Olenek",
    "Oloron",
    "Oranmore",
    "Ormgryte",
    "Orrebygd",
    "Palmi",
    "Panyu",
    "Partry",
    "Pauer",
    "Penhalolen",
    "Perkel",
    "Perski",
    "Planken",
    "Plattland",
    "Pleagne",
    "Pogelveir",
    "Porthcawl",
    "Portimao",
    "Potenza",
    "Praestbygd",
    "Preetsome",
    "Presu",
    "Prettstern",
    "Rantlu",
    "Rappbygd",
    "Rath Luire",
    "Rethel",
    "Riggenthorpe",
    "Rochfort",
    "Roddendor",
    "Roin",
    "Roptille",
    "Roter",
    "Rueve",
    "Sagunto",
    "Saklebille",
    "Salen",
    "Sandwick",
    "Sarab",
    "Sarkanvale",
    "Scandamia",
    "Scarinish",
    "Scourie",
    "Serov",
    "Shanyin",
    "Siegen",
    "Sinan",
    "Sines",
    "Skim",
    "Skokholm",
    "Skomer",
    "Skottskog",
    "Sledmere",
    "Sorisdale",
    "Spakker",
    "Stackforth",
    "Staklesse",
    "Stinchar",
    "Stoer",
    "Strichen",
    "Stroma",
    "Stugslett",
    "Suide",
    "Tabuk",
    "Tarraspan",
    "Tetuan",
    "Thurso",
    "Tiemcen",
    "Tiksi",
    "Tolsta",
    "Toppola",
    "Torridon",
    "Trapani",
    "Tromeforth",
    "Tudela",
    "Turia",
    "Uxelberg",
    "Vaila",
    "Valga",
    "Verguin",
    "Vernlund",
    "Victoria",
    "Waimer",
    "Wett",
    "Xontormia",
    "Yakleks",
    "Yuci",
    "Zaalsehuur",
    "Zamora",
    "Zapulla",
};

//
// The following stuff is just for in this file, to setup the names during
// world setup
//

static int nnames;
static int * nameused;

void SetupNames()
{
    nnames = sizeof regionnames / sizeof (char *);
    nameused = new int[nnames];
    
    for (int i=0; i<nnames; i++) nameused[i] = 0;
}

int AGetName( ARegion *pReg, int town )
{
    int i=0;
    while (i++ < nnames)
    {
        int i = getrandom(nnames);
        if (nameused[i] == 0)
        {
            nameused[i] = 1;
            return i;
        }
    }
    for (i=0; i<nnames; i++) nameused[i] = 0;
    i = getrandom(nnames);
    nameused[i] = 1;
    return i;
}

char *AGetNameString( int name )
{
    return( regionnames[ name ] );
}

char Game::GetRChar(ARegion * r)
{
    int t = r->type;
    char c;
    switch (t) {
    case R_OCEAN:
        return '-';

    case R_PLAIN:
        c = 'p';
        break;
    case R_FOREST:
        c = 'f';
        break;
    case R_MOUNTAIN:
        c = 'm';
        break;
    case R_SWAMP:
        c = 's';
        break;
    case R_JUNGLE:
        c = 'j';
        break;
    case R_DESERT:
        c = 'd';
        break;
    case R_TUNDRA:
        c = 't';
        break;
    case R_CAVERN:
        c = 'c';
        break;
    case R_UFOREST:
        c = 'f';
        break;
    case R_TUNNELS:
        c = 't';
        break;
    default:
        return '?';

    }
    
    if (r->town) {
        c = (c - 'a') + 'A';
    }
    return c;
}

void Game::CreateWorld()
{
    int xx = 0;
    while (xx <= 0)
    {
        Awrite("How wide should the map be? ");
        xx = Agetint();
        if( xx % 8 )
        {
            xx = 0;
            Awrite( "The width must be a multiple of 8." );
        }
    }
    int yy = 0;
    while (yy <= 0)
    {
        Awrite("How tall should the map be? ");
        yy = Agetint();
        if( yy % 8 )
        {
            yy = 0;
            Awrite( "The height must be a multiple of 8." );
        }
    }

    regions.CreateLevels( 3 );

    SetupNames();

    regions.CreateNexusLevel( 0, "nexus" );
    regions.CreateSurfaceLevel( 1, xx, yy, 40, 10, 0 );
    regions.CreateUnderworldLevel( 2, xx / 2, yy / 2, "underworld" );

    regions.MakeShaftLinks( 2, 1, 8 );

    regions.SetACNeighbors( 0, 1, xx, yy );

    regions.InitSetupGates( 1 );
    regions.InitSetupGates( 2 );
    regions.FinalSetupGates();

    regions.CalcDensities();
}

void Game::CreateNPCFactions()
{
    guardfaction = factionseq;
    Faction * f = new Faction(factionseq++);
    AString * temp = new AString("The Guardsmen");
    f->SetName(temp);
    f->SetNPC();
    factions.Add(f);
  
    f = new Faction(factionseq++);
    monfaction = f->num;
    temp = new AString("Creatures");
    f->SetName(temp);
    f->SetNPC();
    factions.Add(f);
}

void Game::CreateCityMon( ARegion *pReg, int percent )
{
    int skilllevel;
    int AC = 0;
    if( pReg->type == R_NEXUS || pReg->IsStartingCity() )
    {
        skilllevel = TOWN_CITY + 12;
        AC = 1;
    }
    else
    {
        skilllevel = pReg->town->TownType() + 1;
    }
    int num = Globals->CITY_GUARD * skilllevel;
    num = num * percent / 100;

    Faction *pFac = GetFaction( &factions, 1 );

    Unit *u = GetNewUnit( pFac );
    AString *s = new AString("City Guard");
    u->SetName( s );
    u->type = U_GUARD;
    u->guard = GUARD_GUARD;
  
    u->SetMen(I_LEADERS,num);
    if (AC) u->items.SetNum(I_MSWORD,num); else u->items.SetNum(I_SWORD,num);
    if (AC) u->items.SetNum(I_AMULETOFI,num);
    u->SetMoney(num * Globals->GUARD_MONEY);	
    u->SetSkill(S_COMBAT,skilllevel);
    u->SetSkill(S_RIDING,skilllevel);
    if (AC) {
        u->SetSkill(S_OBSERVATION,15);
    } else {
        u->SetSkill(S_OBSERVATION,skilllevel);
    }
    u->SetFlag(FLAG_HOLDING,1);
    if (AC) u->SetFlag(FLAG_BEHIND,1);
    
    u->MoveUnit( pReg->GetDummy() );
}

void Game::AdjustCityMons( ARegion *r )
{
    int guard = 0;
    forlist(&r->objects) {
        Object * o = (Object *) elem;
        forlist(&o->units) {
            Unit * u = (Unit *) elem;
            if (u->type == U_GUARD)
            {
                AdjustCityMon( r, u );
                return;
            }
            if (u->guard == GUARD_GUARD) {
                guard = 1;
            }
        }
    }

    if (!guard && getrandom(100) < Globals->GUARD_REGEN)
    {
        CreateCityMon( r, 10 );
    }
}

void Game::AdjustCityMon( ARegion *r, Unit *u )
{
    int towntype;
    int AC = 0;
    if( r->type == R_NEXUS || r->IsStartingCity() )
    {
        towntype = TOWN_CITY + 12;
        AC = 1;
    }
    else
    {
        towntype = r->town->TownType();
    }

    int men = u->GetMen() + (Globals->CITY_GUARD / 10) * (towntype + 1);
    if (men > Globals->CITY_GUARD * (towntype + 1))
        men = Globals->CITY_GUARD * (towntype + 1);
    u->SetMen(I_LEADERS,men);

    u->SetMoney(men * Globals->GUARD_MONEY);
    u->SetSkill(S_COMBAT,towntype + 1);
    u->SetSkill(S_RIDING,towntype + 1);
    if (AC)
    {
        u->SetSkill(S_OBSERVATION,15);
        u->items.SetNum(I_MSWORD,men);
    }
    else
    {
        u->SetSkill(S_OBSERVATION,towntype + 1);
        u->items.SetNum(I_SWORD,men);
    }
    if (AC) u->items.SetNum(I_AMULETOFI,men);
}

int Game::MakeWMon( ARegion *pReg )
{
    if (TerrainDefs[pReg->type].wmonfreq == 0)
    {
        return 0;
    }

    int montype = TerrainDefs[ pReg->type ].smallmon;
    if (getrandom(2))
        montype = TerrainDefs[ pReg->type ].humanoid;
    if (TerrainDefs[ pReg->type ].bigmon != -1 && !getrandom(8)) {
        montype = TerrainDefs[ pReg->type ].bigmon;
    }
    
    int mondef = ItemDefs[montype].index;
    
    Faction *monfac = GetFaction( &factions, 2 );

    Unit *u = GetNewUnit( monfac, 0 );
    u->MakeWMon( MonDefs[mondef].name, montype,
                 (MonDefs[mondef].number +
                  getrandom(MonDefs[mondef].number) + 1) / 2);
    u->MoveUnit( pReg->GetDummy() );
    return( 1 );
}

void Game::MakeLMon( Object *pObj )
{
    int montype = ObjectDefs[ pObj->type ].monster;
    if (montype == I_TRENT)
    {
        montype = TerrainDefs[ pObj->region->type].bigmon;
    }
    if (montype == I_CENTAUR)
    {
        montype = TerrainDefs[ pObj->region->type ].humanoid;
    }

    int mondef = ItemDefs[montype].index;
    Faction *monfac = GetFaction( &factions, 2 );
    Unit *u = GetNewUnit( monfac, 0 );
    if (montype == I_IMP)
    {
        u->MakeWMon( "Demons",
                     I_IMP,
                     getrandom( MonDefs[MONSTER_IMP].number + 1 ));
        u->items.SetNum( I_DEMON,
                         getrandom( MonDefs[MONSTER_DEMON].number + 1 ));
        u->items.SetNum( I_BALROG,
                         getrandom( MonDefs[MONSTER_BALROG].number + 1 ));
    }
    else if (montype == I_SKELETON)
    {
        u->MakeWMon( "Undead",
                     I_SKELETON,
                     getrandom( MonDefs[MONSTER_SKELETON].number + 1 ));
        u->items.SetNum( I_UNDEAD,
                         getrandom( MonDefs[MONSTER_UNDEAD].number + 1 ));
        u->items.SetNum( I_LICH,
                         getrandom( MonDefs[MONSTER_LICH].number + 1 ));
    }
    else
    {
        u->MakeWMon( MonDefs[mondef].name,
                     montype,
                     ( MonDefs[mondef].number +
                       getrandom( MonDefs[mondef].number ) + 1) / 2);
    }

    u->MoveUnit( pObj );
}

int ARegionList::GetRegType( ARegion *pReg )
{
    //
    // Figure out the distance from the equator, from 0 to 3.
    //
    int lat = ( pReg->yloc * 8 ) / ( pRegionArrays[ pReg->zloc ]->y );
    if (lat > 3) 
    {
        lat = (7 - lat);
    }

    if( pReg->zloc == 2 )
    {
        int r = getrandom(4);
        switch (r)
        {
        case 0:
            return R_OCEAN;
        case 1:
            return R_CAVERN;
        case 2:
            return R_UFOREST;
        case 3:
            return R_TUNNELS;
        default:
            return( 0 );
        }
    }

    if( pReg->zloc == 1 )
    {
        int r = getrandom(64);
        switch (lat)
        {
        case 0: /* Arctic regions */
            if (r < 24) return R_TUNDRA;
            if (r < 32) return R_MOUNTAIN;
            if (r < 40) return R_FOREST;
            return R_PLAIN;
        case 1: /* Colder regions */
            if (r < 16) return R_PLAIN;
            if (r < 40) return R_FOREST;
            if (r < 48) return R_MOUNTAIN;
            return R_SWAMP;
        case 2: /* Warmer regions */
            if (r < 20) return R_PLAIN;
            if (r < 28) return R_FOREST;
            if (r < 36) return R_MOUNTAIN;
            if (r < 44) return R_SWAMP;
            if (r < 52) return R_JUNGLE;
            return R_DESERT;
        case 3: /* tropical */
            if (r < 16) return R_PLAIN;
            if (r < 24) return R_MOUNTAIN;
            if (r < 36) return R_SWAMP;
            if (r < 48) return R_JUNGLE;
            return R_DESERT;
        }
        return R_OCEAN;
    }

    if( pReg->zloc == 0 )
    {
        //
        // This really shouldn't ever get called.
        //
        return( R_NEXUS );
    }

    //
    // This really shouldn't get called either
    //
    return( R_OCEAN );
}

int ARegionList::CheckRegionExit( int nDir, ARegion *pFrom, ARegion *pTo )
{
    if( pFrom->zloc != 2 )
    {
        return( 1 );
    }

    int chance = 0;
    if( pFrom->type == R_CAVERN || pFrom->type == R_UFOREST ||
        pTo->type == R_CAVERN || pTo->type == R_UFOREST )
    {
        chance = 25;
    }
    if( pFrom->type == R_TUNNELS || pTo->type == R_TUNNELS)
    {
        chance = 50;
    }
    if (getrandom(100) < chance)
    {
        return( 0 );
    }
    return( 1 );
}

int ARegionList::GetWeather( ARegion *pReg, int month )
{
    if (pReg->zloc == 0)
    {
        return W_NORMAL;
    }

    if( pReg->zloc == 2 )
    {
        return( W_NORMAL );
    }
  
    int ysize = pRegionArrays[ 1 ]->y;

    if ((3*( pReg->yloc+1))/ysize == 0)
    {
        /* Northern third of the world */
        if (month > 9 || month < 2)
        {
            return W_WINTER;
        } 
        else
        {
            return W_NORMAL;
        }
    }
  
    if ((3*( pReg->yloc+1))/ysize == 1)
    {
        /* Middle third of the world */
        if (month == 11 || month == 0 || month == 5 || month == 6)
        {
            return W_MONSOON;
        } 
        else
        {
            return W_NORMAL;
        }
    }
  
    if (month > 3 && month < 8)
    {
        /* Southern third of the world */
        return W_WINTER;
    } 
    else
    {
        return W_NORMAL;
    }
}

int ARegion::CanBeStartingCity( ARegionArray *pRA )
{
    if (!town) return 0;
    if (!IsCoastal()) return 0;
    if (town->pop == 5000) return 0;

    int regs = 0;
    AList inlist;
    AList donelist;
  
    ARegionPtr * temp = new ARegionPtr;
    temp->ptr = this;
    inlist.Add(temp);

    while(inlist.Num())
    {
        ARegionPtr * reg = (ARegionPtr *) inlist.First();
        for (int i=0; i<NDIRS; i++)
        {
            ARegion * r2 = reg->ptr->neighbors[i];
            if (!r2) continue;
            if (r2->type == R_OCEAN) continue;
            if (GetRegion(&inlist,r2->num)) continue;
            if (GetRegion(&donelist,r2->num)) continue;
            regs++;
            if (regs>20) return 1;
            ARegionPtr * temp = new ARegionPtr;
            temp->ptr = r2;
            inlist.Add(temp);
        }
        inlist.Remove(reg);
        donelist.Add(reg);
    }
    return 0;
}

void ARegion::MakeStartingCity() 
{
    gate = -1;
    if( !town )
    {
        AddTown();
    }

    town->pop = 5000;
    town->basepop = 5000;

    markets.DeleteAll();
    for (int i=0; i<NITEMS; i++) {
        if( ItemDefs[ i ].type & IT_NORMAL )
        {
            if (i==I_SILVER || i==I_LIVESTOCK || i==I_FISH || i==I_GRAIN) 
            {
                continue;
            }
            Market * m = new Market(M_BUY,i,(ItemDefs[i].baseprice * 5 / 2),-1,
                                    5000,5000,-1,-1);
            markets.Add(m);
        }
    }

    Market * m = new Market(M_BUY,race,Wages()*4,-1,5000,5000,-1,-1);
    markets.Add(m);
    m = new Market(M_BUY,I_LEADERS,Wages()*8,-1,5000,5000,-1,-1);
    markets.Add(m);
}

int ARegion::IsStartingCity() {
    if (town && town->pop == 5000) return 1;
    return 0;
}

int ARegion::IsSafeRegion()
{
//    return ( IsStartingCity() );
  return 0;
}

ARegion *ARegionList::GetStartingCity( ARegion *AC,
                                       int i,
                                       int level,
                                       int maxX,
                                       int maxY )
{
    ARegionArray *pArr = pRegionArrays[ level ];
    ARegion * reg = 0;

    if( pArr->x < maxX )
    {
        maxX = pArr->x;
    }
    if( pArr->y < maxY )
    {
        maxY = pArr->y;
    }

    while (!reg)
    {
        //
        // We'll just let AC exits be all over the map.
        //
        int x = getrandom( maxX );
        int y = 2 * getrandom( maxY / 2 ) + x % 2;

        reg = GetRegion( x, y, level );
        
        if( !reg->CanBeStartingCity( pArr ))
        {
            reg = 0;
            continue;
        }

        for (int j=0; j<i; j++)
        {
            if (GetDistance(reg,AC->neighbors[j]) < maxY / 10 + 2 )
            {
                reg = 0;
                break;
            }
        }
    }
    return reg;
}

