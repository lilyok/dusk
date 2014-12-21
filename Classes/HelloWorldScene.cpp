#include "HelloWorldScene.h"

USING_NS_CC;

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
    spritecache->addSpriteFramesWithFile("tgirl.plist");
    
    
    this->mysprite = Sprite::createWithSpriteFrameName("Thumbelina01.png");
    // position the sprite on the center of the screen
    mysprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    mysprite->setScale(0.5f);
    // now lets animate the sprite we moved
    
    Vector<SpriteFrame*> animFrames;
    animFrames.reserve(4);
    animFrames.pushBack(spritecache->getSpriteFrameByName("Thumbelina01.png"));
    animFrames.pushBack(spritecache->getSpriteFrameByName("Thumbelina02.png"));
    animFrames.pushBack(spritecache->getSpriteFrameByName("Thumbelina03.png"));
    animFrames.pushBack(spritecache->getSpriteFrameByName("Thumbelina04.png"));

    
    // create the animation out of the frames
    Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
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
    
    auto physicsBody = PhysicsBody::createCircle(mysprite->getContentSize().width/4);
    physicsBody->setDynamic(true);
    //set the body isn't affected by the physics world's gravitational force
    physicsBody->setGravityEnable(false);
    
    
    physicsBody->setLinearDamping(0.5f);
    
    // run it and repeat it forever
    mysprite->runAction(RepeatForever::create(animate));

    // add the sprite as a child to this layer
    addChild(this->mysprite);
    mysprite->setPhysicsBody(physicsBody);

    this->scheduleUpdate();
    return true;
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
        mysprite->runAction(RepeatForever::create(animate));
    }
}

bool HelloWorld::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
    
    if (mysprite->getNumberOfRunningActions() > 0){
        mysprite->stopAllActions();
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();
    if (touch->getLocationInView().x < visibleSize.width/2){
        // Move a sprite 50 pixels to the right, and 0 pixels to the top over 2 seconds.
        
        mysprite->runAction(Repeat::create(animateLeft,2));
        mysprite->getPhysicsBody()->setVelocity(Vec2(-100,0));

    } else {
        mysprite->runAction(Repeat::create(animateRight,2));
        mysprite->getPhysicsBody()->setVelocity(Vec2(100,0));
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
