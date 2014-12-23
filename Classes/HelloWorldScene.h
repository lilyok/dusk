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
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

protected:
    cocos2d::Sprite* mysprite;
    cocos2d::Animate* animate;
    cocos2d::Animate* animateLeft;
    cocos2d::Animate* animateRight;
    
private:
    virtual void onEnter();
    virtual void onExit();
    
    void update(float delta) override;
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    void setPhyWorld(cocos2d::PhysicsWorld* world){m_world = world;}
    void makePhysicsObjAt(cocos2d::Point p, cocos2d::Size size, bool d, float r, float f, float dens, float rest, int boxId);

    cocos2d::PhysicsWorld* m_world;
};

#endif // __HELLOWORLD_SCENE_H__
