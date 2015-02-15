#include "ThirdScene.h"
#include "MenuScene.h"
#include "SecondScene.h"

USING_NS_CC;
#define HERO_SPRITE_TAG 5
#define RAINHORN_SPRITE_TAG 30

#define COLLISION_TAG 10
#define FALLING_TAG 20
#define NEWLEVEL_TAG 40
#define BRICK 1
#define BALL 2
#define COLLISION_V 150.0
#define BACKGROUND_V 90.0
#define RAINBOW_LENGTH 10.0
#define RAINBOW_POINT_COUNT 100

Scene* ThirdScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();
    //scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    // 'layer' is an autorelease object
    auto layer = ThirdScene::create();
    layer->setPhyWorld(scene->getPhysicsWorld());
    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool ThirdScene::init()
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
                                           CC_CALLBACK_1(ThirdScene::menuCloseCallback, this));
    
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
                                           CC_CALLBACK_1(ThirdScene::menuRestartCallback, this));
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
                                              CC_CALLBACK_1(ThirdScene::menuNewLeveltCallback, this));
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
    draw_red_node = DrawNode::create();
    draw_orange_node = DrawNode::create();
    draw_yellow_node = DrawNode::create();
    draw_green_node = DrawNode::create();
    draw_aqua_node = DrawNode::create();
    draw_blue_node = DrawNode::create();
    draw_magenta_node = DrawNode::create();
    
    addChild(draw_red_node, 1, COLLISION_TAG);
    addChild(draw_orange_node, 2, COLLISION_TAG);
    addChild(draw_yellow_node, 3, COLLISION_TAG);
    addChild(draw_green_node, 4, COLLISION_TAG);
    addChild(draw_aqua_node, 5, COLLISION_TAG);
    addChild(draw_blue_node, 6, COLLISION_TAG);
    addChild(draw_magenta_node, 7, COLLISION_TAG);
    
    //////////////////////////////////////////////////

    
    auto bottom_sprite = Sprite::create("white_pixel.png");
    
    bottom_sprite->getTexture()->setTexParameters({.minFilter =  GL_LINEAR, .magFilter =  GL_LINEAR, .wrapS =  GL_REPEAT, .wrapT =  GL_REPEAT});
    bottom_sprite->setTextureRect(Rect(0, 0, visibleSize.width, visibleSize.height/50));
    bottom_sprite->setOpacity(0);
    PhysicsBody* body;
    
    
    body = PhysicsBody::createBox(Size(visibleSize.width, 20),
                                      PhysicsMaterial(1.0f, 1.0f, 0.0f));
    bottom_sprite->setPhysicsBody(body);
    //bottom_sprite->setTag(tag);
    body->setDynamic(false);
    bottom_sprite->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + bottom_sprite->getTextureRect().size.height/2));
    addChild(bottom_sprite, 1);
    
    ///////////////////////////////////////////////////////////////////////////////////////
    auto spritecache = SpriteFrameCache::getInstance();
    
    // the .plist file can be generated with any of the tools mentioned below
    spritecache->addSpriteFramesWithFile("start3.plist");
    
    
    this->mysprite = Sprite::createWithSpriteFrameName("tFalling01.png");
    mysprite->setTag(HERO_SPRITE_TAG);
    myScale = visibleSize.width/5/mysprite->getContentSize().width;
    mysprite->setScale(myScale);
    mysprite->setPosition(Vec2(origin.x + mysprite->getContentSize().width/2*myScale, visibleSize.height + origin.y));
    
    Vector<SpriteFrame*> animFalling;
    animFalling.reserve(2);
    animFalling.pushBack(spritecache->getSpriteFrameByName("tFalling02.png"));
    animFalling.pushBack(spritecache->getSpriteFrameByName("tFalling01.png"));

    
    // create the animation out of the frames
    Animation* animationFalling = Animation::createWithSpriteFrames(animFalling, 0.2f);
    animateFalling = Animate::create(animationFalling);
    animateFalling->retain();
    // now lets animate the sprite we moved
    
    auto physicsBody = PhysicsBody::createCircle(mysprite->getContentSize().width*myScale/8,
                                                 PhysicsMaterial(0.1f, 0.0f, 0.0f));
    physicsBody->setPositionOffset(Vec2(0, mysprite->getContentSize().width*myScale/4));
    physicsBody->setDynamic(true);
    physicsBody->setContactTestBitmask(0xFFFFFFFF);
    //set the body isn't affected by the physics world's gravitational force
    physicsBody->setGravityEnable(true);

    // run it and repeat it forever
    mysprite->runAction(RepeatForever::create(animateFalling));

    // add the sprite as a child to this layer
    addChild(this->mysprite, 2);
    mysprite->setPhysicsBody(physicsBody);
    
    
    Vector<SpriteFrame*> animGo;
    animGo.reserve(10);
    animGo.pushBack(spritecache->getSpriteFrameByName("thgo01.png"));
    animGo.pushBack(spritecache->getSpriteFrameByName("thgo02.png"));
    animGo.pushBack(spritecache->getSpriteFrameByName("thgo03.png"));
    animGo.pushBack(spritecache->getSpriteFrameByName("thgo02.png"));
    animGo.pushBack(spritecache->getSpriteFrameByName("thgo01.png"));
    animGo.pushBack(spritecache->getSpriteFrameByName("thgo04.png"));
    animGo.pushBack(spritecache->getSpriteFrameByName("thgo05.png"));
    animGo.pushBack(spritecache->getSpriteFrameByName("thgo06.png"));
    animGo.pushBack(spritecache->getSpriteFrameByName("thgo05.png"));
    animGo.pushBack(spritecache->getSpriteFrameByName("thgo04.png"));
    
    
    // create the animation out of the frames
    Animation* animationGo = Animation::createWithSpriteFrames(animGo, 0.2f);
    animateGo = Animate::create(animationGo);
    animateGo->retain();
    
    
//////////////////////////////////////////////////////////////////////////////////
    
    auto rainhorn = Sprite::createWithSpriteFrameName("hWaiting01.png");
    rainhorn->setTag(RAINHORN_SPRITE_TAG);
    rainhorn->setScale(myScale);
    rainhorn->setPosition(Vec2(origin.x + rainhorn->getContentSize().width/2*myScale, origin.y + rainhorn->getContentSize().height/2));
    
    Vector<SpriteFrame*> animWaiting;
    animWaiting.reserve(2);
    animWaiting.pushBack(spritecache->getSpriteFrameByName("hWaiting02.png"));
    animWaiting.pushBack(spritecache->getSpriteFrameByName("hWaiting01.png"));
    
    
    // create the animation out of the frames
    Animation* animationWaiting = Animation::createWithSpriteFrames(animWaiting, 0.2f);
    auto animateWaiting = Animate::create(animationWaiting);
    animateWaiting->retain();
    // now lets animate the sprite we moved
    //65
    auto hornBody = PhysicsBody::createBox(Size(rainhorn->getContentSize().width*myScale, 194*myScale), PhysicsMaterial(0.1f, 1.0f, 0.0f));
    hornBody->setPositionOffset(Vec2(0, (97-rainhorn->getContentSize().height/2)*myScale));
    hornBody->setDynamic(true);
    hornBody->setRotationEnable(false);
    
    hornBody->setContactTestBitmask(0xFFFFFFFF);
    //set the body isn't affected by the physics world's gravitational force
    hornBody->setGravityEnable(true);
    
    // run it and repeat it forever
    rainhorn->runAction(RepeatForever::create(animateWaiting));
    
    // add the sprite as a child to this layer
    addChild(rainhorn, 1);
    rainhorn->setPhysicsBody(hornBody);
    
    mysprite->setPositionX(rainhorn->getPositionX());
    ////////////////////////////////////////////////////////////////////////
    
    
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(ThirdScene::onContactBegin,
                                                    this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener,
                                                             this);
    this->scheduleUpdate();
    return true;
}


void ThirdScene::onEnter()
{
    Layer::onEnter();
    
    // Register Touch Event
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchOneByOne::create();
    
    listener->onTouchBegan = CC_CALLBACK_2(ThirdScene::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(ThirdScene::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(ThirdScene::onTouchEnded, this);
    
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void ThirdScene::onExit()
{
    // You don't need to unregister listeners here as new API
    // removes all linked listeners automatically in CCNode's destructor
    // which is the base class for all cocos2d DRAWING classes
    
    Layer::onExit();
}

void ThirdScene::update(float delta)
{

}

bool ThirdScene::onContactBegin(cocos2d::PhysicsContact& contact)
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
            else if (nodeB->getTag() == RAINHORN_SPRITE_TAG || nodeA->getTag() == RAINHORN_SPRITE_TAG)
            {
                Node* node = nodeA;
                if (nodeB->getTag() == RAINHORN_SPRITE_TAG)
                    node = nodeB;
                
                auto pos = node->getPosition();
                auto nwidth = node->getContentSize().width;
                if (node->getNumberOfRunningActions() > 0)
                    node->stopAllActions();
                node->removeFromParent();
                
                if (mysprite->getNumberOfRunningActions() > 0)
                    mysprite->stopAllActions();

                
                mysprite->setPosition(pos);
                mysprite->runAction(RepeatForever::create(animateGo));
                auto body = PhysicsBody::createBox(Size(nwidth*myScale, mysprite->getContentSize().height*myScale), PhysicsMaterial(0.1f, 1.0f, 0.0f));
                mysprite->setPhysicsBody(body);

                
                
            }
        }
    }
    //bodies can collide
    return true;
}

bool ThirdScene::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
    if (!isRestart && !isNewLevel) {

    }

        
    return true;
}


void ThirdScene::drawRainbow(Vec2 last_pos, Vec2 pos){
    auto body_top = PhysicsBody::createEdgeSegment(Vec2(0, 0), Vec2(pos.x - last_pos.x, pos.y - last_pos.y), PhysicsMaterial(0.1f, 1.0f, 0.0f), 15);
    body_top->setDynamic(false);
    auto sprite_top = Sprite::create();
    sprite_top->setPhysicsBody(body_top);
    
    addChild(sprite_top, 3, COLLISION_TAG);
    
    auto dx = (- last_pos.x + pos.x);
    auto dy = (- last_pos.y + pos.y);
    if (fabs(dx) > fabs(dy)) {
        dy = dy / fabs(dx);
        dx = dx / fabs(dx);
    } else {
        if (dy != 0) {
            dx = dx / fabs(dy);
            dy = dy / fabs(dy);
        } else {
            dx = 0.0;
            dy = 0.0;
        }
    }

    
    sprite_top->setPosition(Vec2(last_pos.x - 0*dx - 15*dy, last_pos.y - 0*dy - 15*dx));
 
    draw_red_node->drawSegment(Vec2(last_pos.x, last_pos.y), Vec2(pos.x, pos.y), 10, Color4F(1, 0, 0, 0.9));
    draw_orange_node->drawSegment(Vec2(last_pos.x - 5*dy, last_pos.y - 5*dx), Vec2(pos.x - 5*dy, pos.y - 5*dx), 10, Color4F(1, 0.5, 0, 0.9));
    draw_yellow_node->drawSegment(Vec2(last_pos.x - 10*dy , last_pos.y - 10*dx),
                                  Vec2(pos.x - 10*dy, pos.y - 10*dx), 10, Color4F(0.7, 1, 0, 0.9));
    draw_green_node->drawSegment(Vec2(last_pos.x - 15*dy, last_pos.y - 15*dx), Vec2(pos.x - 15*dy, pos.y - 15*dx), 10, Color4F(0, 1, 0, 0.9));
    draw_aqua_node->drawSegment(Vec2(last_pos.x - 20*dy, last_pos.y - 20*dx),
                                Vec2(pos.x - 20*dy, pos.y - 20*dx), 10, Color4F(0, 0.9, 1, 0.9));
    draw_blue_node->drawSegment(Vec2(last_pos.x - 25*dy, last_pos.y - 25*dx),
                                Vec2(pos.x - 25*dy, pos.y - 25*dx), 10, Color4F(0, 0.7, 1, 0.9));
    draw_magenta_node->drawSegment(Vec2(last_pos.x - 30*dy, last_pos.y - 30*dx),
                                   Vec2(pos.x - 30*dy, pos.y - 30*dx),10, Color4F(0.5, 0, 1, 0.9));
    
}

void ThirdScene::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{
    if (!isRestart && !isNewLevel) {
        drawRainbow(Vec2(touch->getPreviousLocation().x, touch->getPreviousLocation().y),
                    Vec2(touch->getLocation().x, touch->getLocation().y));
     }
}

void ThirdScene::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{

}

void ThirdScene::menuRestartCallback(Ref *pSender)
{
    auto newScene = ThirdScene::createScene();
    cocos2d::Director::getInstance()->replaceScene(newScene);
    restartItem->setVisible(false);
}

void ThirdScene::menuNewLeveltCallback(Ref *pSender)
{
    auto newScene = SecondScene::createScene();
    cocos2d::Director::getInstance()->replaceScene(newScene);
    newlevelItem->setVisible(false);
}

void ThirdScene::menuCloseCallback(Ref* pSender)
{
    auto newScene = MenuScene::createScene();
    cocos2d::Director::getInstance()->replaceScene(newScene);
    newlevelItem->setVisible(false);
}
