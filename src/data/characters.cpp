#include "characters.h"

namespace Data
{
	std::map<CharacterId, Character> getCharacterData()
	{
		return {
			{CharacterId::Experiment, Characters::getExperimentCharacterData()}
		};
	}
}
