
#include "common.h"

#include "cocos2d.h"
#include <iostream>
USING_NS_CC;

struct ENEMY globalEnemyData[]=
{
	{TYPE_ENEMY,ENEMY_KULOUREN,"骷髅人","骷髅人",110,25,5,110},
	{TYPE_ENEMY,ENEMY_KULOUSHIBING,"骷髅士兵","骷髅士兵",150,40,20,150},
	{TYPE_ENEMY,ENEMY_XIAOBIANFU,"小蝙蝠","小蝙蝠",100,20,5,100},
	{TYPE_ENEMY,ENEMY_LVTOUGUAI,"绿头怪","绿头怪",50,20,1,50},
	{TYPE_ENEMY,ENEMY_HONGTOUGUAI,"红头怪","红头怪",70,15,2,70},
	{TYPE_ENEMY,ENEMY_CHUJIFASHI,"初级法师","初级法师",125,50,25,125},
};

struct ITEM globalItemData[]=
{
	{TYPE_ITEM,ITEM_REDDIAMOND,"红宝石","增加攻击力"},
	{TYPE_ITEM,ITEM_BLUEDIAMOND,"蓝宝石","增加防御力"},
	{TYPE_ITEM,ITEM_REDBLOOD,"红血瓶","增加大量生命"},
	{TYPE_ITEM,ITEM_BLUEBLOOD,"蓝血瓶","增加少量生命"},
	{TYPE_ITEM,ITEM_YELLOWKEY,"黄钥匙","开启黄色门"},
	{TYPE_ITEM,ITEM_BLUEKEY,"蓝钥匙","开启蓝色门"},
	{TYPE_ITEM,ITEM_REDKEY,"红钥匙","开启红色门"},
};

ObjManager& ObjManager::getInstance()
{
	static ObjManager manager;
	return manager;
}

ValueMap* ObjManager::InitEnemyByType(int type)
{
	ValueMap* curmap=new ValueMap(enemytemp[type]);
	enemies.push_back(curmap);
	return curmap;
}

ValueMap* ObjManager::InitItemByType(int type)
{
	return &itemtemp[type];
}

ObjManager::ObjManager()
{
	ValueMap curmap;
	for(ENEMY eny:globalEnemyData)
	{
		curmap["category"]=eny.category;
		curmap["type"]=eny.type;
		curmap["name"]=eny.name;
		curmap["desc"]=eny.desc;
		curmap["life"]=eny.life;
		curmap["offense"]=eny.offense;
		curmap["defense"]=eny.defense;
		curmap["money"]=eny.money;
		curmap["selected"]=false;
		enemytemp[eny.type]=curmap;
		curmap.clear();
	}

	for(ITEM item:globalItemData)
	{
		curmap["category"]=item.category;
		curmap["type"]=item.type;
		curmap["name"]=item.name;
		curmap["desc"]=item.desc;
		curmap["selected"]=false;
		itemtemp[item.type]=curmap;
		curmap.clear();
	}
}

ObjManager::~ObjManager()
{
	for(ValueMap* pt:enemies)
	{
		delete pt;
	}
	for(ValueMap* pt:items)
	{
		delete pt;
	}
}


int HeroInitPos[]=
{//in*2 out*2分别代表从楼下到此层的初始位置和从楼上到此层的初始位置
	//					第一层			第二层			第三层			第四层
	-1,-1,  -1,-1,    6,10,  2,1,    2,1,  1,10,    2,11,  10,11,    11,10,  1,10
};
