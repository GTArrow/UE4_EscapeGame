// CopyRight Yijun Chen 2020


#include "OpenDoor.h"
#include "GameFrameWork/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Components/PrimitiveComponent.h"

#define OUT
// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	CurYaw = InitialYaw;
	TargetYaw=InitialYaw+ChangeYaw;
	FindPressurePlate();
	FindAudioComponent();
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if(TotalMassOfActor()>= MassToOpenDoor){	
		TargetYaw=InitialYaw+ChangeYaw;

		CloseDoorSound = false;
		if(!AudioComponent){return;}
		if(!OpenDoorSound){
			AudioComponent->Play();
			OpenDoorSound = true;
		}

		OperateDoor(DeltaTime,DoorOpenSpeed);
		DoorLastOpened = GetWorld()->GetTimeSeconds();
	}else if(GetWorld()->GetTimeSeconds()>=DoorLastOpened+DoorClosedDelay){
			
			OpenDoorSound = false;
			if(!AudioComponent){return;}
			if(!CloseDoorSound){
				AudioComponent->Play();
				CloseDoorSound = true;
			}
			TargetYaw=InitialYaw;
			OperateDoor(DeltaTime,DoorClosedSpeed);
	}
}

void UOpenDoor::OperateDoor(float DeltaTime, int32 Time){
	//UE_LOG(LogTemp,Warning, TEXT("%s"), *(GetOwner()->GetActorRotation().ToString()) );
	//UE_LOG(LogTemp,Warning, TEXT("Yaw: %f"),GetOwner()->GetActorRotation().Yaw);
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	CurYaw = FMath::FInterpTo(CurYaw,TargetYaw,DeltaTime,Time);
	DoorRotation.Yaw = CurYaw;
	GetOwner()->SetActorRotation(DoorRotation);
	// float CurYaw = GetOwner()->GetActorRotation().Yaw;
	// FRotator NewRotator(0.f,0.f,0.f);
	// NewRotator.Yaw = FMath::FInterpTo(CurYaw,TargetYaw,DeltaTime,2);
	// GetOwner()->SetActorRotation(NewRotator);
}

float UOpenDoor::TotalMassOfActor() const{
	float TotalMass = 0.f;

	//Find All OverLapping Actors
	TArray<AActor*> OverlappingActors;
	if(!PressurePlate){
		return TotalMass;
	}
	PressurePlate -> GetOverlappingActors(OUT OverlappingActors);

	//Add up Actors's mass
	for(AActor* Actor: OverlappingActors){
		TotalMass += TotalMass+Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}
	return TotalMass;
}

void UOpenDoor::FindAudioComponent(){
	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();
	if(!AudioComponent){
		UE_LOG(LogTemp, Error, TEXT("%s Missing Audio Component"), *GetOwner()->GetName());
	}
}

void UOpenDoor::FindPressurePlate(){
	if(!PressurePlate){
		UE_LOG(LogTemp, Error, TEXT("%s has oppen door Component on it. But the PressurePlate is not Set!!"),
		*GetOwner()->GetName());
	}
}
