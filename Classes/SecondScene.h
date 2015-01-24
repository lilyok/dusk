//
//  SecondScene.h
//  Dusk
//
//  Created by lilil on 08.01.15.
//
//

#ifndef __Dusk__SecondScene__
#define __Dusk__SecondScene__

#include "cocos2d.h"
#define LEFT 0
#define RIGHT 1
#define TOP 2
#define BOTTOM 3
#define NODIRECTION -1

class SecondScene : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    void menuRestartCallback(cocos2d::Ref* pSender);
    void menuNewLeveltCallback(cocos2d::Ref *pSender);
    // implement the "static create()" method manually
    CREATE_FUNC(SecondScene);
protected:
    cocos2d::TMXTiledMap *map;
    //cocos2d::TMXObjectGroup *walls;
    cocos2d::Sprite* mysprite;
    cocos2d::Sprite* minisprite;
    cocos2d::Sprite* mylife;
    
    cocos2d::Animate* animateBottom;
    cocos2d::Animate* animateLeft;
    cocos2d::Animate* animateRight;
    cocos2d::Animate* animateTop;
    cocos2d::Vector<cocos2d::Sprite*> collisions;
    cocos2d::Vector<cocos2d::Sprite*> portals;
    cocos2d::Vector<cocos2d::Sprite*> fallings;
    cocos2d::Vector<cocos2d::Sprite*> pluses;
    cocos2d::ParticleSystemQuad* m_emitter;
    
private:
    virtual void onEnter();
    virtual void onExit();
    
    void update(float delta) override;
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *event);
    
    void setPhyWorld(cocos2d::PhysicsWorld* world){m_world = world;}
    cocos2d::Vector<cocos2d::Sprite*>  makeObject(int tag, cocos2d::TMXObjectGroup *objects,
            cocos2d::SpriteFrameCache* spritecache, std::string name, int spritecount,
            int animsize, float scale_map, float xZero, float yZero, int form, float time,  bool isDynamic, float rest,
            float fric, float dens, int mask = 0xFFFFFFFF);
    cocos2d::Vector<cocos2d::Sprite*> makeObject(int tag, cocos2d::TMXObjectGroup *objects, float scale_map, float xZero, float yZero, int form, int v = 0, int n = -1, int mask = 0xFFFFFFFF);
    cocos2d::Sprite* makePhysicsObjAt(int tag, cocos2d::Point p, cocos2d::Size size, bool isDynamic, float rest, float fric, float dens, int form, cocos2d::Animate* anim, std::string name, int mask = 0xFFFFFFFF);
    cocos2d::Sprite* makePhysicsObjAt(int tag, cocos2d::Point p, cocos2d::Size size, int form, int v = 0, int n = -1, int mask = 0xFFFFFFFF);
    
    void setTintBy(cocos2d::Sprite* collision, int i);
    void setPositionOffsetAllObjectLayer(cocos2d::Vector<cocos2d::Sprite*> sprites, cocos2d::Vec2 offset);
    void moveAllObjectLayer(cocos2d::Vector<cocos2d::Sprite*> sprites, cocos2d::Vec2 offset);
    void stopAllObjectLayer(cocos2d::Vector<cocos2d::Sprite*> sprites);
    void stopAllObjects();
    void stopScene();
    void goToPoint(float dx, float dy);
    
    bool onContactBegin(const cocos2d::PhysicsContact& contact);
    void onContactSeperate(const cocos2d::PhysicsContact& contact);
    bool isRestart = false;
    bool isNewLevel = false;
    int direction = NODIRECTION;
    float touchX = -500000;
    float touchY = -500000;
    bool isPortal = true;
    bool isSpiderPortal = true;
    float scale_map = 1.0;
    float scale_mini_map = 1.0;
    int life_num = 3;
    cocos2d::Point mini_position;
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
    cocos2d::MenuItemImage* restartItem;
    cocos2d::MenuItemImage* newlevelItem;
    
    
    cocos2d::PhysicsWorld* m_world;
    cocos2d::Vector<Node*> pausedNodes;
};


#endif /* defined(__Dusk__SecondScene__) */
