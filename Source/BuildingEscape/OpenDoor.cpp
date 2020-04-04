// Copyright Xuelong Mu 2020

#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	TargetYaw = GetOwner()->GetActorRotation().Yaw + DegreesToRotate;
	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s has an OpenDoor component but does NOT have a pressure plate set."), *GetOwner()->GetName());
	}

	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (PressurePlate && PressurePlate->IsOverlappingActor(ActorThatOpens))
	{
		OpenDoor(DeltaTime);
		DoorLastOpened = GetWorld()->GetTimeSeconds();
		//DoorLastOpened is when the door was opened
	}
	else
	{
		if (GetWorld()->GetTimeSeconds() > DoorLastOpened + DoorCloseDelay)
			CloseDoor(DeltaTime);
	}
}

void UOpenDoor::OpenDoor(float DeltaTime)
{
	FRotator CurrentRotation = GetOwner()->GetActorRotation();
	UE_LOG(LogTemp, Warning, TEXT("Yaw position: %f"), CurrentRotation.Yaw);
	CurrentRotation.Yaw = FMath::Lerp(CurrentRotation.Yaw, TargetYaw, DeltaTime * 5.f);
	GetOwner()->SetActorRotation(CurrentRotation);

	// CurrentRotation.Yaw = FMath::Lerp(CurrentRotation.Yaw, TargetYaw, DeltaTime * 12.f);
	// UE_LOG(LogTemp, Warning, TEXT("DeltaTime: %f"), DeltaTime);
	// UE_LOG(LogTemp, Warning, TEXT("DeltaTime multiplied: %f"), DeltaTime*12.f);
}

void UOpenDoor::CloseDoor(float DeltaTime)
{
	FRotator CurrentRotation = GetOwner()->GetActorRotation();
	CurrentRotation.Yaw = FMath::Lerp(CurrentRotation.Yaw, InitialYaw, DeltaTime * 5.f);
	GetOwner()->SetActorRotation(CurrentRotation);
}