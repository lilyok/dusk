#ifndef __ThirdScene_SCENE_H__
#define __ThirdScene_SCENE_H__

#include "cocos2d.h"

class ThirdScene : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
//    virtual void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags);

    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    void menuRestartCallback(cocos2d::Ref* pSender);
    void menuNewLeveltCallback(cocos2d::Ref *pSender);
    // implement the "static create()" method manually
    CREATE_FUNC(ThirdScene);

protected:
    cocos2d::Sprite* mysprite;
//    cocos2d::Sprite* rainhorn;
    
    cocos2d::Animate* animateFalling;
   // cocos2d::Animate* animateWaiting;
    cocos2d::Animate* animateGo;
    float myScale = 1.0;

private:
    virtual void onEnter();
    virtual void onExit();
    
    void update(float delta) override;
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    void setPhyWorld(cocos2d::PhysicsWorld* world){m_world = world;}

    
    bool onContactBegin(cocos2d::PhysicsContact& contact);
    bool isRestart = false;
    bool isNewLevel = false;
    float scale_map;
    //cocos2d::TMXTiledMap* map;
    cocos2d::MenuItemImage* restartItem;
    cocos2d::MenuItemImage* newlevelItem;
    
    cocos2d::PhysicsWorld* m_world;
    cocos2d::Vector<Node*> pausedNodes;
    

    cocos2d::DrawNode* draw_red_node;
    cocos2d::DrawNode* draw_orange_node;
    cocos2d::DrawNode* draw_yellow_node;
    cocos2d::DrawNode* draw_green_node;
    cocos2d::DrawNode* draw_aqua_node;
    cocos2d::DrawNode* draw_blue_node;
    cocos2d::DrawNode* draw_magenta_node;

    
    
    void drawRainbow(cocos2d::Vec2 last_pos, cocos2d::Vec2 pos);

    
    
//    std::vector<std::vector<cocos2d::Vec2>> rainbows;
//    cocos2d::Vector<float> touchXs;
//    cocos2d::Vector<float> touchYs;
    
//    float touchX = -500000;
//    float touchY = -500000;
};

#endif // __ThirdScene_SCENE_H__
