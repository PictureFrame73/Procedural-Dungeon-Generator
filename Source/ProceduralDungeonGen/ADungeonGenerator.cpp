
#include "ADungeonGenerator.h"


AADungeonGenerator::AADungeonGenerator()
{
}


void AADungeonGenerator::BeginPlay()
{
	Super::BeginPlay();
	
	GenerateDungeon();
}


// Create the grid for the dungeon
void AADungeonGenerator::InitializeGridLayout()
{
	Grid.SetNum( Width * Height );
}


// Gets a cell from the grid
FRoomCell& AADungeonGenerator::GetCell( int32 X, int32 Y )
{
	return Grid[ X + Y * Width ];
}


// Check if the room cell is inside the grids range
bool AADungeonGenerator::IsInsideGrid( int32 X, int32 Y ) const
{
	if ( (X >= 0 && X < Width) && (Y >= 0 && Y < Height) )
	{
		return true;
	}
	
	return false;
}


// Gets a random point in the grid
FIntPoint AADungeonGenerator::GetRandomPoint()
{
	int32 XPosition{ RandomStream.RandRange(0, Width - 1) };
	int32 YPosition{ RandomStream.RandRange(0, Height - 1) };
	FIntPoint Point{ XPosition, YPosition };
	
	return Point;
}


// Generates the START and END points in the grid
void AADungeonGenerator::GenerateStartEndPoints()
{
	StartPoint = GetRandomPoint();
	EndPoint = GetRandomPoint();
	
	while ( EndPoint == StartPoint )
	{
		EndPoint = GetRandomPoint();
	}
	
	GetCell( StartPoint.X, StartPoint.Y ).bIsStart = true;
	GetCell( StartPoint.X, StartPoint.Y ).bIsRoom = true;
	GetCell( EndPoint.X, EndPoint.Y ).bIsEnd = true;
	GetCell( EndPoint.X, EndPoint.Y ).bIsRoom = true;
}


// Create a path straight from the START to the END
void AADungeonGenerator::GenerateCriticalPath()
{
	FIntPoint StartLocation{ StartPoint.X, StartPoint.Y };
	FIntPoint EndLocation{ EndPoint.X, EndPoint.Y };
	CurrentRoomLocation = StartLocation;

	while ( CurrentRoomLocation != EndLocation )
	{
		if ( CurrentRoomLocation.X > EndLocation.X )
		{
			CurrentRoomLocation.X = CurrentRoomLocation.X - 1;
			GetCell( CurrentRoomLocation.X, CurrentRoomLocation.Y ).bIsRoom = true;
			Queue.Enqueue( CurrentRoomLocation );
		}
		else if ( CurrentRoomLocation.X < EndLocation.X )
		{
			CurrentRoomLocation.X = CurrentRoomLocation.X + 1;
			GetCell( CurrentRoomLocation.X, CurrentRoomLocation.Y ).bIsRoom = true;
			Queue.Enqueue( CurrentRoomLocation );
		}
		else if ( CurrentRoomLocation.Y > EndLocation.Y )
		{
			CurrentRoomLocation.Y = CurrentRoomLocation.Y - 1;
			GetCell( CurrentRoomLocation.X, CurrentRoomLocation.Y ).bIsRoom = true;
			Queue.Enqueue( CurrentRoomLocation );
		}
		else if ( CurrentRoomLocation.Y < EndLocation.Y )
		{
			CurrentRoomLocation.Y = CurrentRoomLocation.Y + 1;
			GetCell( CurrentRoomLocation.X, CurrentRoomLocation.Y ).bIsRoom = true;
			Queue.Enqueue( CurrentRoomLocation );
		}
		
	}
	
}


// Prints the grid
void AADungeonGenerator::PrintGrid() const
{
	for (int32 Y = Height - 1; Y >= 0; Y--)
	{
		FString Row = "";

		for (int32 X = 0; X < Width; X++)
		{
			const FRoomCell& Cell = Grid[X + Y * Width];

			if (Cell.bIsStart)
			{
				Row += "S ";
			}
			else if (Cell.bIsEnd)
			{
				Row += "E ";
			}
			else if (Cell.bIsRoom)
			{
				Row += "X ";
			}
			else
			{
				Row += ". ";
			}
		}

		UE_LOG(LogTemp, Warning, TEXT("%s"), *Row);
	}
}


// Generates the Dungeon
void AADungeonGenerator::GenerateDungeon()
{
	int32 RandomSeed{ FMath::RandRange(0, 10000) };
	RandomStream.Initialize( RandomSeed );
	
	InitializeGridLayout();
	GenerateStartEndPoints();
	GenerateCriticalPath();
	PrintGrid();
}
