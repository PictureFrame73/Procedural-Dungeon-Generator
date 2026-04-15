
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

	while ( CurrentRoomLocation != EndLocation )
	{
		if ( CriticalPathRandomnessRate < (RandomStream.RandRange(0, 100)) )
		{
			switch ( FigureOutRoomGenerationDirection( CurrentRoomLocation, EndLocation ) )
			{
			case 'N':
				CurrentRoomLocation.Y = CurrentRoomLocation.Y + 1;
				GetCell( CurrentRoomLocation.X, CurrentRoomLocation.Y ).bIsRoom = true;
				Queue.Enqueue( CurrentRoomLocation );
			
				break;
			case 'E':
				CurrentRoomLocation.X = CurrentRoomLocation.X + 1;
				GetCell( CurrentRoomLocation.X, CurrentRoomLocation.Y ).bIsRoom = true;
				Queue.Enqueue( CurrentRoomLocation );
			
				break;
			case 'S':
				CurrentRoomLocation.Y = CurrentRoomLocation.Y - 1;
				GetCell( CurrentRoomLocation.X, CurrentRoomLocation.Y ).bIsRoom = true;
				Queue.Enqueue( CurrentRoomLocation );
			
				break;
			case 'W':
				CurrentRoomLocation.X = CurrentRoomLocation.X - 1;
				GetCell( CurrentRoomLocation.X, CurrentRoomLocation.Y ).bIsRoom = true;
				Queue.Enqueue( CurrentRoomLocation );
			
				break;
			
			default:
				UE_LOG(LogTemp, Warning, TEXT( "Generating Critical Path Failed" ));
				
			}
			
		}
		else if ( CriticalPathRandomnessRate > (RandomStream.RandRange(0, 100)) )
		{
			TArray PossibleRandomMoves{ 'N', 'E', 'S', 'W' };
			char ChosenDirection{ PossibleRandomMoves[ RandomStream.RandRange(0, 3) ] };
			
			if ( CurrentRoomLocation.X < Width - 1 && CurrentRoomLocation.Y < Height - 1 )
			{
				switch ( ChosenDirection )
				{
				case 'N':
					CurrentRoomLocation.Y = CurrentRoomLocation.Y + 1;
					GetCell( CurrentRoomLocation.X, CurrentRoomLocation.Y ).bIsRoom = true;
					Queue.Enqueue( CurrentRoomLocation );
			
					break;
				case 'E':
					CurrentRoomLocation.X = CurrentRoomLocation.X + 1;
					GetCell( CurrentRoomLocation.X, CurrentRoomLocation.Y ).bIsRoom = true;
					Queue.Enqueue( CurrentRoomLocation );
			
					break;
				case 'S':
					CurrentRoomLocation.Y = CurrentRoomLocation.Y - 1;
					GetCell( CurrentRoomLocation.X, CurrentRoomLocation.Y ).bIsRoom = true;
					Queue.Enqueue( CurrentRoomLocation );
			
					break;
				case 'W':
					CurrentRoomLocation.X = CurrentRoomLocation.X - 1;
					GetCell( CurrentRoomLocation.X, CurrentRoomLocation.Y ).bIsRoom = true;
					Queue.Enqueue( CurrentRoomLocation );
			
					break;
			
				default:
					UE_LOG(LogTemp, Warning, TEXT( "Generating Critical Path Failed" ));
			
				}
				
			}
			
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT( "Generating Critical Path Failed" ));
		}
		
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
