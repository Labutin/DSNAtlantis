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
#ifdef WIN32
#include <memory.h>  // Needed for memcpy on windows
#endif


#include "game.h"
#include "unit.h"
#include "fileio.h"
#include "astring.h"
#include "rules.h"

Game::Game()
{
    gameStatus = GAME_STATUS_UNINIT;
    ppUnits = 0;
    maxppunits = 0;
}

Game::~Game() {
    delete ppUnits;
    ppUnits = 0;
    maxppunits = 0;
}

int Game::TurnNumber() {
	return (year-1)*12 + month + 1;
}

AString Game::GetXtraMap(ARegion * reg,int type)
{
    int i;
    switch (type) {
    case 0:
        return (reg->IsStartingCity() ? "!" : (reg->HasShaft() ? "*" : " "));
    case 1:
        i = reg->CountWMons();
        return (i ? ((AString) i) : (AString(" ")));
    case 2:
        forlist(&reg->objects) {
            Object * o = (Object *) elem;
            if (!ObjectDefs[o->type].canenter) {
                if (o->units.First()) {
                    return "*";
                } else {
                    return ".";
                }
            }
        }
        return " ";
    case 3:
        if (reg->gate) return "*";
        return " ";
    }
    return( " " );
}

void Game::WriteSurfaceMap( Aoutfile *f, ARegionArray *pArr, int type )
{
    ARegion *reg;
    int yy = 0;
    int xx = 0;
    {
        f->PutStr(AString("Map (") + xx*32 + "," + yy*16 + ")");
        for (int y=0; y < pArr->y; y+=2) {
            AString temp;
            int x;
            for (x=0; x< pArr->x; x+=2) {
                reg = pArr->GetRegion(x+xx*32,y+yy*16 );
                temp += AString(GetRChar(reg));
                temp += GetXtraMap(reg,type);
                temp += "  ";
            }
            f->PutStr(temp);
            temp = "  ";
            for (x=1; x< pArr->x; x+=2) {
                reg = pArr->GetRegion(x+xx*32,y+yy*16+1 );
                temp += AString(GetRChar(reg));
                temp += GetXtraMap(reg,type);
                temp += "  ";
            }
            f->PutStr(temp);
        }
        f->PutStr("");
    }
}

void Game::WriteUnderworldMap( Aoutfile *f, ARegionArray *pArr, int type )
{
    ARegion *reg, *reg2;
    int xx = 0;
    int yy = 0;
    {
        f->PutStr(AString("Map (") + xx*32 + "," + yy*16 + ")");
        for (int y=0; y< pArr->y; y+=2) {
            AString temp = " ";
            AString temp2;
            int x;
            for (x=0; x< pArr->x; x+=2) {
                reg = pArr->GetRegion(x+xx*32,y+yy*16 );
                reg2 = pArr->GetRegion(x+xx*32+1,y+yy*16+1 );
                    
                temp += AString(GetRChar(reg));
                temp += GetXtraMap(reg,type);
                    
                if(reg2->neighbors[D_NORTH])
                    temp += "|"; 
                else
                    temp += " ";

                temp += " ";
                    
                if (reg->neighbors[D_SOUTHWEST]) 
                    temp2 += "/"; 
                else
                    temp2 += " ";

                temp2 += " ";
                if (reg->neighbors[D_SOUTHEAST])
                    temp2 += "\\"; 
                else
                    temp2 += " ";

                temp2 += " ";
            }
            f->PutStr(temp);
            f->PutStr(temp2);
                
            temp = " ";
            temp2 = "  ";
            for (x=1; x< pArr->x; x+=2) {
                reg = pArr->GetRegion(x+xx*32,y+yy*16+1 );
                reg2 = pArr->GetRegion(x+xx*32-1,y+yy*16 );
                    
                if (reg2->neighbors[D_SOUTH]) 
                    temp += "|"; 
                else
                    temp += " ";

                temp += AString(" ");
                temp += AString(GetRChar(reg));
                temp += GetXtraMap(reg,type);
                    
                if (reg->neighbors[D_SOUTHWEST]) 
                    temp2 += "/"; 
                else
                    temp2 += " ";

                temp2 += " ";
                if (reg->neighbors[D_SOUTHEAST]) 
                    temp2 += "\\"; 
                else
                    temp2 += " ";

                temp2 += " ";
            }
            f->PutStr(temp);
            f->PutStr(temp2);
        }
        f->PutStr("");
    }
}

int Game::ViewMap(const AString & typestr,const AString & mapfile)
{
    int type = 0;
    if (AString(typestr) == "wmon") type = 1;
    if (AString(typestr) == "lair") type = 2;
    if (AString(typestr) == "gate") type = 3;
    
    Aoutfile f;
    if( f.OpenByName(mapfile) == -1 )
    {
        return( 0 );
    }
    
    switch (type) {
    case 0:
        f.PutStr("Geographical Map");
        break;
    case 1:
        f.PutStr("Wandering Monster Map");
        break;
    case 2:
        f.PutStr("Lair Map");
        break;
    case 3:
        f.PutStr("Gate Map");
        break;
    }

    int i;
    for( i = 0; i < regions.numLevels; i++ )
    {
        f.PutStr( "" );
        ARegionArray *pArr = regions.pRegionArrays[ i ];
        switch( pArr->levelType )
        {
        case ARegionArray::LEVEL_NEXUS:
            f.PutStr( AString( "Level " ) + i + ": Nexus" );
            break;
        case ARegionArray::LEVEL_SURFACE:
            f.PutStr( AString( "Level " ) + i + ": Surface" );
            WriteSurfaceMap( &f, pArr, type );
            break;
        case ARegionArray::LEVEL_UNDERWORLD:
            f.PutStr( AString( "Level " ) + i + ": Underworld" );
            WriteUnderworldMap( &f, pArr, type );
            break;
        }
    }
    
    f.Close();

    return( 1 );
}

int Game::NewGame()
{
    factionseq = 1;
    unitseq = 1;
    SetupUnitNums();
    shipseq = 100;
    year = 1;
    month = -1;
    gameStatus = GAME_STATUS_NEW;

    //
    // Seed the random number generator with a different value each time.
    //
    seedrandomrandom();

    CreateWorld();
    CreateNPCFactions();

    CreateCityMons();
    CreateWMons();
    CreateLMons();

    return( 1 );
}

int Game::OpenGame()
{
    //
    // The order here must match the order in SaveGame
    //
    Ainfile f;
    if( f.OpenByName( "game.in" ) == -1 )
    {
        return( 0 );
    }
		
    //
    // Read in Globals
    //
    AString *s1 = f.GetStr();
    if( !s1 )
    {
        return( 0 );
    }
    AString *s2 = s1->gettoken();
    delete s1;
    if( !s2 )
    {
        return( 0 );
    }
    if (! (*s2 == "atlantis_game"))
    {
        delete s2;
        f.Close();
        return( 0 );
    }
    delete s2;
  
    ATL_VER eVersion = f.GetInt();
    Awrite( AString( "Saved Game Engine Version: " ) + 
            ATL_VER_STRING( eVersion ) );
    if( ATL_VER_MAJOR( eVersion ) != ATL_VER_MAJOR( CURRENT_ATL_VER ) ||
        ATL_VER_MINOR( eVersion ) != ATL_VER_MINOR( CURRENT_ATL_VER ) )
    {
        Awrite( "Incompatible Engine versions!" );
        return( 0 );
    }
    if( ATL_VER_PATCH( eVersion ) > ATL_VER_PATCH( CURRENT_ATL_VER ))
    {
        Awrite( "This game was created with a more recent Atlantis Engine!" );
        return( 0 );
    }

    AString *gameName = f.GetStr();
    if( !gameName )
    {
        return( 0 );
    }
    if( !( *gameName == Globals->RULESET_NAME ))
    {
        Awrite( "Incompatible rule-set!" );
        return( 0 );
    }

    ATL_VER gVersion = f.GetInt();
    Awrite( AString( "Saved Rule-Set Version: " ) +
            ATL_VER_STRING( gVersion ));
    if( ATL_VER_MAJOR( gVersion ) != 
        ATL_VER_MAJOR( Globals->RULESET_VERSION ) ||
        ATL_VER_MINOR( gVersion ) != 
        ATL_VER_MINOR( Globals->RULESET_VERSION ))
    {
        Awrite( "Incompatible Game versions!" );
        return( 0 );
    }
    if( ATL_VER_PATCH( gVersion ) > ATL_VER_PATCH( Globals->RULESET_VERSION ))
    {
        Awrite( "This game was created with a more recent version of the "
                "rule-set!" );
        return( 0 );
    }

    year = f.GetInt();
    month = f.GetInt();
    seedrandom(f.GetInt());
    factionseq = f.GetInt();
    unitseq = f.GetInt();
    shipseq = f.GetInt();
    guardfaction = f.GetInt();
    monfaction = f.GetInt();

    //
    // Read in the Factions
    //
    int i = f.GetInt();
  
    for (int j=0; j<i; j++)
    {
        Faction * temp = new Faction;
        temp->Readin( &f, eVersion );
        factions.Add(temp);
    }
  
    //
    // Read in the ARegions
    //
    regions.ReadRegions( &f, &factions, eVersion );
    
    SetupUnitNums();
    
    f.Close();
//correction
    forlist (&regions)
    {
      ARegion *r = (ARegion *) elem;
      if (r->IsStartingCity())
      {
        forlist (&r->markets)
        {
          Market *m = (Market *) elem;
	  if (m->amount == -1) {m->amount = 2 + getrandom (50); m->maxamt = m->amount;};
          if (m->minamt == -1) m->minamt = 1;
          if (m->maxamt == -1) m->maxamt = 2 + getrandom (50);
//          Awrite (".");
        }
      }
    }
    return( 1 );
}

int Game::SaveGame()
{
    Aoutfile f;
    if( f.OpenByName( "game.out" ) == -1 )
    {
        return( 0 );
    }
	
    //
    // Write out Globals
    //
    f.PutStr( "atlantis_game" );
    f.PutInt( CURRENT_ATL_VER );
    f.PutStr( Globals->RULESET_NAME );
    f.PutInt( Globals->RULESET_VERSION );
	
    f.PutInt(year);
    f.PutInt(month);
    f.PutInt(getrandom(10000));
    f.PutInt(factionseq);
    f.PutInt(unitseq);
    f.PutInt(shipseq);
    f.PutInt(guardfaction);
    f.PutInt(monfaction);

    //
    // Write out the Factions
    //
    f.PutInt(factions.Num());
	
    forlist(&factions) {
        ((Faction *) elem)->Writeout( &f );
    }

    //
    // Write out the ARegions
    //
    regions.WriteRegions( &f );
  
    f.Close();
    return( 1 );
}

void Game::DummyGame()
{
    //
    // No need to set anything up; we're just syntax checking some orders.
    //
}

#define PLAYERS_FIRST_LINE "AtlantisPlayerStatus"

int Game::WritePlayers()
{
    Aoutfile f;
    if( f.OpenByName( "players.out" ) == -1 )
    {
        return( 0 );
    }

    f.PutStr( PLAYERS_FIRST_LINE );
    f.PutStr( AString( "Version: " ) + CURRENT_ATL_VER );
    f.PutStr( AString( "TurnNumber: " ) + TurnNumber() );

    if( gameStatus == GAME_STATUS_UNINIT )
    {
        return( 0 );
    }
    else if( gameStatus == GAME_STATUS_NEW )
    {
        f.PutStr( AString( "GameStatus: New" ));
    }
    else if( gameStatus == GAME_STATUS_RUNNING )
    {
        f.PutStr( AString( "GameStatus: Running" ));
    }
    else if( gameStatus == GAME_STATUS_FINISHED )
    {
        f.PutStr( AString( "GameStatus: Finished" ));
    }

    f.PutStr( "" );

    forlist( &factions ) {
        Faction *fac = (Faction *) elem;
        fac->WriteFacInfo( &f );
    }

    f.Close();
    return( 1 );
}

int Game::ReadPlayers()
{
    Aorders f;
    if( f.OpenByName( "players.in" ) == -1 )
    {
        return( 0 );
    }

    AString *pLine = 0;
    AString *pToken = 0;

    //
    // Default: failure.
    //
    int rc = 0;

    do
    {
        //
        // The first line of the file should match.
        //
        pLine = f.GetLine();
        if( !( *pLine == PLAYERS_FIRST_LINE ))
        {
            break;
        }
        SAFE_DELETE( pLine );

        //
        // Get the file version number.
        //
        pLine = f.GetLine();
        pToken = pLine->gettoken();
        if( !pToken || !( *pToken == "Version:" ))
        {
            break;
        }
        SAFE_DELETE( pToken );

        pToken = pLine->gettoken();
        if( !pToken )
        {
            break;
        }

        int nVer = pToken->value();
        if( ATL_VER_MAJOR( nVer ) != ATL_VER_MAJOR( CURRENT_ATL_VER ) ||
            ATL_VER_MINOR( nVer ) != ATL_VER_MINOR( CURRENT_ATL_VER ) ||
            ATL_VER_PATCH( nVer ) > ATL_VER_PATCH( CURRENT_ATL_VER ) )
        {
            Awrite( "The players.in file is not compatible with this "
                    "version of Atlantis." );
            break;
        }
        SAFE_DELETE( pToken );
        SAFE_DELETE( pLine );

        //
        // Ignore the turn number line.
        //
        pLine = f.GetLine();
        SAFE_DELETE( pLine );

        //
        // Next, the game status.
        //
        pLine = f.GetLine();
        pToken = pLine->gettoken();
        if( !pToken || !( *pToken == "GameStatus:" ))
        {
            break;
        }
        SAFE_DELETE( pToken );

        pToken = pLine->gettoken();
        if( !pToken )
        {
            break;
        }
        if( *pToken == "New" )
        {
            gameStatus = GAME_STATUS_NEW;
        }
        else if( *pToken == "Running" )
        {
            gameStatus = GAME_STATUS_RUNNING;
        }
        else if( *pToken == "Finished" )
        {
            gameStatus = GAME_STATUS_FINISHED;
        }
        else
        {
            //
            // The status doesn't seem to be valid.
            //
            break;
        }

        //
        // Now, we should have a list of factions.
        //
        pLine = f.GetLine();
        Faction *pFac = 0;

        int lastWasNew = 0;

        //
        // OK, set our return code to success; we'll set it to fail below
        // if necessary.
        //
        rc = 1;

        while( pLine )
        {
            pToken = pLine->gettoken();
            if( !pToken )
            {
                SAFE_DELETE( pLine );
                pLine = f.GetLine();
                continue;
            }

            if( *pToken == "Faction:" )
            {
                //
                // Get the new faction
                //
                SAFE_DELETE( pToken );
                pToken = pLine->gettoken();
                if( !pToken )
                {
                    rc = 0;
                    break;
                }

                if( *pToken == "new" )
                {
                    SAFE_DELETE( pToken );
                    pToken = pLine->gettoken();
                    if ( !pToken ) pFac = AddFaction();
                      else pFac = AddFaction(pToken->value());

                    if( !pFac )
                    {
                        Awrite( "Failed to add a new faction!" );
                        rc = 0;
                        break;
                    }

                    lastWasNew = 1;
                }
                else
                {
                    if( pFac && lastWasNew )
                    {
                        WriteNewFac( pFac );
                    }
                    int nFacNum = pToken->value();
                    pFac = GetFaction( &factions, nFacNum );
                    lastWasNew = 0;
                }
            }
            else if( pFac )
            {
                if( !ReadPlayersLine( pToken, pLine, pFac, lastWasNew ))
                {
                    rc = 0;
                    break;
                }
            }

            SAFE_DELETE( pToken );
            SAFE_DELETE( pLine );
            pLine = f.GetLine();
        }

        if( pFac && lastWasNew )
        {
            WriteNewFac( pFac );
        }
    }
    while( 0 );

    delete pLine;
    delete pToken;
    f.Close();

    return( rc );
}

int Game::ReadPlayersLine( AString *pToken, AString *pLine, Faction *pFac,
                           int newPlayer )
{
    AString *pTemp = 0;

    if( *pToken == "Name:" )
    {
        pTemp = pLine->StripWhite();
        if( pTemp )
        {
            if( newPlayer )
            {
                *pTemp += AString(" (") + (pFac->num) + ")";
            }
            pFac->SetNameNoChange( pTemp );
        }
    }
    else if( *pToken == "RewardTimes" )
    {
        pFac->TimesReward();
    }
    else if( *pToken == "Email:" )
    {
        pTemp = pLine->gettoken();
        if( pTemp )
        {
            delete pFac->address;
            pFac->address = pTemp;
            pTemp = 0;
        }
    }
    else if( *pToken == "Password:" )
    {
        pTemp = pLine->gettoken();
        delete pFac->password;
        if( pTemp )
        {
            pFac->password = pTemp;
            pTemp = 0;
        }
        else
        {
            pFac->password = 0;
        }
    }
    else if( *pToken == "Reward:" )
    {
        pTemp = pLine->gettoken();
        int nAmt = pTemp->value();
        pFac->Event( AString( "Reward of " ) + nAmt + " silver." );
        pFac->unclaimed += nAmt;
    }
    else if( *pToken == "Fine:" )
    {
        pTemp = pLine->gettoken();
        int nAmt = pTemp->value();
        pFac->Event( AString( "Fine of " ) + nAmt + " silver." );
        pFac->unclaimed -= nAmt;
        if (pFac->unclaimed < 0 ) pFac->unclaimed = 0;
    }
    else if( *pToken == "Order:" )
    {
        pTemp = pLine->gettoken();
        if( *pTemp == "quit" )
        {
            pFac->quit = QUIT_BY_GM;
        }
    }
    else if( *pToken == "SendTimes:" )
    {
        // get the token, but otherwise ignore it
        pTemp = pLine->gettoken();
        pFac->times = pTemp->value();
    }
    else
    {
        pTemp = new AString( *pToken + *pLine );
        pFac->extraPlayers.Add( pTemp );
        pTemp = 0;
    }

    delete pTemp;
    return( 1 );
}

void Game::WriteNewFac( Faction *pFac )
{
    AString *strFac = new AString( AString( "Adding " ) + 
                                   *( pFac->address ) + "." );
    newfactions.Add( strFac );
}

void Game::Do1Move(AString * str)
{
    AString * token = str->gettoken();
    if (!token) {
        delete str;
        return;
    }
    int unitno = token->value();
    delete token;
    
    token = str->gettoken();
    if (!token) {
        delete str;
        return;
    }
    int x = token->value();
    delete token;
    
    token = str->gettoken();
    if (!token) {
        delete str;
        return;
    }
    int y = token->value();
    delete token;
    
    token = str->gettoken();
    if (!token) {
        delete str;
        return;
    }
    int z = token->value();
    delete token;
    delete str;
    
    Location * loc = regions.FindUnit(unitno);
    if (!loc) return;
    
    ARegion * newreg = regions.GetRegion(x,y,z);
    if (!newreg) return;
    
    loc->unit->MoveUnit( newreg->GetDummy() );
    delete loc;
}

void Game::Do1Set(AString * str) {
  AString * token = str->gettoken();
  if (!token) {
    delete str;
    return;
  }

  if (*token == "skill") {
    delete token;
    token = str->gettoken();
    if (!token) {
      delete str;
      return;
    }
    int unit = token->value();
    delete token;

    token = str->gettoken();
    if (!token) {
      delete str;
      return;
    }
    int skill = ParseSkill(token);
    delete token;

    token = str->gettoken();
    if (!token) {
      delete str;
      return;
    }
    int value = token->value();
    delete token;
    delete str;

    Location * loc = regions.FindUnit(unit);
    if (!loc) return;

    loc->unit->SetSkillDays(skill,value);
    return;
  }

  if (*token == "item") {
    delete token;
    token = str->gettoken();
    if (!token) {
      delete str;
      return;
    }
    int unit = token->value();
    delete token;

    token = str->gettoken();
    if (!token) {
      delete str;
      return;
    }
    int item = ParseItem(token);
    delete token;

    token = str->gettoken();
    if (!token) {
      delete str;
      return;
    }
    int value = token->value();
    delete token;
    delete str;

    Location * loc = regions.FindUnit(unit);
    if (!loc) return;

    loc->unit->items.SetNum(item,value);
    return;
  }

  delete token;
  delete str;
}

int Game::DoOrdersCheck( const AString &strOrders, const AString &strCheck )
{
    Aorders ordersFile;
    if( ordersFile.OpenByName( strOrders ) == -1 )
    {
        Awrite( "No such orders file!" );
        return( 0 );
    }

    Aoutfile checkFile;
    if( checkFile.OpenByName( strCheck ) == -1 )
    {
        Awrite( "Couldn't open the orders check file!" );
        return( 0 );
    }

    OrdersCheck check;
    check.pCheckFile = &checkFile;

    ParseOrders( 0, &ordersFile, &check );

    ordersFile.Close();
    checkFile.Close();

    return( 1 );
}

int Game::RunGame()
{
    Awrite("Setting Up Turn...");
    PreProcessTurn();
  
    Awrite("Reading the Gamemaster File...");
    if( !ReadPlayers() )
    {
        return( 0 );
    }
	
    if( gameStatus == GAME_STATUS_FINISHED )
    {
        Awrite( "This game is finished!" );
        return( 0 );
    }
    gameStatus = GAME_STATUS_RUNNING;

    Awrite("Reading the Orders File...");
    ReadOrders();
  
    Awrite("QUITting Inactive Factions!");
    RemoveInactiveFactions();

    Awrite("Running the Turn...");
    RunOrders();
  
// dsnlab
    Awrite("Olding...");
    Olding ();
    Awrite("Writing the Report File...");
    WriteReport();
    battles.DeleteAll();
  
    Awrite("Writing Playerinfo File...");
    WritePlayers();

    Awrite("Removing Dead Factions...");
    DeleteDeadFactions();
    Awrite("done");

    return( 1 );
}

int Game::EditGame( int *pSaveGame )
{
    *pSaveGame = 0;

    Awrite( "Editing an Atlantis Game: ");
    do
    {
        int exit = 0;

        Awrite( "Main Menu" );
        Awrite( "  1) Find a region..." );
        Awrite( "  2) Find a unit..." );
        Awrite( "  q) Quit without saving." );
        Awrite( "  x) Exit and save." );
        Awrite( "> " );

        AString *pStr = AGetString();
        Awrite( "" );

        if( *pStr == "q" )
        {
            exit = 1;
            Awrite( "Quiting without saving." );
        }
        else if( *pStr == "x" )
        {
            exit = 1;
            *pSaveGame = 1;
            Awrite( "Exit and save." );
        }
        else if( *pStr == "1" )
        {
            ARegion *pReg = EditGameFindRegion();
            if( pReg )
            {
                EditGameRegion( pReg );
            }
        }
        else if( *pStr == "2" )
        {
            EditGameFindUnit();
        }
        else
        {
            Awrite( "Select from the menu." );
        }

        delete pStr;
        if( exit )
        {
            break;
        }
    }
    while( 1 );

    return( 1 );
}

ARegion *Game::EditGameFindRegion()
{
    ARegion *ret = 0;
    int x, y, z;
    AString *pStr = 0, *pToken = 0;
    Awrite( "Region coords (x y z):" );
    do
    {
        pStr = AGetString();

        pToken = pStr->gettoken();
        if( !pToken )
        {
            Awrite( "No such region." );
            break;
        }
        x = pToken->value();
        SAFE_DELETE( pToken );

        pToken = pStr->gettoken();
        if( !pToken )
        {
            Awrite( "No such region." );
            break;
        }
        y = pToken->value();
        SAFE_DELETE( pToken );

        pToken = pStr->gettoken();
        if( pToken )
        {
            z = pToken->value();
            SAFE_DELETE( pToken );
        }
        else
        {
            z = 0;
        }
        
        ARegion *pReg = regions.GetRegion( x, y, z );
        if( !pReg )
        {
            Awrite( "No such region." );
            break;
        }

        ret = pReg;
    }
    while( 0 );

    delete pStr;
    delete pToken;

    return( ret );
}

void Game::EditGameFindUnit()
{
    AString *pStr;
    Awrite( "Which unit number?" );
    pStr = AGetString();
    int num = pStr->value();
    delete pStr;
    Unit *pUnit = GetUnit( num );
    if( !pUnit )
    {
        Awrite( "No such unit!" );
        return;
    }
    EditGameUnit( pUnit );
}

void Game::EditGameRegion( ARegion *pReg )
{
    // xxxxx
    Awrite( "Not implemented yet." );
}

void Game::EditGameUnit( Unit *pUnit )
{
    do
    {
        Awrite( AString( "Unit: " ) + *( pUnit->name ));
        Awrite( AString( "  in " ) + 
                pUnit->object->region->ShortPrint( &regions ));
        Awrite( "  1) Edit items..." );
        Awrite( "  2) Edit skills..." );
        Awrite( "  3) Move unit..." );
        Awrite( "  q) Stop editing this unit." );

        int exit = 0;
        AString *pStr = AGetString();
        if( *pStr == "1" )
        {
            EditGameUnitItems( pUnit );
        }
        else if( *pStr == "2" )
        {
            EditGameUnitSkills( pUnit );
        }
        else if( *pStr == "3" )
        {
            EditGameUnitMove( pUnit );
        }
        else if( *pStr == "q" )
        {
            exit = 1;
        }
        else
        {
            Awrite( "Select from the menu." );
        }
        delete pStr;

        if( exit )
        {
            break;
        }
    }
    while( 1 );
}

void Game::EditGameUnitItems( Unit *pUnit )
{
    do
    {
        int exit = 0;
        Awrite( AString( "Unit items: " ) + pUnit->items.Report( 2, 1, 1 ));
        Awrite( "  [item] [number] to update an item." );
        Awrite( "  q) Stop editing the items." );
        AString *pStr = AGetString();
        if( *pStr == "q" )
        {
            exit = 1;
        }
        else
        {
            AString *pToken = 0;
            do
            {
                pToken = pStr->gettoken();
                if( !pToken )
                {
                    Awrite( "Try again." );
                    break;
                }

                int itemNum = ParseItem( pToken );
                if( itemNum == -1 )
                {
                    Awrite( "No such item." );
                    break;
                }
                SAFE_DELETE( pToken );

                int num;
                pToken = pStr->gettoken();
                if( !pToken )
                {
                    num = 0;
                }
                else
                {
                    num = pToken->value();
                }

                pUnit->items.SetNum( itemNum, num );
            }
            while( 0 );
            delete pToken;
        }
        delete pStr;

        if( exit )
        {
            break;
        }
    }
    while( 1 );
}

void Game::EditGameUnitSkills( Unit *pUnit )
{
    do
    {
        int exit = 0;
        Awrite( AString( "Unit skills: " ) + 
                pUnit->skills.Report( pUnit->GetMen()));
        Awrite( "  [skill] [days] to update a skill." );
        Awrite( "  q) Stop editing the skills." );
        AString *pStr = AGetString();
        if( *pStr == "q" )
        {
            exit = 1;
        }
        else
        {
            AString *pToken = 0;
            do
            {
                pToken = pStr->gettoken();
                if( !pToken )
                {
                    Awrite( "Try again." );
                    break;
                }

                int skillNum = ParseSkill( pToken );
                if( skillNum == -1 )
                {
                    Awrite( "No such skill." );
                    break;
                }
                SAFE_DELETE( pToken );

                int days;
                pToken = pStr->gettoken();
                if( !pToken )
                {
                    days = 0;
                }
                else
                {
                    days = pToken->value();
                }

                //
                // xxxxx - what about magic???
                //
                pUnit->skills.SetDays( skillNum, days * pUnit->GetMen() );
            }
            while( 0 );
            delete pToken;
        }
        delete pStr;

        if( exit )
        {
            break;
        }
    }
    while( 1 );
}

void Game::EditGameUnitMove( Unit *pUnit )
{
    ARegion *pReg = EditGameFindRegion();
    if( !pReg )
    {
        return;
    }

    pUnit->MoveUnit( pReg->GetDummy() );
}

void Game::PreProcessTurn()
{
    month++;
    if (month>11)
    {
        month = 0;
        year++;
    }
    SetupUnitNums();
    {
        forlist(&factions) {
            ((Faction *) elem)->DefaultOrders();
            ((Faction *) elem)->TimesReward();
        }
    }
    {
        forlist(&regions) {
            ARegion *pReg = (ARegion *) elem;
            if( Globals->WEATHER_EXISTS )
            {
                pReg->SetWeather( regions.GetWeather( pReg, month ));
            }
            pReg->DefaultOrders();
        }
    }
}

void Game::ClearOrders(Faction * f)
{
    forlist(&regions) {
        ARegion * r = (ARegion *) elem;
        forlist(&r->objects) {
            Object * o = (Object *) elem;
            forlist(&o->units) {
                Unit * u = (Unit *) elem;
                if (u->faction == f)
                {
                    u->ClearOrders();
                }
            }
        }
    }
}

void Game::ReadOrders()
{
    forlist( &factions ) {
        Faction *fac = (Faction *) elem;
        if( !fac->IsNPC() )
        {
            AString str = "orders.";
            str += fac->num;

            Aorders file;
            if( file.OpenByName( str ) != -1 )
            {
                ParseOrders( fac->num, &file, 0 );
                file.Close();
            }
        }
    }
}

void Game::MakeFactionReportLists()
{
    FactionVector vector(factionseq);
    
    forlist(&regions) {
        vector.ClearVector();
        
        ARegion *reg = (ARegion *) elem;
        
        {
            forlist(&reg->farsees) {
                Faction *fac = ((Farsight *) elem)->faction;
                vector.SetFaction(fac->num, fac);
            }
        }
        
        {
            forlist(&reg->objects) {
                Object *obj = (Object *) elem;
                
                forlist(&obj->units) {
                    Unit *unit = (Unit *) elem;
                    vector.SetFaction(unit->faction->num, unit->faction);
                }
            }
        }
        
        for (int i=0; i<vector.vectorsize; i++)
        {
            if (vector.GetFaction(i))
            {
                ARegionPtr *ptr = new ARegionPtr;
                ptr->ptr = reg;
                vector.GetFaction(i)->present_regions.Add(ptr);
            }
        }
    }
}

void Game::CountItems (int ** citems)
{
  int i = 0;
  forlist (&factions)
  {
    Faction * fac = (Faction *) elem;
    if (!fac->IsNPC())
    {
      for (int j = 0; j < NITEMS; j++)
      {
        citems[i][j] = CountItem (fac, j);
      }
      i++;
    }
  }
}
int Game::CountItem (Faction * fac, int item)
{
  int all = 0;
      forlist (&(fac->present_regions))
      {
        ARegionPtr * r = (ARegionPtr *) elem;
        forlist (&r->ptr->objects)
        {
          Object * obj = (Object *) elem;
          forlist (&obj->units)
          {
            Unit * unit = (Unit *) elem;
            if (unit->faction == fac)
              all += unit->items.GetNum (item);
          }
        }
      }
   return all;
}
/*
int Game::CountItem (Faction * fac, int item)
{
  int all = 0;
  for (int i = 1; i < unitseq; i++)
  {
    Unit *u = GetUnit(i);
    if (u)
    {
      if (u->faction == fac)
      {
        all += u->items.GetNum (item);
      }
    }
  }
  return all;
}
*/
void Game::WriteReport()
{
    Areport f;
    
    MakeFactionReportLists();
    CountAllMages();
    int ** citems = new int* [factionseq];
    for (int i = 0; i < factionseq; i++)
    {
      citems [i] = new int [NITEMS];
      for (int j = 0; j < NITEMS; j++)
      {
        citems [i][j] = 0;
      }
    }
    CountItems(citems);

    forlist(&factions) {
        Faction * fac = (Faction *) elem;
        AString str = "report.";
        str = str + fac->num;

        f.OpenByName( str );
        fac->WriteReport( &f, this, citems );
        f.Close();

        Adot();
    }
    for (i = 0; i < factionseq; i++)
    {
      delete citems [i];
    }
    delete citems;
}

void Game::DeleteDeadFactions()
{
    forlist(&factions) {
        Faction * fac = (Faction *) elem;
        if (!fac->IsNPC() && !fac->exists)
        {
            factions.Remove(fac);
            forlist((&factions))
                ((Faction *) elem)->RemoveAttitude(fac->num);
            delete fac;
        }
    }
}

Faction *Game::AddFaction(int nn)
{
    //
    // set up faction
    //
//    Faction *temp = new Faction( factionseq );
    int fl = 1;
    int newnum = getrandom(10000);
    if (nn != -1) newnum = nn;
    while (fl)
    {
       if (GetFaction (&factions,newnum))
         newnum = getrandom(10000);
         else fl=0;
    }
    Faction *temp = new Faction( newnum );

    AString x( "NoAddress" );
    temp->SetAddress( x );
    temp->lastorders = TurnNumber();

    if( SetupFaction( temp ))
    {
        factions.Add(temp);
        factionseq++;

        return( temp );
    }
    else
    {
        delete temp;
        return( 0 );
    }
}

void Game::ViewFactions()
{
    forlist((&factions))
        ((Faction *) elem)->View();
}

void Game::SetupUnitNums()
{
    delete ppUnits;
    maxppunits = unitseq + 10000;
    ppUnits = new Unit *[ maxppunits ];
    
    int i;
    for( i = 0; i < maxppunits; i++ )
    {
        ppUnits[ i ] = 0;
    }

    forlist(&regions) {
        ARegion * r = (ARegion *) elem;
        forlist(&r->objects) {
            Object * o = (Object *) elem;
            forlist(&o->units) {
                Unit *u = (Unit *) elem;
                ppUnits[ u->num ] = u;
            }
        }
    }
}

Unit *Game::GetNewUnit( Faction *fac, int an )
{
    int i;
    for( i = 1; i < unitseq; i++ )
    {
        if( !ppUnits[ i ] )
        {
            Unit *pUnit = new Unit( i, fac, an );
            ppUnits[ i ] = pUnit;
            return( pUnit );
        }
    }

    Unit *pUnit = new Unit( unitseq, fac, an );
    ppUnits[ unitseq ] = pUnit;
    unitseq++;

	if(unitseq >= maxppunits) {
		Unit **temp = new Unit*[maxppunits+10000];
		memcpy(temp, ppUnits, maxppunits);
		maxppunits += 10000;
		delete ppUnits;
		ppUnits = temp;
	}

    return( pUnit );
}

Unit *Game::GetUnit( int num )
{
    if ((num <= 0) || (num >= maxppunits)) return NULL;
    return( ppUnits[ num ] );
}

void Game::CountAllMages()
{
    forlist(&factions) {
        ((Faction *) elem)->nummages = 0;
    }

    {
        forlist(&regions) {
            ARegion * r = (ARegion *) elem;
            forlist(&r->objects) {
                Object * o = (Object *) elem;
                forlist(&o->units) {
                    Unit * u = (Unit *) elem;
                    if (u->type == U_MAGE)
                    {
                      u->faction->nummages++;
                    }
                }
            }
        }
    }
}

void Game::GameStatistic()
{
  Aoutfile f;
  Awrite ("Opening stat.txt");
  if (f.OpenByName ("stat.txt") == -1)
  {
    Awrite("Couldn't open file!");
  }else
  {
    Awrite ("Collecting statistics");
    int maxfacnum = 0;
    {
      forlist ((&factions))
      {
         Faction *fac = (Faction *) elem;
         if (maxfacnum < fac->num) maxfacnum = fac->num;
      }
    }
    int ** stat = new int* [maxfacnum];
    int i = 0;
    {
      forlist ((&factions))
      {
        Faction *fac = (Faction *) elem;
        stat[fac->num] = new int [NITEMS];
      }
    }
    for (i = 1; i < unitseq; i++)
    {
      Unit *u = GetUnit(i);
      if (u)
      {
        forlist (&u->items)
        {
          Item *i = (Item *) elem;
          stat[u->faction->num][i->type] += i->num;
        }
      }
    }
    Awrite ("Writing statistics");
    int maxitems [NITEMS];
    int allitems [NITEMS];
    for (i = 0; i < NITEMS; i++)
      maxitems [i] = allitems [i] = 0;
    {
      forlist ((&factions))
      {
        Faction *fac = (Faction *) elem;
        AString str = AString (fac->num);
        for (int j = 0; j < NITEMS; j++)
        {
          if (stat[fac->num][j])
          {
          str += AString (" ") + ItemString (j, stat[fac->num][j]);
          if (!fac->IsNPC())
          {
            allitems [j] += stat[fac->num][j];
            if (maxitems[j] < stat[fac->num][j])
              maxitems[j] = stat[fac->num][j];
          }
          }
        }
        f.PutStr (str);
      }
      f.PutStr ("Maximum and All");
      for (i = 0; i < NITEMS; i++)
      {
        AString str = ItemString (i, maxitems[i]) + " from " + AString (allitems[i]) + " ";
        if (ItemDefs[i].type & IT_MONSTER && ItemDefs[i].index == MONSTER_ILLUSION)
        {
          str += AString (" (illusion)");
        }
        f.PutStr (str);
      }
    }
    f.Close ();
  }
}

void Game::UnitFactionMap()
{
    Aoutfile f;
    int i;
    Unit *u;

    Awrite("Opening units.txt");
    if (f.OpenByName("units.txt") == -1)
    {
        Awrite("Couldn't open file!");
    } else {
        Awrite(AString("Writing ") + unitseq + " units");
        for (i = 1; i < unitseq; i++)
        {
            u = GetUnit(i);
            if (!u)
            {
              Awrite("doesn't exist");
            } else {
              Awrite(AString(i) + ":" + u->faction->num);
              f.PutStr(AString(i) + ":" + u->faction->num);
            }
        }
    }
    f.Close();
    if (f.OpenByName("units1.txt") == -1)
    {
        Awrite("Couldn't open file!");
    } else {
    forlist (&regions)
    {
      ARegion * r = (ARegion *) elem;
      forlist (&r->objects)
      {
        Object * o = (Object *) elem;
        forlist (&o->units)
        {
          Unit * u = (Unit *) elem;
          f.PutStr(AString(u->num) + ":" + u->faction->num + ":" + r->xloc + " " + r->yloc + " " + r->zloc);
        }
      }
    }
    f.Close();
	}
}

void Game::RemoveInactiveFactions()
{
int cturn;
 
cturn = TurnNumber();
 
 forlist(&factions)
 {
  Faction * fac = (Faction *) elem;
  if ((cturn - fac->lastorders) >= Globals->MAX_INACTIVE_TURNS && !fac->IsNPC())
  {
   fac->quit = QUIT_BY_GM;
  }
 }
}

void Game::Olding ()
{
  forlist(&regions) {
    ARegion * r = (ARegion *) elem;
    forlist(&r->objects) {
      Object * o = (Object *) elem;
      forlist(&o->units) {
        Unit *u = (Unit *) elem;
          u->age++;
      }
    }
  }
}