//
//  SecondScene.cpp
//  Dusk
//
//  Created by lilil on 08.01.15.
//
//

#include "SecondScene.h"
#include "MenuScene.h"

USING_NS_CC;
#define HERO_SPRITE_TAG 5
#define COLLISION_TAG 10
#define BRICK 1
#define BALL 2
#define ANIMATION_DELAY 0.1f
#define NEWLEVEL_TAG 40

Scene* SecondScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
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
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
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
    
    /////////////////////////////
    // 3. add your codes below...
    
    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = LabelTTF::create("Hello World", "Arial", 24);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));
    
    // add the label as a child to this layer
    this->addChild(label, 1);
    
    //////////////////////////////////////////////////
    //auto map = TMXTiledMap::create("test.tmx");
    auto map = TMXTiledMap::create("labirint.tmx");
    Size s = map->getContentSize();
    auto scale_mini_map = visibleSize.width / s.width/2;//*5/4;
    auto scale_map = visibleSize.height / s.height;
    map->setScale(scale_map);
    auto yZero = origin.y;
    map->setPosition(Vec2(origin.x - s.width*scale_map/2, yZero));
    addChild(map, 0);
    
//    auto backLayer = map->getLayer("background");

    TMXObjectGroup *walls = map->getObjectGroup("collisions");
    makeObject(COLLISION_TAG, walls, scale_map, origin.x - s.width*scale_map/2, yZero, BRICK, 0, 0);
    walls->setPositionOffset(Vec2(origin.x - s.width*scale_map/2, yZero));
    
//    TMXObjectGroup *fallings = map->getObjectGroup("fallings");
//    makeObject(FALLING_TAG, fallings, spidercache, "spider", spidersCount, spidersAnimSize, origin, scale_map, yZero, BALL, 0.5f);
    
//    TMXObjectGroup *limiter = map->getObjectGroup("limiter");
//    makeObject(LIMITER_TAG, limiter , origin, scale_map, yZero, BRICK, -COLLISION_V, n);
//    
//    TMXObjectGroup *newlevel = map->getObjectGroup("newlevel");
//    makeObject(NEWLEVEL_TAG, newlevel, origin, scale_map, yZero, BRICK, -COLLISION_V, n);
    
    
    //////////////////////////////////////////////////
    auto spidercache = SpriteFrameCache::getInstance();
    spidercache->addSpriteFramesWithFile("spider.plist");
    auto spidersCount = 3;
    auto spidersAnimSize = 2;
    
    // load the Sprite Sheet
    auto spritecache = SpriteFrameCache::getInstance();
    
    // the .plist file can be generated with any of the tools mentioned below
    spritecache->addSpriteFramesWithFile("tgirlgo.plist");
    
    
    this->mysprite = Sprite::createWithSpriteFrameName("ThumbelinaGo02left.png");
    // position the sprite on the center of the screen
    mysprite->setTag(HERO_SPRITE_TAG);
    mysprite->setPosition(Vec2(visibleSize.width/3*2 + origin.x, origin.y + mysprite->getContentSize().height));
    mysprite->setScale(scale_map);
    // now lets animate the sprite we moved
    
    
    Vector<SpriteFrame*> animLeftFrames;
    animLeftFrames.reserve(2);
    animLeftFrames.pushBack(spritecache->getSpriteFrameByName("ThumbelinaGo01left.png"));
    animLeftFrames.pushBack(spritecache->getSpriteFrameByName("ThumbelinaGo02left.png"));
    // create the animation out of the frames
    Animation* animationLeft = Animation::createWithSpriteFrames(animLeftFrames, ANIMATION_DELAY);
    animateLeft = Animate::create(animationLeft);
    animateLeft->retain();
    
    Vector<SpriteFrame*> animRightFrames;
    animRightFrames.reserve(2);
    animRightFrames.pushBack(spritecache->getSpriteFrameByName("ThumbelinaGo01right.png"));
    animRightFrames.pushBack(spritecache->getSpriteFrameByName("ThumbelinaGo02right.png"));
    // create the animation out of the frames
    Animation* animationRight = Animation::createWithSpriteFrames(animRightFrames, ANIMATION_DELAY);
    animateRight = Animate::create(animationRight);
    animateRight->retain();
    
    Vector<SpriteFrame*> animTopFrames;
    animTopFrames.reserve(2);
    animTopFrames.pushBack(spritecache->getSpriteFrameByName("ThumbelinaGo01top.png"));
    animTopFrames.pushBack(spritecache->getSpriteFrameByName("ThumbelinaGo02top.png"));
    // create the animation out of the frames
    Animation* animationTop = Animation::createWithSpriteFrames(animTopFrames, ANIMATION_DELAY);
    animateTop = Animate::create(animationTop);
    animateTop->retain();
    
    Vector<SpriteFrame*> animBottomFrames;
    animBottomFrames.reserve(2);
    animBottomFrames.pushBack(spritecache->getSpriteFrameByName("ThumbelinaGo01bottom.png"));
    animBottomFrames.pushBack(spritecache->getSpriteFrameByName("ThumbelinaGo02bottom.png"));
    // create the animation out of the frames
    Animation* animationBottom = Animation::createWithSpriteFrames(animBottomFrames, ANIMATION_DELAY);
    animateBottom = Animate::create(animationBottom);
    animateBottom->retain();
    
    auto w = mysprite->getContentSize().width;
    auto h = mysprite->getContentSize().height;
    auto physicsBody = PhysicsBody::createBox(Size(w*scale_map*2/3, w*scale_map*2/3), PhysicsMaterial(0.0f, 0.0f, 0.0f), Vec2(0, -h*scale_map/2 + w*scale_map/3));
    physicsBody->setRotationEnable(false);
    physicsBody->setDynamic(true);
    physicsBody->setContactTestBitmask(0xFFFFFFFF);
    //set the body isn't affected by the physics world's gravitational force
    physicsBody->setGravityEnable(false);
    
    // run it and repeat it forever
    
    // add the sprite as a child to this layer
    addChild(this->mysprite, 1);
    mysprite->setPhysicsBody(physicsBody);
    
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactPostSolve = CC_CALLBACK_1(SecondScene::PhysicsContactPostSolve,
                                                    this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener,
                                                             this);
    this->scheduleUpdate();
    return true;
}

void SecondScene::makeObject(int tag, TMXObjectGroup *objects, SpriteFrameCache* spritecache, std::string name, int spritecount, int animsize, float scale_map, float xZero, float yZero, int form, float rest, int mask)
{
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
            Animate *animate = Animate::create(Animation::createWithSpriteFrames(spiderFrames, 0.5f));
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
            this->makePhysicsObjAt(tag, _point, _size, 0, 0.5f, 0.5f, rest, form, anim.at(i%spritecount), res, mask);
            i++;
        }
    }
    
}



void SecondScene::makeObject(int tag, TMXObjectGroup *objects, float scale_map, float xZero, float yZero, int form, int v, int n, int mask)
{
    if (objects != nullptr)
    {
        float x, y, w, h;
        ValueVector objectsPoint = objects->getObjects();
        for(auto objPointMap : objectsPoint)
        {
            ValueMap objPoint = objPointMap.asValueMap();
            x = objPoint.at("x").asFloat();
            y = objPoint.at("y").asFloat();
            w = objPoint.at("width").asFloat();
            h = objPoint.at("height").asFloat();
            
            Point _point = Point(xZero+scale_map*(x + w / 2.0f), yZero+scale_map*(y+h/2.0f));
            Size _size = Size(scale_map*w, scale_map*h);
            this->makePhysicsObjAt(tag, _point, _size, form, v, n, mask);
        }
    }
    
}

void SecondScene::makePhysicsObjAt(int tag, Point p, Size size, int form, int v, int n, int mask)
{
    auto sprite = Sprite::create();
    PhysicsBody* body;
    if (form == BRICK){
        body = PhysicsBody::createBox(size,
                                      PhysicsMaterial(0.0f, 0.0f, 0.0f));
    } else if (form == BALL){
        body = PhysicsBody::createCircle(MAX(size.width, size.height)/2.0,
                                         PhysicsMaterial(0.1f, 1.0f, 0.0f));
    }
    sprite->setTag(tag);
    body->setDynamic(false);
    body->setContactTestBitmask(mask);
    sprite->setPhysicsBody(body);
    sprite->setPosition(p);
    addChild(sprite, 1);
    
    
    auto moveBy2 = MoveBy::create(1, Vec2(0, v));
    if (n < 0)
        sprite->runAction(RepeatForever::create(moveBy2));
    else
        sprite->runAction(Repeat::create(moveBy2, n));
}

void SecondScene::makePhysicsObjAt(int tag, Point p, Size size, float r, float f, float dens, float rest, int form, Animate* anim, std::string name, int mask)
{
    auto sprite = Sprite::createWithSpriteFrameName(name);
    auto sprite_scale = size.width/sprite->getContentSize().width;
    sprite->setScale(sprite_scale*1.5);
    sprite->runAction(RepeatForever::create(anim));
    PhysicsBody* body;
    if (form == BRICK){
        body = PhysicsBody::createBox(size,
                                      PhysicsMaterial(0.0f, 0.0f, 0.0f));
    } else if (form == BALL){
        body = PhysicsBody::createCircle(MAX(size.width, size.height)/2.0,
                                         PhysicsMaterial(0.1f, 1.0f, 0.0f));
    }
    sprite->setTag(tag);
    body->getShape(0)->setRestitution(rest);
    body->getShape(0)->setFriction(f);
    body->getShape(0)->setDensity(dens);
    body->setDynamic(true);
    //body->setGravityEnable(true);
    body->setContactTestBitmask(mask); //(0xFFFFFFFF);
    sprite->setPhysicsBody(body);
    sprite->setPosition(p);
    
    addChild(sprite, 1);
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
    
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
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
    if (mysprite->getNumberOfRunningActions() <= 0){
        if (isRestart || isNewLevel)
        {
            this->pausedNodes = cocos2d::Director::getInstance()->getActionManager()->pauseAllRunningActions();
            if (isRestart)
            {
                restartItem->setVisible(true);
                mysprite->getPhysicsBody()->setGravityEnable(true);
            }
            else
                newlevelItem->setVisible(true);
            
        }
//        else
//            mysprite->runAction(RepeatForever::create(animateLeft));
    }
    
}

void SecondScene::PhysicsContactPostSolve(cocos2d::PhysicsContact& contact)
{
    if (!isRestart && !isNewLevel){
        auto nodeA = contact.getShapeA()->getBody()->getNode();
        auto nodeB = contact.getShapeB()->getBody()->getNode();
        if (nodeA && nodeB && (nodeA->getTag() == HERO_SPRITE_TAG or nodeB->getTag() == HERO_SPRITE_TAG))
        {
//            if (mysprite->getNumberOfRunningActions() > 0){
//                mysprite->stopAllActions();
//            }
            
            if (nodeA->getTag() == NEWLEVEL_TAG || nodeB->getTag() == NEWLEVEL_TAG)
            {
                isNewLevel = true;
            }

        }
    }
    //bodies can collide
   // return true;
}

bool SecondScene::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
    cocos2d::log("You touched %f, %f", touch->getLocation().x, touch->getLocation().y);
    cocos2d::log("tgirl %f, %f", mysprite->getPositionX(), mysprite->getPositionY());
    
    if (!isRestart && !isNewLevel){
        if (mysprite->getNumberOfRunningActions() > 0){
            mysprite->stopAllActions();
        }
        auto dx = touch->getLocation().x - mysprite->getPositionX();
        auto dy = touch->getLocation().y - mysprite->getPositionY();
        auto step = 10;
        auto num_step = 10;
        if (abs(dx) > abs(dy)){
            if (dx > 0) {
                mysprite->runAction(Repeat::create(animateRight,num_step));
                mysprite->runAction(Repeat::create(MoveBy::create(2*ANIMATION_DELAY, Vec2(step, 0)), num_step));
            }
            else {
                mysprite->runAction(Repeat::create(animateLeft,num_step));
                mysprite->runAction(Repeat::create(MoveBy::create(2*ANIMATION_DELAY, Vec2(-step, 0)), num_step));
            }
        }
        else {
            if (dy < 0) {
                mysprite->runAction(Repeat::create(animateBottom,num_step));
                mysprite->runAction(Repeat::create(MoveBy::create(2*ANIMATION_DELAY, Vec2(0, -step)), num_step));            }
            else {
                mysprite->runAction(Repeat::create(animateTop,num_step));
                mysprite->runAction(Repeat::create(MoveBy::create(2*ANIMATION_DELAY, Vec2(0, step)), num_step));
            }
        }
    }

    
    
    return true;
}

void SecondScene::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{
    
}

void SecondScene::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
    
    
}

void SecondScene::menuRestartCallback(Ref *pSender)
{
    auto newScene = SecondScene::createScene();
    cocos2d::Director::getInstance()->replaceScene(newScene);
    restartItem->setVisible(false);
}

void SecondScene::menuNewLeveltCallback(Ref *pSender)
{
    auto newScene = MenuScene::createScene();
    cocos2d::Director::getInstance()->replaceScene(newScene);
    newlevelItem->setVisible(false);
}

void SecondScene::menuCloseCallback(Ref* pSender)
{
    auto newScene = MenuScene::createScene();
    cocos2d::Director::getInstance()->replaceScene(newScene);
    newlevelItem->setVisible(false);
}