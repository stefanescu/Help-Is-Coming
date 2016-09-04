#include "Player.h"
#include "globals.h"
#include <vector>

Player * Player::create()
{
    Player* pSprite = new Player();

    //if(pSprite->initWithSpriteFrameName("idle"))
    if (pSprite->initWithSpriteFrameName("p1_stand.png"))
    {
        pSprite->setAnchorPoint(Point::ZERO);
        pSprite->playerSize = Size(pSprite->getBoundingBox().size.width, pSprite->getBoundingBox().size.height);
        return pSprite;
    }
    return NULL;
}

Rect Player::getCollisionBox()
{
    Rect bbox = getBoundingBox();

    Rect rect = Rect(
                bbox.getMinX() + velocityX,
                bbox.getMinY() ,//+ 1.0f,
                bbox.size.width,
                bbox.size.height);
    return rect;
}

Rect Player::getUpperCollisionBox()
{
    Rect bbox = getBoundingBox();

    Rect rect = Rect(
                bbox.getMinX() + bbox.size.width * 0.33f,
                bbox.getMidY(),
                bbox.size.width * 0.4f,
                bbox.size.height * 1.5f);
    return rect;
}

bool Player::isGrounded ()
{
    return grounded;
}

void Player::setGrounded (bool n)
{
    grounded = n;
}

float Player::getVelocityX ()
{
    return velocityX;
}

float Player::getVelocityY ()
{
    return velocityY;
}

void Player::setVelocityX (float velX)
{
    velocityX = velX;
}

void Player::setVelocityY (float velY)
{
    velocityY = velY;
}

void Player::move (int dir)
{
    //this->stopAllActions();
    if (grounded)
        this->runAction(RepeatForever::create(walking));

    direction = dir;
    velocityX = dir > 0 ? PLAYER_MAX_VELOCITY : -PLAYER_MAX_VELOCITY;
    moving = true;
}

void Player::jump ()
{
    this->stopAllActions();
    if (grounded)
        setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("p1_jump.png"));

    velocityY = PLAYER_JUMP_VELOCITY;
    grounded = false;


}

Sprite* Player::shootLaser()
{
    auto projectile = Sprite::create("laserprojectile.png");
    projectile->retain();
    //auto travel = MoveBy::create(5, Vec2(50 * direction,0));
    //projectile->runAction(travel);
    return projectile;
}

void Player::fall()
{
    velocityY = -velocityY;
}

void Player::idle ()
{
    this->stopAllActions();
    this->runAction(RepeatForever::create(idling));

    velocityX = 0;
    //velocityY = 0;
    moving = false;
}

bool Player::isMoving()
{
    return moving;
}

Size Player::getPlayerSize()
{
    return playerSize;
}

void Player::applyGravity()
{
    velocityY -= GRAVITY;
}

void Player::updateState (float delta)
{
    if (direction == 1)
        setFlippedX(false);
    else
        setFlippedX(true);

    if (velocityX != 0)
    {
        setPositionX(getPositionX() + velocityX);
        //velocityX = 0;
    }
    if (velocityY != 0)
    {
        setPositionY(getPositionY() + velocityY);
        //applyGravity();
    }
}

/*void Player::setupAnimation(const char* name)
{
    walk = Animate::create( AnimationCache::getInstance()->getAnimation(name));
    walk->startWithTarget(this);
    walk->setDuration(0.80f);
    walk->retain();
}*/

Vector<SpriteFrame*> Player::buildAnimation(const char *format, int count)
{
    auto spritecache = SpriteFrameCache::getInstance();
    Vector<SpriteFrame*> animFrames;
    char str[100];
    for(int i = 1; i <= count; i++)
    {
        sprintf(str, format, i);
        animFrames.pushBack(spritecache->getSpriteFrameByName(str));
    }
    return animFrames;
}

int Player::getDirection()
{
    return direction;
}

Player::Player(void)
{
    velocityX = 0;
    velocityY = 0;
    direction = 0;
    grounded = true;
    moving = false;

    grounded = true;
    moving = false;

    //walk = NULL;
    setScale(0.3f);
    /*walkFrames.reserve(11);
    walkFrames.pushBack(SpriteFrame::create("Base pack/Player/p1_walk/PNG/p1_walk03.png", Rect(0,0,72,97)));
    walkFrames.pushBack(SpriteFrame::create("Base pack/Player/p1_walk/PNG/p1_walk02.png", Rect(0,0,72,97)));
    walkFrames.pushBack(SpriteFrame::create("Base pack/Player/p1_walk/PNG/p1_walk01.png", Rect(0,0,72,97)));
    walkFrames.pushBack(SpriteFrame::create("Base pack/Player/p1_walk/PNG/p1_walk04.png", Rect(0,0,72,97)));
    walkFrames.pushBack(SpriteFrame::create("Base pack/Player/p1_walk/PNG/p1_walk05.png", Rect(0,0,72,97)));
    walkFrames.pushBack(SpriteFrame::create("Base pack/Player/p1_walk/PNG/p1_walk06.png", Rect(0,0,72,97)));
    walkFrames.pushBack(SpriteFrame::create("Base pack/Player/p1_walk/PNG/p1_walk07.png", Rect(0,0,72,97)));
    walkFrames.pushBack(SpriteFrame::create("Base pack/Player/p1_walk/PNG/p1_walk08.png", Rect(0,0,72,97)));
    walkFrames.pushBack(SpriteFrame::create("Base pack/Player/p1_walk/PNG/p1_walk09.png", Rect(0,0,72,97)));
    walkFrames.pushBack(SpriteFrame::create("Base pack/Player/p1_walk/PNG/p1_walk10.png", Rect(0,0,72,97)));
    walkFrames.pushBack(SpriteFrame::create("Base pack/Player/p1_walk/PNG/p1_walk11.png", Rect(0,0,72,97)));
*/

    walkFrames = buildAnimation("p1_walk%02d.png", 11);
    auto moveAnimation = Animation::createWithSpriteFrames(walkFrames, 0.3f);
    walking = Animate::create(moveAnimation);
    walking->setTag(1);
    walking->retain();
    idleFrames.pushBack(SpriteFrame::create("Base pack/Player/p1_stand.png", Rect(0,0,66,92)));
    idleFrames.pushBack(SpriteFrame::create("Base pack/Player/p1_front.png", Rect(0,0,66,92)));
    auto idleAnimation = Animation::createWithSpriteFrames(idleFrames, 3.0f);
    idling = Animate::create(idleAnimation);
    idling->retain();

    //setupAnimation("walk");
}

Player::~Player(void)
{
    walking->release();
    idling->release();
}
