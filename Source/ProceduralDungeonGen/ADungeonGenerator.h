
#pragma once


#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Containers/Queue.h"
#include "ADungeonGenerator.generated.h"



USTRUCT( BlueprintType )
struct FRoomCell
{
	GENERATED_BODY()

	UPROPERTY( BlueprintReadOnly )
	bool bIsRoom = false;
	UPROPERTY( BlueprintReadOnly )
	bool bIsStart = false;
	UPROPERTY( BlueprintReadOnly )
	bool bIsEnd = false;
	UPROPERTY( BlueprintReadOnly )
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
	
	UPROPERTY( BlueprintReadOnly )
	TArray<FRoomCell> Grid;
	
	TQueue<FIntPoint> Queue;
	
	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Grid Size" )
	int32 Width{ 10 };
	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Grid Size" )
	int32 Height{ 10 };
	
	UFUNCTION( BlueprintCallable )
	void GenerateDungeon();

protected:
	UPROPERTY( BlueprintReadOnly )
	FIntPoint StartPoint;
	UPROPERTY( BlueprintReadOnly )
	FIntPoint EndPoint;
	UPROPERTY( BlueprintReadOnly )
	FIntPoint CurrentRoomLocation;
	FRandomStream RandomStream;

	UFUNCTION( BlueprintImplementableEvent )
	void OnDungeonGenerated();
	
private:
	void InitializeGridLayout();
	bool IsInsideGrid( int32 X, int32 Y ) const;
	void GenerateCriticalPath();
	void GenerateStartEndPoints();
	FIntPoint GetRandomPoint();
	void PrintGrid() const;

	UFUNCTION( BlueprintCallable )
	FRoomCell& GetCell( int32 X, int32 Y );
};
