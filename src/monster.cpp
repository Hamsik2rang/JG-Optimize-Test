#include "monster.h"
#include <unordered_map>

Player* s_player = new Player();

static int SPAWN_CYCLE = 5;
static int DAMAGE_CYCLE = 3;
static int COLLIDE_CYCLE = 2;
static int HEAL_CYCLE = 7;

static int spawnIndex;
static int damageIndex;
static int collideIndex;
static int healIndex;

Material::Material()
{
	shader = (void*)Random::NextUInt64();
	shaderParameter = (void*)Random::NextUInt64();

	hash = HashCombine(PointerHash(shader), PointerHash(shaderParameter));
}

Mesh::Mesh()
{
	position = (Vector*)Random::NextUInt64();
	color = (Vector*)Random::NextUInt64();
	texcoord = (Vector*)Random::NextUInt64();
	normal = (Vector*)Random::NextUInt64();
	tangent = (Vector*)Random::NextUInt64();
	bitangent = (Vector*)Random::NextUInt64();
	boneWeights = (Vector*)Random::NextUInt64();
	boneIndices = (Vector*)Random::NextUInt64();

	hash = HashCombine(PointerHash(position), PointerHash(color));
	hash = HashCombine(hash, PointerHash(texcoord));
	hash = HashCombine(hash, PointerHash(normal));
	hash = HashCombine(hash, PointerHash(tangent));
	hash = HashCombine(hash, PointerHash(bitangent));
	hash = HashCombine(hash, PointerHash(boneWeights));
	hash = HashCombine(hash, PointerHash(boneIndices));
}

Texture::Texture()
{
	image = (void*)Random::NextUInt64();
	type = Random::NextInt32(0, 3);
	format = Random::NextInt32(0, 4);
	isCompressed = Random::NextBool();
	width = Random::NextUInt32(64, 4096);
	height = Random::NextUInt32(64, 4096);
	depth = Random::NextUInt32(1, 6);

	hash = HashCombine(PointerHash(image), (uint32_t)type);
	hash = HashCombine(hash, (uint32_t)format);
	hash = HashCombine(hash, (uint32_t)isCompressed);
	hash = HashCombine(hash, width);
	hash = HashCombine(hash, height);
	hash = HashCombine(hash, depth);
}

Monster::Monster()
	: mesh(new Mesh())
	, material(new Material())
	, texture(new Texture())
	, name("DefaultMonster")
{

}

Monster::Monster
(
	const std::string& name,
	Monster::Type type,
	Monster::Immune immune,
	Mesh* mesh,
	Material* material,
	Texture* texture
)
	: name(name)
	, type(type)
	, immune(immune)
	, mesh(mesh ? mesh : new Mesh())
	, material(material ? material : new Material())
	, texture(texture ? texture : new Texture())
{
	hash = HashCombine((uint32_t)type, (uint32_t)immune);
	hash = HashCombine(hash, mesh->hash);
	hash = HashCombine(hash, material->hash);
	hash = HashCombine(hash, texture->hash);
}

Monster::~Monster()
{
	if (mesh) delete mesh;
	mesh = nullptr;
	if (material) delete material;
	material = nullptr;
	if (texture) delete texture;
	texture = nullptr;
}

int update_monsters(Monster** monsters, size_t size, float deltaTime)
{
	for (size_t i = 0; i < size; i++)
	{
		Monster* monster = monsters[i];

		// 5% 확률로 몬스터가 부활합니다.
		if (!monster->isActive && 0 < Random::NextInt32(-15, 5))
		{
			monsters[i]->isActive = true;
			monsters[i]->hp = 100.0f;
			continue;
		}

		if (is_collided(s_player, monster))
		{
			process_collision(monster);
		}

		// 체력이 0 이하라면 몬스터는 죽습니다.
		if (monster->hp < 0.0f)
		{
			monster->isActive = false;
			continue;
		}

		if (is_player_in_range(s_player, monster))
		{
			//...

			// 플레이어를 공격할 때 공격력의 5%만큼 체력을 회복합니다.
			if (Random::NextBool())
			{
				monster->hp += monster->atk * 0.05f;
			}
		}
		//...
	}
	return 4;
}


int update_monsters_allocate(Monster** monsters, size_t size, float deltaTime)
{
	reset_index();
	for (size_t i = 0; i < size; i++)
	{
		Monster* monster = monsters[i];

		if (monster == nullptr)
		{
			if (spawnIndex)
			{
				monsters[i] = new Monster();
			}
			continue;
		}

		if (is_collided(s_player, monster))
		{
			process_collision(monster);
		}

		// 체력이 0 이하라면 몬스터는 죽습니다.
		if (monster->hp < 0.0f)
		{
			delete monster;
			monsters[i] = nullptr;
			continue;
		}

		if (is_player_in_range(s_player, monster))
		{
			//...

			// 플레이어를 공격할 때 공격력의 5%만큼 체력을 회복합니다.
			if (healIndex)
			{
				monster->hp += monster->atk * 0.05f;
			}
		}
		//...
		update_index();
	}
	return 4;
}

int update_monsters_pooling(Monster** monsters, size_t size, float deltaTime)
{
	reset_index();
	for (size_t i = 0; i < size; i++)
	{
		Monster* monster = monsters[i];
		if (monster == nullptr) continue;
		// 20% 확률로 몬스터가 부활합니다.
		if (!monster->isActive && 0 < spawnIndex)
		{
			monster->isActive = true;
			monster->hp = 100.0f;
			continue;
		}

		if (is_collided(s_player, monster))
		{
			process_collision(monster);
		}

		// 체력이 0 이하라면 몬스터는 죽습니다.
		if (monster->hp < 0.0f)
		{
			monster->isActive = false;
			continue;
		}

		if (is_player_in_range(s_player, monster))
		{
			//...

			// 플레이어를 공격할 때 공격력의 5%만큼 체력을 회복합니다.
			if (Random::NextBool())
			{
				monster->hp += monster->atk * 0.05f;
			}
		}
		//...
		update_index();
	}
	return 4;
}

bool is_collided(Player* player, Monster* monster)
{
	return  collideIndex == 0;

}

bool process_collision(Monster* monster)
{
	if (damageIndex)
	{
		monster->hp -= Random::NextFloat(0.0f, 100.0f); // Example damage
	}
	return true;
}

bool is_player_in_range(const Player* player, const Monster* monster)
{
	float distance =
		std::pow(player->position.data[0] - monster->position.data[0], 2) +
		std::pow(player->position.data[1] - monster->position.data[1], 2) +
		std::pow(player->position.data[2] - monster->position.data[2], 2);

	return distance <= (monster->atkRange * monster->atkRange);
}

void update_index()
{
	spawnIndex = (spawnIndex + 1) % SPAWN_CYCLE;
	damageIndex = (damageIndex + 1) % DAMAGE_CYCLE;
	collideIndex = (collideIndex + 1) % COLLIDE_CYCLE;
	healIndex = (healIndex + 1) % HEAL_CYCLE;
}

void reset_index()
{
	spawnIndex = 0;
	damageIndex = 0;
	collideIndex = 0;
	healIndex = 0;
}

void initialize_monsters(Monster** monsters, size_t size)
{
	for (size_t i = 0; i < size; i++)
	{
		if (monsters[i] == nullptr)
		{
			monsters[i] = new Monster();
		}
		monsters[i]->isActive = true;
		monsters[i]->hp = 100.0f;
		monsters[i]->atk = 20.0f;
	}
}

void release_monsters(Monster** monsters, size_t size)
{
	if (monsters)
	{
		for (size_t i = 0; i < size; i++)
		{
			if (monsters[i])
			{
				delete monsters[i];
				monsters[i] = nullptr;
			}
		}
	}
}

Monster* find_monster_by_type(Monster** monsters, size_t size, Monster::Type monsterType)
{
	for (size_t i = 0; i < size; i++)
	{
		if (monsters[i] == nullptr || monsters[i]->isActive) continue;

		if (monsters[i]->type == monsterType)
		{
			return monsters[i];
		}
	}

	return nullptr;
}

Monster* find_monster_by_attributes(Monster** monsters, size_t size, const Monster* monsterAttr)
{
	for (size_t i = 0; i < size; i++)
	{
		if (monsters[i] == nullptr || monsters[i]->isActive) continue;

		if (monsters[i]->type == monsterAttr->type &&
			monsters[i]->immune == monsterAttr->immune &&
			monsters[i]->material == monsterAttr->material &&
			monsters[i]->texture == monsterAttr->texture &&
			monsters[i]->mesh == monsterAttr->mesh)
		{
			return monsters[i];
		}
	}

	return nullptr;
}

Monster* find_monster_by_hash(Monster** monsters, size_t size, const Monster* monsterHash)
{
	for (size_t i = 0; i < size; i++)
	{
		if (monsters[i] == nullptr || monsters[i]->isActive) continue;

		if (monsters[i]->hash == monsterHash->hash)
		{
			return monsters[i];
		}
	}
	return nullptr;
}


int count_monster_by_attributes(Monster** monsters, size_t size, const Monster* target)
{
	int count = 0;
	for (size_t i = 0; i < size; i++)
	{
		if (monsters[i] == nullptr || monsters[i]->isActive) continue;

		if (monsters[i]->type == target->type &&
			monsters[i]->immune == target->immune &&
			*(monsters[i]->material) == *(target->material) &&
			*(monsters[i]->texture) == *(target->texture) &&
			*(monsters[i]->mesh) == *(target->mesh))
		{
			count++;
		}
	}

	return count;
}

int count_monster_by_hash(Monster** monsters, size_t size, const Monster* target)
{
	int count = 0;
	for (size_t i = 0; i < size; i++)
	{
		if (monsters[i] == nullptr || monsters[i]->isActive) continue;

		if (monsters[i]->hash == target->hash)
		{
			count++;
		}
	}
	return count;
}

int classify_monster_by_attributes(Monster** monsters, size_t size, 
	std::vector<std::vector<Monster*>>& bucket)
{
	for (size_t i = 0; i < size; i++)
	{
		Monster* iMonster = monsters[i];
		if (iMonster == nullptr || iMonster->isActive) continue;

		// 새로운 버킷을 생성하고 iMonster를 추가
		bucket.push_back({});
		bucket.back().push_back(iMonster);

		for (size_t j = i + 1; j < size; j++)
		{
			Monster* jMonster = monsters[j];
			if (jMonster == nullptr || jMonster->isActive) continue;

			// iMonster와 jMonster의 속성이 동일한지 확인
			if (iMonster->type == jMonster->type &&
				iMonster->immune == jMonster->immune &&
				*(iMonster->material) == *(jMonster->material) &&
				*(iMonster->texture) == *(jMonster->texture) &&
				*(iMonster->mesh) == *(jMonster->mesh))
			{
				// 동일한 속성을 가진 몬스터를 같은 버킷에 추가
				bucket.back().push_back(jMonster);
			}
		}
	}
	return 6;
}

int classify_monster_by_hash(Monster** monsters, size_t size, 
	std::vector<std::vector<Monster*>>& bucket)
{
	// Hash table
	std::unordered_map<uint32_t, std::vector<Monster*>> hashBucket;
	for (size_t i = 0; i < size; i++)
	{
		Monster* monster = monsters[i];
		if (monster == nullptr || monster->isActive) continue;

		// 해시 테이블에 몬스터의 해시 버킷이 존재하지 않으면 추가
		if (!hashBucket.contains(monster->hash))
		{
			hashBucket.insert(std::make_pair(monster->hash, 
				std::vector<Monster*>{}));
		}
		// 해시 키에 매핑된 버킷에 바로 삽입
		hashBucket[monster->hash].push_back(monster);
	}

	for (auto& b : hashBucket)
	{
		// 각 해시 버킷으로 사용한 컨테이너를 그대로 '이동'
		bucket.emplace_back(std::move(b.second));
	}
	return 6;
}