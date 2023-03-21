// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemy.h"

#include "EggTowerPlanRuntime/Component/HealthComponent.h"

// Sets default values
ABaseEnemy::ABaseEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("MeshRootComp"));
	RootComponent = MeshRootComponent;
	
	ST_MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ST_MeshComp"));
	ST_MeshComp->SetupAttachment(MeshRootComponent);
	ST_MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ST_MeshComp->SetCollisionProfileName(FName("AttackerMesh"));
	ST_MeshComp->bCastDynamicShadow = false;
	ST_MeshComp->bCastStaticShadow = false;
	
	SK_MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SK_MeshComp"));
	SK_MeshComp->SetupAttachment(MeshRootComponent);
	SK_MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SK_MeshComp->SetCollisionProfileName(FName("AttackerMesh"));
	SK_MeshComp->bCastDynamicShadow = false;
	SK_MeshComp->bCastStaticShadow = false;
	
	WidgetComp_Cooling = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComp_Cooling"));
	WidgetComp_Cooling->SetupAttachment(RootComponent);

	ArrowComponent =  CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComp"));
	ArrowComponent->SetupAttachment(MeshRootComponent);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComp"));
}

// Called when the game starts or when spawned
void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseEnemy::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

// Called every frame
void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

