#include "HelloWorldScene.h"

USING_NS_CC;
#define HERO_SPRITE_TAG 5
#define BRICK_SPRITE_TAG 10
#define BALL_SPRITE_TAG 20
#define BRICK 1
#define BALL 2
Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();
    layer->setPhyWorld(scene->getPhysicsWorld());
    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
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
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));
    
    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    
    
    this->restartItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuRestartCallback, this));
    restartItem->setVisible(false);
    restartItem->setPosition(Vec2(origin.x + visibleSize.width/2 - closeItem->getContentSize().width/2 ,
                                origin.y + visibleSize.height/2 + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menuRestart = Menu::create(restartItem, NULL);
    menuRestart->setPosition(Vec2::ZERO);
    this->addChild(menuRestart, 1);

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
    // load the Sprite Sheet
    auto spritecache = SpriteFrameCache::getInstance();
    
    // the .plist file can be generated with any of the tools mentioned below
    spritecache->addSpriteFramesWithFile("sprites.plist");
    
    
    this->mysprite = Sprite::createWithSpriteFrameName("Thumbelina01.png");
    // position the sprite on the center of the screen
    mysprite->setTag(HERO_SPRITE_TAG);
    mysprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    auto myScale = visibleSize.width/5/mysprite->getContentSize().width;
    mysprite->setScale(myScale);
    // now lets animate the sprite we moved
    
    Vector<SpriteFrame*> animFrames;
    animFrames.reserve(2);
//    animFrames.pushBack(SpriteFrame::create("negar_cat.png", Rect(0,0,visibleSize.width,visibleSize.height)));
//    animFrames.pushBack(SpriteFrame::create("negar_cat_2.png", Rect(0,0,visibleSize.width,visibleSize.height)));
    
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
    Animation* animationLeft = Animation::createWithSpriteFrames(animLeftFrames, 0.1f);
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
    Animation* animationRight = Animation::createWithSpriteFrames(animRightFrames, 0.1f);
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
    Animation* animationSplash = Animation::createWithSpriteFrames(animSplashFrames, 0.1f);
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
    auto scale_map =visibleSize.width / s.width;
    map->setScale(scale_map);
    auto yZero = origin.y+ visibleSize.height - map->getContentSize().height;
    int n = ceil((s.height + visibleSize.height/2.0 - yZero*scale_map)/50.0);
    //int n_back = ceil((s.height + visibleSize.height/2.0 - yZero*scale_map)/20.0);

    map->setPosition(Vec2(origin.x, yZero));
    
    addChild(map, 0);
  
    auto moveBy = MoveBy::create(1, Vec2(0, -20/scale_map));
    
    auto backLayer = map->getLayer("back");
    backLayer->runAction(Repeat::create(moveBy, n)); //*50/20
    auto moveBy1 = MoveBy::create(1, Vec2(0, -50/scale_map));
    cocos2d::log("height %f, yZero %f, n %i", s.height, yZero, n);
    auto collisionsLayer = map->getLayer("collisionslayer");
    collisionsLayer->runAction(Repeat::create(moveBy1, n));

    TMXObjectGroup *walls = map->getObjectGroup("collisions");
    makeObject(walls, origin, scale_map, yZero, BRICK, 0.0f, false, -50, n);
    
    TMXObjectGroup *fallings = map->getObjectGroup("fallings");
    makeObject(fallings, origin, scale_map, yZero, BALL, 0.5f, true);

//    auto contactListener = EventListenerPhysicsContact::create();
//    contactListener->onContactBegin = CC_CALLBACK_1(HelloWorld::onContactBegin,
//                                                    this);
//    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener,
//                                                             this);
    this->scheduleUpdate();
    return true;
}

void HelloWorld::makeObject(TMXObjectGroup *objects, Point origin, float scale_map, float yZero, int form, float rest, bool is_dynamic, int v, int n)
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
            if (is_dynamic)
                this->makePhysicsObjAt(_point, _size, 0, 0.0f, 0.0f, rest, form);
            else
                this->makePhysicsObjAt(_point, _size, form, v, n);
        }
    }
    
}


void HelloWorld::makePhysicsObjAt(Point p, Size size, int form, int v, int n)
{
    auto sprite = Sprite::create();
    PhysicsBody* body;
    if (form == BRICK){
        body = PhysicsBody::createBox(size,
                                      PhysicsMaterial(0.1f, 1.0f, 0.0f));
        sprite->setTag(BRICK_SPRITE_TAG);
    } else if (form == BALL){
        body = PhysicsBody::createCircle(MAX(size.width, size.height)/2.0,
                                         PhysicsMaterial(0.1f, 1.0f, 0.0f));
        sprite->setTag(BALL_SPRITE_TAG);
    }

    body->setDynamic(false);
    body->setContactTestBitmask(0xFFFFFFFF);
    sprite->setPhysicsBody(body);
    sprite->setPosition(p);
    addChild(sprite, 1);
    
    
    auto moveBy2 = MoveBy::create(1, Vec2(0, v));
    if (n < 0)
        sprite->runAction(RepeatForever::create(moveBy2));
    else
        sprite->runAction(Repeat::create(moveBy2, n));
}

void HelloWorld::makePhysicsObjAt(Point p, Size size, float r, float f, float dens, float rest, int form)
{
    auto sprite = Sprite::create();
    PhysicsBody* body;
    if (form == BRICK){
        body = PhysicsBody::createBox(size,
                                       PhysicsMaterial(0.1f, 1.0f, 0.0f));
        sprite->setTag(BRICK_SPRITE_TAG);
    } else if (form == BALL){
        body = PhysicsBody::createCircle(MAX(size.width, size.height)/2.0,
                                      PhysicsMaterial(0.1f, 1.0f, 0.0f));
        sprite->setTag(BALL_SPRITE_TAG);
    }
    body->getShape(0)->setRestitution(rest);
    body->getShape(0)->setFriction(f);
    body->getShape(0)->setDensity(dens);
    body->setDynamic(true);
    body->setGravityEnable(true);
    body->setContactTestBitmask(0xFFFFFFFF);
    sprite->setPhysicsBody(body);
    sprite->setPosition(p);
    addChild(sprite, 1);
}

void HelloWorld::onEnter()
{
    Layer::onEnter();
    
    // Register Touch Event
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchOneByOne::create();
    
    listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
    
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void HelloWorld::onExit()
{
    // You don't need to unregister listeners here as new API
    // removes all linked listeners automatically in CCNode's destructor
    // which is the base class for all cocos2d DRAWING classes
    
    Layer::onExit();
}

void HelloWorld::update(float delta)
{
    if (mysprite->getNumberOfRunningActions() <= 0){
        if (isPaused)
        {
            this->pausedNodes = cocos2d::Director::getInstance()->getActionManager()->pauseAllRunningActions();
            restartItem->setVisible(true);
            mysprite->getPhysicsBody()->setGravityEnable(true);

//            mysprite->getPhysicsBody()->setDynamic(false);
        }
        else
            mysprite->runAction(RepeatForever::create(animate));
    }
    
}

bool HelloWorld::onContactBegin(cocos2d::PhysicsContact& contact)
{
    if (!isPaused){
        auto nodeA = contact.getShapeA()->getBody()->getNode();
        auto nodeB = contact.getShapeB()->getBody()->getNode();
        if (nodeA && nodeB)
        {
            if (nodeA->getTag() == HERO_SPRITE_TAG or nodeB->getTag() == HERO_SPRITE_TAG)
            {
                if (mysprite->getNumberOfRunningActions() > 0){
                    mysprite->stopAllActions();
                }
                mysprite->runAction(Repeat::create(animateSplash, 1));
                isPaused = true;
                
            }
        }
    }
    //bodies can collide
    return true;
}

bool HelloWorld::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
    if (!isPaused){
        if (mysprite->getNumberOfRunningActions() > 0){
            mysprite->stopAllActions();
        }
        
  //      Size visibleSize = Director::getInstance()->getVisibleSize();
        
        if (touch->getLocationInView().x < mysprite->getPositionX()){
            // Move a sprite 50 pixels to the right, and 0 pixels to the top over 2 seconds.
            
            mysprite->runAction(Repeat::create(animateLeft,2));
            mysprite->getPhysicsBody()->setVelocity(Vec2(-100,0));
            
        } else {
            mysprite->runAction(Repeat::create(animateRight,2));
            mysprite->getPhysicsBody()->setVelocity(Vec2(100,0));
        }
    }
    cocos2d::log("You touched %f, %f", touch->getLocationInView().x, touch->getLocationInView().y);
        
    return true;
}

void HelloWorld::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{
    
}

void HelloWorld::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{

    
}

void HelloWorld::menuRestartCallback(Ref *pSender)
{
    isPaused = true;
    auto newScene = HelloWorld::createScene();
    cocos2d::Director::getInstance()->replaceScene(newScene);
//    cocos2d::Director::getInstance()->getActionManager()->resumeTargets(this->pausedNodes);
    restartItem->setVisible(false);
    //mysprite->getPhysicsBody()->setGravityEnable(false);
    //mysprite->getPhysicsBody()->setDynamic(true);

}

void HelloWorld::menuCloseCallback(Ref* pSender)
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
