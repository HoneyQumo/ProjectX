// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectXGameInstance.h"

bool UProjectXGameInstance::ProcessConsoleExec(const TCHAR* Cmd, FOutputDevice& Ar, UObject* Executor)
{
	bool bResult = Super::ProcessConsoleExec(Cmd, Ar, Executor);

	if (!bResult)
	{
		TArray<UGameInstanceSubsystem*> Subsystems = GetSubsystemArrayCopy<UGameInstanceSubsystem>();
		for (UGameInstanceSubsystem* Subsystem : Subsystems)
		{
			bResult |= Subsystem->ProcessConsoleExec(Cmd, Ar, Executor);
		}
	}

	return bResult;
}
