#include "FirstScene.h"
#include "MenuScene.h"
#include "SecondScene.h"

USING_NS_CC;
#define HERO_SPRITE_TAG 5
#define COLLISION_TAG 10
#define FALLING_TAG 20
#define LIMITER_TAG 30
#define NEWLEVEL_TAG 40
#define BRICK 1
#define BALL 2
#define COLLISION_V 150.0
#define BACKGROUND_V 90.0

Scene* FirstScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();
  //  scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    // 'layer' is an autorelease object
    auto layer = FirstScene::create();
    layer->setPhyWorld(scene->getPhysicsWorld());
    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool FirstScene::init()
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

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(FirstScene::menuCloseCallback, this));
    
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
                                           CC_CALLBACK_1(FirstScene::menuRestartCallback, this));
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
                                              CC_CALLBACK_1(FirstScene::menuNewLeveltCallback, this));
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
    auto spidercache = SpriteFrameCache::getInstance();
    spidercache->addSpriteFramesWithFile("spider.plist");
    auto spidersCount = 3;
    auto spidersAnimSize = 2;
    
    // load the Sprite Sheet
    auto spritecache = SpriteFrameCache::getInstance();
    
    // the .plist file can be generated with any of the tools mentioned below
    spritecache->addSpriteFramesWithFile("tgirl.plist");
    
    
    this->mysprite = Sprite::createWithSpriteFrameName("Thumbelina01.png");
    // position the sprite on the center of the screen
    mysprite->setTag(HERO_SPRITE_TAG);
    mysprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    auto myScale = visibleSize.width/5/mysprite->getContentSize().width;
    mysprite->setScale(myScale);
    // now lets animate the sprite we moved
    
    Vector<SpriteFrame*> animFrames;
    animFrames.reserve(4);
    animFrames.pushBack(spritecache->getSpriteFrameByName("Thumbelina01.png"));
    animFrames.pushBack(spritecache->getSpriteFrameByName("Thumbelina02.png"));
    animFrames.pushBack(spritecache->getSpriteFrameByName("Thumbelina03.png"));
    animFrames.pushBack(spritecache->getSpriteFrameByName("Thumbelina04.png"));

    
    // create the animation out of the frames
    Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.2f);
    animate = Animate::create(animation);
    animate->retain();
    // now lets animate the sprite we moved
    
    Vector<SpriteFrame*> animLeftFrames;
    animLeftFrames.reserve(5);
    animLeftFrames.pushBack(spritecache->getSpriteFrameByName("ThumbelinaLeft01.png"));
    animLeftFrames.pushBack(spritecache->getSpriteFrameByName("ThumbelinaLeft02.png"));
    animLeftFrames.pushBack(spritecache->getSpriteFrameByName("ThumbelinaLeft03.png"));
    animLeftFrames.pushBack(spritecache->getSpriteFrameByName("ThumbelinaLeft02.png"));
    animLeftFrames.pushBack(spritecache->getSpriteFrameByName("ThumbelinaLeft01.png"));
  

    // create the animation out of the frames
    Animation* animationLeft = Animation::createWithSpriteFrames(animLeftFrames, 0.2f);
    animateLeft = Animate::create(animationLeft);
    animateLeft->retain();
    
    // now lets animate the sprite we moved
    
    Vector<SpriteFrame*> animRightFrames;
    animRightFrames.reserve(5);
    animRightFrames.pushBack(spritecache->getSpriteFrameByName("ThumbelinaRight01.png"));
    animRightFrames.pushBack(spritecache->getSpriteFrameByName("ThumbelinaRight02.png"));
    animRightFrames.pushBack(spritecache->getSpriteFrameByName("ThumbelinaRight03.png"));
    animRightFrames.pushBack(spritecache->getSpriteFrameByName("ThumbelinaRight02.png"));
    animRightFrames.pushBack(spritecache->getSpriteFrameByName("ThumbelinaRight01.png"));
    
    // create the animation out of the frames
    Animation* animationRight = Animation::createWithSpriteFrames(animRightFrames, 0.2f);
    animateRight = Animate::create(animationRight);
    animateRight->retain();
    
    
    Vector<SpriteFrame*> animSplashFrames;
    animSplashFrames.reserve(8);
    animSplashFrames.pushBack(spritecache->getSpriteFrameByName("splash01.png"));
    animSplashFrames.pushBack(spritecache->getSpriteFrameByName("splash01a.png"));
    animSplashFrames.pushBack(spritecache->getSpriteFrameByName("splash01b.png"));
    animSplashFrames.pushBack(spritecache->getSpriteFrameByName("splash02.png"));
    animSplashFrames.pushBack(spritecache->getSpriteFrameByName("splash02a.png"));
    animSplashFrames.pushBack(spritecache->getSpriteFrameByName("splash02b.png"));
    animSplashFrames.pushBack(spritecache->getSpriteFrameByName("splash03.png"));
    animSplashFrames.pushBack(spritecache->getSpriteFrameByName("splash03a.png"));
    
    // create the animation out of the frames
    Animation* animationSplash = Animation::createWithSpriteFrames(animSplashFrames, 0.2f);
    animateSplash = Animate::create(animationSplash);
    animateSplash->retain();
    
    
    auto physicsBody = PhysicsBody::createCircle(mysprite->getContentSize().width*myScale/2,
                                                 PhysicsMaterial(0.1f, 1.0f, 0.0f));
    physicsBody->setDynamic(true);
    physicsBody->setContactTestBitmask(0xFFFFFFFF);
    //set the body isn't affected by the physics world's gravitational force
    physicsBody->setGravityEnable(false);
    
    
    physicsBody->setLinearDamping(0.5f);
    
    // run it and repeat it forever
    mysprite->runAction(RepeatForever::create(animate));

    // add the sprite as a child to this layer
    addChild(this->mysprite, 1);
    mysprite->setPhysicsBody(physicsBody);

    
    //auto map = TMXTiledMap::create("test.tmx");

    
    auto map = TMXTiledMap::create("holeTile.tmx");
    Size s = map->getContentSize();
    scale_map =visibleSize.width / s.width;
    
    map->setScale(scale_map);
    auto yZero = origin.y+ visibleSize.height - map->getContentSize().height*scale_map;
    float cvelocity = COLLISION_V*scale_map;
    int n = ceil(s.height*scale_map/cvelocity);
    map->setPosition(Vec2(origin.x, origin.y));//yZero));
   //cocos2d: scale_map = 2.500000, sheight 1280.000000, vheight = 1136.000000, yZero -144.000000, n 45, n1 15
    addChild(map, 0);
  
    auto moveBy = MoveBy::create(1, Vec2(0, -BACKGROUND_V));
    
    auto backLayer = map->getLayer("back");
    backLayer->runAction(Repeat::create(moveBy, n));
    auto collisionMoveBy = MoveBy::create(1, Vec2(0, -cvelocity/scale_map));
    cocos2d::log("height %f, yZero %f, n %i", s.height, yZero, n);
    auto collisionsLayer = map->getLayer("collisionslayer");
    collisionsLayer->runAction(Repeat::create(collisionMoveBy, n));
    
    auto crowdholeMoveBy = MoveBy::create(1, Vec2(0, -cvelocity/scale_map));
    auto crowdhole = map->getLayer("crowhole");
    crowdhole->runAction(Repeat::create(crowdholeMoveBy, n));
    auto crowdLayerMoveBy = MoveBy::create(1, Vec2(0, -cvelocity/scale_map));
    auto crowdLayer = map->getLayer("crowdlayer");
    crowdLayer->runAction(Repeat::create(crowdLayerMoveBy, n));
    crowdLayer->setGlobalZOrder(2);// setPositionZ(2);
  //  map->reorderChild(crowdLayer, 1);
    
    TMXObjectGroup *walls = map->getObjectGroup("collisions");
    makeObject(COLLISION_TAG, walls, origin, scale_map, 0, BRICK, -cvelocity, n);
    
    TMXObjectGroup *fallings = map->getObjectGroup("fallings");
    makeObject(FALLING_TAG, fallings, spidercache, "spider", spidersCount, spidersAnimSize, origin, scale_map, 0, BALL, 0.5f);

    TMXObjectGroup *limiter = map->getObjectGroup("limiter");
    makeObject(LIMITER_TAG, limiter , origin, scale_map, 0, BRICK, -cvelocity, n);
    
    TMXObjectGroup *newlevel = map->getObjectGroup("newlevel");
    makeObject(NEWLEVEL_TAG, newlevel, origin, scale_map, 0, BRICK, -cvelocity, n);
    
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(FirstScene::onContactBegin,
                                                    this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener,
                                                             this);
    this->scheduleUpdate();
    return true;
}

void FirstScene::makeObject(int tag, TMXObjectGroup *objects, SpriteFrameCache* spritecache, std::string name, int spritecount, int animsize, Point origin, float scale_map, float yZero, int form, float rest, int mask)
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
            
            Point _point = Point(origin.x+scale_map*(x + w / 2.0f), yZero+scale_map*(y+h/2.0f));
            Size _size = Size(scale_map*w, scale_map*h);
            char* res = new char[50];
            std::sprintf(res, "%s%02i.png", name.c_str(), (i%spritecount)*animsize + 1);
            this->makePhysicsObjAt(tag, _point, _size, 0, 0.5f, 0.5f, rest, form, anim.at(i%spritecount), res, mask);
            i++;
        }
    }
    
}



void FirstScene::makeObject(int tag, TMXObjectGroup *objects, Point origin, float scale_map, float yZero, int form, int v, int n, int mask)
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
            
            Point _point = Point(origin.x+scale_map*(x + w / 2.0f), yZero+scale_map*(y+h/2.0f));
            Size _size = Size(scale_map*w, scale_map*h);
            this->makePhysicsObjAt(tag, _point, _size, form, v, n, mask);
        }
    }
    
}

void FirstScene::makePhysicsObjAt(int tag, Point p, Size size, int form, int v, int n, int mask)
{
    auto sprite = Sprite::create();
    PhysicsBody* body;
    if (form == BRICK){
        body = PhysicsBody::createBox(size,
                                      PhysicsMaterial(0.1f, 1.0f, 0.0f));
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

void FirstScene::makePhysicsObjAt(int tag, Point p, Size size, float r, float f, float dens, float rest, int form, Animate* anim, std::string name, int mask)
{
    auto sprite = Sprite::createWithSpriteFrameName(name);
    
    auto sprite_scale = size.width/sprite->getContentSize().width;
    sprite->setScale(sprite_scale*1.5);
    sprite->runAction(RepeatForever::create(anim));
    PhysicsBody* body;
    if (form == BRICK){
        body = PhysicsBody::createBox(size,
                                       PhysicsMaterial(0.1f, 1.0f, 0.0f));
    } else if (form == BALL){
        body = PhysicsBody::createCircle(MAX(size.width, size.height)/2.0,
                                      PhysicsMaterial(0.1f, 1.0f, 0.0f));
    }
    sprite->setTag(tag);
    body->getShape(0)->setRestitution(rest);
    body->getShape(0)->setFriction(f);
    body->getShape(0)->setDensity(dens);
    body->setDynamic(true);
    body->setRotationEnable(false);
    //body->setGravityEnable(true);
    body->setContactTestBitmask(mask); //(0xFFFFFFFF);
    sprite->setPhysicsBody(body);
    sprite->setPosition(p);
    
    addChild(sprite, 1);
}

void FirstScene::onEnter()
{
    Layer::onEnter();
    
    // Register Touch Event
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchOneByOne::create();
    
    listener->onTouchBegan = CC_CALLBACK_2(FirstScene::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(FirstScene::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(FirstScene::onTouchEnded, this);
    
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void FirstScene::onExit()
{
    // You don't need to unregister listeners here as new API
    // removes all linked listeners automatically in CCNode's destructor
    // which is the base class for all cocos2d DRAWING classes
    
    Layer::onExit();
}

void FirstScene::update(float delta)
{
   // if (mysprite->getNumberOfRunningActions() <= 0){
        if (isRestart || isNewLevel)
        {
            if (mysprite->getNumberOfRunningActions() <= 0)
                this->pausedNodes = cocos2d::Director::getInstance()->getActionManager()->pauseAllRunningActions();
            if (isRestart)
            {
                restartItem->setVisible(true);
                
                mysprite->getPhysicsBody()->setGravityEnable(true);
            }
            else
                newlevelItem->setVisible(true);
            
        }
        else if (mysprite->getNumberOfRunningActions() <= 0)
            mysprite->runAction(RepeatForever::create(animate));
 //   }
}

bool FirstScene::onContactBegin(cocos2d::PhysicsContact& contact)
{
    if (!isRestart && !isNewLevel){
        auto nodeA = contact.getShapeA()->getBody()->getNode();
        auto nodeB = contact.getShapeB()->getBody()->getNode();
        if (nodeA && nodeB && (nodeA->getTag() == HERO_SPRITE_TAG or nodeB->getTag() == HERO_SPRITE_TAG))
        {
            if (nodeA->getTag() == NEWLEVEL_TAG || nodeB->getTag() == NEWLEVEL_TAG)
            {
                isNewLevel = true;
            }
            else if (nodeB->getTag() != LIMITER_TAG && nodeA->getTag() != LIMITER_TAG)
            {
                if (mysprite->getNumberOfRunningActions() > 0){
                    mysprite->stopAllActions();
                }
                mysprite->runAction(Repeat::create(animateSplash, 1));
                isRestart = true;
                
            }
        }
    }
    //bodies can collide
    return true;
}

bool FirstScene::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
    if (!isRestart && !isNewLevel){
        if (mysprite->getNumberOfRunningActions() > 0){
            mysprite->stopAllActions();
        }
        
        if (touch->getLocationInView().x < mysprite->getPositionX()){
            // Move a sprite 50 pixels to the right, and 0 pixels to the top over 2 seconds.
            
            mysprite->runAction(Repeat::create(animateLeft,2));
            mysprite->getPhysicsBody()->setVelocity(Vec2(-COLLISION_V*scale_map/1.5,0));
            
        } else {
            mysprite->runAction(Repeat::create(animateRight,2));
            mysprite->getPhysicsBody()->setVelocity(Vec2(COLLISION_V*scale_map/1.5,0));
        }
    }
    cocos2d::log("You touched %f, %f", touch->getLocationInView().x, touch->getLocationInView().y);
        
    return true;
}

void FirstScene::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{
    
}

void FirstScene::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{

    
}

void FirstScene::menuRestartCallback(Ref *pSender)
{
    auto newScene = FirstScene::createScene();
    cocos2d::Director::getInstance()->replaceScene(newScene);
    restartItem->setVisible(false);
}

void FirstScene::menuNewLeveltCallback(Ref *pSender)
{
    auto newScene = SecondScene::createScene();
    cocos2d::Director::getInstance()->replaceScene(newScene);
    newlevelItem->setVisible(false);
}

void FirstScene::menuCloseCallback(Ref* pSender)
{
    auto newScene = MenuScene::createScene();
    cocos2d::Director::getInstance()->replaceScene(newScene);
    newlevelItem->setVisible(false);
}
