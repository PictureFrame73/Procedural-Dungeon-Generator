
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
void AADungeonGenerator::GenerateStartEndPoints( int32 Seed )
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
	FIntPoint GridLocation( Width, Height );
	
	
	
}


// Generates the Dungeon
void AADungeonGenerator::GenerateDungeon()
{
	int32 RandomSeed{ FMath::RandRange(0, 100) };
	RandomStream.Initialize(RandomSeed);
	
	InitializeGridLayout();
	GenerateStartEndPoints( RandomSeed );
	GenerateCriticalPath();
	
}
