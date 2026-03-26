
#pragma once


#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Containers/Queue.h"
#include "ADungeonGenerator.generated.h"



USTRUCT()
struct FRoomCell
{
	GENERATED_BODY()
	
	bool bIsRoom = false;
	bool bIsStart = false;
	bool bIsEnd = false;
	bool bIsVisited = false;
};


UCLASS()
class PROCEDURALDUNGEONGEN_API AADungeonGenerator : public AActor
{
	GENERATED_BODY()

public:
	AADungeonGenerator();

protected:
	virtual void BeginPlay() override;
	
	int32 Seed{ 0 };
	
	UPROPERTY()
	TArray<FRoomCell> Grid;

	TQueue<FIntPoint> Queue;
	
	UPROPERTY(EditAnywhere, Category = "Grid Size")
	int32 Width{ 40 };
	UPROPERTY(EditAnywhere, Category = "Grid Size")
	int32 Height{ 40 };
	
	UFUNCTION(BlueprintCallable)
	void GenerateDungeon();
	
private:
	void InitializeGridLayout();
	bool IsInsideGrid( int32 X, int32 Y ) const;
	void GenerateCriticalPath();
	void GenerateStartEndPoints();
	FIntPoint GetRandomPoint();
	void PrintGrid() const;
	
	FRoomCell& GetCell( int32 X, int32 Y );
	
	FIntPoint StartPoint;
	FIntPoint EndPoint;
	FIntPoint CurrentRoomLocation;
	FRandomStream RandomStream;
};
