#pragma once
#define __IGAMESCENE_H__

enum GameSceneResultCode;
struct ID2D1RenderTarget;
//
//  class IGameScene
//
class IGameScene
{
public:
	virtual ~IGameScene() = 0;  //  �������z�f�X�g���N�^
	virtual GameSceneResultCode    move() = 0;
	virtual void    draw(ID2D1RenderTarget *pRenderTarget) = 0;
};
