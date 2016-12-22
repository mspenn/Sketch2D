#include <iostream>

#include "ui/CocosGUI.h"
#include "cocos2d.h"

#include "ItemManageScene.h"
#include "resource/Resources.h"
#include "util/FileWalker.h"
#include "util/SampleFileIOHelper.h"
#include "scene/SceneManager.h"
#include "DrawableSprite.h"
#include "CanvasLayer.h"

using namespace cocos2d;
static cocos2d::Size designResolutionSize = cocos2d::Size(1024, 768);
/// ItemListLayer
ItemListLayer::ItemListLayer()
{
}

ItemListLayer::~ItemListLayer()
{
}

bool ItemListLayer::init()
{
    if (Layer::init())
    {
        setContentSize(CCSizeMake(designResolutionSize.width * 0.4, designResolutionSize.height));

        // score view
        _scrollView = ui::ListView::create();
        _scrollView->setBounceEnabled(true);
        _scrollView->setContentSize(Size(200.0f, getContentSize().height * 9 / 10 - 180));
        _scrollView->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        _scrollView->setPosition(Vec2(30, getContentSize().height - 30 - getContentSize().height * 2 / 10));
        _scrollView->setBackGroundImage("cocosui/gray.png");
        _scrollView->setBackGroundImageScale9Enabled(true);
        _scrollView->setDirection(ui::ScrollView::Direction::VERTICAL);
        _scrollView->setScrollBarWidth(4);
        _scrollView->setScrollBarPositionFromCorner(Vec2(7, 7));
        _scrollView->setScrollBarColor(Color3B::WHITE);
        _scrollView->setItemsMargin(40.0f);
        
        _current_index = 0;
        this->addChild(_scrollView);

        // check box
        _meshCheckBox = initCheckBox("mesh", Vec2(_scrollView->getPosition().x, getContentSize().height * 9 / 10));
        _meshCheckBox->addEventListener(CC_CALLBACK_2(ItemListLayer::meshcheckBoxListener, this));
        _meshCheckBox->setSelected(true);

        _markCheckBox = initCheckBox("mark", Vec2(_scrollView->getPosition().x, _meshCheckBox->getPosition().y - 50));
        _markCheckBox->addEventListener(CC_CALLBACK_2(ItemListLayer::markcheckBoxListener, this));
        _markCheckBox->setSelected(true);

        _scoller_y = _scrollView->getInnerContainerSize().height;
        std::cout << "add child!";
        loadItems();

        _scrollView->addEventListener((ui::ListView::ccListViewCallback)CC_CALLBACK_2(ItemListLayer::selectedItemEvent, this));
        return true;
    }
    return false;
}

void ItemListLayer::loadItems()
{

    vector<string> files;
    dir(RES_ITEM_DICT "/marker/*.png", files);

    for (string &fileName : files)
    {
        GestureItem item = GestureItem(RES_ITEM_DICT "/marker/" + fileName, GestureItem::MARK);

        _itemList.push_back(item);
        item.getSprite()->retain();
    }
    files.clear();

    dir(RES_ITEM_DICT "/mesh/*.png", files);
    for (string &fileName : files)
    {
        GestureItem item = GestureItem(RES_ITEM_DICT "/mesh/" + fileName, GestureItem::MESH);

        _itemList.push_back(item);
        item.getSprite()->retain();
    }

    for (auto &item : _itemList)
    {
        addItem(item);
    }
}

void ItemListLayer::markcheckBoxListener(cocos2d::Ref *ref, ui::CheckBox::EventType type)
{
    filter(GestureItem::Type::MARK, type);
}
void ItemListLayer::meshcheckBoxListener(cocos2d::Ref *ref, ui::CheckBox::EventType type)
{
    filter(GestureItem::Type::MESH, type);
}

void ItemListLayer::addItem(GestureItem &item)
{

    ui::Layout *custom_item = ui::Layout::create();
    custom_item->setContentSize(item.getSprite()->getContentSize());

    item.getSprite()->setPosition(Vec2(_scrollView->getContentSize().width / 2.0f, custom_item->getContentSize().height / 2.0f));
    custom_item->addChild(item.getSprite());

    ui::Text *name = Text::create(item._name, "fonts/Marker Felt.ttf", 20);
    name->setColor(Color3B::BLACK);
    name->setPosition(Vec2(_scrollView->getContentSize().width / 2.0f, 0));

    custom_item->addChild(name, name->getLocalZOrder(), "text");
    custom_item->setUserData(&item);
    custom_item->setColor(Color3B::BLUE);

    _scrollView->pushBackCustomItem(custom_item);
    _scoller_y -= 100;
}

void ItemListLayer::clearAllItem()
{
    _scoller_y = _scrollView->getInnerContainerSize().height;
    _scrollView->removeAllChildrenWithCleanup(false);
}

void ItemListLayer::filter(GestureItem::Type itemType, ui::CheckBox::EventType type)
{
    clearAllItem();
    uint16_t filter = ((_meshCheckBox->isSelected() << 0) & GestureItem::MESH) |
                      ((_markCheckBox->isSelected() << 1) & GestureItem::MARK);

    for (auto &item : _itemList)
    {
        if ((filter & item._type) != 0)
        {
            addItem(item);
        }
    }
}

ui::CheckBox *ItemListLayer::initCheckBox(const std::string &str, const Vec2 &pos)
{
    auto checkBox = ui::CheckBox::create(
        "cocosui/check_box_normal.png",
        "cocosui/check_box_normal_press.png",
        "cocosui/check_box_active.png",
        "cocosui/check_box_normal_disable.png",
        "cocosui/check_box_active_disable.png");
    checkBox->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    checkBox->setPosition(pos);

    ui::Text *text = ui::Text::create(str, "fonts/Marker Felt.ttf", 30);
    text->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    text->setPosition(Vec2(checkBox->getPosition().x + 50, checkBox->getPosition().y));

    addChild(checkBox);
    addChild(text);
    return checkBox;
}

void ItemListLayer::selectedItemEvent(Ref *pSender, ListView::EventType type)
{
    if (type == ListView::EventType::ON_SELECTED_ITEM_START)
    {
        EventCustom event(EVENT_ON_LOAD_GESTURE);

        auto lastItem = _scrollView->getItem(_current_index);
        if (lastItem != nullptr)
        {
            lastItem->getChildByName("text")->setColor(Color3B::BLACK);
        }
        _current_index = _scrollView->getCurSelectedIndex();
        auto item = _scrollView->getItem(_current_index);
        item->getChildByName("text")->setColor(Color3B::BLUE);
        GestureItem *gesture = static_cast<GestureItem *>(item->getUserData());

        event.setUserData((void *)&(
            gesture->_name));
        _eventDispatcher->dispatchEvent(&event);
    }
}

string ItemListLayer::getGestureName(string &fileName)
{
    int end = fileName.find_last_of(".");
    int first = fileName.find_last_of("/");
    return fileName.substr(first + 1, end - first - 1);
}

///////////////////////////////

GestureShowLayer::GestureShowLayer() : _state(GestureShowLayer::STATE::NONE)
{
}

GestureShowLayer::~GestureShowLayer()
{
}

bool GestureShowLayer::init()
{
    if (Layer::init())
    {

        //layer
        setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        setContentSize(Size(designResolutionSize.width * 0.6f, designResolutionSize.height));
        setPosition(Vec2(designResolutionSize.width * 0.3f, 60));

        // view
        _gestureListView = ui::ListView::create();
        _gestureListView->setBounceEnabled(true);
        _gestureListView->setContentSize(Size(700, 200));
        _gestureListView->setPosition(Vec2(30, getContentSize().height - 300));
        _gestureListView->setBackGroundImage("cocosui/green_edit.png");
        _gestureListView->setBackGroundImageScale9Enabled(true);
        _gestureListView->setDirection(ui::ScrollView::Direction::HORIZONTAL);
        _gestureListView->setScrollBarWidth(4);
        _gestureListView->setScrollBarPositionFromCorner(Vec2(7, 7));
        _gestureListView->setScrollBarColor(Color3B::GRAY);

        // listener
        _loadGestureListener = EventListenerCustom::create(EVENT_ON_LOAD_GESTURE, CC_CALLBACK_1(GestureShowLayer::onLoadGesture, this));
        _eventDispatcher->addEventListenerWithFixedPriority(_loadGestureListener, 1);

        // gesture
        _gestureBackgroundView = ImageView::create("gesture_background.png");
        _gestureBackgroundView->setContentSize(Size(450, 450));
        _gestureBackgroundView->setScale9Enabled(true);
        _gestureBackgroundView->setPosition(Vec2(getContentSize().width / 2.0f, _gestureListView->getPosition().y - 180));
        addChild(_gestureBackgroundView);

        //itr  button
        _itr = _gestureList.begin();
        _itemPrevButton = MenuItemImage::create(
            "cocosui/UITest/b1.png",
            "cocosui/UITest/b2.png",
            "cocosui/UITest/b3.png",
            [&](Ref *sender) {
                if (_itr != _gestureList.begin())
                {
                    _itr--;
                    loadCurrentGestureSample();
                    updateButtonStatus();
                }
            });
        _itemNextButton = MenuItemImage::create(
            "cocosui/UITest/f1.png",
            "cocosui/UITest/f2.png",
            "cocosui/UITest/f3.png",
            [&](Ref *sender) {
                if (_itr != _gestureList.end() && (_itr + 1) != _gestureList.end())
                {
                    _itr++;
                    loadCurrentGestureSample();
                    updateButtonStatus();
                }
            });
        updateButtonStatus();

        // add / remove button
        auto deleteButtom = MenuItemImage::create(
            "cocosui/UITest/remove.png",
            "cocosui/UITest/remove.png",

            [&](Ref *sender) {
                removeCurrentGesture();
            });
        deleteButtom->setContentSize(Size(49, 49));

        _itemAddButton = MenuItemImage::create("cocosui/UITest/add.png", "cocosui/UITest/r2.png",
                                               [&](Ref *sender) {
                                                   if (_state == STATE::NORMAL)
                                                   {
                                                       loadDrawableLayer();
                                                   }
                                                   else if (_state == STATE::NEW)
                                                   {
                                                       saveGesture();
                                                   }
                                               });

        // menu
        _gestureOperationMenu = Menu::create(_itemPrevButton, deleteButtom, _itemAddButton, _itemNextButton, nullptr);
        _gestureOperationMenu->alignItemsHorizontally();
        _gestureOperationMenu->setAnchorPoint(Vec2(0.5, 0.5));
        _gestureOperationMenu->setPosition(Vec2(230, 30));
        addChild(_gestureOperationMenu);

        // text
        _description = ui::Text::create("ff", "fonts/Marker Felt.ttf", 30);
        _description->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        _description->setPosition(Vec2(90, getContentSize().height * 8 / 10));
        addChild(_description);

        //state
        changeState(STATE::INIT);

        //end
        return true;
    }
    return false;
}

void GestureShowLayer::updateButtonStatus()
{
    bool prevEnabled = !(_itr == _gestureList.begin());
    if (_itemPrevButton->isEnabled() != prevEnabled)
    {
        _itemPrevButton->setEnabled(prevEnabled);
    }
    bool nextEnabled = !(_itr == _gestureList.end() || _itr == _gestureList.end() - 1);
    if (_itemNextButton->isEnabled() != nextEnabled)
    {
        _itemNextButton->setEnabled(nextEnabled);
    }
}

void GestureShowLayer::changeState(GestureShowLayer::STATE s)
{
    if (s != this->_state)
    {
        switch (s)
        {
        case STATE::INIT:
        {
            _gestureOperationMenu->setEnabled(false);
            _description->setText("select a gesture type from the left");
            break;
        }
        case STATE::NEW:
        {
            _itemAddButton->setNormalImage(Sprite::create("cocosui/UITest/save.png"));
            _gestureOperationMenu->setEnabled(true);
            _description->setText("draw a gesture on the board.");
            break;
        }
        case STATE::NORMAL:
        {
            _itemAddButton->setNormalImage(Sprite::create("cocosui/UITest/add.png"));
            if (_state == STATE::NEW)
            {
                _description->setText("review the gesture list");
            }
            else
            {
                _description->setText("review the gesture list");
            }
            _gestureOperationMenu->setEnabled(true);
        }
        }
        this->_state = s;
    }
}

void GestureShowLayer::onLoadGesture(cocos2d::EventCustom *event)
{
    vector<string> gestureFileList;

    // get path str
    _current_gesture = *static_cast<string *>(event->getUserData());
    string prefixPath = "gestures/" + GestureItem::getGestureName(_current_gesture) + "/";
    string searchPath = prefixPath + "*.ges";
    dir(searchPath.data(), gestureFileList);

    // load gesture list
    _gestureList.clear();
    for (auto &gestureFileName : gestureFileList)
    {
        DollarRecognizer::MultiStrokeGesture gesture;
        string gestureSamplePath = prefixPath + gestureFileName;
        SampleFileIOHelper::loadSample(gestureFileName, gestureSamplePath.data(), gesture);
        _gestureList.push_back(make_pair(gestureSamplePath.data(), gesture));
    }

    _itr = _gestureList.begin();
    updateButtonStatus();

    // load gesture sprite
    loadCurrentGestureSample();
    changeState(STATE::NORMAL);
}

#define DRAFT_GESTURE_NODE_NAME "DRAFT_GESTURE"

void GestureShowLayer::loadDrawableLayer()
{
    clearCurrentGestureSample();
    TrainingCanvasLayer *canvasLayer = TrainingCanvasLayer::create();
    canvasLayer->setContentSize(_gestureBackgroundView->getContentSize());
    canvasLayer->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    canvasLayer->setPosition(Vec2::ZERO);
    canvasLayer->setName(DRAFT_GESTURE_NODE_NAME);

    canvasLayer->setPosition(Vec2(_gestureBackgroundView->getContentSize()) / 2.0f);
    _gestureBackgroundView->addChild(canvasLayer);
    changeState(STATE::NEW);
}

DrawableSprite *GestureShowLayer::createGestureSprite(DollarRecognizer::MultiStrokeGesture &gesture)
{
    DrawableSprite *sprite = DrawableSprite::createWithMultiStrokeGesture(gesture);
    sprite->setContentSize(Size(400, 400));
    sprite->setScale(4);
    sprite->setBrushColor(Color4F::WHITE);
    sprite->setPosition(
        Vec2(_gestureBackgroundView->getContentSize().width / 2.0f,
             _gestureBackgroundView->getContentSize().height / 2.0f));
    sprite->redraw();
    return sprite;
}

bool GestureShowLayer::loadCurrentGestureSample()
{
    _gestureBackgroundView->removeAllChildren();

    if (_itr != _gestureList.end())
    {
        auto gestureSprite = createGestureSprite((*_itr).second);

        _gestureBackgroundView->addChild(gestureSprite);
    }
    return true;
}

bool GestureShowLayer::saveGesture()
{
    TrainingCanvasLayer *layer = dynamic_cast<TrainingCanvasLayer *>(
        _gestureBackgroundView->getChildByName("DRAFT_GESTURE"));
    if (layer != nullptr)
    {
        DollarRecognizer::MultiStrokeGesture gesture;
        layer->getDrawNode()->getMultiStrokeGesture(gesture);
        string prefixPath = "gestures/" + GestureItem::getGestureName(_current_gesture) + "/";

        char no[10];
        sprintf(no, "%d", _gestureList.size());
        string savePath = prefixPath + string(no) + ".ges";
        SampleFileIOHelper::storeSample(gesture, savePath.data());

        //reload the gesture
        SampleFileIOHelper::loadSample(savePath, savePath.data(), gesture);

        _gestureList.push_back(make_pair(savePath, gesture));
        _itr = _gestureList.end() - 1;
        updateButtonStatus();
        loadCurrentGestureSample();
        changeState(STATE::NORMAL);
        _description->setText("save success, review the gesture list");
    }

    return true;
}

bool GestureShowLayer::removeCurrentGesture()
{
    if (_state == STATE::NEW)
    {
        loadCurrentGestureSample();
        changeState(STATE::NORMAL);
    }
    else
    {
        if (_itr != _gestureList.end())
        {
            //delete file
            wchar_t *wc = new wchar_t[(*_itr).first.size()];
            swprintf(wc, L"%S", (*_itr).first.c_str());
            DeleteFile(wc);
            _description->setText("remove success, review the gesture list");
            _itr = _gestureList.erase(_itr);
            loadCurrentGestureSample();
            updateButtonStatus();
        }
    }
    return true;
}

void GestureShowLayer::clearCurrentGestureSample()
{
    _gestureBackgroundView->removeAllChildren();
}

//////////////////////////////////
GestureItem::GestureItem(const std::string &str, GestureItem::Type type) : _type(type)
{
    _itemSprite = Button::create(str);
    _name = string(getGestureName(str));
}

GestureItem::~GestureItem()
{
}

/////////////////////////////

ItemManageScene::ItemManageScene()
{
}

ItemManageScene::~ItemManageScene()
{
}

Scene *ItemManageScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    // return the scene
    ItemListLayer *itemListLayer = ItemListLayer::create();
    itemListLayer->setColor(Color3B::BLUE);
    itemListLayer->setPosition(Point(0, 0));
    GestureShowLayer *gestureShowLayer = GestureShowLayer::create();
    // gestureShowLayer->setPosition(Point(scene->getContentSize().width * 2/ 5.0f , scene->getContentSize().height * 7 / 10.0f));
    scene->addChild(itemListLayer);
    scene->addChild(gestureShowLayer);
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto _labelBack = Label::create("Back", "fonts/Marker Felt.ttf", 32, Size::ZERO, TextHAlignment::LEFT, TextVAlignment::CENTER);
    auto backItem = MenuItemLabel::create(_labelBack, [](Ref *pSender) {
        auto scene = SceneManager::GetMenuScene();
        Director::getInstance()->replaceScene(scene);
    });
    backItem->setPosition(Vec2(visibleSize.width + origin.x - 100, origin.y + 90));
    auto menu = Menu::create(backItem, nullptr);
    menu->setPosition(Vec2::ZERO);
    scene->addChild(menu);

    return scene;
}