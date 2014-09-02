#include "MagicTower.h"
#include "..\external\win32-specific\icon\include\iconv.h"

USING_NS_CC;
using namespace CocosDenshion;

extern int HeroInitPos[];

#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#pragma comment(lib,"libiconv.lib")
bool IConvConvert(const char *from_charset, const char *to_charset, const char *inbuf, int inlen, char *outbuf, int outlen) 
{
	iconv_t cd = iconv_open(to_charset, from_charset);
	if (cd == 0) return false;
	const char **pin = &inbuf;
	char **pout = &outbuf;
	memset(outbuf,0,outlen);
	size_t ret = iconv(cd,pin,(size_t *)&inlen,pout,(size_t *)&outlen);
	iconv_close(cd);
	return ret == (size_t)(-1) ? false : true;
}

std::string IConvConvert_GBKToUTF8(const std::string& str)
{
	const char* textIn = str.c_str();
	char textOut[256];
	bool ret = IConvConvert("gb2312", "utf-8", textIn, strlen(textIn),textOut, 256);
	return ret ? std::string(textOut) : std::string();
}
#else
std::string IConvConvert_GBKToUTF8(const std::string& str)
{
	return str;
}
#endif

// on "init" you need to initialize your instance
MagicTower::MagicTower(int level)
{
	CCLog("Enter MagicTower");
	thislevel=level;
	auto director=Director::getInstance();
	//director->getOpenGLView()->setFrameSize(416,416);
    //////////////////////////////
    // 1. super init first
    Layer::init();

    Size visibleSize = director->getVisibleSize();
    Vec2 origin = director->getVisibleOrigin();
    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create("CloseNormal.png","CloseSelected.png",CC_CALLBACK_1(MagicTower::menuCloseCallback, this));
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menuclose = Menu::create(closeItem, NULL);
    menuclose->setPosition(Vec2::ZERO);
    this->addChild(menuclose, 1);

    /////////////////////////////
    // 3. add your codes below...
	InitMap();
	InitHero();
	FlushBoard();

#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	//初始换方向键回调
	auto listener=EventListenerKeyboard::create();
	listener->onKeyPressed=CC_CALLBACK_2(MagicTower::OnMove,this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener,this);
#else
	//初始化触摸滑动
	auto listener=EventListenerTouchAllAtOnce::create();
	listener->onTouchesMoved=[this](const std::vector<Touch*>& touches,Event* event)
	{
		auto touch=touches[0];
		map->setPosition(map->getPosition()+touch->getDelta());
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener,this);
#endif
	CCLog("Leave MagicTower");
}


void MagicTower::InitMap()
{
	CCLog("Enter InitMap");
	//加载地图背景
	auto director=Director::getInstance();
	Size visibleSize = director->getVisibleSize();
	char mapname[64];
	sprintf(mapname,"%d.tmx",thislevel);
	map=TMXTiledMap::create(mapname);
	map->setAnchorPoint(Vec2(0.5f,0.5f));
//#if(CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
	map->setScale(1.5);
//#endif
	board.board=TMXTiledMap::create("background.tmx");
	board.board->setAnchorPoint(Vec2(0.5f,0.5f));
	map->setPosition(visibleSize.width/2,visibleSize.height/2+board.board->getContentSize().height/2);
	board.board->setPosition(visibleSize.width/2,board.board->getContentSize().height/2);
	addChild(map);
	addChild(board.board);
	auto listenerm=EventListenerTouchAllAtOnce::create();
	listenerm->onTouchesMoved=[this](const std::vector<Touch*>& touches, Event  *event)
	{
		auto touch=touches[0];
		auto diff=touch->getDelta();
		map->setPosition(map->getPosition()+diff);
	};
//	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerm,map);

	auto arrowright=Sprite::create("arrow.png");
	auto arrowdown=Sprite::create("arrow.png");
	auto arrowleft=Sprite::create("arrow.png");
	auto arrowup=Sprite::create("arrow.png");
	arrowright->setAnchorPoint(Vec2(0,0.5));
	arrowright->setScale(3);
	arrowdown->setAnchorPoint(Vec2(0,0.5));
	arrowdown->setRotation(90);
	arrowdown->setScale(3);
	arrowleft->setAnchorPoint(Vec2(0,0.5));
	arrowleft->setRotation(180);
	arrowleft->setScale(3);
	arrowup->setAnchorPoint(Vec2(0,0.5));
	arrowup->setRotation(270);
	arrowup->setScale(3);
	arrowright->setPosition(visibleSize.width/2,visibleSize.height/2);
	arrowdown->setPosition(visibleSize.width/2,visibleSize.height/2);
	arrowleft->setPosition(visibleSize.width/2,visibleSize.height/2);
	arrowup->setPosition(visibleSize.width/2,visibleSize.height/2);
	addChild(arrowright,240,24);
	arrowright->setTag(24);
	addChild(arrowdown,260,26);
	arrowdown->setTag(26);
	addChild(arrowleft,230,23);
	arrowleft->setTag(23);
	addChild(arrowup,250,25);
	arrowup->setTag(25);

	//初始化点击事件
	auto listenerll=EventListenerTouchOneByOne::create();
	listenerll->setSwallowTouches(true);
	listenerll->onTouchEnded=[=](Touch* touch,Event* event)
	{
		auto animCache=AnimationCache::getInstance();
		Animation* curanimation=NULL;
		Vec2 delta;
		auto target = static_cast<Sprite*>(event->getCurrentTarget());

		switch(target->getTag())
		{
			case 25:
				curanimation=animCache->getAnimation("up");
				delta.setPoint(0,blocksize);
				break;

			case 26:
				curanimation=animCache->getAnimation("down");
				delta.setPoint(0,-blocksize);
				break;

			case 23:
				curanimation=animCache->getAnimation("left");
				delta.setPoint(-blocksize,0);
				break;

			case 24:
				curanimation=animCache->getAnimation("right");
				delta.setPoint(blocksize,0);
				break;
		}
		if(curanimation && !ismoving)
		{
			MoveHero(hero.heroSprite->getPosition()+delta,curanimation,target->getTag());
		}
	};

	listenerll->onTouchBegan=[](Touch* touch,Event* event)
	{
		CCLog("Touch Began!");
		auto target = static_cast<Sprite*>(event->getCurrentTarget());
		Vec2 locationInNode=target->convertToNodeSpace(touch->getLocation());
		Size s=target->getContentSize();
		Rect rect=Rect(0,0,s.width,s.height);
		if(!rect.containsPoint(locationInNode))
			return false;
		return true;
	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerll,arrowright);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerll->clone(),arrowdown);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerll->clone(),arrowleft);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerll->clone(),arrowup);


	InitBoard();

	//初始化地图元素
	Vector<SpriteFrame*> animFrames(4);
	auto enemylayer=map->getLayer("enemy");
	auto enemyTexture=enemylayer->getTexture();

	for(int i=0;i<map->getMapSize().width;i++)
	{
		for(int j=0;j<map->getMapSize().height;j++)
		{
			int id=enemylayer->getTileGIDAt(Vec2(i,j));
			if(id)
			{
				int index=(id-ENEMY_BEGIN)/4;
				auto frame0=SpriteFrame::createWithTexture(enemyTexture,Rect(blocksize*0,blocksize*index,blocksize,blocksize));
				auto frame1=SpriteFrame::createWithTexture(enemyTexture,Rect(blocksize*1,blocksize*index,blocksize,blocksize));
				auto frame2=SpriteFrame::createWithTexture(enemyTexture,Rect(blocksize*2,blocksize*index,blocksize,blocksize));
				auto frame3=SpriteFrame::createWithTexture(enemyTexture,Rect(blocksize*3,blocksize*index,blocksize,blocksize));
				animFrames.pushBack(frame0);
				animFrames.pushBack(frame1);
				animFrames.pushBack(frame2);
				animFrames.pushBack(frame3);
				auto animation=Animation::createWithSpriteFrames(animFrames,0.1f);
				animFrames.clear();
				CCLog("x=%d,y=%d",i,j);
				auto cursprite=enemylayer->getTileAt(Vec2(i,j));
				if(!cursprite)
					return;
				cursprite->runAction(RepeatForever::create(Animate::create(animation)));
				cursprite->setUserData(ObjManager::getInstance().InitEnemyByType(id));
			}
		}
	}

	
	//初始化点击事件
	auto listener=EventListenerTouchOneByOne::create();
	listener->onTouchBegan=[this,enemylayer](Touch* touch,Event* event)
	{
		CCLog("Touch Began!");
		Vec2 locationInNode=enemylayer->convertToNodeSpace(touch->getLocation());
		Size s=enemylayer->getContentSize();
		Rect rect=Rect(0,0,s.width,s.height);
		if(!rect.containsPoint(locationInNode))
			return false;

		Vec2 tilepos;
		MapPosToTilePos(Vec2(locationInNode.x,locationInNode.y),tilepos);
		if(tilepos.x >= 0 && tilepos.y >=0 && 
			tilepos.x < enemylayer->getLayerSize().width && tilepos.y < enemylayer->getLayerSize().height)
		{
			Sprite* node=enemylayer->getTileAt(tilepos);
			if(node)
			{
				SelectSprite(node);
			}
		}
		//根据位置获取单元格，设置为选中状态并刷新数据到面板
		return true;
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener,enemylayer);
	

	auto itemlayer=map->getLayer("item");
	for(int i=0;i<map->getMapSize().width;i++)
	{
		for(int j=0;j<map->getMapSize().height;j++)
		{
			int id=itemlayer->getTileGIDAt(Vec2(i,j));
			CCLog("x=%d,y=%d",i,j);
			auto cursprite=enemylayer->getTileAt(Vec2(i,j));
			//if(id && cursprite)
			//{
			//	cursprite->setUserData(ObjManager::getInstance().InitItemByType(id));
			//}
		}
	}

	//初始化门动画
	auto doorTexture=map->getLayer("door")->getTexture();
	int doordata[]={0,1,2,3};
	std::string doorname[]={"DOOR_YELLOW","DOOR_BLUE","DOOR_RED","DOOR_GREEN"};
	for(int i=0;i<getNum(doordata);i++)
	{
		doordata[i];
		auto frame0=SpriteFrame::createWithTexture(doorTexture,Rect(blocksize*doordata[i],blocksize*0,blocksize,blocksize));
		auto frame1=SpriteFrame::createWithTexture(doorTexture,Rect(blocksize*doordata[i],blocksize*1,blocksize,blocksize));
		auto frame2=SpriteFrame::createWithTexture(doorTexture,Rect(blocksize*doordata[i],blocksize*2,blocksize,blocksize));
		auto frame3=SpriteFrame::createWithTexture(doorTexture,Rect(blocksize*doordata[i],blocksize*3,blocksize,blocksize));
		animFrames.pushBack(frame0);
		animFrames.pushBack(frame1);
		animFrames.pushBack(frame2);
		animFrames.pushBack(frame3);
		auto animation=Animation::createWithSpriteFrames(animFrames,0.05f);
		animFrames.clear();
		AnimationCache::getInstance()->addAnimation(animation,doorname[i]);
	}

	CCLog("Leave InitMap");
}

void MagicTower::InitBoard()
{
	CCLog("Enter InitBoard");
	//初始化面板
	char title[64];
#if(CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
	sprintf(title,"魔塔 第%d层",thislevel);
#else
	sprintf(title,"MagicTower %dth",thislevel);
#endif
	board.maintitle=Label::createWithSystemFont(IConvConvert_GBKToUTF8(title),"黑体",30);
	board.maintitle->setColor(Color3B::GREEN);
	board.maintitle->setAnchorPoint(Vec2(0.5f,0.5f));
	board.maintitle->setPosition(board.board->getContentSize().width/2,blocksize*5+16);
	board.board->addChild(board.maintitle);

	board.heroname=Label::createWithSystemFont(IConvConvert_GBKToUTF8(hero.name),"黑体",24);
	board.heroname->setColor(Color3B::RED);
	board.heroname->setAnchorPoint(Vec2(0.5f,0.5f));
	board.heroname->setPosition(board.board->getContentSize().width/4,blocksize*5-16);
	board.board->addChild(board.heroname);

	board.enemyname=Label::createWithSystemFont(IConvConvert_GBKToUTF8("敌方"),"黑体",24);
	board.enemyname->setColor(Color3B::RED);
	board.enemyname->setAnchorPoint(Vec2(0.5f,0.5f));
	board.enemyname->setPosition(board.board->getContentSize().width*7/8,blocksize*5-16);
	board.board->addChild(board.enemyname);

	board.life_hero=Label::createWithBMFont("myfont.fnt","life");
	board.life_hero->setAnchorPoint(Vec2(0,0));
	board.life_hero->setScale(0.4f);
	board.life_hero->setPosition(blocksize*1,blocksize*3+5);
	board.board->addChild(board.life_hero);

	board.offense_hero=Label::createWithBMFont("myfont.fnt","offense");
	board.offense_hero->setAnchorPoint(Vec2(0,0));
	board.offense_hero->setScale(0.4f);
	board.offense_hero->setPosition(blocksize*1,blocksize*2+5);
	board.board->addChild(board.offense_hero);

	board.defense_hero=Label::createWithBMFont("myfont.fnt","defense");
	board.defense_hero->setAnchorPoint(Vec2(0,0));
	board.defense_hero->setScale(0.4f);
	board.defense_hero->setPosition(blocksize*1,blocksize*1+5);
	board.board->addChild(board.defense_hero);

	board.money_hero=Label::createWithBMFont("myfont.fnt","money");
	board.money_hero->setAnchorPoint(Vec2(0,0));
	board.money_hero->setScale(0.4f);
	board.money_hero->setPosition(blocksize*1,blocksize*0+5);
	board.board->addChild(board.money_hero);

	board.yellowkey=Label::createWithBMFont("myfont.fnt","yellowkey");
	board.yellowkey->setAnchorPoint(Vec2(0,0));
	board.yellowkey->setScale(0.4f);
	board.yellowkey->setPosition(blocksize*4,blocksize*3+5);
	board.board->addChild(board.yellowkey);

	board.bluekey=Label::createWithBMFont("myfont.fnt","bluekey");
	board.bluekey->setAnchorPoint(Vec2(0,0));
	board.bluekey->setScale(0.4f);
	board.bluekey->setPosition(blocksize*4,blocksize*2+5);
	board.board->addChild(board.bluekey);

	board.redkey=Label::createWithBMFont("myfont.fnt","redkey");
	board.redkey->setAnchorPoint(Vec2(0,0));
	board.redkey->setScale(0.4f);
	board.redkey->setPosition(blocksize*4,blocksize*1+5);
	board.board->addChild(board.redkey);

	board.life_enemy=Label::createWithBMFont("myfont.fnt","life");
	board.life_enemy->setAnchorPoint(Vec2(0,0));
	board.life_enemy->setScale(0.4f);
	board.life_enemy->setPosition(blocksize*11,blocksize*3+5);
	board.board->addChild(board.life_enemy);

	board.offense_enemy=Label::createWithBMFont("myfont.fnt","offense");
	board.offense_enemy->setAnchorPoint(Vec2(0,0));
	board.offense_enemy->setScale(0.4f);
	board.offense_enemy->setPosition(blocksize*11,blocksize*2+5);
	board.board->addChild(board.offense_enemy);

	board.defense_enemy=Label::createWithBMFont("myfont.fnt","defense");
	board.defense_enemy->setAnchorPoint(Vec2(0,0));
	board.defense_enemy->setScale(0.4f);
	board.defense_enemy->setPosition(blocksize*11,blocksize*1+5);
	board.board->addChild(board.defense_enemy);

	CCLog("Leave InitBoard");
}

void MagicTower::InitHero()
{
	CCLog("Enter InitHero");
	ismoving=false;
	//加载英雄图片
	auto director=Director::getInstance();
	Size visibleSize = director->getVisibleSize();
	auto heroTexture=Director::getInstance()->getTextureCache()->addImage("hero.png");
	auto frame0=SpriteFrame::createWithTexture(heroTexture,Rect(blocksize*0,blocksize*1,blocksize,blocksize));
	hero.heroSprite=Sprite::createWithSpriteFrame(frame0);
	hero.heroSprite->setAnchorPoint(Vec2(0,0));
	MoveUnitInTile(hero.nexttilepos,hero.heroSprite);

	map->addChild(hero.heroSprite);

	//存储英雄移动动画
	std::string dirname[]={"down","left","right","up"};
	Vector<SpriteFrame*> animFrames(4);
	for(int i=0;i<4;i++)
	{
		auto frame0=SpriteFrame::createWithTexture(heroTexture,Rect(blocksize*0,blocksize*i,blocksize,blocksize));
		auto frame1=SpriteFrame::createWithTexture(heroTexture,Rect(blocksize*1,blocksize*i,blocksize,blocksize));
		auto frame2=SpriteFrame::createWithTexture(heroTexture,Rect(blocksize*2,blocksize*i,blocksize,blocksize));
		auto frame3=SpriteFrame::createWithTexture(heroTexture,Rect(blocksize*3,blocksize*i,blocksize,blocksize));
		animFrames.pushBack(frame0);
		animFrames.pushBack(frame1);
		animFrames.pushBack(frame2);
		animFrames.pushBack(frame3);
		auto animation=Animation::createWithSpriteFrames(animFrames,0.05f);
		animation->setRestoreOriginalFrame(false);
		AnimationCache::getInstance()->addAnimation(animation,dirname[i]);
		animFrames.clear();
	}

	//初始化攻击动画
	auto attackTexture= Director::getInstance()->getTextureCache()->addImage("attack.png");
	Size unitsize=attackTexture->getContentSizeInPixels();
	for(int i=0;i<25;i++)
	{
		auto frame0=SpriteFrame::createWithTexture(attackTexture,
			Rect(i/5*unitsize.width/5,i%5*unitsize.height/5,unitsize.width/5,unitsize.height/5));
		animFrames.pushBack(frame0);
	}
	AnimationCache::getInstance()->addAnimation(Animation::createWithSpriteFrames(animFrames,0.05f),"attack");
	animFrames.clear();

	CCLog("Leave InitHero");
}

void MagicTower::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
void MagicTower::OnMove(EventKeyboard::KeyCode keyCode, Event* event)
{
	CCLog("Enter OnMove");
	auto animCache=AnimationCache::getInstance();
	Animation* curanimation=NULL;
	Vec2 delta;
	int direction;

	switch(keyCode)
	{
		case EventKeyboard::KeyCode::KEY_UP_ARROW:
			curanimation=animCache->getAnimation("up");
			delta.setPoint(0,blocksize);
			direction=25;
			break;

		case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
			curanimation=animCache->getAnimation("down");
			delta.setPoint(0,-blocksize);
			direction=26;
			break;

		case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
			curanimation=animCache->getAnimation("left");
			delta.setPoint(-blocksize,0);
			direction=23;
			break;

		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
			curanimation=animCache->getAnimation("right");
			delta.setPoint(blocksize,0);
			direction=24;
			break;

		default:
			DefaultKeyboardDispatcher(keyCode);
			return;
	}

	//判断英雄位置是否可以移动
	if(curanimation && !ismoving)
	{
		MoveHero(hero.heroSprite->getPosition()+delta,curanimation,direction);
	}
	CCLog("Leave OnMove");
}
#endif

void MagicTower::PlayHeroMovement(const Vec2& newpos,Animation* anim,int direction,const std::function<void(void)>& afteraction)
{//anim：帧动画  afteraction：状态更新回调
	CCLog("Enter PlayeHeroMovement");
	hero.heroSprite->setSpriteFrame(anim->getFrames().at(0)->getSpriteFrame());
	
	//移动动画和帧动画同时进行
	auto spawn=Spawn::createWithTwoActions(Animate::create(anim),MoveTo::create(0.2f,newpos));

	//帧动画+移动动画->状态更新->可通行->连击
	hero.heroSprite->runAction(Sequence::create(spawn,CallFuncN::create([this,afteraction,direction](Node* node)
	{//如果有回调函数需要写一起，否则单另作为Sequence依次执行会失效，不知道是不是bug
		afteraction();
		ismoving=false;
		CCLog("ismoving");
		FlushBoard();
	}),NULL));
	CCLog("Leave PlayeHeroMovement");
}

void MagicTower::MoveHero(const Vec2& newpos,Animation* anim,int direction)
{//若成功，则捡物品或攻击
	CCLog("Enter MoveHero");

	ismoving=true;

	Vec2 tilepos;
	MapPosToTilePos(newpos+Vec2(blocksize/2,blocksize/2),tilepos);

	if(map->getLayer("floor")->getTileGIDAt(tilepos) == FLOOR_WALL)
	{//禁止通行
		ismoving=false;
		return;
	}

	//若是地板则进行上层判断
	auto layer=map->getLayer("item");
	switch(layer->getTileGIDAt(tilepos))
	{
		case ITEM_REDDIAMOND:
			PlayHeroMovement(newpos,anim,direction,[this,layer,tilepos]()
			{//刷新+音乐+物品动画
				hero.offense += 100;
				CCLog("x=%d,y=%d",tilepos.x,tilepos.y);
				layer->removeChild(layer->getTileAt(tilepos),true);
				SimpleAudioEngine::getInstance()->playEffect("eat.mp3");
			});
			return;

		case ITEM_BLUEDIAMOND:
			PlayHeroMovement(newpos,anim,direction,[this,layer,tilepos]()
			{//刷新+音乐+物品动画
				hero.defense += 50;
				CCLog("x=%d,y=%d",tilepos.x,tilepos.y);
				layer->removeChild(layer->getTileAt(tilepos),true);
				SimpleAudioEngine::getInstance()->playEffect("eat.mp3");
			});
			return;

		case ITEM_REDBLOOD:
			PlayHeroMovement(newpos,anim,direction,[this,layer,tilepos]()
			{//刷新+音乐+物品动画
				hero.life += 400;
				CCLog("x=%d,y=%d",tilepos.x,tilepos.y);
				layer->removeChild(layer->getTileAt(tilepos),true);
				SimpleAudioEngine::getInstance()->playEffect("eat.mp3");
			});
			return;

		case ITEM_BLUEBLOOD:
			PlayHeroMovement(newpos,anim,direction,[this,layer,tilepos]()
			{//刷新+音乐+物品动画
				hero.life += 200;
				CCLog("x=%d,y=%d",tilepos.x,tilepos.y);
				layer->removeChild(layer->getTileAt(tilepos),true);
				SimpleAudioEngine::getInstance()->playEffect("eat.mp3");
			});
			return;
			
		case ITEM_YELLOWKEY:
			PlayHeroMovement(newpos,anim,direction,[this,layer,tilepos]()
			{//刷新+音乐+物品动画
				hero.keyyellow++;
				CCLog("x=%d,y=%d",tilepos.x,tilepos.y);
				layer->removeChild(layer->getTileAt(tilepos),true);
				SimpleAudioEngine::getInstance()->playEffect("eat.mp3");
			});
			return;
			
		case ITEM_BLUEKEY:
			PlayHeroMovement(newpos,anim,direction,[this,layer,tilepos]()
			{//刷新+音乐+物品动画
				hero.keyblue++;
				CCLog("x=%d,y=%d",tilepos.x,tilepos.y);
				layer->removeChild(layer->getTileAt(tilepos),true);
				SimpleAudioEngine::getInstance()->playEffect("eat.mp3");
			});
			return;
			
		case ITEM_REDKEY:
			PlayHeroMovement(newpos,anim,direction,[this,layer,tilepos]()
			{//刷新+音乐+物品动画
				hero.keyred++;
				CCLog("x=%d,y=%d",tilepos.x,tilepos.y);
				layer->removeChild(layer->getTileAt(tilepos),true);
				SimpleAudioEngine::getInstance()->playEffect("eat.mp3");
			});
			return;
			
		default://若为空，执行上层判断
			break;
	}

	layer=map->getLayer("enemy");
	if(layer->getTileGIDAt(tilepos))
	{//若有敌人
		PlayHeroMovement(newpos,anim,direction,[this,layer,tilepos]()
		{
			auto animation=AnimationCache::getInstance()->getAnimation("attack");
			auto attacksprite=Sprite::createWithSpriteFrame(animation->getFrames().at(0)->getSpriteFrame());
			attacksprite->setAnchorPoint(Vec2(0,0));
			attacksprite->setPosition(-attacksprite->getContentSize().width/4,-attacksprite->getContentSize().height/4);
			//初始化攻击动画+音乐
			hero.heroSprite->addChild(attacksprite);
			attackEnemy(layer,tilepos,attacksprite,Animate::create(animation));	
		});
		return;
	}
	//若为空，执行上层判断

	layer=map->getLayer("door");
	switch(layer->getTileGIDAt(tilepos))
	{
		case DOOR_YELLOW:
			{
				hero.keyyellow--;
				auto animation=AnimationCache::getInstance()->getAnimation("DOOR_YELLOW");
				auto spawn=Spawn::createWithTwoActions(Sequence::createWithTwoActions(Animate::create(animation),
					CallFuncN::create([layer,tilepos](Node*){
					CCLog("x=%d,y=%d",tilepos.x,tilepos.y);
					layer->removeChild(layer->getTileAt(tilepos),true);
				})),CallFuncN::create([](Node*){
					SimpleAudioEngine::getInstance()->playEffect("open.mp3");
				}));
				CCLog("x=%d,y=%d",tilepos.x,tilepos.y);
				layer->getTileAt(tilepos)->runAction(Sequence::createWithTwoActions(spawn,
					CallFuncN::create([this,newpos,anim,direction](Node*){
						PlayHeroMovement(newpos,anim,direction,[](){});
				})));			
			}
			return;

		case DOOR_BLUE:
			{
				hero.keyblue--;
				auto animation=AnimationCache::getInstance()->getAnimation("DOOR_BLUE");
				auto spawn=Spawn::createWithTwoActions(Sequence::createWithTwoActions(Animate::create(animation),
					CallFuncN::create([layer,tilepos](Node*){
					CCLog("x=%d,y=%d",tilepos.x,tilepos.y);
					layer->removeChild(layer->getTileAt(tilepos),true);
				})),CallFuncN::create([](Node*){
					SimpleAudioEngine::getInstance()->playEffect("open.mp3");
				}));
				CCLog("x=%d,y=%d",tilepos.x,tilepos.y);
				layer->getTileAt(tilepos)->runAction(Sequence::createWithTwoActions(spawn,
					CallFuncN::create([this,newpos,anim,direction](Node*){
						PlayHeroMovement(newpos,anim,direction,[](){});
				})));		
			}
			return;

		case DOOR_RED:
			{
				hero.keyred--;
				auto animation=AnimationCache::getInstance()->getAnimation("DOOR_RED");
				auto spawn=Spawn::createWithTwoActions(Sequence::createWithTwoActions(Animate::create(animation),
					CallFuncN::create([layer,tilepos](Node*){
					CCLog("x=%d,y=%d",tilepos.x,tilepos.y);
					layer->removeChild(layer->getTileAt(tilepos),true);
				})),CallFuncN::create([](Node*){
					SimpleAudioEngine::getInstance()->playEffect("open.mp3");
				}));
				CCLog("x=%d,y=%d",tilepos.x,tilepos.y);
				layer->getTileAt(tilepos)->runAction(Sequence::createWithTwoActions(spawn,
					CallFuncN::create([this,newpos,anim,direction](Node*){
						PlayHeroMovement(newpos,anim,direction,[](){});
				})));	
			}
			return;
		
		case DOOR_GREEN:
			ismoving=false;
			return;

		case DOOR_UPSTAIRS:
			{
				//更新地图
				Director::getInstance()->setDepthTest(false);
				hero.nexttilepos=Vec2(HeroInitPos[(thislevel+1)*4],HeroInitPos[(thislevel+1)*4+1]);
				auto trans=TransitionCrossFade::create(0.5,MagicTowerLayerManager::getInstance().getScene(thislevel+1));
				if(trans)
					Director::getInstance()->replaceScene(trans);
				ismoving=false;
			}
			return;

		case DOOR_DOWNSTAIRS:
			{
				//更新地图
				Director::getInstance()->setDepthTest(false);
				hero.nexttilepos=Vec2(HeroInitPos[(thislevel-1)*4+2],HeroInitPos[(thislevel-1)*4+3]);
				auto trans=TransitionCrossFade::create(0.5,MagicTowerLayerManager::getInstance().getScene(thislevel-1));
				if(trans)
					Director::getInstance()->replaceScene(trans);
				ismoving=false;
			}
			return;
	}

	//若无对象存在，则移动英雄
	PlayHeroMovement(newpos,anim,direction,[](){});
	//在执行PlayerHeroMovement的地方，ismoving交给该函数处理
	CCLog("Leave MoveHero");
}

void MagicTower::attackEnemy(TMXLayer* layer,Vec2 tilepos,Sprite* attacksprite,Animate* animate)
{
	CCLog("Enter attackEnemy");

	//判断英雄是否移动了，如果移动则去除animate并结束
	Vec2 herointile;
	MapPosToTilePos(hero.heroSprite->getPosition()+Vec2(blocksize/2,blocksize/2),herointile);
	CCLog("hero:(%f %f)",herointile.x,herointile.y);
	if(herointile != tilepos)
	{
		attacksprite->stopAllActions();
		hero.heroSprite->removeChild(attacksprite,true);
		return;
	}

	attacksprite->runAction(Sequence::create(animate,CallFuncN::create([this,layer,tilepos,animate](Node* attacksprite)
	{
		CCLog("x=%d,y=%d",tilepos.x,tilepos.y);
		ValueMap* val=static_cast<ValueMap*>(layer->getTileAt(tilepos)->getUserData());
		int enemylife=(*val)["life"].asInt();
		int enemyoffense=(*val)["offense"].asInt();
		int enemydefense=(*val)["defense"].asInt();
		int harmetoh=(hero.defense>enemyoffense)?0:enemyoffense-hero.defense;
		//enemy对hero造成的伤害，如果hero的防>enemy的攻，则无伤害
		int harmhtoe=(enemydefense>hero.offense)?0:hero.offense-enemydefense;
		hero.life -= harmetoh;
		enemylife -= harmhtoe;
		SimpleAudioEngine::getInstance()->playEffect("fight.mp3");	
		(*val)["life"]=enemylife;
		FlushBoard(val);

		if(enemylife < 0 || hero.life < 0)
		{
			if(hero.life < 0)
			{//英雄挂了
			}
			else
			{
				attacksprite->stopAllActions();
				hero.money += (*val)["money"].asInt();
				hero.heroSprite->removeChild(attacksprite,true);
				CCLog("x=%d,y=%d",tilepos.x,tilepos.y);
				layer->removeChild(layer->getTileAt(tilepos),true);
				FlushBoard(val);
				//刷新面板
			}
		}
		else
		{
			attackEnemy(layer,tilepos,static_cast<Sprite*>(attacksprite),animate);
		}
	}),NULL));

	CCLog("Leave attackEnemy");
}

void MagicTower::DefaultKeyboardDispatcher(EventKeyboard::KeyCode keycode)
{//处理其他键盘事件
	
}

void MagicTower::TilePosToMapPos(const Vec2& pos,Vec2& objpos)
{//pos为Tile坐标系，相对于窗口左上角
	float rex=pos.x*blocksize;
	float rey=map->getContentSize().height-pos.y*blocksize;
	objpos.setPoint(rex,rey);
}

void MagicTower::MapPosToTilePos(const Vec2& pos,Vec2& objpos)
{//pos为map坐标系，相对于左下角
	float rex=pos.x/(blocksize);
	float rey=(map->getContentSize().height-pos.y)/(blocksize);
	objpos.setPoint(int(rex),int(rey));
}

void MagicTower::MoveUnitInTile(const Vec2& pos,Sprite* obj)
{//pos为Tile坐标系，相对于左上角
	float rex=pos.x*blocksize;
	float rey=map->getContentSize().height-pos.y*blocksize-blocksize;
	obj->setPosition(rex,rey);
}

void MagicTower::FlushBoard(ValueMap* obj)
{
	CCLog("Enter FlushBoard");
	char title[64];
#if(CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
	sprintf(title,"魔塔 第%d层",thislevel);
#else
	sprintf(title,"MagicTower %dth",thislevel);
#endif
	board.maintitle->setString(IConvConvert_GBKToUTF8(title));
	board.heroname->setString(IConvConvert_GBKToUTF8(hero.name));
	sprintf(title,"%d",hero.life);
	board.life_hero->setString(IConvConvert_GBKToUTF8(title));
	sprintf(title,"%d",hero.offense);
	board.offense_hero->setString(IConvConvert_GBKToUTF8(title));
	sprintf(title,"%d",hero.defense);
	board.defense_hero->setString(IConvConvert_GBKToUTF8(title));
	sprintf(title,"%d",hero.money);
	board.money_hero->setString(IConvConvert_GBKToUTF8(title));
	sprintf(title,"%d",hero.keyyellow);
	board.yellowkey->setString(IConvConvert_GBKToUTF8(title));
	sprintf(title,"%d",hero.keyblue);
	board.bluekey->setString(IConvConvert_GBKToUTF8(title));
	sprintf(title,"%d",hero.keyred);
	board.redkey->setString(IConvConvert_GBKToUTF8(title));

	if(obj)
	{
		int category=(*obj)["category"].asInt();
		if(category == TYPE_ENEMY)
		{
			board.enemyname->setString(IConvConvert_GBKToUTF8((*obj)["name"].asString()));
			sprintf(title,"%d",(*obj)["life"].asInt());
			board.life_enemy->setString(IConvConvert_GBKToUTF8(title));
			sprintf(title,"%d",(*obj)["offense"].asInt());
			board.offense_enemy->setString(IConvConvert_GBKToUTF8(title));
			sprintf(title,"%d",(*obj)["defense"].asInt());
			board.defense_enemy->setString(IConvConvert_GBKToUTF8(title));
		}
		else if(category == TYPE_ITEM)
		{
		}
	}
	CCLog("Leave FlushBoard");

}

void MagicTower::SelectSprite(Sprite* sprite)
{
	CCLog("Enter SelectSprite");
	auto data=static_cast<ValueMap*>(sprite->getUserData());
	if((*data)["selected"].asBool())
	{
		sprite->removeChildByTag(1000);
		(*data)["selected"]=false;
	}
	else
	{
		auto texture=map->getLayer("enemy")->getTexture();
		int hei=texture->getContentSize().height;
		auto selsprite=Sprite::createWithTexture(texture,Rect(0,texture->getContentSize().height-blocksize,blocksize,blocksize));
		//设置为最后一个
		selsprite->setAnchorPoint(Vec2(0,0));
		selsprite->setPosition(0,0);
		sprite->addChild(selsprite,selsprite->getLocalZOrder(),1000);
		FlushBoard(data);
		(*data)["selected"]=true;
	}
	CCLog("Leave SelectSprite");
}

//MagicTowerLayerManager管理场景切换
MagicTowerLayerManager& MagicTowerLayerManager::getInstance()
{
	static MagicTowerLayerManager manager;
	return manager;
}

Scene* MagicTowerLayerManager::getScene(int layer)
{
	curlayer=layer;
	auto curscene=Scene::create();
	auto curlayer=new MagicTower(layer);
	if (curscene && curlayer)
	{
		curscene->addChild(curlayer);
		curlayer->autorelease();
	}
	return curscene;
}

Hero MagicTower::hero;
Board MagicTower::board;

MagicTowerLayerManager::MagicTowerLayerManager()
{
	curlayer=1;
	MagicTower::hero.name="勇士";
	MagicTower::hero.life=1000;
	MagicTower::hero.offense=200;
	MagicTower::hero.defense=200;
	MagicTower::hero.keyyellow=10;
	MagicTower::hero.keyblue=10;
	MagicTower::hero.keyred=10;
	MagicTower::hero.money=1000;
	MagicTower::hero.nexttilepos=Vec2(6,10);//初始位置
	SimpleAudioEngine::getInstance()->playBackgroundMusic("background.mp3",true);
}
