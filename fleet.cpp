//----------------------------------------------------------------------------
// File:		fleet.cpp
// 
// Functions:	 
//				setShipInfo()	
//				allocMem() 
//				deleteMem()
//				printShip() 
//				printGrid() 
//				initializePlayer() 
//				setships()	      
//				saveGrid()
//				loadGridFromFile()
//				getCoord()
//				isValidLocation() 
//				header() 
//				endBox() 
//----------------------------------------------------------------------------

#include "fleet.h"

const char* shipNames[SHIP_SIZE_ARRAYSIZE] =
	{"No Ship", "Mine Sweeper", "Submarine",
	 "Frigate", "Battleship", "Aircraft Carrier"};
const int TOTALPIECES = 17; // Total pieces in all ships

//----------------------------------------------------------------------------
// Function:	setShipInfo()
//
// Title:		Set ship information
//
// Description: Sets fields of "ShipInfo" structure
//
// Programmer:	Albert Shymanskyy
// 
// Date:		12/20/05
//
// Version:		0.1
// 
// Environment: 
//              Software: Windows 10 
//              Compiles under Microsoft Visual C++ 2017
//
// Calls: None
//
// Called By: initializePlayer()
//
// Parameters:	shipInfoPtr: ShipInfo *; pointer to the ShipInfo to be set
//	name: Ship;	enumerated name of type of ship; default: NOSHIP
//	orientation: Direction;	enumerated direction; default: HORIZONTAL
//	row: unsigned short; row-coordinate in grid; default: 0
//	col: unsigned short; column-coordinate in grid; default: 0
// 
// Returns:	void
//
// History Log:
//		12/20/05 PB completed v 0.1
//		01/22/18 Function completed
//   
//----------------------------------------------------------------------------
void setShipInfo(ShipInfo * shipInfoPtr, Ship name, Direction orientation,
	unsigned short row, unsigned short col)
{
	shipInfoPtr->m_name = name;
	shipInfoPtr->m_orientation = orientation;
	shipInfoPtr->m_bowLocation.m_row = row;
	shipInfoPtr->m_bowLocation.m_col = col;
	shipInfoPtr->m_piecesLeft = shipSize[name];
} 

//----------------------------------------------------------------------------
// Function:	allocMem()
// Title:		Allocate Memory
// Description:
//		allocates memory for current grids
// Programmer:	Paul Bladek
// modified by:	
// 
// Date:	9/13/06
//
// Version:	1.01
// 
// Environment: Hardware: i3 
//              Software: OS: Windows 10; 
//              Compiles under Microsoft Visual C++ 2015
//
// Calls:
//
// Called By:	main()
//
// Parameters:	players: Player[]; 	the array of 2 players in the game
//		size: char;	'	S' or 'L' (small or large)
// 
// Returns:	void
//
// History Log:
//		12/20/05 PB completed v 1.0 
//		9/13/06  PB completed v 1.01
//		1/20/17  PB completed v 1.02
//
//----------------------------------------------------------------------------
void allocMem(Player players[], char size)
{
	short numberOfRows = (toupper(size) == 'L') ? LARGEROWS : SMALLROWS;
	short numberOfCols = (toupper(size) == 'L') ? LARGECOLS : SMALLCOLS;

	try
	{
		for (int whichGrid = 0; whichGrid < NUMPLAYERS; whichGrid++) // loop through both grid types -- current player and opponent.
		{
			for (short i = 0; i < NUMPLAYERS; ++i) // loop through both players -- player 1 and player 2 (remember, each player has 2 grids. His and opponents.
			{
				players[i].m_gameGrid[whichGrid] = nullptr; // initialize to null pointer -- simplifies debugging if exception thrown by line below --cleans this spot
				players[i].m_gameGrid[whichGrid] = new Ship * [numberOfRows]; // allocate memory for array of pointers to ships -- each item constitutes a row pointer.
				for (short j = 0; j < numberOfRows; ++j)
				{
					players[i].m_gameGrid[whichGrid][j] = nullptr;
					players[i].m_gameGrid[whichGrid][j] = new Ship [numberOfCols];

					for(short k = 0; k < numberOfCols; ++k)
					{
						players[i].m_gameGrid[whichGrid][j][k] = NOSHIP; // initialize all items in row to NOSHIP
					} // end for k
				} // end for j

			} // end for i
		} // end for whichGrid
	}

	catch(exception e)
	{
		deleteMem(players, size);
		cerr << "exception: " << e.what() << endl;
		cout << "shutting down" << endl;
		cin.ignore(FILENAME_MAX, '\n');
		exit(EXIT_FAILURE);
	}
}

//----------------------------------------------------------------------------
// Function:	deleteMem()
// Title:		Delete Memory
// Description:
//		Safely deletes memory for grids
// Programmer:
// 
// Date:	12/20/05
//
// Version:	0.1
// 
// Environment: Hardware: i3 
//              Software: OS: Windows 10; 
//              Compiles under Microsoft Visual C++ 2017
//
// Calls:
//
// Called By:	main()
//
// Parameters:	players: Player[]; 	the array of the 2 Players
//		size: char;	'	S' or 'L'
// 
// Returns:	void
//
// History Log:
//		12/20/05 PB completed v 0.1
//   
//----------------------------------------------------------------------------
void deleteMem(Player players[], char size) 
{
	short numberOfRows = (toupper(size) == 'L') ? LARGEROWS : SMALLROWS;

	for (int whichGrid = 0; whichGrid < NUMPLAYERS; whichGrid++) // loop through both types of grids
	{
		for (short i = 0; i < NUMPLAYERS; i++) // loop through both players
		{
			for (short j = 0; j < numberOfRows; j++)
			{
				// if pointer is NOT null --> delete the array of ships this row pointer points to.
				if (players[i].m_gameGrid[whichGrid][j] != nullptr)
					delete[] players[i].m_gameGrid[whichGrid][j];
			}
			if (players[i].m_gameGrid[whichGrid] != nullptr)
				delete[] players[i].m_gameGrid[whichGrid];
		}
	}
	//cout << "Delete function completed" << endl;
}

//----------------------------------------------------------------------------
// Function:	printShip()
// Title:	Print Ship 
// Description:
//		Print grid element for the proper ship
// Programmer:	Paul Bladek
// 
// Date:	9/12/06
//
// Version:	1.0
// 
// Environment: Hardware: i3 
//              Software: OS: Windows 10; 
//              Compiles under Microsoft Visual C++ 2017
//
// Output:		three characters representing one ship to sout
//
// Calls:
//
// Called By:	printGrid()
//
// Parameters:	sout: ostream&;	the stream to print to
//		thisShip: Ship;	the Ship to be printed in the grid format
// 
// Returns:	void
//
// History Log: 
//		9/12/06 PB comleted v 1.0
//     
//----------------------------------------------------------------------------
void printShip(ostream & sout, Ship thisShip)
{	
	sout << ' ' ;
	switch(thisShip)
	{
		case NOSHIP: sout << ' ';
			break;
		case MINESWEEPER: sout << 'M';
			break;
		case SUB: sout << 'S';
			break;
		case FRIGATE: sout << 'F';
			break;
		case BATTLESHIP: sout << 'B';
			break;
		case CARRIER: sout << 'C';
			break;
		case HIT: sout << 'H';
			break;
		case MISSED: sout << MISS;
			break;
		default: sout << 'X';
	}
	sout << VERT ;
}

//----------------------------------------------------------------------------
// Function:	printGrid()
// Title:	Print Ship 
// Description:
//		Print grid element for the proper ship
// Programmer:
// 
// Date:	12/20/05
//
// Version:	0.1
// 
// Environment: Hardware: i3 
//              Software: OS: Windows 10; 
//              Compiles under Microsoft Visual C++ 2017
//
// Output:	a single grid to sout
//
// Calls:	printShip()
//
// Called By:	main()
//		setships()
//		saveGrid()
//
// Parameters:	sout: ostream&;	the stream to print to
//		grid: Ship**;	the 2-D array of Ships 
//		size: char;	'S' or 'L'
// 
// Returns:	void
//
// History Log:
//		12/20/05 PB completed v 0.1
//     
//----------------------------------------------------------------------------
void printGrid(ostream& sout, Ship** grid, char size)
{
	short numberOfRows = (toupper(size) == 'L') ? LARGEROWS : SMALLROWS;
	short numberOfCols = (toupper(size) == 'L') ? LARGECOLS : SMALLCOLS;

	for(short j = 1; j <= numberOfCols; ++j)
		sout << setw(3) << j;
	sout  << endl;
	char ch = 'A';
	for (short i = 0; i < numberOfRows; ++i)
	{
		sout << ch++;
		for (short j = 0; j < numberOfCols; ++j)
		{
			printShip(sout, grid[i][j]);
		}
		sout << endl;
		sout << HORIZ;
		for (short j = 0; j < numberOfCols; ++j)
		{
			sout << HORIZ << HORIZ << CR;
		}
		sout << endl;
	} 
} 

//----------------------------------------------------------------------------
// Function:	initializePlayer()
// Title:	Initialize Player 
// Description:
//		sets initial values for m_ships and m_piecesLeft
// Programmer:	Paul Bladek
// 
// Date:	9/12/06
//
// Version:	1.0
// 
// Environment: Hardware: i3 
//              Software: OS: Windows 10; 
//              Compiles under Microsoft Visual C++ 2017
//
// Calls:
//
// Called By:	main()
//
// Parameters:	playerPtr: Player*;	a pointer to the current Player
// 
// Returns:	void
//
// History Log: 
//		9/12/06 PB comleted v 1.0
//     
//----------------------------------------------------------------------------
void initializePlayer(Player* playerPtr)
{
	for(short i = 0; i < SHIP_SIZE_ARRAYSIZE; i++)
		setShipInfo(playerPtr->m_ships + i, static_cast<Ship>(i));
	
	playerPtr->m_piecesLeft = TOTALPIECES; 
}

//----------------------------------------------------------------------------
// Function:	setships()
// Title:	Set Ships 
// Description:
//		Allows user to put ships in grid
// Programmer:	Paul Bladek
// modified by:	
// 
// Date:	9/12/06
//
// Version:	0.5
// 
// Environment: Hardware: i3 
//              Software: OS: Windows 10; 
//              Compiles under Microsoft Visual C++ 2017
//
// Input:	location and orientation using getCoord from cin
//
// Output:	prompts to cout
//
// Calls:	printGrid()
//		safeChoice()
//		getCoord()
//		saveGrid()
//
// Called By:	main()
//
// Parameters:	players: Player[];	the array of 2 players 
//		size: char;		'S' or 'L'
//		whichPlayer: short;	the player number (0 or 1)
// 
// Returns:	void
//
// History Log: 
//		9/12/06 PB comleted v 0.5
//     
//----------------------------------------------------------------------------
void setships(Player players[], char size, short whichPlayer)
{
	char input = 'V';
	char ok = 'Y';
	char save = 'N';
	ostringstream outSStream;
	Cell location = {0, 0};
	for(short j = 1; j < SHIP_SIZE_ARRAYSIZE; j++)
	{
		system("cls");
		printGrid(cout, players[whichPlayer].m_gameGrid[0], size);
		outSStream.str("");
		outSStream << "Player " << whichPlayer + 1 << " Enter "
			<< shipNames[j] << " orientation";
		input = safeChoice(outSStream.str(), 'V', 'H');
		players[whichPlayer].m_ships[j].m_orientation
			= (input == 'V') ? VERTICAL : HORIZONTAL;
		cout << "Player " << whichPlayer + 1 << " Enter " << 
			shipNames[j] <<	
			" bow coordinates <row letter><col #>:" << endl;
		players[whichPlayer].m_ships[j].m_bowLocation = location
			= getCoord(cin, size); 
		//^ sets 'Cell' part of struct to coordinates entered by user ^

		// if ok
		if(!isValidLocation(players[whichPlayer], j, size))
		{
			cout << "invalid location. Press <enter>" ;
			cin.get();
			j--; // redo

			continue;
		}

		// Update the grid with cells of a new ship
		if (input == 'V')
			for (short k = 0; k < shipSize[j]; k++)
				players[whichPlayer].m_gameGrid[0][location.m_row + k]
				[location.m_col] = static_cast<Ship>(j);
		else
			for (short k = 0; k < shipSize[j]; k++)
				players[whichPlayer].m_gameGrid[0][location.m_row]
				[location.m_col + k] = static_cast<Ship>(j);

		system("cls");

		printGrid(cout, players[whichPlayer].m_gameGrid[0], size);
		cout << "Player " << whichPlayer + 1 << " " << shipNames[j];
		if (safeChoice(" OK?", 'Y', 'N') == 'N')
		{
			// Clear the cells of a discarded ship
			if (input == 'V')
				for (short k = 0; k < shipSize[j]; k++)
					players[whichPlayer].m_gameGrid[0][location.m_row + k]
					[location.m_col] = NOSHIP;
			
			else
				for (short k = 0; k < shipSize[j]; k++)
					players[whichPlayer].m_gameGrid[0][location.m_row]
					[location.m_col + k] = NOSHIP;
			j--;

			continue;
		}


	} // end for j
	save = safeChoice("\nSave starting grid?", 'Y', 'N');
	if(save == 'Y')
		saveGrid(players, whichPlayer, size);
}

//----------------------------------------------------------------------------
// Function:	saveGrid()
// Title:	Save Grid 
// Description:
//		Saves the ship grid to a file
// Programmer:
// 
// Date:	12/20/05
//
// Version:	0.1
// 
// Environment: Hardware: i3 
//              Software: OS: Windows 10; 
//              Compiles under Microsoft Visual C++ 2017
//
// Output:	grid to specified file
//
// Calls:	printGrid()
//
// Called By:	setships()
//
// Parameters:	player: sPlayer[];	the array of 2 players
//		whichPlayer: short; the player number (0 or 1) 
//		size: char;	'S' or 'L'
// 
// Returns:	void
//
// History Log:
//		12/20/05 PB completed v 0.1
//     
//----------------------------------------------------------------------------
void saveGrid(Player players[], short whichPlayer, char size)
{
	short numberOfRows = (toupper(size) == 'L') ? LARGEROWS : SMALLROWS;
	short numberOfCols = (toupper(size) == 'L') ? LARGECOLS : SMALLCOLS;	
	
	ofstream saveFile;
	string saveFilename;
	cout << "Enter name of file to save to (.shp will be added):";
	cin >> saveFilename;
	saveFile.open(saveFilename.append(".shp"));
	printGrid(saveFile, players[whichPlayer].m_gameGrid[0], size);
	saveFile.close();
}

//----------------------------------------------------------------------------
// Function:	loadGridFromFile()
// Title:	loadGridFromFile 
// Description:
//		Reads grid from a file and properly sets the ships
// Programmer:	Paul Bladek & Cameron Stevenson
// modified by:	
// 
// Date:	9/12/06
//
// Version:	0.5
// 
// Environment: Hardware: i3 
//              Software: OS: Windows 10; 
//              Compiles under Microsoft Visual C++ 2017
//
// Input:	grid from specified file
//
// Output:	prompts to cout
//
// Calls:
//
// Called By:	main()
//
// Parameters:	players: Player[];	the array of 2 players
//		whichPlayer: short;	the player number  (0 or 1) 
//		size: char;		'S' or 'L'
//		string fileName:	the name of the file to be opened
// 
// Returns: bool -- 	true if the file is opened and read;
//			false otherwise
//
// History Log: 
//		9/12/06 PB comleted v 0.5
//     
//----------------------------------------------------------------------------
bool loadGridFromFile(Player players[], short whichPlayer, char size, 
	string fileName)
{
	string line;
	ifstream ifs;
	Ship ship = NOSHIP;
	short shipCount[SHIP_SIZE_ARRAYSIZE] = {0};
	char cell = ' ';
	char fsize = 'S';
	short numberOfRows = (toupper(size) == 'L') ? LARGEROWS : SMALLROWS;
	short numberOfCols = (toupper(size) == 'L') ? LARGECOLS : SMALLCOLS;

	try
	{
		ifs.open(fileName.c_str());
		if(!ifs)
		{
			cout << "could not open file " << fileName << endl
				<< " press <enter> to continue" << endl;
			cin.ignore(FILENAME_MAX, '\n');
			return false;
		}
	} 
	catch(exception e)
	{
		cout << "could not open file " << fileName << endl
			<< " press <enter> to continue" << endl;
		cin.ignore(FILENAME_MAX, '\n');
		return false;
	}	
	// YOUR CODE GOES HERE ...

	for (short j = 0; j < numberOfCols; ++j) //read in the upper row.
	{
		ifs.get();
		ifs.get();
		ifs.get();
	}
	ifs.get(); //read in newline character
	for (short i = 0; i < numberOfRows; ++i)// for each row after read skip one, grab the character, skip the bar
	{
		for (short j = 0; j < numberOfCols; ++j)
		{
			ifs.get(); //read in row-letter/vertical bar
			ifs.get(); //read in space
			players[whichPlayer].m_gameGrid[0][i][j] = loadShip(ifs.get());
			
		}
		ifs.get(); //read in the vertical bar after each row
		ifs.get(); //read in the newline character after each row
		for (short j = 0; j < numberOfCols; ++j) //read in HORZ/CR rows
		{
			ifs.get();
			ifs.get();
			ifs.get();
		}
		ifs.get(); //read in the vertical bar
		ifs.get(); // read in the newlines character
	}
	system("cls");
	printGrid(cout, players[whichPlayer].m_gameGrid[0], size);
	
	return true;
}

//----------------------------------------------------------------------------
// Function:	getCoord()
// Title:	Get Coordinates 
// Description:
//		Returns a cell with coordinates set by user
// Programmer:	Paul Bladek
// 
// Date:	9/12/06
//
// Version:	1.0
// 
// Environment: Hardware: i3 
//              Software: OS: Windows 10; 
//              Compiles under Microsoft Visual C++ 2017
//
// Input:	cell coordinates (in the form "A13" from sin
//
// Output:	prompts to cout
//
// Calls:	none
//
// Called By:	main()
//		setships()
//
// Parameters:	sin: istream&;	the stream to read from
//		size: char;	'S' or 'L'
// 
// Returns:	Cell location -- a cell containing the input coordinates
//
// History Log: 
//		9/12/06 PB comleted v 1.0
//     
//----------------------------------------------------------------------------
Cell getCoord(istream& sin, char size)
{
	short numberOfRows = (toupper(size) == 'L') ? LARGEROWS : SMALLROWS;
	short numberOfCols = (toupper(size) == 'L') ? LARGECOLS : SMALLCOLS;
	char highChar = static_cast<char>(numberOfRows - 1) + 'A';
	char row  = 'A';
	short col = 0;
	Cell location = {0, 0};
	do
	{
		col = 0;
		cout << "Row must be a letter from A to " << highChar 
			<< " and column must be from 1 to "  
			<< numberOfCols << ": ";
		while((row = toupper(sin.get())) < 'A' || row  > highChar)
		{
			sin.ignore(FILENAME_MAX, '\n');
			cout << "Row must be a letter from A to " << highChar 
				<< " and column must be from 1 to "  
				<< numberOfCols << ": ";
		}
		sin >> col;
		if(!sin)
			sin.clear();
		sin.ignore(FILENAME_MAX, '\n');
	}
	while(col < 1 || col > numberOfCols);
	
	location.m_col = col - 1;
	location.m_row = static_cast<short>(row - 'A');
	return location;
}

//----------------------------------------------------------------------------
// Function:	isValidLocation()
// Title:	Valid Location 
// Description:
//		Can the ship legitimately go there?
// Programmer:
// 
// Date:	12/20/05
//
// Version:	0.1
// 
// Environment: Hardware: i3 
//              Software: OS: Windows 10; 
//              Compiles under Microsoft Visual C++ 2017
//
// Calls:
//
// Called By:
//
// Parameters:	player: const Player&;	a reference to the specific Player
//		shipNumber: short;	the number of the ship (1 - 5)
//					in the array player.m_ships[]
//		size: char;		'S' or 'L'
// 
// Returns: bool -- 	true if the ship would not go off the edge
//				or cross another ship;
//			false otherwise
//
// History Log:
//		12/20/05 PB completed v 0.1
//     
//----------------------------------------------------------------------------
bool isValidLocation(const Player& player, short shipNumber, char size)
{
	short numberOfRows = (toupper(size) == 'L') ? LARGEROWS : SMALLROWS;
	short numberOfCols = (toupper(size) == 'L') ? LARGECOLS : SMALLCOLS;

	Cell location = player.m_ships[shipNumber].m_bowLocation;
	Direction orientation = player.m_ships[shipNumber].m_orientation;

	// Check if new ship is out of bounds vertically and horizontally
	if (orientation == VERTICAL)
	{
		if (location.m_row + shipSize[shipNumber] - 1 >= numberOfRows)
			return false;
	}
	else
		if (location.m_col + shipSize[shipNumber] - 1 >= numberOfCols)
			return false;

	// Check if new ship's cells are occupied vertically and horizontally
	if (orientation == VERTICAL)
	{
		for (short k = 0; k < shipSize[shipNumber]; k++)
			if (player.m_gameGrid[0][location.m_row + k][location.m_col]
				!= NOSHIP)
				return false;
	}
	else
	{
		for (short k = 0; k < shipSize[shipNumber]; k++)
			if (player.m_gameGrid[0][location.m_row][location.m_col + k]
				!= NOSHIP)
				return false;
	}

	// replace the return value
	return true;
}

//---------------------------------------------------------------------------
// Function:	header()
// Title:	header 
// Description:
//		Prints opening graphic
// Programmer:	Paul Bladek
// 
// Date:	9/12/06
//
// Version:	1.0
// 
// Environment: Hardware: i3 
//              Software: OS: Windows 10; 
//              Compiles under Microsoft Visual C++ 2017
//
// Output:	
//
// Calls:	boxTop()
//		boxLine()
//		boxBottom()
//
// Called By:	main()
//
// Parameters:	sout: ostream&;	the stream to print to
// 
// Returns:	void
//
// History Log: 
//		9/12/06 PB comleted v 1.0
//     
//---------------------------------------------------------------------------
void header(ostream& sout)
{
	const string empty;
	const string sink("SINK THE FLEET!");
	// your name goes here!
	const string by("Edmonds Community College CS 132");
	boxTop(sout, BOXWIDTH);
	boxLine(sout, empty, BOXWIDTH);
	boxLine(sout, sink , BOXWIDTH, 'C');
	boxLine(sout, empty, BOXWIDTH);
	boxLine(sout, by, BOXWIDTH, 'C');
	boxLine(sout, empty, BOXWIDTH);
	boxBottom(sout, BOXWIDTH);
}

//----------------------------------------------------------------------------
// Function:	endBox()
// Title:	End Box 
// Description:
//		prints closinging graphic
// Programmer:	Paul Bladek
// 
// Date:	9/12/06
//
// Version:	1.0
// 
// Environment: Hardware: i3 
//              Software: OS: Windows 10; 
//              Compiles under Microsoft Visual C++ 2017
//
// Output:	
//
// Calls:	boxTop()
//		boxLine()
//		boxBottom()
//
// Called By:	main()
//
// Parameters:	player: short; the number of the winner (0 or 1)
// 
// Returns:	void
//
// History Log: 
//		9/12/06 PB comleted v 1.0
//     
//---------------------------------------------------------------------------
void endBox(short player)
{
	const string empty;
	ostringstream msg;
	msg << "Congratulations player " << player + 1 << "!";
	boxTop(cout, BOXWIDTH);
	boxLine(cout, empty, BOXWIDTH);
	boxLine(cout, msg.str() , BOXWIDTH, 'C');
	boxLine(cout, empty, BOXWIDTH);
	boxBottom(cout, BOXWIDTH);
}
//----------------------------------------------------------------------------
// Function:	loadShip()
// Title:	Load Ship 
// Description:
//		reverse of printShip() takes in a character and translates it to Ship
// Programmer:	Cameron Stevenson & Albert Shymanskyy
// 
// Date:	9/12/06
//
// Version:	1.0
// 
// Environment: Hardware: i3 
//              Software: OS: Windows 10; 
//              Compiles under Microsoft Visual C++ 2017
//
// Output:		What type of ship according to the Ship enum
//
// Calls:
//
// Called By:	loadGridFromFile
//
// Parameters:	char characterRead; the array element in question
// 
// Returns:	Ship 
//
// History Log: 
//		9/12/06 PB comleted v 1.0
//     
//----------------------------------------------------------------------------
Ship loadShip(char characterRead)
{
	switch (characterRead)
	{
	case ' ': characterRead = NOSHIP;
		break;
	case 'M': characterRead = MINESWEEPER;
		break;
	case 'S': characterRead = SUB;
		break;
	case 'F': characterRead = FRIGATE;
		break;
	case 'B': characterRead = BATTLESHIP;
		break;
	case 'C': characterRead = CARRIER;
		break;
	}
	return static_cast<Ship>(characterRead);
}

