
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


// Create a path from the START to the END
void AADungeonGenerator::GenerateCriticalPath()
{
	FIntPoint StartLocation{ StartPoint.X, StartPoint.Y };
	FIntPoint EndLocation{ EndPoint.X, EndPoint.Y };
	CurrentRoomLocation = StartLocation;
	char Direction;

	while ( CurrentRoomLocation != EndLocation )
	{
		if ( CriticalPathRandomnessRate < (RandomStream.RandRange(0, 100)) )
		{
			Direction = FigureOutRoomGenerationDirection( CurrentRoomLocation, EndLocation );
		}
		else
		{
			TArray PossibleRandomMoves{ 'N', 'E', 'S', 'W' };
			Direction = PossibleRandomMoves[ RandomStream.RandRange(0, 3) ];
		}
		
		switch ( Direction )
		{
			case 'N':
				Destination.Y = CurrentRoomLocation.Y + 1;
				Destination.X = CurrentRoomLocation.X;
				break;
			
			case 'E':
				Destination.X = CurrentRoomLocation.X + 1;
				Destination.Y = CurrentRoomLocation.Y;
				break;
			
			case 'S':
				Destination.Y = CurrentRoomLocation.Y - 1;
				Destination.X = CurrentRoomLocation.X;
				break;
			
			case 'W':
				Destination.X = CurrentRoomLocation.X - 1;
				Destination.Y = CurrentRoomLocation.Y;
				break;
			
			default:
				UE_LOG( LogTemp, Warning, TEXT("Generating Critical Path Failed") );
				continue;
		}

		if ( IsInsideGrid( Destination.X, Destination.Y ) == false )
		{
			continue;
		}
		
		if ( IsDestinationARoom(Destination) == true && GetCell(Destination.X, Destination.Y).bIsEnd == false )
		{
			continue;
		}

		GetCell( Destination.X, Destination.Y ).bIsRoom = true;
		Queue.Enqueue( Destination );
		CurrentRoomLocation = Destination;
		
	}
	
}


// Figures out the room generation direction
char AADungeonGenerator::FigureOutRoomGenerationDirection( FIntPoint CurrentCell, FIntPoint EndLocation )
{
	TArray<char> PossibleMoves{};
	
	if ( CurrentCell.X > EndLocation.X )
	{
		PossibleMoves.Add( 'W' );
	}
	
	if ( CurrentCell.X < EndLocation.X )
	{
		PossibleMoves.Add( 'E' );
	}
	
	if ( CurrentCell.Y > EndLocation.Y )
	{
		PossibleMoves.Add( 'S' );
	}
	
	if ( CurrentCell.Y < EndLocation.Y )
	{
		PossibleMoves.Add( 'N' );
	}
	
	int32 MaxSize{ PossibleMoves.Num() };
	char ChosenMove{ PossibleMoves[ RandomStream.RandRange(0, MaxSize - 1) ] };
	
	return ChosenMove;
}


// Checks if the target room is an existing room
bool AADungeonGenerator::IsDestinationARoom( FIntPoint DestinationRoom )
{
	if ( GetCell( DestinationRoom.X, DestinationRoom.Y ).bIsRoom == true )
	{
		return true;
	}

	return false;
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
	OnDungeonGenerated();
}
