// 


#include "CategoryQuestTemplate.h"

#include "RobotHunter/QuestSystem/Asset/QuestAsset.h"


void UCategoryQuestTemplate::InitializeCategory(UVerticalBox* _VB,
                                                const UQuestAsset* _QuestAsset, UQuestATH* QuestAth)
{
	QuestAsset = _QuestAsset;
	VB = _VB;
	questATH = QuestAth;

	onInitQuest.Broadcast(QuestAth);
}
