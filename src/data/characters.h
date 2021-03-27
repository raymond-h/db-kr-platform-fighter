#ifndef DBKRPLATFORMFIGHTER_CHARACTERS_H
#define DBKRPLATFORMFIGHTER_CHARACTERS_H

#include "characters/experiment.h"

#include <map>

namespace Data
{
	enum class CharacterId
	{
		Experiment
	};

	std::map<CharacterId, Character> getCharacterData();
}

#endif //DBKRPLATFORMFIGHTER_CHARACTERS_H
