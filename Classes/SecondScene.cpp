//
//  SecondScene.cpp
//  Dusk
//
//  Created by lilil on 08.01.15.
//
//
#include "SecondScene.h"
#include "MenuScene.h"
#include "ThirdScene.h"
//#include "CCActionCatmullRom.h"

USING_NS_CC;
#define HERO_SPRITE_TAG 5
#define COLLISION_TAG 10
#define FALLING_TAG 20
#define PLUS_TAG 30

#define PORTAL_TAG 100

#define BRICK 1
#define BALL 2
#define ANIMATION_DELAY 0.1f
#define NEWLEVEL_TAG 40

#define MIN_FOR_DIRECTION 10

#define MY_VELOCITY 300

Scene* SecondScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();
   // scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    // 'layer' is an autorelease object
    auto layer = SecondScene::create();
    layer->setPhyWorld(scene->getPhysicsWorld());
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool SecondScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    std::vector<std::string> searchPaths;
    searchPaths.push_back("TileMaps");
    searchPaths.push_back("fonts");
    FileUtils::getInstance()->setSearchPaths(searchPaths);
 
    this->visibleSize = Director::getInstance()->getVisibleSize();
    this->origin = Director::getInstance()->getVisibleOrigin();
    
    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.
    
    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(SecondScene::menuCloseCallback, this));
    
    closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));
    
    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    closeItem->setGlobalZOrder(3);
    closeItem->getNormalImage()->setGlobalZOrder(3);
    closeItem->getSelectedImage()->setGlobalZOrder(3);
    
    this->restartItem = MenuItemImage::create(
                                              "CloseNormal.png",
                                              "CloseSelected.png",
                                              CC_CALLBACK_1(SecondScene::menuRestartCallback, this));
    restartItem->setVisible(false);
    restartItem->setPosition(Vec2(origin.x + visibleSize.width/2 - closeItem->getContentSize().width/2 ,
                                  origin.y + visibleSize.height/2 + closeItem->getContentSize().height/2));
    
    // create menu, it's an autorelease object
    auto menuRestart = Menu::create(restartItem, NULL);
    menuRestart->setPosition(Vec2::ZERO);
    this->addChild(menuRestart, 1);
    restartItem->setGlobalZOrder(3);
    restartItem->getNormalImage()->setGlobalZOrder(3);
    restartItem->getSelectedImage()->setGlobalZOrder(3);
    ///////////////////////////////////////////////////////////////////////////
    this->newlevelItem = MenuItemImage::create(
                                               "CloseNormal.png",
                                               "CloseSelected.png",
                                               CC_CALLBACK_1(SecondScene::menuNewLeveltCallback, this));
    newlevelItem->setVisible(false);
    newlevelItem->setPosition(Vec2(origin.x + visibleSize.width/2 - closeItem->getContentSize().width/2 ,
                                   origin.y + visibleSize.height/2 + closeItem->getContentSize().height/2));
    
    
    // create menu, it's an autorelease object
    auto menuNewLevel = Menu::create(newlevelItem, NULL);
    menuNewLevel->setPosition(Vec2::ZERO);
    this->addChild(menuNewLevel, 1);
    newlevelItem->setGlobalZOrder(3);
    newlevelItem->getNormalImage()->setGlobalZOrder(3);
    newlevelItem->getSelectedImage()->setGlobalZOrder(3);

    //////////////////////////////////////////////////
    //auto map = TMXTiledMap::create("test.tmx");
    this->map = TMXTiledMap::create("labirint.tmx");
    Size s = map->getContentSize();
    this->scale_mini_map = visibleSize.width / s.width /4;
    this->scale_map = visibleSize.height / s.height;
    map->setScale(scale_map);
    auto yZero = origin.y;
    map->setPosition(Vec2(origin.x + visibleSize.width - s.width*scale_map, yZero));
    addChild(map, 0);

    auto labirintcache = SpriteFrameCache::getInstance();
    labirintcache->addSpriteFramesWithFile("lb.plist");
    auto labirint = Sprite::createWithSpriteFrameName("labirint01.png");

    
    Vector<SpriteFrame*> animLabirintFrames;
    animLabirintFrames.reserve(4);
    animLabirintFrames.pushBack(labirintcache->getSpriteFrameByName("labirint03.png"));
    animLabirintFrames.pushBack(labirintcache->getSpriteFrameByName("labirint01.png"));
    animLabirintFrames.pushBack(labirintcache->getSpriteFrameByName("labirint02.png"));
    animLabirintFrames.pushBack(labirintcache->getSpriteFrameByName("labirint01.png"));

    
    // create the animation out of the frames
    Animation* animationLabirint = Animation::createWithSpriteFrames(animLabirintFrames, 0.5f);
    auto animateLabirint = Animate::create(animationLabirint);
    labirint->runAction(RepeatForever::create(animateLabirint));
    labirint->setPosition(Vec2(labirint->getContentSize().width/2, s.height/2+20));
    map->addChild(labirint,2);
    
    auto backLayer = Sprite::create("mini_labirint.png");
    backLayer->setScale(scale_mini_map*4);
    backLayer->setOpacity(230);
    backLayer->setBlendFunc((BlendFunc) {GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA});

    backLayer->setPosition(Vec2(origin.x + visibleSize.width - backLayer->getContentSize().width*scale_mini_map*2, origin.y  + visibleSize.height - backLayer->getContentSize().height*scale_mini_map*2));
    addChild(backLayer,2);

    
    TMXObjectGroup *walls = map->getObjectGroup("collisions");

    this->collisions = makeObject(COLLISION_TAG, walls, scale_map, origin.x + visibleSize.width - s.width*scale_map, yZero, BRICK, 0, 0);


    TMXObjectGroup *holes = map->getObjectGroup("portals");

    this->portals = makeObject(PORTAL_TAG, holes, scale_map, origin.x + visibleSize.width - s.width*scale_map, yZero, BRICK, 0, 0);


    auto spidercache = SpriteFrameCache::getInstance();
    spidercache->addSpriteFramesWithFile("spider.plist");
    auto spidersCount = 3;
    auto spidersAnimSize = 2;
    
    TMXObjectGroup *fallings = map->getObjectGroup("fallings");
    this->fallings = makeObject(FALLING_TAG, fallings, spidercache, "spider",
            spidersCount, spidersAnimSize, scale_map, origin.x + visibleSize.width - s.width*scale_map, yZero, BALL,
            0.8f, true, 1.0f, 0.2f, 1.0f);


    
    //////////////////////////////////////////////////
    auto healthcache = SpriteFrameCache::getInstance();
    healthcache->addSpriteFramesWithFile("plus.plist");
    TMXObjectGroup *health = map->getObjectGroup("health");
    auto plusCount = 1;
    auto plusAnimSize = 4;
    this->pluses = makeObject(PLUS_TAG, health, healthcache, "plus",
                                plusCount, plusAnimSize, scale_map, origin.x + visibleSize.width - s.width*scale_map, yZero, BALL, 0.1f, false, 0.0f, 0.0f, 0.0f);

    
    
    //////////////////////////////////////////////////
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("life.plist");
    this->mylife = Sprite::createWithSpriteFrameName("life3.png");
    this->mylife->setScale(scale_map);
    mylife->setOpacity(230);
    mylife->setBlendFunc((BlendFunc) {GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA});
    mylife->setPosition(origin.x + visibleSize.width/2, yZero /*+ visibleSize.height*/ + mylife->getContentSize().height*scale_map/2);
    addChild(mylife, 3);

    // load the Sprite Sheet
    auto spritecache = SpriteFrameCache::getInstance();
    
    // the .plist file can be generated with any of the tools mentioned below
    spritecache->addSpriteFramesWithFile("tgirlgo.plist");
    
    
    this->mysprite = Sprite::createWithSpriteFrameName("topleft01.png");
    // position the sprite on the center of the screen
    mysprite->setTag(HERO_SPRITE_TAG);
    mysprite->setPosition(Vec2(visibleSize.width/2 + origin.x, origin.y + mysprite->getContentSize().height));
    mysprite->setScale(scale_map*1.5);
    // now lets animate the sprite we moved

    
    Vector<SpriteFrame*> animLeftFrames;
    animLeftFrames.reserve(8);
    animLeftFrames.pushBack(spritecache->getSpriteFrameByName("topleft01.png"));
    animLeftFrames.pushBack(spritecache->getSpriteFrameByName("topleft02.png"));
    animLeftFrames.pushBack(spritecache->getSpriteFrameByName("topleft03.png"));
    animLeftFrames.pushBack(spritecache->getSpriteFrameByName("topleft02.png"));
    animLeftFrames.pushBack(spritecache->getSpriteFrameByName("topleft01.png"));
    animLeftFrames.pushBack(spritecache->getSpriteFrameByName("topleft06.png"));
    animLeftFrames.pushBack(spritecache->getSpriteFrameByName("topleft07.png"));
    animLeftFrames.pushBack(spritecache->getSpriteFrameByName("topleft02.png"));
    
    // create the animation out of the frames
    Animation* animationLeft = Animation::createWithSpriteFrames(animLeftFrames, ANIMATION_DELAY);
    animateLeft = Animate::create(animationLeft);
    animateLeft->retain();
    
    Vector<SpriteFrame*> animRightFrames;
    animRightFrames.reserve(8);
    animRightFrames.pushBack(spritecache->getSpriteFrameByName("topright01.png"));
    animRightFrames.pushBack(spritecache->getSpriteFrameByName("topright02.png"));
    animRightFrames.pushBack(spritecache->getSpriteFrameByName("topright03.png"));
    animRightFrames.pushBack(spritecache->getSpriteFrameByName("topright02.png"));
    animRightFrames.pushBack(spritecache->getSpriteFrameByName("topright01.png"));
    animRightFrames.pushBack(spritecache->getSpriteFrameByName("topright06.png"));
    animRightFrames.pushBack(spritecache->getSpriteFrameByName("topright07.png"));
    animRightFrames.pushBack(spritecache->getSpriteFrameByName("topright02.png"));

    // create the animation out of the frames
    Animation* animationRight = Animation::createWithSpriteFrames(animRightFrames, ANIMATION_DELAY);
    animateRight = Animate::create(animationRight);
    animateRight->retain();
    
    Vector<SpriteFrame*> animTopFrames;
    animTopFrames.reserve(8);
    animTopFrames.pushBack(spritecache->getSpriteFrameByName("toptop01.png"));
    animTopFrames.pushBack(spritecache->getSpriteFrameByName("toptop02.png"));
    animTopFrames.pushBack(spritecache->getSpriteFrameByName("toptop03.png"));
    animTopFrames.pushBack(spritecache->getSpriteFrameByName("toptop02.png"));
    animTopFrames.pushBack(spritecache->getSpriteFrameByName("toptop01.png"));
    animTopFrames.pushBack(spritecache->getSpriteFrameByName("toptop06.png"));
    animTopFrames.pushBack(spritecache->getSpriteFrameByName("toptop07.png"));
    animTopFrames.pushBack(spritecache->getSpriteFrameByName("toptop02.png"));


    // create the animation out of the frames
    Animation* animationTop = Animation::createWithSpriteFrames(animTopFrames, ANIMATION_DELAY);
    animateTop = Animate::create(animationTop);
    animateTop->retain();
    
    Vector<SpriteFrame*> animBottomFrames;
    animBottomFrames.reserve(8);
    animBottomFrames.pushBack(spritecache->getSpriteFrameByName("topbottom01.png"));
    animBottomFrames.pushBack(spritecache->getSpriteFrameByName("topbottom02.png"));
    animBottomFrames.pushBack(spritecache->getSpriteFrameByName("topbottom03.png"));
    animBottomFrames.pushBack(spritecache->getSpriteFrameByName("topbottom02.png"));
    animBottomFrames.pushBack(spritecache->getSpriteFrameByName("topbottom01.png"));
    animBottomFrames.pushBack(spritecache->getSpriteFrameByName("topbottom06.png"));
    animBottomFrames.pushBack(spritecache->getSpriteFrameByName("topbottom07.png"));
    animBottomFrames.pushBack(spritecache->getSpriteFrameByName("topbottom02.png"));

    // create the animation out of the frames
    Animation* animationBottom = Animation::createWithSpriteFrames(animBottomFrames, ANIMATION_DELAY);
    animateBottom = Animate::create(animationBottom);
    animateBottom->retain();
    
    auto w = mysprite->getContentSize().width;
    auto h = mysprite->getContentSize().height;
    auto physicsBody = PhysicsBody::createBox(Size(w*scale_map/3, h*scale_map),
            PhysicsMaterial(1.0f, 0.0f, 0.0f), Vec2(0, 0));
    physicsBody->setRotationEnable(false);
    physicsBody->setDynamic(true);
    physicsBody->setContactTestBitmask(0xFFFFFFFF);
    //set the body isn't affected by the physics world's gravitational force
    physicsBody->setGravityEnable(false);
    
    addChild(this->mysprite, 1);
    mysprite->setPhysicsBody(physicsBody);
    
    
    this->minisprite = Sprite::create("minisprite.png");
    minisprite->setScale(scale_mini_map);
 
    mini_position = Vec2(backLayer->getPositionX() - backLayer->getContentSize().width*scale_mini_map*2,
                         backLayer->getPositionY()- backLayer->getContentSize().height*scale_mini_map*2-mysprite->getContentSize().height*2/3*scale_mini_map);
    minisprite->setPosition(Vec2(mini_position.x + (mysprite->getPositionX() - map->getPositionX())/scale_map*scale_mini_map,
                                 mini_position.y + ((mysprite->getPositionY() - map->getPositionY())/scale_map)*scale_mini_map));
    
    addChild(minisprite, 2);

    
    this->m_emitter = ParticleFire::create();
    this->m_emitter->setScale(scale_map/2);
    this->m_emitter->stopSystem();
    addChild(m_emitter,3);

    for (auto portal : this->portals){
        ParticleSystemQuad* p_emitter = ParticleGalaxy::create();
        p_emitter->setScale(scale_map/2);
        p_emitter->setPosition(0,0);//32*scale_map,32*scale_map);
        portal->addChild(p_emitter);
    }

    
    this->scheduleUpdate();
    return true;
}

Vector<Sprite*> SecondScene::makeObject(int tag, TMXObjectGroup *objects, SpriteFrameCache* spritecache,
        std::string name, int spritecount, int animsize, float scale_map, float xZero, float yZero, int form, float time,
        bool isDynamic, float rest, float fric, float dens, int mask)
{
    Vector<Sprite*> sprites;
    if (objects != nullptr)
    {
        Vector<Animate*> anim;
        
        Vector<SpriteFrame*> spiderFrames;
        for (int i = 0; i < spritecount; i++)
        {
            Vector<SpriteFrame*> spiderFrames;
            spiderFrames.reserve(animsize);
            for (int j = 1; j <= animsize; j++) {
                char* res = new char[50];
                std::sprintf(res, "%s%02i.png", name.c_str(), i*animsize+j);
                spiderFrames.pushBack(spritecache->getSpriteFrameByName(res));
            }
            Animate *animate = Animate::create(Animation::createWithSpriteFrames(spiderFrames, time));
            //   animate->retain();
            anim.pushBack(animate);
        }
        
        float x, y, w, h;
        ValueVector objectsPoint = objects->getObjects();
        int i = 0;
        for(auto objPointMap : objectsPoint)
        {
            ValueMap objPoint = objPointMap.asValueMap();
            x = objPoint.at("x").asFloat();
            y = objPoint.at("y").asFloat();
            w = objPoint.at("width").asFloat();
            h = objPoint.at("height").asFloat();
            
            Point _point = Point(xZero+scale_map*(x + w / 2.0f), yZero+scale_map*(y+h/2.0f));
            Size _size = Size(scale_map*w, scale_map*h);
            char* res = new char[50];
            std::sprintf(res, "%s%02i.png", name.c_str(), (i%spritecount)*animsize + 1);
            auto sprite = this->makePhysicsObjAt(tag, _point, _size, isDynamic, rest, fric, dens, form,
                    anim.at(i%spritecount)->clone(), res, mask);

            auto sprite_name = objPoint.at("name").asString();
            if (sprite_name == "") sprite_name = std::to_string(i);
            i++;

            sprite->setName(sprite_name);
            sprites.pushBack(sprite);
        }
    }
    return sprites;
}



Vector<Sprite*> SecondScene::makeObject(int tag, TMXObjectGroup *objects, float scale_map, float xZero, float yZero,
        int form, int v, int n, int mask)
{
    Vector<Sprite*> sprites;
    if (objects != nullptr)
    {
        float x, y, w, h;
        ValueVector objectsPoint = objects->getObjects();
        
        int i = 0;
        for(auto objPointMap : objectsPoint)
        {
            ValueMap objPoint = objPointMap.asValueMap();
            x = objPoint.at("x").asFloat();
            y = objPoint.at("y").asFloat();
            w = objPoint.at("width").asFloat();
            h = objPoint.at("height").asFloat();
            auto name = objPoint.at("name").asString();
            Point _point = Point(xZero+scale_map*(x + w / 2.0f), yZero+scale_map*(y+h/2.0f));
            Size _size = Size(scale_map*w, scale_map*h);
            auto tagname = tag;
            if (name == "dummy") tagname = 0;
            else if (name == "newlevel") tagname = NEWLEVEL_TAG;
            auto sprite = this->makePhysicsObjAt(tagname, _point, _size, form, v, n, mask);
            
            if (name == "") name = std::to_string(i);
            i++;
            sprite->setName(name);
            sprites.pushBack(sprite);
            
        }
    }
    return sprites;
}

Sprite* SecondScene::makePhysicsObjAt(int tag, Point p, Size size, int form, int v, int n, int mask)
{
    auto sprite = Sprite::create("white_pixel.png");
    
    sprite->getTexture()->setTexParameters({.minFilter =  GL_LINEAR, .magFilter =  GL_LINEAR, .wrapS =  GL_REPEAT, .wrapT =  GL_REPEAT});
    sprite->setTextureRect(Rect(p.x - size.width/2, p.y - size.height/2, size.width, size.height));
    sprite->setOpacity(0);
    PhysicsBody* body;
    
    
    if (form == BRICK){
        body = PhysicsBody::createBox(size,
                                      PhysicsMaterial(1.0f, 1.0f, 0.0f));
    } else if (form == BALL){
        body = PhysicsBody::createCircle(MAX(size.width, size.height)/2.0,
                                         PhysicsMaterial(0.1f, 1.0f, 0.0f));
    }
    
    sprite->setPhysicsBody(body);
    sprite->setTag(tag);
    body->setDynamic(false);
    body->setContactTestBitmask(mask);
//    sprite->setPhysicsBody(body);
    sprite->setPosition(p);
    addChild(sprite, 3);
    
    
    auto moveBy2 = MoveBy::create(1, Vec2(0, v));
    if (n < 0)
        sprite->runAction(RepeatForever::create(moveBy2));
    else
        sprite->runAction(Repeat::create(moveBy2, n));
    return sprite;
}

Sprite* SecondScene::makePhysicsObjAt(int tag, Point p, Size size, bool isDynamic, float rest, float fric, float dens,
        int form, Animate* anim, std::string name, int mask) {
    auto sprite = Sprite::createWithSpriteFrameName(name);
    auto sprite_scale = size.width / sprite->getContentSize().width;
    sprite->setScale(sprite_scale * 1.5);
    sprite->runAction(RepeatForever::create(anim));
    PhysicsBody *body;
    if (form == BRICK) {
        body = PhysicsBody::createBox(size,
                PhysicsMaterial(1.0f, 1.0f, 0.0f));
    } else if (form == BALL) {
        body = PhysicsBody::createCircle(MAX(size.width, size.height) / 2.0,
                PhysicsMaterial(1.0f, 1.0f, 0.2f));
    }
    sprite->setTag(tag);
    if (isDynamic) {
        body->getShape(0)->setRestitution(rest);
        body->getShape(0)->setFriction(fric);
        body->getShape(0)->setDensity(dens);
        body->setVelocity(Vec2(100,0));
        body->setRotationEnable(false);
    }
    body->setDynamic(isDynamic);
    body->setContactTestBitmask(mask); //(0xFFFFFFFF);
    sprite->setPhysicsBody(body);
    sprite->setPosition(p);
    
    addChild(sprite, 1);
    return sprite;
}

void SecondScene::onEnter()
{
    Layer::onEnter();
    
    // Register Touch Event
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchOneByOne::create();
    
    listener->onTouchBegan = CC_CALLBACK_2(SecondScene::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(SecondScene::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(SecondScene::onTouchEnded, this);
    listener->onTouchCancelled = CC_CALLBACK_2(SecondScene::onTouchCancelled, this);
   
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(SecondScene::onContactBegin,
                                                    this);
    
    contactListener->onContactSeperate = CC_CALLBACK_1(SecondScene::onContactSeperate,
                                                       this);

    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    dispatcher->addEventListenerWithSceneGraphPriority(contactListener,
                                                       this);
}

void SecondScene::onExit()
{
    // You don't need to unregister listeners here as new API
    // removes all linked listeners automatically in CCNode's destructor
    // which is the base class for all cocos2d DRAWING classes
    
    Layer::onExit();
}

void SecondScene::update(float delta)
{
    
    if (mysprite->getNumberOfRunningActions() <= 0) {
        if (isRestart || isNewLevel) {
            this->pausedNodes = cocos2d::Director::getInstance()->getActionManager()->pauseAllRunningActions();
            if (isRestart) {
                restartItem->setVisible(true);
                mysprite->getPhysicsBody()->setGravityEnable(true);
            }
            else {
                stopScene();
                newlevelItem->setVisible(true);
            }

        }
    }
    
    if (!isRestart && !isNewLevel) {
        for (auto plus: pluses){
            if (plus->getOpacity() < 255) {
                if(plus->getOpacity() == 0) {
                    Vector<SpriteFrame*> plusFrames;
                    plusFrames.reserve(4);
                    plusFrames.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("plus01.png"));
                    plusFrames.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("plus02.png"));
                    plusFrames.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("plus03.png"));
                    plusFrames.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("plus04.png"));
 
                    Animate *animate = Animate::create(Animation::createWithSpriteFrames(plusFrames, 0.1f));
                    plus->runAction(RepeatForever::create(animate));
                }
                plus->setOpacity(plus->getOpacity() + 1);
            }
        }


        auto dx = 0.0;
        float posX = mysprite->getPositionX();
        float mapPosX = map->getPositionX();
        Size myspriteSize = mysprite->getContentSize();
        if (posX < origin.x + fmax(myspriteSize.height, myspriteSize.width) * scale_map * 4)
            dx = origin.x + fmax(myspriteSize.height, myspriteSize.width) * scale_map * 4 - posX;
        else if (posX > origin.x + visibleSize.width - fmax(myspriteSize.height, myspriteSize.width) * scale_map * 4)
            dx = origin.x + visibleSize.width - fmax(myspriteSize.height, myspriteSize.width) * scale_map * 4 - posX;


        if (mapPosX + dx > origin.x)
            dx = origin.x - mapPosX;
        else if (mapPosX + map->getContentSize().width * scale_map + dx < origin.x + visibleSize.width)
            dx = origin.x + visibleSize.width - mapPosX - map->getContentSize().width * scale_map;

        if (dx != 0) {
            mysprite->setPositionX(mysprite->getPositionX() + dx);

            map->setPositionX(map->getPositionX() + dx);
            setPositionOffsetAllObjectLayer(collisions, Vec2(dx, 0));
            setPositionOffsetAllObjectLayer(portals, Vec2(dx, 0));
            setPositionOffsetAllObjectLayer(fallings, Vec2(dx, 0));
            setPositionOffsetAllObjectLayer(pluses, Vec2(dx, 0));


        }
        minisprite->setPosition(Vec2(mini_position.x + (mysprite->getPositionX() - map->getPositionX()) / scale_map * scale_mini_map,
                mini_position.y + ((mysprite->getPositionY() - map->getPositionY()) / scale_map) * scale_mini_map));
    }
}

void SecondScene::onContactSeperate(const cocos2d::PhysicsContact& contact)
{
    if (!isRestart && !isNewLevel) {
        if ((touchX != -500000) && (touchY != -500000)) {
            auto dx = touchX - mysprite->getPositionX();
            auto dy = touchY - mysprite->getPositionY() + mysprite->getContentSize().height * scale_map / 2;
            direction = NODIRECTION;
            goToPoint(dx, dy);
        }
        
        auto nodeA = contact.getShapeA()->getBody()->getNode();
        auto nodeB = contact.getShapeB()->getBody()->getNode();
        
        if (nodeA->getTag() == HERO_SPRITE_TAG or nodeB->getTag() == HERO_SPRITE_TAG) {
            if (nodeA->getTag() == PLUS_TAG or nodeB->getTag() == PLUS_TAG) {
                Node* node;
                if (nodeA->getTag() == PLUS_TAG) node = nodeA;
                else node = nodeB;
                if (node->getOpacity() == 255){
                    node->setOpacity(0);
                    node->setGlobalZOrder(0);
                }
            } else if (nodeA->getTag() == COLLISION_TAG or nodeB->getTag() == COLLISION_TAG){
                m_emitter->stopSystem();
            } else if (nodeA->getTag() == PORTAL_TAG or nodeB->getTag() == PORTAL_TAG){
                isPortal = true;
            }
        }
        
        if (!isSpiderPortal) {
            if ((nodeA->getTag() == PORTAL_TAG || nodeB->getTag() == PORTAL_TAG) &&
                    (nodeA->getTag() == FALLING_TAG || nodeB->getTag() == FALLING_TAG)) {
                isSpiderPortal = true;
                PhysicsBody* body;
                if (nodeA->getTag() == PORTAL_TAG)
                    body = contact.getShapeB()->getBody();
                else
                    body = contact.getShapeA()->getBody();
                if (body->getVelocity().x <= 0)
                    body->setVelocity(Vec2(100,100));
                else
                    body->setVelocity(Vec2(-100,-100));
            }
        }
    }
}


bool SecondScene::onContactBegin(const cocos2d::PhysicsContact& contact)
{
    if (!isRestart && !isNewLevel){
        auto nodeA = contact.getShapeA()->getBody()->getNode();
        auto nodeB = contact.getShapeB()->getBody()->getNode();
        if (nodeA && nodeB /*&& (nodeA->getTag() == HERO_SPRITE_TAG or nodeB->getTag() == HERO_SPRITE_TAG)*/)
        {
            if (nodeA->getTag() != PORTAL_TAG && nodeB->getTag() != PORTAL_TAG) {
                if (nodeA->getTag() == HERO_SPRITE_TAG or nodeB->getTag() == HERO_SPRITE_TAG) {
                    if (nodeA->getTag() == FALLING_TAG or nodeB->getTag() == FALLING_TAG) {
                        if (life_num > 0) {
                            life_num--;

                            char *res = new char[50];
                            std::sprintf(res, "life%i.png", life_num);
                            SpriteFrame *sp = SpriteFrameCache::getInstance()->getSpriteFrameByName(res);
                            mylife->setSpriteFrame(sp);
                            int colorint = 0;
                            Sprite* falling;
                            if (nodeA->getTag() == FALLING_TAG) {
                                colorint = std::stoi(nodeA->getName()) % 3;
                                falling = fallings.at(std::stoi(nodeA->getName()));
                            }
                            else {
                                colorint = std::stoi(nodeB->getName()) % 3;
                                falling = fallings.at(std::stoi(nodeB->getName()));

                            }
                            
                            falling->runAction(cocos2d::Sequence::create(TintTo::create(0.5f, 44, 215, 243), TintTo::create(0.5, 255, 255, 255),  NULL));

                            if (life_num == 0) {
                                mysprite->runAction(TintTo::create(1.0f, 243, 44, 239));
                                isRestart = true;
                                stopScene();
                                restartItem->setVisible(true);
                            } else
                                mysprite->runAction(cocos2d::Sequence::create(TintTo::create(0.5f, 243, 44, 239), TintTo::create(0.5, 255, 255, 255),  NULL));
                        }
                    }
                    else if (nodeA->getTag() == PLUS_TAG or nodeB->getTag() == PLUS_TAG) {
                        SpriteFrame *sp = SpriteFrameCache::getInstance()->getSpriteFrameByName("plussplash.png");
                        Node* node;
                        if (nodeA->getTag() == PLUS_TAG) node = nodeA;
                        else node = nodeB;
                        if (node->getOpacity() == 255) {
                            auto plus = pluses.at(std::stoi(node->getName()));
                            if (plus->getNumberOfRunningActions() > 0){
                                plus->stopAllActions();
                            }
                            if(life_num < 3) {
                                life_num++;
                                char *res = new char[50];
                                std::sprintf(res, "life%i.png", life_num);
                                SpriteFrame *splife = SpriteFrameCache::getInstance()->getSpriteFrameByName(res);
                                mylife->setSpriteFrame(splife);
                                
                                plus->setSpriteFrame(sp);
                                plus->setGlobalZOrder(3);
                                mysprite->runAction(cocos2d::Sequence::create(TintTo::create(0.5f, 252, 255, 0), TintTo::create(0.5, 255, 255, 255),  NULL));
                            }
                        }
                        return false;
                    }
                    else  if (nodeA->getTag() != NEWLEVEL_TAG and nodeB->getTag() != NEWLEVEL_TAG){

                        this->m_emitter->setPosition(contact.getContactData()->points[0]);
                        m_emitter->resetSystem();
                    } else {
                        this->m_emitter = ParticleSmoke::create();
                        this->m_emitter->setPosition(contact.getContactData()->points[0]);
                        m_emitter->resetSystem();
                        isNewLevel = true;
                        return false;
                    }
                }
                else if (nodeA->getTag() == FALLING_TAG or nodeB->getTag() == FALLING_TAG) {
                    if (nodeA->getTag() == COLLISION_TAG) {
                        auto collision = collisions.at(std::stoi(nodeA->getName()));
                        setTintBy(collision, std::stoi(nodeB->getName()) % 3);
                        
                    }
                    else if (nodeB->getTag() == COLLISION_TAG) {
                        auto collision = collisions.at(std::stoi(nodeB->getName()));

                        setTintBy(collision, std::stoi(nodeA->getName()) % 3);
                    }
                    
                }
            }
            else if (isPortal or isSpiderPortal){
                std::string portal_name;
                Node* node;
                if (nodeA->getTag() == PORTAL_TAG){
                    portal_name = nodeA->getName();
                    node = nodeB;
                }
                else{
                    portal_name = nodeB->getName();
                    node = nodeA;
                }

                node->runAction(cocos2d::Sequence::create(TintTo::create(0.0f, 0, 255, 144), TintTo::create(0.5, 255, 255, 255),  NULL));
                long pos = portal_name.length() - 1;
                int num = 1;
                if (portal_name.at(pos) == '1') num = 2;
                std::string res = portal_name.substr(0, pos).append(std::to_string(num));
               // cocos2d::log("start = %s, stop = %s", portal_name.c_str(), res.c_str());
                Vec2 nextPortalPos = Vec2(0,0);
                Vec2 dPos = Vec2(0,0);
                for (auto portal : portals){
                    if (portal->getName() == res){
                        nextPortalPos = portal->getPosition();
                        auto dPos = Vec2(nextPortalPos.x - node->getPosition().x, 0);
                       // stopAllObjects();
                        
                        if (nodeA->getTag() == HERO_SPRITE_TAG or nodeB->getTag() == HERO_SPRITE_TAG){
                            stopAllObjects();
                            direction = NODIRECTION;
                            stopAllObjects();
                            touchX = -500000;
                            touchY = -500000;
                            isPortal = false;
                            node->setPosition(nextPortalPos);
                        }
                        else if (isSpiderPortal){
                            isSpiderPortal = false;
                            node->setPositionX(nextPortalPos.x);
                            node->setPositionY(nextPortalPos.y - portal->getContentSize().height*scale_map);
                        }

                        break;
                     }
                }

            }
            

        }
    }
    return true;
}

void SecondScene::setTintBy(Sprite* collision, int i)
{
    int dr = 0;
    int dg = 0;
    int db = 0;
    if (i == 0) {
        dr = 255;
        dg = 0;
        db = 0;
    }
    if (i == 2) {
        dr = 150;
        dg = 0;
        db = 255;
    }
    if (i == 1) {
        dr = 0;
        dg = 0;
        db = 255;
    }
    
//    auto sequence = cocos2d::Sequence::create(FadeTo::create(0.0f, 40),TintBy::create(1.5, dr, dg, db), FadeTo::create(0.0f, 0),  NULL);
    auto sequence = cocos2d::Sequence::create(FadeTo::create(0.0f, 40),TintTo::create(0.5, dr, dg, db), FadeTo::create(0.2f, 0),  NULL);
    collision->runAction(sequence);
    
}

void SecondScene::setPositionOffsetAllObjectLayer(Vector<Sprite*> sprites, Vec2 offset)
{
    for (auto sprite : sprites){
        sprite->setPosition(Vec2(sprite->getPosition().x + offset.x, sprite->getPosition().y + offset.y));
    }
}

void SecondScene::moveAllObjectLayer(Vector<Sprite*> sprites, Vec2 offset)
{
    for (auto sprite : sprites){
        sprite->runAction(RepeatForever::create(MoveBy::create(2*ANIMATION_DELAY, offset)));
    }
}

void SecondScene::stopAllObjectLayer(Vector<Sprite*> sprites)
{
    for (auto sprite : sprites){
        if (sprite->getNumberOfRunningActions() > 0){
            sprite->stopAllActions();
        }
    }
}

bool SecondScene::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
    if (!isRestart && !isNewLevel){
        touchX = touch->getLocation().x;
        touchY = touch->getLocation().y;

        auto dx = touch->getLocation().x - mysprite->getPositionX();
        auto dy = touch->getLocation().y - mysprite->getPositionY() + mysprite->getContentSize().height*scale_map/2;
        direction = NODIRECTION;
        goToPoint(dx, dy);
    }

    
    
    return true;
}

void SecondScene::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{
    if (!isRestart && !isNewLevel && isPortal){
        touchX = touch->getLocation().x;
        touchY = touch->getLocation().y;
    }
}

void SecondScene::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
    direction = NODIRECTION;
    stopAllObjects();
    touchX = -500000;
    touchY = -500000;
}

void SecondScene::onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *event) {
    onTouchEnded(touch, event);
}

void SecondScene::goToPoint(float dx, float dy)
{
    if((touchX != -500000)&&(touchY!=-500000)){
        float vx_old = mysprite->getPhysicsBody()->getVelocity().x;
        float vy_old = mysprite->getPhysicsBody()->getVelocity().y;
        bool isChangedDirection = false;
        float vx = 0.0;
        float vy = 0.0;
        if (dx > 0) vx = MY_VELOCITY*scale_map;
        else vx = -MY_VELOCITY*scale_map;
        
        if (dy > 0) vy = MY_VELOCITY*scale_map;
        else vy = -MY_VELOCITY*scale_map;
        
        auto body = mysprite->getPhysicsBody();
        auto pos = body->getPositionOffset();
        
        if (std::abs(dx) > std::abs(dy)){
            if (dx == 0) vy = 0.0;
            else vy = vy * std::abs(dy/dx);


            if ((std::abs(vy_old - vy) >= MIN_FOR_DIRECTION)||(std::abs(vx_old - vx) >= MIN_FOR_DIRECTION)){
                if (dx > 0) {
                    if (direction != RIGHT) {
                        stopAllObjects();
                        auto action = RepeatForever::create(animateRight);
                        action->setTag(RIGHT);
                        isChangedDirection = true;
                        direction = RIGHT;
                        mysprite->runAction(action);
                        body->setRotationOffset(0);
                        if (pos.x <= 0 ) {
                            pos.y = 0;
                            pos.x = mysprite->getContentSize().width*scale_map*2/3;
                        }
                        body->setPositionOffset(pos);
                            
                    }
                }
                else {
                    if (direction != LEFT) {
                        stopAllObjects();
                        auto action = RepeatForever::create(animateLeft);
                        action->setTag(LEFT);
                        isChangedDirection=true;
                        direction = LEFT;
                        mysprite->runAction(action);
                        mysprite->getPhysicsBody()->setRotationOffset(0);
                        if (pos.x >= 0) {
                            pos.y = 0;
                            pos.x = -mysprite->getContentSize().width*scale_map*2/3;
                        }
                        body->setPositionOffset(pos);

                        
                    }
                }
            }
        }
        else {
            if (dy == 0) vx = 0.0;
            else vx = vx * std::abs(dx/dy);
            if ((std::abs(vy_old - vy) >= MIN_FOR_DIRECTION)||(std::abs(vx_old - vx) >= MIN_FOR_DIRECTION)){
                if (dy < 0) {
                    if (direction != BOTTOM) {
                        stopAllObjects();
                        isChangedDirection=true;
                        direction = BOTTOM;
                        auto action = RepeatForever::create(animateBottom);
                        action->setTag(BOTTOM);
                        mysprite->runAction(action);
                        body->setRotationOffset(90);
                        if (pos.y >= 0 ) {
                            pos.y = -mysprite->getContentSize().width*scale_map*2/3;
                            pos.x = 0;
                        }
                        body->setPositionOffset(pos);
                    }
                }
                else {
                    if (direction != TOP) {
                        stopAllObjects();
                        isChangedDirection=true;
                        direction = TOP;
                        auto action = RepeatForever::create(animateTop);
                        action->setTag(TOP);
                        mysprite->runAction(action);
//                        mysprite->runAction(RepeatForever::create(animateTop));
                        body->setRotationOffset(90);
                        if (pos.y <= 0 ) {
                            pos.y = mysprite->getContentSize().width*scale_map*2/3;
                            pos.x = 0;
                        }
                        body->setPositionOffset(pos);
                    }
                }
                
            }
        }
        if (isChangedDirection){
            mysprite->getPhysicsBody()->setVelocity(Vec2(vx,vy));
        }
    }
    else {
        stopAllObjects();
        mysprite->getPhysicsBody()->setVelocity(Vec2(0,0));
    }

}

void SecondScene::stopScene()
{
    stopAllObjects();

    for (auto sprite: fallings) {
        sprite->getPhysicsBody()->setVelocity(Vec2(0,0));
        sprite->getPhysicsBody()->setGravityEnable(false);
    }
    stopAllObjectLayer(portals);
    stopAllObjectLayer(fallings);
    stopAllObjectLayer(pluses);
}

void SecondScene::stopAllObjects()
{
    if (mysprite->getNumberOfRunningActions() > 0){
        mysprite->stopActionByTag(TOP);
        mysprite->stopActionByTag(BOTTOM);
        mysprite->stopActionByTag(LEFT);
        mysprite->stopActionByTag(RIGHT);
        
        mysprite->getPhysicsBody()->setVelocity(Vec2(0,0));
    }
    if (map->getNumberOfRunningActions() > 0)
        map->stopAllActions();
    for (auto c : collisions) {
        c->getPhysicsBody()->setVelocity(Vec2(0,0));
    }
}

void SecondScene::menuRestartCallback(Ref *pSender)
{
    auto newScene = SecondScene::createScene();
    cocos2d::Director::getInstance()->replaceScene(newScene);
    restartItem->setVisible(false);
}

void SecondScene::menuNewLeveltCallback(Ref *pSender)
{
    auto newScene = ThirdScene::createScene();
    cocos2d::Director::getInstance()->replaceScene(newScene);
    newlevelItem->setVisible(false);
}

void SecondScene::menuCloseCallback(Ref* pSender)
{
    auto newScene = MenuScene::createScene();
    cocos2d::Director::getInstance()->replaceScene(newScene);
    newlevelItem->setVisible(false);
}