/***************************************************************************
 *   Copyright (C) 2009 by The qGo Project                                 *
 *                                                                         *
 *   This file is part of qGo.   					   *
 *                                                                         *
 *   qGo is free software: you can redistribute it and/or modify           *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, see <http://www.gnu.org/licenses/>   *
 *   or write to the Free Software Foundation, Inc.,                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


#include "matrix.h"
//#include <stdlib.h>
//#ifndef NO_DEBUG
#include <iostream>
//#endif
#include <algorithm>		//some versions of g++ require this, some do not

Matrix::Matrix(int s)
: size(s)
{
	Q_ASSERT(size > 0 && size <= 36);
	
	matrix = new unsigned short*[size];
	Q_CHECK_PTR(matrix);
	
	for (int i=0; i<size; i++)
	{
		matrix[i] = new unsigned short[size];
		Q_CHECK_PTR(matrix[i]);
		
		for (int j=0; j<size; j++)
			matrix[i][j] = stoneNone;
	}
	
	markTexts = NULL;
}

Matrix::Matrix(const Matrix &m)
{
	size = m.getSize();
	Q_ASSERT(size > 0 && size <= 36);
	
	matrix = new unsigned short*[size];
	Q_CHECK_PTR(matrix);
	
	for (int i=0; i<size; i++)
	{
		matrix[i] = new unsigned short[size];
		Q_CHECK_PTR(matrix[i]);
		
		for (int j=0; j<size; j++)
			matrix[i][j] = m.at(i, j);
	}
	
	markTexts = NULL;
}

Matrix::Matrix(const Matrix &m, bool cleanup)
{
	size = m.getSize();
	Q_ASSERT(size > 0 && size <= 36);
	
	matrix = new unsigned short*[size];
	Q_CHECK_PTR(matrix);
	
	for (int i=0; i<size; i++)
	{
		matrix[i] = new unsigned short[size];
		Q_CHECK_PTR(matrix[i]);
		
		for (int j=0; j<size; j++)
		{
				
			if(cleanup)
				matrix[i][j] = m.at(i, j) & (0x000f | MX_STONEDEAD);	//clearAllMarks
			else
				matrix[i][j] = m.at(i, j) & 0x2fff;			//absMatrix
			matrix[i][j] |= MX_STONEDIRTY;
		}
	}

	markTexts = NULL;
}

Matrix & Matrix::operator=(const Matrix &m)
{
	if(this != &m)
	{
		size = m.getSize();
		init();
		for (int i=0; i<size; i++)
			for (int j=0; j<size; j++)
				matrix[i][j] = m.at(i, j);
	}
	return *this;
}

Matrix::~Matrix()
{
	Q_ASSERT(size > 0 && size <= 36);
	
	for (int i=0; i<size; i++)
		delete [] matrix[i];
	delete [] matrix;
	
	delete markTexts;
}

void Matrix::init()
{
	
}

void Matrix::initMarkTexts()
{
	markTexts = new QStringList();
	Q_CHECK_PTR(markTexts);
}

void Matrix::clear()
{
	Q_ASSERT(size > 0 && size <= 36);
	
	for (int i=0; i<size; i++)
		for (int j=0; j<size; j++)
			matrix[i][j] = stoneNone;
}

#ifndef NO_DEBUG
void Matrix::debug() const
{
	Q_ASSERT(size > 0 && size <= 36);
	
	int i, j;
	
	std::cout << "\n  ";
	for (i=0; i<size; i++)
		std::cout << (i+1)%10 << " ";
	std::cout << std::endl;
	
	for (i=0; i<size; i++)
	{
		std::cout << (i+1)%10 << " ";
		for (j=0; j<size; j++)
		{
#if 0
			switch (abs(matrix[j][i]))
			{
			case stoneNone:
			case stoneErase: std::cout << ". "; break;
			case stoneBlack: std::cout << "B "; break;
			case stoneWhite: std::cout << "W "; break;
			case markSquare*10: std::cout << "[ "; break;
			case markCircle*10: std::cout << "O "; break;
			case markTriangle*10: std::cout << "T "; break;
			case markCross*10: std::cout << "X "; break;
			case markText*10: std::cout << "A "; break;
			case markNumber*10: std::cout << "1 "; break;
			case markSquare*10+stoneBlack: std::cout << "S "; break;
			case markCircle*10+stoneBlack: std::cout << "C "; break;
			case markTriangle*10+stoneBlack: std::cout << "D "; break;
			case markCross*10+stoneBlack: std::cout << "R "; break;
			case markText*10+stoneBlack: std::cout << "A "; break;
			case markNumber*10+stoneBlack: std::cout << "N "; break;
			case markSquare*10+stoneWhite: std::cout << "s "; break;
			case markCircle*10+stoneWhite: std::cout << "c "; break;
			case markTriangle*10+stoneWhite: std::cout << "d "; break;
			case markCross*10+stoneWhite: std::cout << "r "; break;
			case markText*10+stoneWhite: std::cout << "a "; break;
			case markNumber*10+stoneWhite: std::cout << "n "; break;
			default: std::cout << "? ";
			}
#else
			std::cout << matrix[j][i] << " ";
#endif
		}
		std::cout << (i+1)%10 << std::endl;
	}
	
	std::cout << "  ";
	for (i=0; i<size; i++)
		std::cout << (i+1)%10 << " ";
	std::cout << std::endl;
	
	if (markTexts != NULL && !markTexts->isEmpty())
	{
		std::cout << markTexts->count() << " mark texts in the storage.\n";
		
		for (QStringList::Iterator it=markTexts->begin(); it != markTexts->end(); ++it)
			std::cout << it->toLatin1().constData() << std::endl;

	}
}
#endif

void Matrix::insertStone(int x, int y, StoneColor c, bool fEdit)
{
	Q_ASSERT(x > 0 && x <= size &&
		y > 0 && y <= size);
	
	matrix[x-1][y-1] = (matrix[x-1][y-1] & 0xfff0) | c;
	matrix[x-1][y-1] |= MX_STONEDIRTY;
	if(fEdit)
		matrix[x-1][y-1] |= MX_STONEEDIT;
}

unsigned short Matrix::at(int x, int y) const
{	
	//qDebug("assert line %d: %d %d < %d", __LINE__, x, y, size); 
	Q_ASSERT(x >= 0 && x < size &&
		y >= 0 && y < size);
	
	return (matrix[x][y] & ~MX_STONEDIRTY);
}

StoneColor Matrix::getStoneAt(int x, int y)
{	
	Q_ASSERT(x > 0 && x <= size &&
		y > 0 && y <= size);
	
	return  (StoneColor) (matrix[x - 1][y -1] & 0x000f);
}

bool Matrix::isStoneDead(int x, int y)
{	
	//qDebug("xy: %d %d", x, y);
	Q_ASSERT(x > 0 && x <= size &&
			y > 0 && y <= size);
	
	return (matrix[x - 1][y -1] & MX_STONEDEAD);
}

bool Matrix::isStoneDirty(int x, int y)
{	
	return (matrix[x - 1][y -1] & MX_STONEDIRTY);
}

void Matrix::stoneUpdated(int x, int y)
{	
	matrix[x-1][y-1] &= ~MX_STONEDIRTY;
}

void Matrix::invalidateStone(int x, int y)
{
	matrix[x-1][y-1] |= MX_STONEDIRTY;
}

void Matrix::markChangesDirty(Matrix & m)
{
	for (int i=0; i<size; i++)
		for (int j=0; j<size; j++)
		{
			if((matrix[i][j] & 0x00ff) != (m.matrix[i][j] & 0x00ff))
				matrix[i][j] |= MX_STONEDIRTY;
		}
}

MarkType Matrix::getMarkAt(int x, int y)
{	
	Q_ASSERT(x > 0 && x <= size &&
		y > 0 && y <= size);
	
	return  (MarkType) ((matrix[x -1][y-1] >> 4) & 0x000f);
}

void Matrix::set(int x, int y, int n)
{
	Q_ASSERT(x >= 0 && x < size &&
		y >= 0 && y < size);
	
	matrix[x][y] = n;
	matrix[x][y] |= MX_STONEDIRTY;
}

void Matrix::insertMark(int x, int y, MarkType t)
{
	Q_ASSERT(x > 0 && x <= size && y > 0 && y <= size);
	//if (!(x > 0 && x <= size && y > 0 && y <= size))
	//	return;
	matrix[x-1][y-1] |= (t << 4);
	matrix[x-1][y-1] |= MX_STONEDIRTY;
}

void Matrix::removeMark(int x, int y)
{
	Q_ASSERT(x > 0 && x <= size &&
		y > 0 && y <= size);
	
	matrix[x-1][y-1] &= 0xff0f;
	matrix[x-1][y-1] |= MX_STONEDIRTY;

	if (markTexts != NULL && !markTexts->isEmpty())
	{
		QStringList::Iterator it = getMarkTextIterator(x, y);
		if (it != markTexts->end())
			markTexts->erase(it);
	}
}

void Matrix::clearAllMarks()
{
	Q_ASSERT(size > 0 && size <= 36);
	
	for (int i=0; i<size; i++)
		for (int j=0; j<size; j++)
		{
			matrix[i][j] &= (0x000f | MX_STONEDEAD);
			matrix[i][j] |= MX_STONEDIRTY;
		}
	if (markTexts != NULL)
	{
		delete markTexts;
		markTexts = NULL;
	}
}

void Matrix::clearTerritoryMarks()
{
	Q_ASSERT(size > 0 && size <= 36);
	
	int data;
	
	for (int i=0; i<size; i++)
		for (int j=0; j<size; j++)
			if ((data = getMarkAt(i + 1, j + 1)) == markTerrBlack ||
				data == markTerrWhite)
			{
				matrix[i][j] &= (0x000f | MX_STONEDEAD);
				matrix[i][j] |= MX_STONEDIRTY;
			}
}

// Called when leaving score mode
void Matrix::absMatrix()
{
	Q_ASSERT(size > 0 && size <= 36);
	
	for (int i=0; i<size; i++)
	{
		for (int j=0; j<size; j++)
		{
			//matrix[i][j] = abs(matrix[i][j]);
			matrix[i][j] &= 0x2fff;		//remove dead and edit?
			if (getStoneAt(i + 1, j + 1) == stoneErase)
				insertStone(i + 1, j + 1, stoneNone);
			matrix[i][j] |= MX_STONEDIRTY;
		}
	}
}

void Matrix::setMarkText(int x, int y, const QString &txt)
{
	Q_ASSERT(x > 0 && x <= size &&
		y > 0 && y <= size);
	
	// We only create the markTexts list if we really need it.
	if (markTexts == NULL)
		initMarkTexts();
	
	QStringList::Iterator it = getMarkTextIterator(x, y);
	if (it != markTexts->end())  // Mark already exists at this position, remove old text.
		markTexts->erase(it);
	
	QString tmp = QString::number(coordsToKey(x, y)) + "#" + txt;
	markTexts->append(tmp);
}

QStringList::Iterator Matrix::getMarkTextIterator(int x, int y)
{
	if (markTexts == NULL)
		return NULL;
	
	QString s, tmp;
	int pos, tmpX, tmpY, counter=0;
	bool check = false;
	long key;
	QStringList::Iterator it;
	
	for (it=markTexts->begin(); it != markTexts->end(); ++it)
	{
		s = (QString)(*it);
		
		// Get the splitting '#', everything left of it is our key.
		pos = s.indexOf('#');
		if (pos == -1)  // Whoops
		{
			qWarning("   *** Corrupt text marks in matrix! ***");
			continue;
		}
		
		// Transform key to coordinates
		tmp = s.left(pos);
		key = tmp.toLong(&check);
		if (!check)
		{
			qWarning("   *** Corrupt text marks in matrix! ***");
			continue;
		}
		keyToCoords(key, tmpX, tmpY);
		
		// This is our hit?
		if (tmpX == x && tmpY == y)
			return it;
		
		// Nope, wasn't
		counter++;
	}
	return markTexts->end();
}

const QString Matrix::getMarkText(int x, int y)
{
	// We didn't store any texts in this matrix.
	if (markTexts == NULL || markTexts->isEmpty())
		return NULL;
	
	QStringList::Iterator it = getMarkTextIterator(x, y);
	if (it == markTexts->end())  // Nope, this entry does not exist.
		return NULL;
	
	QString s = (QString)(*it);
	s = s.right(s.length() - s.indexOf('#') - 1);
	
	return s;
}

/*
 * returns a string representing the marks in the matrix, in SGF format
 */
const QString Matrix::saveMarks()
{
	Q_ASSERT(size > 0 && size <= 36);
	
	QString txt, sSQ = "", sCR = "", sTR = "", sMA = "", sLB = "", sTB = "", sTW = "";
	int i, j, colw = 0, colb = 0;
	
	for (i=0; i<size; i++)
	{
		for (j=0; j<size; j++)
		{
			switch (getMarkAt(i + 1, j + 1))
			{
			case markSquare:
				if (sSQ.isEmpty())
					sSQ += "SQ";
				sSQ += "[" + coordsToString(i, j) + "]";
				break;
			case markCircle:
				if (sCR.isEmpty())
					sCR += "CR";
				sCR += "[" + coordsToString(i, j) + "]";
				break;
			case markTriangle:
				if (sTR.isEmpty())
					sTR += "TR";
				sTR += "[" + coordsToString(i, j) + "]";
				break;
			case markCross:
				if (sMA.isEmpty())
					sMA += "MA";
				sMA += "[" + coordsToString(i, j) + "]";
				break;
			case markText: 
			case markNumber:
				if (sLB.isEmpty())
					sLB += "LB";
				sLB += "[" + coordsToString(i, j);
				sLB += ":";
				txt = getMarkText(i+1, j+1);
				if (txt.isNull() || txt.isEmpty())
					sLB += "?";  // Whoops
				else
					sLB += txt;
				sLB += "]";
				break;
			case markTerrBlack:
				if (sTB.isEmpty())
				{
					sTB += "\nTB";
					colb = 0;
				}
				sTB += "[" + coordsToString(i, j) + "]";
				if (++colb % 15 == 0)
					sTB += "\n  ";
				break;
			case markTerrWhite:
				if (sTW.isEmpty())
				{
					sTW += "\nTW";
					colw = 0;
				}
				sTW += "[" + coordsToString(i, j) + "]";
				if (++colw % 15 == 0)
					sTW += "\n  ";
				break;
			default: continue;
			}
		}
	}
	
	return sSQ + sCR + sTR + sMA + sLB + sTB + sTW;
}

/*
 * returns a string representing the edited stones in the matrix, in SGF format
 */
const QString Matrix::saveEditedMoves(Matrix *parent)
{
	Q_ASSERT(size > 0 && size <= 36);
	
	QString sAB="", sAW="", sAE="";
	int i, j;
	int z;
	for (i=0; i<size; i++)
	{
		for (j=0; j<size; j++)
		{
			z = getStoneAt(i + 1, j + 1);
			if (z != 0)
				z= 0;
			if(!(matrix[i][j] & MX_STONEEDIT))
				continue;
			switch (getStoneAt(i + 1, j + 1))
			{
			case stoneBlack:
				if (parent != NULL &&
					parent->getStoneAt(i + 1, j + 1) == stoneBlack)
					break;
				if (sAB.isEmpty())
					sAB += "AB";
				sAB += "[" + coordsToString(i, j) + "]";
				break;
				
			case stoneWhite:
				if (parent != NULL &&
					parent->getStoneAt(i + 1, j + 1) == stoneWhite)
					break;
				if (sAW.isEmpty())
					sAW += "AW";
				sAW += "[" + coordsToString(i, j) + "]";
				break;
				
			case stoneErase:
				if (parent != NULL &&
					(parent->getStoneAt(i + 1, j + 1) == stoneNone ||
					parent->getStoneAt(i + 1, j + 1) == stoneErase))
					break;
				if (sAE.isEmpty())
					sAE += "AE";
				sAE += "[" + coordsToString(i, j) + "]";
				break;
			default:
			case stoneNone:
				break;
			}
		}
	}
	
	return sAB + sAW + sAE;
}

const QString Matrix::printMe(ASCII_Import *charset)
{
	Q_ASSERT(size > 0 && size <= 36);
	
#if 0
	qDebug("BLACK STONE CHAR %c\n"
		"WHITE STONE CHAR %c\n"
		"STAR POINT  CHAR %c\n"
		"EMPTY POINT CHAR %c\n",
		charset->blackStone,
		charset->whiteStone,
		charset->starPoint,
		charset->emptyPoint);
#endif
	
	int i, j;
	QString str;
	
	str = "\n    ";
	for (i=0; i<size; i++)
		str += QString(QChar(static_cast<const char>('A' + (i<8?i:i+1)))) + " ";
	str += "\n   +";
	for (i=0; i<size-1; i++)
		str += "--";
	str += "-+\n";
	
	for (i=0; i<size; i++)
	{
		if (size-i < 10) str += " ";
		str += QString::number(size-i) + " |";
		for (j=0; j<size; j++)
		{
			switch (getStoneAt(j + 1, i + 1))
			{
			case stoneBlack: str += QChar(charset->blackStone); str += " "; break;
			case stoneWhite: str += QChar(charset->whiteStone); str += " "; break;
			default:
				// Check for starpoints
				if (size > 9)  // 10x10 or larger
				{
					if ((i == 3 && j == 3) ||
						(i == size-4 && j == 3) ||
						(i == 3 && j == size-4) ||
						(i == size-4 && j == size-4) ||
						(i == (size+1)/2 - 1 && j == 3) ||
						(i == (size+1)/2 - 1 && j == size-4) ||
						(i == 3 && j == (size+1)/2 - 1) ||
						(i == size-4 && j == (size+1)/2 - 1) ||
						(i == (size+1)/2 - 1 && j == (size+1)/2 - 1))
					{
						str += QChar(charset->starPoint);
						str += " ";
						break;
					}
				}
				else  // 9x9 or smaller
				{
					if ((i == 2 && j == 2) ||
						(i == 2 && j == size-3) ||
						(i == size-3 && j == 2) ||
						(i == size-3 && j == size-3))
					{
						str += QChar(charset->starPoint);
						str += " ";
						break;
					}
				}
				
				str += QChar(charset->emptyPoint);
				str += " ";
				break;
			}
		}
		str = str.left(str.length()-1);
		str += "| ";
		if (size-i < 10) str += " ";
		str += QString::number(size-i) + "\n";
	}
	
	str += "   +";
	for (i=0; i<size-1; i++)
		str += "--";
	str += "-+\n    ";
	for (i=0; i<size; i++)
		str += QString(QChar(static_cast<const char>('A' + (i<8?i:i+1)))) + " ";
	str += "\n";
	
	return str;
}

/*
 * Checks if x,y is empty. The case being, increases the liberties counter, 
 * and stores x,y as visited
 */
void Matrix::checkNeighbourLiberty(int x, int y, QList<int> &libCounted, int &liberties)
{
	if (!x || !y)
		return;
	
	if (	x <= size && y <= size && x > 0 && y > 0 &&
	    	!libCounted.contains(100*x + y) &&
	    	(getStoneAt(x, y) == stoneNone ))  // ?? check stoneErase ?
	{
		  libCounted.append(100*x + y);
		  liberties ++;
	}
}

/*
 * Checks if x,y is already marked. The case being, increases the liberties counter, 
 * and stores x,y as visited
 */
void Matrix::checkScoredNeighbourLiberty(int x, int y, QList<int> &libCounted, int &liberties)
{
	if (!x || !y)
		return;
	
//	Stone *s;
	
	if (	x <= size && y <= size && x >= 0 && y >= 0 &&
		!libCounted.contains(100*x + y) &&
		((at(x - 1, y - 1) & MARK_TERRITORY_DONE_BLACK) ||
		(at(x - 1, y - 1) & MARK_TERRITORY_DONE_WHITE)))
	{
		libCounted.append(100*x + y);
		liberties ++;
	}
}

/*
 * checks wether the positions x,y holds a stone 'color'.
 * if yes, appends the stone and returns the group
 */
Group* Matrix::checkNeighbour(int x, int y, StoneColor color, Group *group, Group *** groupMatrix) 
{
	// Are we into the board, and is the tentative stone present in the matrix ?
//	if (x == 0 || x == size + 1  || y == 0 || y == size + 1 ||  (at(x - 1, y - 1) != color))
	if (x == 0 || x == size + 1  || y == 0 || y == size + 1)
		return group;
	StoneColor c = getStoneAt(x, y);
	if(groupMatrix && c == stoneNone)
	{
		std::vector<unsigned short>::iterator i = std::find(tempLibertyList.begin(), tempLibertyList.end(), (x << 8) + y);
		if(i == tempLibertyList.end())
		{
			tempLibertyList.push_back((x << 8) + y);
			group->liberties++;
		}
		return group;
	}
	else if(c != color)
		return group;

	MatrixStone *tmp= new MatrixStone;
	tmp->x = x;
	tmp->y = y;	
	tmp->c =  color;

	int mark = 0;
	bool found = false;

	while ( mark < group->count() && !found )
	{
		//Do we find the same stone in the group ?
		if (! group->compareItems(tmp, group->at(mark)))
			found = true ;

		mark ++;
	}
	
	// if not, we add it to the group
	if (!found)
	{
		
		if(groupMatrix)
			group->append(tmp, groupMatrix);
		else
			group->append(tmp);
	}

	return group;
}

/*
 * Counts and returns the number of liberties of a group of adjacetn stones
 * Since we replaced checkPosition, this might only be called by checkfalseEye
 * if anything.  */
int Matrix::countLiberties(Group *group) 
{
	int liberties = 0;
	QList<int> libCounted;
	
	// Walk through the horizontal and vertial directions, counting the
	// liberties of this group.
	for (int i=0; i<group->count(); i++)
	{
		MatrixStone *tmp = group->at(i);
		//CHECK_PTR(tmp);
		
		int 	x = tmp->x,
			y = tmp->y;
		
		// North
		checkNeighbourLiberty(x, y-1, libCounted, liberties);
		// West
		checkNeighbourLiberty(x-1, y, libCounted, liberties);
		// South
		checkNeighbourLiberty(x, y+1, libCounted, liberties);
		// East
		checkNeighbourLiberty(x+1, y, libCounted, liberties);
	}
	return liberties;
}

/*
 * Same as above, but used for the scoring phase
 */
int Matrix::countScoredLiberties(Group *group)
{
	int liberties = 0;
	QList<int> libCounted;
	
	// Walk through the horizontal and vertial directions, counting the
	// liberties of this group.
	for ( int i=0; i<group->count(); i++)
	{
		MatrixStone *tmp = group->at(i);
//		CHECK_PTR(tmp);
		
		int x = tmp->x,
			y = tmp->y;
		
		// North
		checkScoredNeighbourLiberty(x, y-1, libCounted, liberties);
		
		// West
		checkScoredNeighbourLiberty(x-1, y, libCounted, liberties);
		
		// South
		checkScoredNeighbourLiberty(x, y+1, libCounted, liberties);
		
		// East
		checkScoredNeighbourLiberty(x+1, y, libCounted, liberties);
	}
	return liberties;
}



/*
 * Explores a territorry for marking all of its enclosure
 */
void Matrix::traverseTerritory( int x, int y, StoneColor &col)
{
	// Mark visited
	set(x, y, MARK_TERRITORY_VISITED);
	
	// North
	if (checkNeighbourTerritory( x, y-1, col))
		traverseTerritory( x, y-1, col);
	
	// East
	if (checkNeighbourTerritory( x+1, y, col))
		traverseTerritory( x+1, y, col);
	
	// South
	if (checkNeighbourTerritory(x, y+1, col))
		traverseTerritory(x, y+1, col);
	
	// West
	if (checkNeighbourTerritory(x-1, y, col))
		traverseTerritory( x-1, y, col);
}

/*
 * Returns true if the territory at x,y is empty, false else
 * In this case, 'col' is modified depending on the stone status found
 */
bool Matrix::checkNeighbourTerritory(const int &x, const int &y, StoneColor &col)
{
	// Off the board ? Dont continue
	if (x < 0 || x >= size || y < 0 || y >= size)
		return false;
	
	// No stone ? Continue
	if (at(x , y) == 0 || (at(x, y) & MX_STONEDEAD))
		return true;
	
	// A stone, but no color found yet? Then set this color and dont continue
	// The stone must not be marked as alive in seki.
	if (col == stoneNone && at(x, y) < MARK_SEKI)
	{
		col = getStoneAt(x + 1, y + 1);
		return false;
	}
	
	// A stone, but wrong color? Set abort flag but continue to mark the rest of the dame points
	StoneColor tmpCol = stoneNone;
	if (col == stoneBlack)
		tmpCol = stoneWhite;
	else if (col == stoneWhite)
		tmpCol = stoneBlack;
	if ((tmpCol == stoneBlack || tmpCol == stoneWhite) && at(x, y) == tmpCol)
	{
		col = stoneErase;
		return false;
	}
	
	// A stone, correct color, or already visited, or seki. Dont continue
	return false;
}

/*
 * Returns the group (QList) of matrix stones adjacent to the 'stone'
 */
Group* Matrix::assembleGroup(MatrixStone *stone, Group *** groupMatrix)
{
	Group *group = new Group();
	Group *tmp;
	Q_CHECK_PTR(group);	

	if(groupMatrix)
	{
		tempLibertyList.clear();
		group->append(stone, groupMatrix);
	}
	else
		group->append(stone);
	
	int mark = 0;
	
	// Walk through the horizontal and vertical directions and assemble the
	// attached stones to this group.
	while (mark < group->count())
	{
		stone = group->at(mark);
		
		if (getStoneAt(stone->x, stone->y) != stoneNone )		//FIXME how can this be stoneNone?
		{
			int 	stoneX = stone->x,
				stoneY = stone->y;
			StoneColor col = stone->c;
			
			// North
			tmp = checkNeighbour(stoneX, stoneY-1, col, group, groupMatrix);
			// West
			tmp = checkNeighbour(stoneX-1, stoneY, col, group, groupMatrix);
			// South
			tmp = checkNeighbour(stoneX, stoneY+1, col, group, groupMatrix);
			// East
			tmp = checkNeighbour(stoneX+1, stoneY, col, group, groupMatrix);
		}
		mark ++;
	}
	
	return group;
}

int Matrix::sharedLibertyWithGroup(int x, int y, Group * g, Group *** gm)
{
	if(x > 0 && gm[x - 1][y] == g)
		return 1;
	if(x < size - 1 && gm[x + 1][y] == g)
		return 1;
	if(y > 0 && gm[x][y - 1] == g)
		return 1;
	if(y < size - 1 && gm[x][y + 1] == g)
		return 1;
	return 0;
}

/* This puts groups that the stone connects into joins and groups that the
 * stone captures into captures.
 * It also assembles new groups where necessary and stores them in the groupMatrix
 * but besides this it does nothing permanent so the caller can check if the
 * move is legal before playing it.
 * checkPosition calls this and removes the joins or the new group depending
 * on legality.*/
Group* Matrix::checkStoneWithGroups(MatrixStone * stone, Group *** groupMatrix, Group * joins[4], Group * enemyGroups[4])
{
	int & x = stone->x;
	int & y = stone->y;
	StoneColor ourColor = stone->c;
	StoneColor theirColor = (ourColor == stoneWhite ? stoneBlack : stoneWhite);
	StoneColor c;
	Group * g = NULL;
	Group * tmp = NULL;
	int i;

	for(i = 0; i < 4; i++)
	{
		joins[i] = NULL;
		enemyGroups[i] = NULL;
	}

	if(x > 1)
	{
		c = getStoneAt(x - 1, y);
		if(c == ourColor)
		{
			g = groupMatrix[x - 2][y - 1];
			if(!g)
				g = assembleGroup(new MatrixStone(x - 1, y, c), groupMatrix);
			joins[0] = g;
		}
		else if(c == theirColor)
		{
			tmp = groupMatrix[x - 2][y - 1];
			if(!tmp)
				tmp = assembleGroup(new MatrixStone(x - 1, y, c), groupMatrix);
			enemyGroups[0] = tmp;
		}
	}
#ifdef CHECKPOSITION_DEBUG
	if(g)
		printf("Group west with %d liberties\n", g->liberties);
	else if(tmp)
		printf("Enemy west with %d liberties\n", tmp->liberties);
#endif //CHECKPOSITION_DEBUG
	if(x < size)
	{
		c = getStoneAt(x + 1, y);
		if(c == ourColor)
		{
			g = groupMatrix[x][y - 1];
			if(!g)
				g = assembleGroup(new MatrixStone(x + 1, y, c), groupMatrix);
			if(g != joins[0])
				joins[1] = g;
		}
		else if(c == theirColor)
		{
			tmp = groupMatrix[x][y - 1];
			if(!tmp)
				tmp = assembleGroup(new MatrixStone(x + 1, y, c), groupMatrix);
			enemyGroups[1] = tmp;
		}
	}
#ifdef CHECKPOSITION_DEBUG
	if(joins[1])
		printf("Group east with %d liberties\n", joins[1]->liberties);
	else if(enemyGroups[1])
		printf("Enemy east with %d liberties\n", enemyGroups[1]->liberties);
#endif //CHECKPOSITION_DEBUG
	if(y > 1)
	{
		c = getStoneAt(x, y - 1);
		if(c == ourColor)
		{
			g = groupMatrix[x - 1][y - 2];
			if(!g)
				g = assembleGroup(new MatrixStone(x, y - 1, c), groupMatrix);
			if(g != joins[1] && g != joins[0])
				joins[2] = g;
		}
		else if(c == theirColor)
		{
			tmp = groupMatrix[x - 1][y - 2];
			if(!tmp)
				tmp = assembleGroup(new MatrixStone(x, y - 1, c), groupMatrix);
			enemyGroups[2] = tmp;
		}
	}
#ifdef CHECKPOSITION_DEBUG
	if(joins[2])
		printf("Group north with %d liberties\n", joins[2]->liberties);
	else if(enemyGroups[2])
		printf("Enemy north with %d liberties\n", enemyGroups[2]->liberties);
#endif //CHECKPOSITION_DEBUG
	if(y < size)
	{
		c = getStoneAt(x, y + 1);
		if(c == ourColor)
		{
			g = groupMatrix[x - 1][y];
			if(!g)
				g = assembleGroup(new MatrixStone(x, y + 1, c), groupMatrix);
			if(g != joins[2] && g != joins[1] && g != joins[0])
				joins[3] = g;
		}
		else if(c == theirColor)
		{
			tmp = groupMatrix[x - 1][y];
			if(!tmp)
				tmp = assembleGroup(new MatrixStone(x, y + 1, c), groupMatrix);
			enemyGroups[3] = tmp;
		}
	}
#ifdef CHECKPOSITION_DEBUG
	if(joins[3])
		printf("Group south with %d liberties\n", joins[3]->liberties);
	else if(enemyGroups[3])
		printf("Enemy south with %d liberties\n", enemyGroups[3]->liberties);
#endif //CHECKPOSITION_DEBUG
	insertStone(x, y, ourColor);
	g = assembleGroup(stone, groupMatrix);
#ifdef CHECKPOSITION_DEBUG
	printf("New stone liberties %d\n", g->liberties);
#endif //CHECKPOSITION_DEBUG
	return g;
}

/* Note that checkStoneWithGroups would be called before this meaning that
 * their could be no adjacent stones not in groups */
void Matrix::removeGroup(Group * g, Group *** groupMatrix, Group * killer)
{
	int i;
	int x, y;
	Group * enemyGroup;
	Group * libertines[3];
	killer = NULL;			//FIXME remove me

	for(i = 0; i < g->count(); i++)
	{
		x = (*g)[i]->x;
		y = (*g)[i]->y;
		if(x > 1)
		{
			enemyGroup = groupMatrix[x - 2][y - 1];
			if(enemyGroup && enemyGroup != g && enemyGroup != killer)
			{
				enemyGroup->liberties++;
				libertines[0] = enemyGroup;
			}
			else
				libertines[0] = NULL;
		}
		else
				libertines[0] = NULL;
		if(x < size)
		{
			enemyGroup = groupMatrix[x][y - 1];
			if(enemyGroup && enemyGroup != g && enemyGroup != killer && 
				enemyGroup != libertines[0])
			{
				enemyGroup->liberties++;
				libertines[1] = enemyGroup;
			}
			else
				libertines[1] = NULL;
		}
		else
			libertines[1] = NULL;
		if(y > 1)
		{
			enemyGroup = groupMatrix[x - 1][y - 2];
			if(enemyGroup && enemyGroup != g && enemyGroup != killer && 
				enemyGroup != libertines[0] && enemyGroup != libertines[1])
			{
				enemyGroup->liberties++;
				libertines[2] = enemyGroup;
			}
			else
				libertines[2] = NULL;
		}
		else
			libertines[2] = NULL;
		if(y < size)
		{
			enemyGroup = groupMatrix[x - 1][y];
			if(enemyGroup && enemyGroup != g && enemyGroup != killer &&
				enemyGroup != libertines[0] && enemyGroup != libertines[1] && enemyGroup != libertines[2])
				enemyGroup->liberties++;
		}
				
		groupMatrix[x - 1][y - 1] = NULL;
		insertStone(x, y, stoneNone);
	}
	delete g;
}

void Matrix::removeStoneFromGroups(MatrixStone * stone, Group *** groupMatrix)
{
	int & x = stone->x;
	int & y = stone->y;
	StoneColor ourColor = stone->c;
	StoneColor theirColor = (ourColor == stoneWhite ? stoneBlack : stoneWhite);
	StoneColor c;
	Group * g = NULL;
	Group * tmp = NULL;
	int i;
	Group * joins[4];
	Group * enemyGroups[4];
	
	for(i = 0; i < 4; i++)
	{
		joins[i] = NULL;
		enemyGroups[i] = NULL;
	}

	insertStone(x, y, stoneErase);
	if(x > 1)
	{
		c = getStoneAt(x - 1, y);
		if(c == ourColor)
		{
			g = groupMatrix[x - 2][y - 1];
			if(g)
			{
				g->remove(stone);
				g->liberties++;
				joins[0] = g;
			}
			if(!g)
				joins[0] = assembleGroup(new MatrixStone(x - 1, y, c), groupMatrix);
		}
		else if(c == theirColor)
		{
			tmp = groupMatrix[x - 2][y - 1];
			if(tmp)
			{
				tmp->liberties++;
				enemyGroups[0] = tmp;
			}
			else
				enemyGroups[0] = assembleGroup(new MatrixStone(x - 1, y, c), groupMatrix);
			
		}
	}

	if(x < size)
	{
		c = getStoneAt(x + 1, y);
		if(c == ourColor)
		{
			g = groupMatrix[x][y - 1];
			if(g && g != joins[0])
			{
				g->remove(stone);
				g->liberties++;
				joins[1] = g;
			}
			if(!g)
				joins[1] = assembleGroup(new MatrixStone(x + 1, y, c), groupMatrix);			
		}
		else if(c == theirColor)
		{
			tmp = groupMatrix[x][y - 1];
			if(tmp && tmp != enemyGroups[0])
			{
				tmp->liberties++;
				enemyGroups[1] = tmp;
			}
			if(!tmp)
				enemyGroups[1] = assembleGroup(new MatrixStone(x + 1, y, c), groupMatrix);
			
		}
	}

	if(y > 1)
	{
		c = getStoneAt(x, y - 1);
		if(c == ourColor)
		{
			g = groupMatrix[x - 1][y - 2];
			if(g && g != joins[1] && g != joins[0])
			{
				g->remove(stone);
				g->liberties++;
				joins[2] = g;
			}
			if(!g)
				joins[2] = assembleGroup(new MatrixStone(x, y - 1, c), groupMatrix);
		}
		else if(c == theirColor)
		{
			tmp = groupMatrix[x - 1][y - 2];
			if(tmp && tmp != enemyGroups[0] && tmp != enemyGroups[1])
			{
				tmp->liberties++;
				enemyGroups[2] = tmp;
			}
			if(!tmp)
				enemyGroups[2] = assembleGroup(new MatrixStone(x, y - 1, c), groupMatrix);
		}
	}

	if(y < size)
	{
		c = getStoneAt(x, y + 1);
		if(c == ourColor)
		{
			g = groupMatrix[x - 1][y];
			if(g && g != joins[2] && g != joins[1] && g != joins[0])
			{
				g->remove(stone);
				g->liberties++;
				joins[3] = g;
			}
			if(!g)
				joins[3] = assembleGroup(new MatrixStone(x, y + 1, c), groupMatrix);
		}
		else if(c == theirColor)
		{
			tmp = groupMatrix[x - 1][y];
			if(tmp && tmp != enemyGroups[0] && tmp != enemyGroups[1] && tmp != enemyGroups[2])
			{
				tmp->liberties++;
				enemyGroups[3] = tmp;	//FIXME unnecessary for debug
			}
			if(!tmp)
				enemyGroups[3] = assembleGroup(new MatrixStone(x, y + 1, c), groupMatrix);
		}
	}
}

void Matrix::invalidateChangedGroups(Matrix & m, Group *** gm)
{
	std::vector<Group*> groupList;
	std::vector<Group*>::iterator k;
	Group * g;
	int i, j;

	for (i=0; i<size; i++)
		for (j=0; j<size; j++)
		{
			if((matrix[i][j] & 0x00ff) != (m.matrix[i][j] & 0x00ff))
			{
				g = gm[i][j];
				if(!g)
					continue;
				k = std::find(groupList.begin(), groupList.end(), g);
				if(k == groupList.end())
				{
					groupList.push_back(g);
					findInvalidAdjacentGroups(g, gm, groupList);
				}
			}
		}
	for(k = groupList.begin(); k != groupList.end(); k++)
	{
		for(i = 0; i < (*k)->count(); i++)
			gm[(*k)->at(i)->x - 1][(*k)->at(i)->y - 1] = NULL;
		delete *k;
	}
}

void Matrix::invalidateAdjacentGroups(MatrixStone m, Group *** gm)
{
	std::vector<Group*> groupList;
	std::vector<Group*>::iterator j, k;
	Group * g;
	int i;
	if(m.x == 20 && m.y == 20)		//awkward passing check FIXME
		return;
	if(m.x == -1 || m.y == -1)		//can happen from sgf, FIXME, probably related to assignCurrent call from setCurrent
		return;
	if(m.x > 1 && (g = gm[m.x - 2][m.y - 1]))
	{
		j = std::find(groupList.begin(), groupList.end(), g);
		if(j == groupList.end())
		{
			groupList.push_back(g);
			findInvalidAdjacentGroups(g, gm, groupList);
		}
	}
	if(m.x < size && (g = gm[m.x][m.y - 1]))
	{
		j = std::find(groupList.begin(), groupList.end(), g);
		if(j == groupList.end())
		{
			groupList.push_back(g);
			findInvalidAdjacentGroups(g, gm, groupList);
		}
	}
	if(m.y > 1 && (g = gm[m.x - 1][m.y - 2]))
	{
		j = std::find(groupList.begin(), groupList.end(), g);
		if(j == groupList.end())
		{
			groupList.push_back(g);
			findInvalidAdjacentGroups(g, gm, groupList);
		}
	}
	if(m.y < size && (g = gm[m.x - 1][m.y]))
	{
		j = std::find(groupList.begin(), groupList.end(), g);
		if(j == groupList.end())
		{
			groupList.push_back(g);
			findInvalidAdjacentGroups(g, gm, groupList);
		}
	}
	for(k = groupList.begin(); k != groupList.end(); k++)
	{
		for(i = 0; i < (*k)->count(); i++)
			gm[(*k)->at(i)->x - 1][(*k)->at(i)->y - 1] = NULL;
		delete *k;
	}
}

void Matrix::findInvalidAdjacentGroups(Group * g, Group *** gm, std::vector<Group*> & groupList)
{
	int i;
	MatrixStone * m;
	Group * f;
	std::vector<Group*>::iterator j;

	for(i = 0; i < g->count(); i++)
	{
		m = g->at(i);
		if(m->x > 1)
		{
			if((f = gm[m->x - 2][m->y - 1]) && f != g)
			{
				j = std::find(groupList.begin(), groupList.end(), f);
				if(j == groupList.end())
					groupList.push_back(f);
			}
		}
		if(m->x < size)
		{
			if((f = gm[m->x][m->y - 1]) && f != g)
			{
				j = std::find(groupList.begin(), groupList.end(), f);
				if(j == groupList.end())
					groupList.push_back(f);
			}
		}
		if(m->y > 1)
		{
			if((f = gm[m->x - 1][m->y - 2]) && f != g)
			{
				j = std::find(groupList.begin(), groupList.end(), f);
				if(j == groupList.end() && f != g)
					groupList.push_back(f);
			}
		}
		if(m->y < size)
		{
			if((f = gm[m->x - 1][m->y]) && f != g)
			{
				j = std::find(groupList.begin(), groupList.end(), f);
				if(j == groupList.end())
					groupList.push_back(f);
			}
		}
	}
}

/* FIXME A little confusing here is that "markChangesDirty" is called usually with an invalidateAllGroups
 * but really seems like the two could be combined somehow. 
 * On second thought, I feel like markChangesDirty might not be called from setCurrent calls but that the
 * solution is to hide it somewhere, to wrap any change to current... yes...*/
void Matrix::invalidateAllGroups(Group *** gm)
{
	int i, j;
#ifdef CHECKPOSITION_DEBUG
	qDebug("invalidating %p", gm);
#endif //CHECKPOSITION_DEBUG
	std::vector<Group*> groupList;
	std::vector<Group*>::iterator k;
	for(i = 0; i < size; i++)
	{
		for(j = 0; j < size; j++)
		{
			if(gm[i][j])
			{
				k = std::find(groupList.begin(), groupList.end(), gm[i][j]);
				if(k == groupList.end())
					groupList.push_back(gm[i][j]);
				gm[i][j] = NULL;
			}
		}
	}
	for(k = groupList.begin(); k != groupList.end(); k++)
		delete *k;
}

/* This is kind of ugly but I'm trying to use the existing matrix
 * code for something weird 
 * Could there be a potential miscalc if called on empty vertex?
 * Would it be*/
Group* Matrix::assembleAreaGroups(MatrixStone *stone)
{
	Group *group = new Group();
	Q_CHECK_PTR(group);
	StoneColor oppColor;

	group->append(stone);
	
	int mark = 0;
	
	if(stone->c == stoneWhite)
		oppColor = stoneBlack;
	else
		oppColor = stoneWhite;
	StoneColor col = stone->c;
	// Walk through the horizontal and vertical directions and assemble the
	// attached stones to this group.
	while (mark < group->count())
	{
		stone = group->at(mark);
		
		if (getStoneAt(stone->x, stone->y) != oppColor )
		{
			int 	stoneX = stone->x,
			stoneY = stone->y;
			
			
			// North
			group = checkNeighbour(stoneX, stoneY-1, col, group);
			group = checkNeighbour(stoneX, stoneY-1, stoneNone, group);
			// West
			group = checkNeighbour(stoneX-1, stoneY, col, group);
			group = checkNeighbour(stoneX-1, stoneY, stoneNone, group);
			// South
			group = checkNeighbour(stoneX, stoneY+1, col, group);
			group = checkNeighbour(stoneX, stoneY+1, stoneNone, group);
			// East
			group = checkNeighbour(stoneX+1, stoneY, col, group);
			group = checkNeighbour(stoneX+1, stoneY, stoneNone, group);
		}
		mark ++;
	}
	
	return group;
}

/*
 * Returns true if the stone at x,y belongs to a groups with only 1 liberty
 * Previously checkfalseEye worked on the scored liberties.  The problem
 * was that it missed certain eyes on disconnected groups that could be
 * connected.  I don't know why it would only look at scored liberties.
 * If group is dead that's another matter, but I don't think that's an
 * issue. 
*/
bool Matrix::checkfalseEye( int x, int y, StoneColor col)
{
	MatrixStone *tmp= new MatrixStone ;

	tmp->c =  col;

	// Stone to the North?
	if (y - 1 >= 0 && getStoneAt(x + 1, y) == col)
	{
		tmp->x = x +1 ;
		tmp->y = y ;
		//if (countScoredLiberties(assembleGroup(tmp)) == 1) 
		//	return true;
		if (countLiberties(assembleGroup(tmp)) == 1) 
			return true;
	}

	// Stone to the west?
	if (x - 1 >= 0 && getStoneAt(x, y + 1) == col)
	{
		tmp->x = x  ;
		tmp->y = y + 1 ;
		//if (countScoredLiberties(assembleGroup(tmp)) == 1) 
		//	return true;
		if (countLiberties(assembleGroup(tmp)) == 1) 
			return true;
	}

	// Stone to the south?
	if (y + 1 < size && getStoneAt(x + 1, y + 2) == col)
	{
		tmp->x = x + 1 ;
		tmp->y = y + 2 ;
		//if (countScoredLiberties(assembleGroup(tmp)) == 1) 
		//	return true;
		if (countLiberties(assembleGroup(tmp)) == 1) 
			return true;
	}
 
	// Stone to the east?
	if (x + 1 < size && getStoneAt(x + 2, y + 1) == col)
	{
		tmp->x = x + 2 ;
		tmp->y = y + 1 ;
		//if (countScoredLiberties(assembleGroup(tmp)) == 1) 
		//	return true;
		if (countLiberties(assembleGroup(tmp)) == 1) 
			return true;
	}
	
	return false;
}

/*
 * This function marks all the stones of a group as dead (or alive if they were dead)
 */
void Matrix::toggleGroupAt( int x, int y)
{
	StoneColor col = getStoneAt(x, y);

	if ( col != stoneWhite && col != stoneBlack )
		return ;

	MatrixStone *s = new MatrixStone;
	s->x = x;
	s->y = y;
	s->c = col;

	Group *g = assembleGroup(s);			//FIXME use the view

	for (int i=0; i<g->count(); i++)
	{
		s = g->at(i);
		toggleStoneAt(s->x, s->y);
	}

}

/* These are all kind of redundant, some aren't used, etc..
 * the other thing that bothers me is that the stone.cpp class
 * should probably hold "dirty" and then we've considered before
 * that matrix could be removed for something that takes up
 * less space, but if this works for now, that's fine. FIXME */

void Matrix::toggleStoneAt(int x, int y)
{
	matrix[x -1][y -1] ^= MX_STONEDEAD;
	matrix[x -1][y -1] |= MX_STONEDIRTY;
}

void Matrix::markStoneDead(int x, int y)
{
	if(isStoneDead(x, y))
		return;
	toggleStoneAt(x, y);
}

void Matrix::markGroupDead(int x, int y)
{
	if(isStoneDead(x, y))
		return;
	toggleGroupAt(x, y);
}

void Matrix::markGroupAlive(int x, int y)
{
	if(!isStoneDead(x, y))
		return;
	toggleGroupAt(x, y);
}

/* Any stones connected to this xy stone by empty
 * vertices are to be marked dead */
void Matrix::toggleAreaAt( int x, int y)
{
	StoneColor col = getStoneAt(x, y);

	if ( col != stoneWhite && col != stoneBlack )
		return ;

	MatrixStone *s = new MatrixStone;
	s->x = x;
	s->y = y;
	s->c = col;

	Group *g = assembleAreaGroups(s);

	for (int i=0; i<g->count(); i++)
	{
		s = g->at(i);
		toggleStoneAt(s->x, s->y);
	}

}

void Matrix::markAreaDead(int x, int y)
{
	if(isStoneDead(x, y))
		return;
	toggleAreaAt(x, y);
}

void Matrix::markAreaAlive(int x, int y)
{
	if(!isStoneDead(x, y))
		return;
	toggleAreaAt(x, y);
}

/*
 * This function returns the first letter or number available in the letter marks
 */
QString Matrix::getFirstTextAvailable(MarkType t)
{
	if (markTexts == NULL || markTexts->count() == 0)
		return (t == markText ? "A" : "1");

	QString mark, rxm;

	switch (t)
	{
		case markText :
		{
			for (int n = 0 ; n < 51 ; n++)
			{
				mark = QString(QChar(static_cast<const char>('A' + (n>=26 ? n+6 : n))));
				rxm = mark;
				rxm.prepend("*#");
				QRegExp rx(rxm,Qt::CaseSensitive, QRegExp::Wildcard );
				if (markTexts->indexOf(rx) == -1)
					break;
			}
			break;
		}

		case markNumber :
		{
			for (int n = 1 ; n < 400 ; n++)
			{
				mark = QString::number(n);
				rxm = mark;
				rxm.prepend("*#");
				QRegExp rx(rxm,Qt::CaseSensitive, QRegExp::Wildcard );
				if (markTexts->indexOf(rx) == -1)
					break;
			}
			break;
		}
		//should not happen
		default:
			break;
	}

	return mark;
	
}
