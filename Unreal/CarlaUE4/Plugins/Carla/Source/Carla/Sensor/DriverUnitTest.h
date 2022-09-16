// Copyright (c) 2022 Connected Motorcycle Consortium (CMC).
//
// This work is licensed under the terms of the MIT license.
// For a copy, see <https://opensource.org/licenses/MIT>.

#pragma once

#include "Carla/Sensor/Sensor.h"

#include "Carla/Actor/ActorDefinition.h"
#include "Carla/Actor/ActorDescription.h"

#include <compiler/disable-ue4-macros.h>
#include <compiler/enable-ue4-macros.h>

#include "DriverUnitTest.generated.h"


UCLASS()
class CARLA_API ADriverUnitTest : public ASensor
{
    GENERATED_BODY()

public:
    ADriverUnitTest(const FObjectInitializer &ObjectInitializer);

    static FActorDefinition GetSensorDefinition();

    void Set(const FActorDescription &FActorDescription) override;

    virtual void PostPhysTick(UWorld *World, ELevelTick TickType, float DeltaTime) override;

    void SetOwner(AActor *Owner) override;

    float ComputeReactionTime();

    float ComputeBraking();

    float Rmax;

    float Rmin;
    
    bool reaction_random;
    
    float reaction_fix;

    bool braking_random;

    float braking_fix;
    
  

   
protected:

  void BeginPlay() override;

    


};