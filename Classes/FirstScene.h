#ifndef __FirstScene_SCENE_H__
#define __FirstScene_SCENE_H__

#include "cocos2d.h"

class FirstScene : public cocos2d::Layer
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
    CREATE_FUNC(FirstScene);

protected:
    cocos2d::Sprite* mysprite;
    cocos2d::Animate* animate;
    cocos2d::Animate* animateLeft;
    cocos2d::Animate* animateRight;
    cocos2d::Animate* animateSplash;
    

private:
    virtual void onEnter();
    virtual void onExit();
    
    void update(float delta) override;
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    void setPhyWorld(cocos2d::PhysicsWorld* world){m_world = world;}
    void makeObject(int tag, cocos2d::TMXObjectGroup *objects, cocos2d::SpriteFrameCache* spritecache, std::string name, int spritecount, int animsize, cocos2d::Point origin, float scale_map, float yZero, int form, float rest, int mask = 0xFFFFFFFF);
    void makeObject(int tag, cocos2d::TMXObjectGroup *objects, cocos2d::Point origin, float scale_map, float yZero, int form, int v = 0, int n = -1, int mask = 0xFFFFFFFF);
    void makePhysicsObjAt(int tag, cocos2d::Point p, cocos2d::Size size, float r, float f, float dens, float rest, int form, cocos2d::Animate* anim, std::string name, int mask = 0xFFFFFFFF);
    void makePhysicsObjAt(int tag, cocos2d::Point p, cocos2d::Size size, int form, int v = 0, int n = -1, int mask = 0xFFFFFFFF);
    bool onContactBegin(cocos2d::PhysicsContact& contact);
    bool isRestart = false;
    bool isNewLevel = false;
    float scale_map;
    //cocos2d::TMXTiledMap* map;
    cocos2d::MenuItemImage* restartItem;
    cocos2d::MenuItemImage* newlevelItem;
    
    
    cocos2d::PhysicsWorld* m_world;
    cocos2d::Vector<Node*> pausedNodes;
};

#endif // __FirstScene_SCENE_H__
