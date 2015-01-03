#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class HelloWorld : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    

    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    void menuRestartCallback(cocos2d::Ref* pSender);
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

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
    void makeObject(cocos2d::TMXObjectGroup *objects, cocos2d::Point origin, float scale_map, float yZero, int form, float rest, bool is_dynamic, int v = 0, int n = -1);
    void makePhysicsObjAt(cocos2d::Point p, cocos2d::Size size, float r, float f, float dens, float rest, int form);
    void makePhysicsObjAt(cocos2d::Point p, cocos2d::Size size, int form, int v = 0, int n = -1);
    bool onContactBegin(cocos2d::PhysicsContact& contact);
    bool isPaused = false;
    
    cocos2d::MenuItem* restartItem;
    cocos2d::PhysicsWorld* m_world;
    cocos2d::Vector<Node*> pausedNodes;
};

#endif // __HELLOWORLD_SCENE_H__
