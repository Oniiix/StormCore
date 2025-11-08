


#include "QuestBox.h"
#include "RobotHunter/Utils/DebugUtils.h"
#include "RobotHunter/Utils/FileLogger.h"

void AQuestBox::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	if (questManager)
	{
		if(questToInit)
			questManager->AddQuest(questToInit);
		if(questToValid)
			questManager->ValidQuest(questToValid);

	}
}
