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
#ifndef FILE_IO
#define FILE_IO

#include "astring.h"

class ifstream;
class ofstream;

class Ainfile {
public:
  Ainfile();
  ~Ainfile();
	
  void Open(const AString &);
  int OpenByName(const AString &);
  void Close();
  
  AString * GetStr();
  AString * GetStrNoSkip();
  int GetInt();
  
  ifstream * file;
};

class Aoutfile {
public:
  Aoutfile();
  ~Aoutfile();
  
  void Open(const AString &);
  int OpenByName(const AString &);
  void Close();
	
  void PutStr(char *);
  void PutStr(const AString &);
  void PutInt(int);
  
  ofstream * file;
};

class Aorders {
public:
  Aorders();
  ~Aorders();
  
  void Open(const AString &);
  int OpenByName(const AString &);
  void Close();
  
  AString * GetLine();
  
  ifstream * file;
};

class Areport {
public:
  Areport();
  ~Areport();
	
  void Open(const AString &);
  int OpenByName(const AString &);
  void Close();
  
  void AddTab();
  void DropTab();
  void ClearTab();
  
  void PutStr(const AString &,int = 0);
  void PutNoFormat(const AString &);
  void EndLine();
	
  ofstream * file;
  int tabs;
};

#endif
