//
//  common.h
//  JungleOptimizeTest
//
//  Created by Yongsik Im on 7/19/25.
//
#pragma once
#ifndef __MONSTER_H__
#define __MONSTER_H__

#include "Predefine.h"
#include "common.h"

struct Hashable
{
	virtual uint32_t GetHash() const { return 0; }
	
	uint32_t hash;
};

struct Material : public Hashable
{
	Material();

	void* shader;
	void* shaderParameter;

	//...
	bool operator==(const Material& rhs) const
	{
		return hash == rhs.hash;
	}

	bool operator!=(const Material& rhs) const
	{
		return !(*this == rhs);
	}
};

struct Mesh : public Hashable
{
	Mesh();

	Vector* position;
	Vector* color;
	Vector* texcoord;
	Vector* normal;
	Vector* tangent;
	Vector* bitangent;
	Vector* boneWeights;
	Vector* boneIndices;

	bool operator==(const Mesh& rhs) const
	{
		return hash == rhs.hash;
	}

	bool operator!=(const Mesh& rhs) const
	{
		return !(*this == rhs);
	}
};

struct Texture : public Hashable
{
	Texture();

	void* image;
	int type;
	int format;
	bool isCompressed;
	uint32_t width;
	uint32_t height;
	uint32_t depth;
	uint32_t mipLevel;

	//...
	bool operator==(const Texture& rhs) const
	{
		return hash == rhs.hash;
	}

	bool operator!=(const Texture& rhs) const
	{
		return !(*this == rhs);
	}
};

struct Monster : public Hashable
{
	enum class Type
	{
		None = 0,
		Orc,
		Goblin,
		Troll,
		Dragon,
		Elf,
		Count
	}type;

	enum class Immune
	{
		None = 0,
		Fire,
		Ice,
		Poison,
		Electric,
		Water,
		Count
	}immune;

	Monster();
	Monster(const std::string& name, Monster::Type type, Monster::Immune immune, Mesh* mesh, Material* material, Texture* texture);
	~Monster(); 

	std::string name;

	Mesh* mesh;
	Material* material;
	Texture* texture;

	float atk = 20.0f;
	float def;
	float hp = 100.0f;
	float atkRange;

	Vector front;
	Vector position;
	Vector rotation;
	Vector scale;

	bool isActive;
};

struct Player
{
	//...
	Vector position;
	Vector rotation;
	Vector scale;
};

#pragma region >>> Global function
int update_monsters(Monster** monsters, size_t size, float deltaTime = 0.05f);
int update_monsters_allocate(Monster** monsters, size_t size, float deltaTime = 0.05f);
int update_monsters_pooling(Monster** monsters, size_t size, float deltaTime = 0.05f);
bool is_collided(Player* player, Monster* monster);
bool process_collision(Monster* monster);
bool is_player_in_range(const Player* player, const Monster* monster);

void update_index();
void reset_index();

void initialize_monsters(Monster** monsters, size_t size);
void release_monsters(Monster** monsters, size_t size);

Monster* find_monster_by_type(Monster** monsters, size_t size, Monster::Type monsterType);
Monster* find_monster_by_attributes(Monster** monsters, size_t size, const Monster* monsterAttr);
Monster* find_monster_by_hash(Monster** monsters, size_t size, const Monster* monsterHash);

int count_monster_by_attributes(Monster** monsters, size_t size, const Monster* target);
int count_monster_by_hash(Monster** monsters, size_t size, const Monster* target);

int classify_monster_by_attributes(Monster** monsters, size_t size, std::vector<std::vector<Monster*>>& bucket);
int classify_monster_by_hash(Monster** monsters, size_t size, std::vector<std::vector<Monster*>>& bucket);
#pragma endregion

#endif