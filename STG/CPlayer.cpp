#include "CPlayer.h"
#include "CRender.h"
#include "math.h"

CPlayer::CPlayer()
	:CPlayer( CHARA{} )
{
}

CPlayer::CPlayer(CHARA player)
	: m_Player(player)
	, m_ps()
	, m_ps_shotFlag()
	, m_Sight({0,0})
	, m_playerSprite ( {0,0} )
	, m_rotationAngle (0.0f)
	, m_Life	(10)
{ 

}

CPlayer::~CPlayer()
{
}

void CPlayer::Shoot()
{
	for (int i = 0; i < PS_MAX; i++)
	{
		//弾が発射されていない
		if (m_ps_shotFlag[i] == false)
		{
			//自機の位置に弾を揃える
			m_ps[i].x = m_Player.position.x;
			m_ps[i].y = m_Player.position.y;
			m_ps_shotFlag[i] = true;		//発射フラグを立てる.

			break;
		}
	}
}

void CPlayer::MoveUp()
{
	m_Player.position.y -= P_SPD;

}

void CPlayer::MoveLeft()
{
	m_Player.position.x -= P_SPD;
}

void CPlayer::MoveRight()
{
	m_Player.position.x += P_SPD;
}

void CPlayer::MoveDown()
{
	m_Player.position.y += P_SPD;
}

void CPlayer::PlayerBulletsInit()
{
	//自機の弾の初期配置
	for (int i = 0; i < PS_MAX; i++)	//自機の弾の数だけ処理する
	{
		m_ps[i].x = WND_W;
		m_ps[i].y = WND_H;
		m_ps_shotFlag[i] = false;
	}
}

void CPlayer::PlayerBulletsMove()
{
	//自機の弾の動作.
	for (int i = 0; i < PS_MAX; i++)
	{
		//自機の弾が発射中なら.
		if (m_ps_shotFlag[i] == true)
		{
			m_ps[i].y -= PS_SPD;	//弾を上へ移動

			//画面の外へ出た
			if (m_ps[i].y < -C_SIZE) {
				m_ps[i].x = WND_W;
				m_ps[i].y = WND_H;
				m_ps_shotFlag[i] = false;		//発射フラグ降ろす
			}
		}
	}
}

void CPlayer::Destroy()
{
	m_Player.ExpAnimCnt++;

	if (m_Player.ExpAnimCnt > 15)//爆発画像が0~15の１６コマ.
	{
		m_Player.state = enCharaState::Living;		//状態を生存中
		//初期位置に戻す
		m_Player.position.x = (WND_W / 2) - (C_SIZE / 2);
		m_Player.position.y = WND_H - (C_SIZE + 16);
	}
}

void CPlayer::Draw(HDC hdc, HDC hMemDc, HBITMAP hChara, HBITMAP hExplosion) {
	
	VEC2 playerPos = GetPlayerPos();
	VEC2 playerSprite = GetPlayerSpritePos();
	int playerAnimCnt = GetPlayerAnimCnt();

	//自機が生存中
	if (GetPlayerState() == enCharaState::Living) {
		//キャラクターの画像をメモリDCへコピー
		SelectObject(hMemDc, hChara);
		//自機の表示
		TransBlt(hdc,	//デバイスコンテキスト
			playerPos.x, playerPos.y,						//表示位置ｘ、ｙ座標
			C_SIZE, C_SIZE,									//画像幅、高さ
			hMemDc,											//メモリＤＣ
			playerSprite.x, playerSprite.y);				//元画像ｘ、ｙ座標
		

	// *******************************
	//Drawing Aimsight
	// *******************************
		//int radius = 5;
		//int left	= m_Sight.x - radius;
		//int top		= m_Sight.y - radius;
		//int right	= m_Sight.x + radius;
		//int bottom	= m_Sight.y + radius;

		//HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 255));
		//HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
		//// Draw the circle
		//Ellipse(hdc, left, top, right, bottom);
		//// Clean up
		//SelectObject(hdc, hOldBrush);
		//DeleteObject(hBrush);

	}

	//自機が死亡中
	else if (GetPlayerState() == enCharaState::Dying) {
		//爆発の画像をメモリDCへコピー
		SelectObject(hMemDc, hExplosion);
		//爆発の表示
		TransBlt(hdc,										//デバイスコンテキスト
			playerPos.x, playerPos.y,						//表示位置ｘ、ｙ座標
			C_SIZE, C_SIZE,									//画像幅、高さ
			hMemDc,											//メモリＤＣ
			(playerAnimCnt % 8) * C_SIZE,					//元画像ｘ、ｙ座標:8で割った余りを出す
			(playerAnimCnt / 8) * C_SIZE);					//元画像ｘ、ｙ座標:8で割った値をだす
	}


}

void CPlayer::UpdatePosToCursor(POINT cursor)
{
	m_Player.position.x = cursor.x - (C_SIZE / 2);
	m_Player.position.y = cursor.y - (C_SIZE / 2);

	VEC2 center = Normalize(m_Player.position);
	VEC2 target = Normalize({ cursor.x,cursor.y });

	/*m_Sight.x = m_Player.position.x + C_SIZE * 0.5;
	m_Sight.y = m_Player.position.y + C_SIZE * 0.7;*/

	//int radius = sqrt((m_Sight.x - m_Player.position.x) * (m_Sight.x - m_Player.position.x) + (m_Sight.y - m_Player.position.y) * (m_Sight.y - m_Player.position.y));
	//
	//m_Sight.x = cx + v.x * radius;
	//m_Sight.y = cy + v.y * radius;
}

int CPlayer::UpdateLife(int dmg)
{
	m_Life += dmg;
	return m_Life;
}

VEC2 Normalize(VEC2 v) {
	int lnt = sqrt(v.x * v.x + v.y * v.y);
	v.x = v.x / lnt;
	v.y = v.y / lnt;

	return v;
}






















////自機が生存中
//if (m_pPlayer->GetPlayerState() == enCharaState::Living) {
//	VEC2 playerPos = m_pPlayer->GetPlayerPos();
//	VEC2 playerSprite = m_pPlayer->GetPlayerSpritePos();
//	//キャラクターの画像をメモリDCへコピー
//	SelectObject(m_hMemDC, m_hChara);
//	//自機の表示
//	TransBlt(m_pGameWnd->hScreenDC,	//デバイスコンテキスト
//		playerPos.x, playerPos.y,		//表示位置ｘ、ｙ座標
//		C_SIZE, C_SIZE,				//画像幅、高さ
//		m_hMemDC,					//メモリＤＣ
//		playerSprite.x, playerSprite.y);						//元画像ｘ、ｙ座標
//}
//
////自機が死亡中
//else if (m_pPlayer->GetPlayerState() == enCharaState::Dying) {
//	VEC2 playerPos = m_pPlayer->GetPlayerPos();
//	int playerAnimCnt = m_pPlayer->GetPlayerAnimCnt();
//	//爆発の画像をメモリDCへコピー
//	SelectObject(m_hMemDC, m_hExplosion);
//	//爆発の表示
//	TransBlt(m_pGameWnd->hScreenDC,					//デバイスコンテキスト
//		playerPos.x, playerPos.y,						//表示位置ｘ、ｙ座標
//		C_SIZE, C_SIZE,								//画像幅、高さ
//		m_hMemDC,									//メモリＤＣ
//		(playerAnimCnt % 8) * C_SIZE,			//元画像ｘ、ｙ座標:8で割った余りを出す
//		(playerAnimCnt / 8) * C_SIZE);		//元画像ｘ、ｙ座標:8で割った値をだす
//}