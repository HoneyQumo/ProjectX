// Fill out your copyright notice in the Description page of Project Settings.


#include "PlatformTrigger.h"

void APlatformTrigger::Invoke()
{
	OnInvocatorActivated.Broadcast();
}
