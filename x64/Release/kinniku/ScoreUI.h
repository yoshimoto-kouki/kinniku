#pragma once

struct ID2D1RenderTarget;

class CScoreUI 
{
public:
	CScoreUI(ID2D1RenderTarget *pRenderTarget);
	virtual ~CScoreUI();
	void move();

	void Draw(ID2D1RenderTarget *pRenderTarget, FLOAT x, FLOAT y, FLOAT h);
	void SetScore(int score);
	INT  GetScore();
	void AddScore(int value,int score);
protected:
	ID2D1Bitmap * m_pTexture;
	int  m_iScore;
};