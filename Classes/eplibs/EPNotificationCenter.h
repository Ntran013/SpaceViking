//
//  EPNotificationCenter.h
//  EnjoyYourMoney
//
//  Created by Erawppa on 2011/8/30.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//
#ifndef __EPNOTIFICATIONCENTER_H__
#define __EPNOTIFICATIONCENTER_H__

#include "cocos2d.h"

class EPNotificationCenter : public cocos2d::CCObject
{
public:
    EPNotificationCenter();
    ~EPNotificationCenter();
    
    static EPNotificationCenter *sharedNotifCenter(void);
	static void purgeSharedNotifCenter();
    
    void addObserver(cocos2d::SelectorProtocol *target, 
                     cocos2d::SEL_CallFuncO selector,
                     const char *name,
                     cocos2d::CCObject *obj);
    
    void removeObserver(cocos2d::SelectorProtocol *target,const char *name);
    
    void postNotification(const char *name);
    void postNotification(const char *name, cocos2d::CCObject *object);
    
private:
    //
    // internal functions
    //
    bool observerExisted(cocos2d::SelectorProtocol *target,const char *name);
    
    //
    // variables
    //
    cocos2d::CCArray *m_observers;
};

class EPNotificationObserver : public cocos2d::CCObject
{
public:
    EPNotificationObserver(cocos2d::SelectorProtocol *target, 
                           cocos2d::SEL_CallFuncO selector,
                           const char *name,
                           cocos2d::CCObject *obj);
    ~EPNotificationObserver();      
    
    void performSelector(cocos2d::CCObject *obj);
private:
    CC_PROPERTY_READONLY(cocos2d::SelectorProtocol *, m_target, Target);
    CC_PROPERTY_READONLY(cocos2d::SEL_CallFuncO, m_selector, Selector);
    CC_PROPERTY_READONLY(char *, m_name, Name);
    CC_PROPERTY_READONLY(cocos2d::CCObject *, m_object, Object);
};

#endif//__EPNOTIFICATIONCENTER_H__