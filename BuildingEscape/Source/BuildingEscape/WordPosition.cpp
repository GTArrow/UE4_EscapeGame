// Fill out your copyright notice in the Description page of Project Settings.


#include "WordPosition.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UWordPosition::UWordPosition()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UWordPosition::BeginPlay()
{
	Super::BeginPlay();

	//FString Log = TEXT("Hello");
	
	//FString* PrtLog = &Log; //Creat a Pointer
	//(*PrtLog).Len();
	// PrtLog->Len() ;
	// UE_LOG(LogTemp,Warning, TEXT("%s"), **PrtLog);

	// UE_LOG(LogTemp,Warning, TEXT("%s"), *Log);
	//UE_LOG(LogTemp,Display, TEXT("This is a message"));
	//UE_LOG(LogTemp,Error, TEXT("This is an Error"));

	FString ObjectName =  GetOwner()->GetName();
	FString ObjectPosition = GetOwner()-> GetActorLocation().ToString();
	UE_LOG(LogTemp,Warning, TEXT("Object Name is :%s, Location is: %s"), *ObjectName, *ObjectPosition);
}


// Called every frame
void UWordPosition::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

