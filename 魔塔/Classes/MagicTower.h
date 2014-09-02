#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "common.h"
USING_NS_CC;

class MagicTower : public cocos2d::Layer
{
public:
    MagicTower(int level); 
    void menuCloseCallback(cocos2d::Ref* pSender);
	void InitMap();
	void InitHero();
	void InitBoard();

	static Hero hero;
	static Board board;

private:
	bool ismoving;//防止MoveHero重入
	TMXTiledMap* map;
	int thislevel;

	void MoveHero(const Vec2& newpos,Animation* anim,int direction);//tile坐标系
	void attackEnemy(TMXLayer* layer,Vec2 tilepos,Sprite* attacksprite,Animate* animate);//tile坐标系
	void FlushBoard(ValueMap* obj=NULL);//刷新面板显示
	void SelectSprite(Sprite* sprite);
	void PlayHeroMovement(const Vec2& newpos,Animation* anim,int direction,const std::function<void(void)>& afteraction);//tile坐标系
#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	void OnMove(EventKeyboard::KeyCode keycode, Event* event);
#endif
	void DefaultKeyboardDispatcher(EventKeyboard::KeyCode keycode);
	void MapPosToTilePos(const Vec2& pos,Vec2& objpos);//map坐标系 tile坐标系
	void TilePosToMapPos(const Vec2& pos,Vec2& objpos);//map坐标系 tile坐标系
	void MoveUnitInTile(const Vec2& position,Sprite* obj);//tile坐标系
};

class MagicTowerLayerManager
{
public:
	static MagicTowerLayerManager& getInstance();
	Scene* getScene(int layer=1);

private:
	int curlayer;
	MagicTowerLayerManager();
};

#endif // __HELLOWORLD_SCENE_H__
