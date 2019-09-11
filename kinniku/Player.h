#pragma once
#ifndef __IGAMEOBJECT_H__
#include "IGameObject.h"
#endif
struct ID2D1RenderTarget;
struct ID2D1Bitmap;
struct ID2D1SolidColorBrush;

class CStage;

class CPlayer : public IGameObject
{
public:
	CPlayer(CStage *pStage);
	virtual ~CPlayer();
	virtual bool    move() override;
	virtual void    draw(ID2D1RenderTarget *pRenderTarget) override;
	virtual bool collide(float x, float y, float w, float h) override;
	virtual bool collide(IGameObject *pObj) override;
	virtual void hit(float amount) override;
	virtual float    getX() {
		return m_fX;
	}
	virtual float    getY() {
		return m_fY;
	}
	virtual float getLongTimePush() {
		return m_bLongPushSpace;
	}
protected:
	CStage		* m_pStage;
	//Player画像用
	ID2D1Bitmap *m_pBitmapP;
	ID2D1Bitmap *m_pBitmapPT;
	ID2D1Bitmap *m_pBitmapPTC;
	ID2D1Bitmap *m_pBitmapC;
	ID2D1Bitmap *m_pBitmapNT;
	ID2D1Bitmap *m_pBitmapNT2;
	ID2D1Bitmap *m_pBitmapNT3;
	ID2D1Bitmap *m_pBitmapNT4;
	//アイテム個数用
	ID2D1Bitmap *m_pBitmapItemNum;

	FLOAT		m_fX, m_fY;
	FLOAT		m_fKeyTime;
	FLOAT		m_fThrowTime;
	FLOAT		m_fItemGetScore;
	BOOL		m_bTama;
	BOOL		m_bTC;
	BOOL		m_bLongPushSpace;
	ID2D1SolidColorBrush		*m_pBrush;

};

