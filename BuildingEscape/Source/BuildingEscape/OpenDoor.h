// CopyRight Yijun Chen 2020

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "Components/AudioComponent.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void OperateDoor(float DeltaTime, int32 Time);

	float TotalMassOfActor() const;
	void FindAudioComponent();
	void FindPressurePlate();

	bool OpenDoorSound = false;
	bool CloseDoorSound = true;

private:
	UPROPERTY(EditAnywhere)
	float MassToOpenDoor=50.f;	
	UPROPERTY(EditAnywhere)
	float ChangeYaw=90.f;
	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate = nullptr;

	UPROPERTY(EditAnywhere)
	float DoorClosedDelay = 1.f;
	UPROPERTY(EditAnywhere)
	float DoorOpenSpeed = 2.f;
	UPROPERTY(EditAnywhere)
	float DoorClosedSpeed= 4.f;

	UPROPERTY()
	UAudioComponent* AudioComponent = nullptr;

	float InitialYaw;
	float CurYaw;
	float TargetYaw;

	float DoorLastOpened=0.f;
		
};
