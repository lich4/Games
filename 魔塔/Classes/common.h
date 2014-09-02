
#pragma once

#include "cocos2d.h"
#include <iostream>
USING_NS_CC;

#ifndef __HELLOWORLD_MANAGER_H__
#define __HELLOWORLD_MANAGER_H__

#define blocksize 32

enum direction
{
	kDown=0,
	kLeft=1,
	kRight=2,
	kUp=3,
};

enum category
{
	TYPE_FLOOR,
	TYPE_ITEM,
	TYPE_ENEMY,
	TYPE_DOOR,
};

//对应于tileid
enum floor//floor层对象
{
	FLOOR_BEGIN=0,
	FLOOR_WALL=FLOOR_BEGIN+7,//墙体
	FLOOR_FLOOR=FLOOR_BEGIN+12,//地板
};

enum item//item层对象
{
	ITEM_BEGIN=72,
	ITEM_REDDIAMOND=ITEM_BEGIN+1,//红宝石
	ITEM_BLUEDIAMOND=ITEM_BEGIN+2,//蓝宝石
	ITEM_REDBLOOD=ITEM_BEGIN+5,//红血瓶
	ITEM_BLUEBLOOD=ITEM_BEGIN+6,//蓝血瓶
	ITEM_YELLOWKEY=ITEM_BEGIN+17,//黄钥匙
	ITEM_BLUEKEY=ITEM_BEGIN+18,//蓝钥匙
	ITEM_REDKEY=ITEM_BEGIN+19,//红钥匙
};

enum enemy
{
	ENEMY_BEGIN=136,
	ENEMY_KULOUREN=ENEMY_BEGIN+1,//骷髅人
	ENEMY_KULOUSHIBING=ENEMY_BEGIN+5,//骷髅士兵
	ENEMY_XIAOBIANFU=ENEMY_BEGIN+17,//小蝙蝠
	ENEMY_LVTOUGUAI=ENEMY_BEGIN+33,//绿头怪
	ENEMY_HONGTOUGUAI=ENEMY_BEGIN+37,//红头怪
	ENEMY_CHUJIFASHI=ENEMY_BEGIN+49,//初级法师
};

enum door
{
	DOOR_BEGIN=392,
	DOOR_YELLOW=DOOR_BEGIN+1,//黄门
	DOOR_BLUE=DOOR_BEGIN+2,//蓝门
	DOOR_RED=DOOR_BEGIN+3,//红门
	DOOR_GREEN=DOOR_BEGIN+4,//绿门
	DOOR_UPSTAIRS=DOOR_BEGIN+17,//上楼梯
	DOOR_DOWNSTAIRS=DOOR_BEGIN+18,//下楼梯
};

struct ENEMY
{
	int category;
	int type;
	std::string name;//名称
	std::string desc;//描述
	int life;
	int offense;//攻击力
	int defense;//防御力
	int money;//金钱
};

struct ITEM
{
	int category;
	int type;
	std::string name;//名称
	std::string desc;//描述
};

typedef std::unordered_map<int,ValueMap> ObjMap;

#define getNum(x) (sizeof(x)/(sizeof(x[0])))

class ObjManager
{//设置怪物动画
public:
	static ObjManager& getInstance();
	ValueMap* InitEnemyByType(int type);
	ValueMap* InitItemByType(int type);

private:
	ObjMap enemytemp;
	ObjMap itemtemp;
	std::vector<ValueMap*> enemies;
	std::vector<ValueMap*> items;

private:
	ObjManager();//单例模式
	virtual ~ObjManager();
};

struct Hero
{
	Sprite* heroSprite;
	std::string name;
	int life;
	int offense;
	int defense;
	int keyyellow;
	int keyblue;
	int keyred;
	int money;
	Vec2 nexttilepos;
};

struct Board
{
	TMXTiledMap* board;
	Label* maintitle;
	//hero
	Label* heroname;
	Label* life_hero;
	Label* offense_hero;
	Label* defense_hero;
	Label* money_hero;
	Label* yellowkey;
	Label* bluekey;
	Label* redkey;
	Label* weapon;
	Label* shield;
	//enemy
	Label* enemyname;
	Label* life_enemy;
	Label* offense_enemy;
	Label* defense_enemy;
};

#endif
