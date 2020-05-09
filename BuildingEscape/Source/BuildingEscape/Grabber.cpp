// CopyRight Yijun Chen 2020

#include "Grabber.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"


#define OUT 

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandle();
	SetUpInputComp();
}

void UGrabber::FindPhysicsHandle(){
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if(PhysicsHandle==nullptr){
		UE_LOG(LogTemp, Error,TEXT("No Physics Handle Component Found on %s"), *GetOwner()->GetName());
	}
}

void UGrabber::SetUpInputComp(){
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if(!InputComponent){
		UE_LOG(LogTemp, Error,TEXT("No Input Component Found on %s"), *GetOwner()->GetName());
	}else{
		InputComponent-> BindAction("Grab",IE_Pressed, this, &UGrabber::Grab);
		InputComponent-> BindAction("Grab",IE_Released, this, &UGrabber::Release);
	}
}

void UGrabber::Grab(){
	//UE_LOG(LogTemp,Warning, TEXT("Key F Pressed"));
	FHitResult HitResult = GetFirstPhysicsBodyInReach();

	UPrimitiveComponent* ComponentGrab = HitResult.GetComponent();
	if(HitResult.GetActor()){
		if(!PhysicsHandle){
			return;
		}
		PhysicsHandle->GrabComponentAtLocation(
			ComponentGrab,
			NAME_None,
			GetTraceEnd()
		);
	}
}

void UGrabber::Release(){
	//UE_LOG(LogTemp,Warning, TEXT("Key F Released"));
	if(!PhysicsHandle){
			return;
		}
	PhysicsHandle->ReleaseComponent();

}

FVector UGrabber::GetTraceEnd() const{

	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);
	return PlayerViewPointLocation + PlayerViewPointRotation.Vector()*Reach;
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if(!PhysicsHandle){
			return;
		}
	if(PhysicsHandle->GrabbedComponent){
		PhysicsHandle->SetTargetLocation(GetTraceEnd());
	}
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const{
	//Get Player ViewPoint
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);
	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector()*Reach;
	//Logging the ViewPoint Every Tick(For Debugging)
	// UE_LOG(LogTemp, Warning,TEXT("The Vector is:%s,The Rotator is:%s"),
	// *PlayerViewPointLocation.ToString(), 
	// *PlayerViewPointRotation.ToString()
	// );

	//Draw a Line from the player showing the reach(For Debugging)
	// DrawDebugLine(
	// 	GetWorld(),PlayerViewPointLocation,LineTraceEnd,FColor(255,0,0),
	// 	false,
	// 	0.f,
	// 	0,
	// 	5.f
	// );

	//Ray Cast out to certain distance
	FHitResult Hit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType( OUT Hit,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);
	//AActor* ActorHit = Hit.GetActor();
	// if(ActorHit){
	// 	UE_LOG(LogTemp, Warning,TEXT("The Objecy is: %s"), *(ActorHit->GetName()));
	// }
	return Hit;
}

