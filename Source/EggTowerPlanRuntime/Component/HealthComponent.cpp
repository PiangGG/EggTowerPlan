// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"


// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UHealthComponent::InitializeComponent()
{
	Super::InitializeComponent();
	
	if (GetOwner())
	{
		GetOwner()->OnTakeAnyDamage.AddDynamic(this,&UHealthComponent::OnOwnerTakeAnyDamge);
	}
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


void UHealthComponent::OnOwnerTakeAnyDamge(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* InstigatedBy, AActor* DamageCauser)
{
	if (CurrentHealth-Damage<=0)
	{
		GetOwner()->Destroy();
	}
	else
	{
		CurrentHealth-=Damage;
		CurrentHealth = FMath::Clamp(CurrentHealth,0.0f,MaxHealth);
	}
}

bool UHealthComponent::ApplyChangeHealth()
{
	//CurrentHealth = FMath::Clamp()
	return true;
}

