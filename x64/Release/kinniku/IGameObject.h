#pragma once
#define __IGAMEOBJECT_H__
#include "IGameObject.h"

struct ID2D1RenderTarget;

class IGameObject
{
public:
	virtual ~IGameObject() = 0;
	virtual bool move() = 0;
	virtual void draw(ID2D1RenderTarget *pRenderTarget) = 0;
	virtual bool collide(float x, float y, float w, float h) {
		return false;
	}	virtual bool collide(float x, float w) {
		return false;
	}
	virtual bool collide(IGameObject *Obj) {
		return false;
	}
	virtual bool collide(IGameObject *Obj,int x) {
		return false;
	}
	virtual bool hitType() { return false; };
	virtual void hit(float amount) {};
	virtual bool make() { return false; };
	virtual int TreeScoreBack() { return 0; };//���̎��X�R�A��Ԃ�
	virtual float ScoreBack() { return 0; };//���̎��X�R�A��Ԃ�
	virtual bool StarHitFlag() { return 0; };//�����ۂ�
	virtual float TSPointBackx() { return 0; };//�e�̌��݈ʒux��Ԃ�
	virtual float TSPointBacky() { return 0; };//�e�̌��݈ʒuy��Ԃ�
};


//  �}�N����`
#undef SAFE_RELEASE
#undef SAFE_DELETE
#undef SAFE_DELETE_ARRAY
#define SAFE_RELEASE(o) if (o){ (o)->Release(); o = NULL; }
#define SAFE_DELETE(o)  if (o){ delete (o); o = NULL; }
#define SAFE_DELETE_ARRAY(o) if (o){ delete [] (o); o = NULL; }