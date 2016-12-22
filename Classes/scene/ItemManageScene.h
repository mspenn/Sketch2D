#pragma once

#include <vector>

#include "cocos2d.h"
#include "ui/CocosGUI.h"

#include "gesture/GeometricRecognizer.h"
#include "DrawableSprite.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace std;

/*
    GestureItem is the model of gusture type for ItemManageScene
*/
class GestureItem
{
  public:
    enum Type
    {
        MESH = 1 << 0,
        MARK = 1 << 1
    };

    GestureItem(const std::string &str, GestureItem::Type type);
    ~GestureItem();
    Button *getSprite() { return _itemSprite; }
    const Type _type;
    string _name;

    /**
     * get the name of the gesture from file
    **/
    static const string getGestureName(const string &fileName)
    {
        int end = fileName.find_last_of(".");
        int first = fileName.find_last_of("/");
        return fileName.substr(first + 1, end - first - 1);
    }

  protected:

    /**
     * sprite of gesture 
     **/
    Button *_itemSprite;
};

/**
 *  Gesture Select List Layer
 **/
class ItemListLayer : public Layer
{

  public:
    ItemListLayer();
    ~ItemListLayer();
    virtual bool init();
    CREATE_FUNC(ItemListLayer);

    /**
     * load the gesture item to the list 
     **/
    void addItem(GestureItem &item);

    /**
     * clear all in the gesture item list for reloading
     **/
    void clearAllItem();

  protected:
    ui::CheckBox *_meshCheckBox;
    ui::CheckBox *_markCheckBox;
    ui::ListView *_scrollView;
    
    /**
     * The current selected index of the gesture in the list. 
     **/
    int _current_index;

    vector<GestureItem> _itemList;
    int _scoller_y;

    ui::CheckBox *initCheckBox(const std::string &str, const Vec2 &pos);
    
    /**
     * Invoke the method when the meshcheckBox is triggered. 
     **/
    void meshcheckBoxListener(cocos2d::Ref *ref, ui::CheckBox::EventType type);

    /**
     *  Invoke the method when the markcheckBox is triggered. 
     **/
    void markcheckBoxListener(cocos2d::Ref *ref, ui::CheckBox::EventType type);

    /**
     * filter the gesture list base on what user select on the checkbox above.
     **/
    void filter(GestureItem::Type itemType, ui::CheckBox::EventType type);

    /**
     * load the gesture images from files. 
     **/
    void loadItems();

    /**
     * When we select a gesture type, we need to send event to the GestureShowLayer
       so that it can load the samples of corresponding gesture type. 
     **/
    void selectedItemEvent(Ref *pSender, ListView::EventType type);


    string getGestureName(string &fileName);
};

/*Event name of loading samples of a specified gesture type*/
#define EVENT_ON_LOAD_GESTURE "EVENT_ON_LOAD_GESTURE"


/**
 *  Gesture Sample show Layer
 **/
class GestureShowLayer : public Layer
{
  public:
    enum STATE
    {
        NONE = 0,
        INIT = 1,
        NEW = 2,
        NORMAL = 3
    };

    GestureShowLayer();
    ~GestureShowLayer();
    virtual bool init();
    CREATE_FUNC(GestureShowLayer);

    /**
     * 
     * This method is invoked when user select a gesture type in the list on the left 
     **/
    void onLoadGesture(cocos2d::EventCustom *event);


    /**
     * loading samples of a gesture type. 
     **/
    bool loadCurrentGestureSample();
    
    DrawableSprite *createGestureSprite(DollarRecognizer::MultiStrokeGesture &gesture);

    /**
     * save the gesture sample the use draw to the file. 
     **/
    bool saveGesture();

    /**
     * reset rhe current gesture sample so that user can rewrite the gesture. 
     **/
    void clearCurrentGestureSample();

    /**
     * delete the currently showing gesture sample. 
     **/
    bool removeCurrentGesture();

    /**
     * load the DrawableLayer so that user can write gesture on the board. 
     **/
    void loadDrawableLayer();
    EventListenerCustom *_loadGestureListener;

  private:
    void changeState(STATE s);
    void updateButtonStatus();
    ui::ListView *_gestureListView;
    ui::ImageView *_gestureBackgroundView;
    ui::Text *_description;
    STATE _state;
    // gesture
    string _current_gesture;

    vector<pair<string, DollarRecognizer::MultiStrokeGesture>> _gestureList;
    vector<pair<string, DollarRecognizer::MultiStrokeGesture>>::iterator _itr;
    Menu *_gestureOperationMenu;
    MenuItemImage *_itemPrevButton;
    MenuItemImage *_itemNextButton;
    MenuItemImage *_itemAddButton;
};

class ItemManageScene : public Layer
{

  public:
    static Scene *createScene();
    ItemManageScene();
    ~ItemManageScene();
    CREATE_FUNC(ItemManageScene);

  protected:
    ItemListLayer *_itemListLayer;
    GestureShowLayer *_gestureShowLayer;
};