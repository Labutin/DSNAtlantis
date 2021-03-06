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
#include "gamedefs.h"
#include "game.h"
#include "items.h"
#include "skills.h"
#include "rules.h"

void usage()
{
    Awrite("atlantis new <gamefile> <playerfile>");
    Awrite("");
    Awrite("atlantis map <gamefile> <type> <mapfile>");
    Awrite("");
    Awrite("atlantis run <ingame> <admin> <orders> <reports> <outgame> "
           "<playerfile> <times header>");
}

void CheckOneAbbr(char * abr) {
  int i;
  int c = 0;
  AString temp;

  for (i=0; i<NITEMS; i++) {
    if (AString(abr) == AString(ItemDefs[i].abr)) {
      c ++;
      temp += AString(abr) + " : item  ";
    }
  }

  for (i=0; i<NSKILLS; i++) {
    if (AString(abr) == AString(SkillDefs[i].abbr)) {
      c ++;
      temp += AString(abr) + " : skill  ";
    }
  }

  if (c>1) {
    Awrite(temp);
  }
}

void CheckAbbrs()
{
    Awrite("Checking Abbreviations:");
    
    int i;
    for (i=0; i<NITEMS; i++) {
        CheckOneAbbr(ItemDefs[i].abr);
    }
    
    for (i=0; i<NSKILLS; i++) {
        CheckOneAbbr(SkillDefs[i].abbr);
    }
}

/*
int main(int argc, char *argv[])
{
    Game game;

    initIO();

    Awrite( AString( "Atlantis Engine Version: " ) + 
            ATL_VER_STRING( CURRENT_ATL_VER ));
    Awrite( AString( Globals->RULESET_NAME ) + ", Version: " +
            ATL_VER_STRING( Globals->RULESET_VERSION ));
    Awrite( "" );


#ifdef NEVER
    CheckAbbrs();
#endif
    
    if (argc == 1)
    {
        usage();
        
        doneIO();
        return 0;
    }

    do
    {
        if (AString(argv[1]) == "new")
        {
            if( !game.NewGame() )
            {
                Awrite( "Couldn't make the new game!" );
                break;
            }

            if( !game.SaveGame() )
            {
                Awrite( "Couldn't save the game!" );
                break;
            }

            if( !game.WritePlayers() )
            {
                Awrite( "Couldn't write the players file!" );
                break;
            }
        }

        if (AString(argv[1]) == "map")
        {
            if (argc == 4)
            {
                if( !game.OpenGame() )
                {
                    Awrite( "Couldn't open the game file!" );
                    break;
                }

                if( !game.ViewMap( argv[2], argv[3] ))
                {
                    Awrite( "Couldn't write the map file!" );
                    break;
                }
            }
            else
            {
                usage();
                break;
            }
        }

        if (AString(argv[1]) == "run")
        {
            if( !game.OpenGame() )
            {
                Awrite( "Couldn't open the game file!" );
                break;
            }

            if( !game.RunGame() )
            {
                Awrite( "Couldn't run the game!" );
                break;
            }

            if( !game.SaveGame() )
            {
                Awrite( "Couldn't save the game!" );
                break;
            }
        }

        if (AString(argv[1]) == "edit")
        {
            if( !game.OpenGame() )
            {
                Awrite( "Couldn't open the game file!" );
                break;
            }

            int saveGame = 0;
            if( !game.EditGame( &saveGame ) )
            {
                Awrite( "Couldn't edit the game!" );
                break;
            }

            if( saveGame )
            {
                if( !game.SaveGame() )
                {
                    Awrite( "Couldn't save the game!" );
                    break;
                }
            }
        }

        if( AString( argv[1] ) == "check" )
        {
            if( argc == 4 )
            {
                game.DummyGame();
                if( !game.DoOrdersCheck( argv[ 2 ], argv[ 3 ] ))
                {
                    Awrite( "Couldn't check the orders!" );
                    break;
                }
            }
            else
            {
                usage();
                break;
            }
        }
    }
    while( 0 );

    doneIO();
    return 0;
}
*/
int main(int argc, char *argv[])
{
    Game game;

    initIO();

    Awrite( AString( "Atlantis Engine Version: " ) + 
            ATL_VER_STRING( CURRENT_ATL_VER ));
    Awrite( AString( Globals->RULESET_NAME ) + ", Version: " +
            ATL_VER_STRING( Globals->RULESET_VERSION ));
    Awrite( "" );


#ifdef NEVER
    CheckAbbrs();
#endif
    
    if (argc == 1)
    {
        usage();
        
        doneIO();
        return 0;
    }

    do
    {
        if (AString(argv[1]) == "new")
        {
            if( !game.NewGame() )
            {
                Awrite( "Couldn't make the new game!" );
                break;
            }

            if( !game.SaveGame() )
            {
                Awrite( "Couldn't save the game!" );
                break;
            }

            if( !game.WritePlayers() )
            {
                Awrite( "Couldn't write the players file!" );
                break;
            }
        }

        if (AString(argv[1]) == "map")
        {
            if (argc == 4)
            {
                if( !game.OpenGame() )
                {
                    Awrite( "Couldn't open the game file!" );
                    break;
                }

                if( !game.ViewMap( argv[2], argv[3] ))
                {
                    Awrite( "Couldn't write the map file!" );
                    break;
                }
            }
            else
            {
                usage();
                break;
            }
        }

        if (AString(argv[1]) == "run")
        {
            if( !game.OpenGame() )
            {
                Awrite( "Couldn't open the game file!" );
                break;
            }

            if( !game.RunGame() )
            {
                Awrite( "Couldn't run the game!" );
                break;
            }

            if( !game.SaveGame() )
            {
                Awrite( "Couldn't save the game!" );
                break;
            }
        }

        if (AString(argv[1]) == "edit")
        {
            if( !game.OpenGame() )
            {
                Awrite( "Couldn't open the game file!" );
                break;
            }

            int saveGame = 0;
            if( !game.EditGame( &saveGame ) )
            {
                Awrite( "Couldn't edit the game!" );
                break;
            }

            if( saveGame )
            {
                if( !game.SaveGame() )
                {
                    Awrite( "Couldn't save the game!" );
                    break;
                }
            }
        }

        if( AString( argv[1] ) == "check" )
        {
            if( argc == 4 )
            {
                game.DummyGame();
                if( !game.DoOrdersCheck( argv[ 2 ], argv[ 3 ] ))
                {
                    Awrite( "Couldn't check the orders!" );
                    break;
                }
            }
            else
            {
                usage();
                break;
            }
        }
        // LLS
        if( AString( argv[1] ) == "mapunits" )
        {
            if( argc == 2 )
            {
                if( !game.OpenGame() )
                {
                    Awrite( "Couldn't open the game file!" );
                    break;
                }
                game.UnitFactionMap();
            }
            else
            {
                usage();
                break;
            }
        }
// dsnlab
        if( AString( argv[1] ) == "statistic" )
        {
            if( argc == 2 )
            {
                if( !game.OpenGame() )
                {
                    Awrite( "Couldn't open the game file!" );
                    break;
                }
                game.GameStatistic();
            }
            else
            {
                usage();
                break;
            }
        }
    }
    while( 0 );

    doneIO();
    return 0;
}




