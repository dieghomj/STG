#include "CGame.h"
#include <time.h>
#include <gdiplus.h>
#include "Sound.h"
#include "CSV.h"

CGame::CGame()
	: CGame( nullptr )
{
}

CGame::CGame(GameWindow* pGameWnd)
	: m_pGameWnd(pGameWnd)
	, m_pXInput(nullptr)
	, m_hMemDC(nullptr)
	, m_hFont(nullptr)
	, m_hBack(nullptr)
	, m_hChara(nullptr)
	, m_hExplosion(nullptr)
	, m_hTitle(nullptr)
	, m_hGameOver(nullptr)
	, m_hEnding(nullptr)
	, m_Alpha(0)
	, m_Scene(enScene::Title)
	, m_Score()
	, m_pPlayer(nullptr)
	, m_pPBullets(nullptr)
	, m_pPBulletsFlags(nullptr)
	, m_Enemy()
	, m_back_y()
	, m_BGMNo()
	, m_pBGM1(nullptr)
	, m_IsMuted( true )
{
}

CGame::~CGame()
{
}

//初期化関数.
void CGame::InitializeGame()
{

	//自機初期配置
	CHARA player;
	player.position.x = (WND_W / 2) - (C_SIZE / 2);
	player.position.y = WND_H - C_SIZE - 16;	//補正：画面の下からキャラ１体分＋16上の位置.
	player.state = enCharaState::Living;
	player.ExpAnimCnt = 0;
	
	m_pPlayer = new CPlayer(player);

	//自機の弾の初期配置
	m_pPlayer->PlayerBulletsInit();

	m_pPBullets = m_pPlayer->m_ps;				//*************CHANGE THIS LATER*********************
	m_pPBulletsFlags = m_pPlayer->m_ps_shotFlag;//*************CHANGE THIS LATER*********************

	//敵機の初期配置
	for (int i = 0; i < E_MAX; i++)
	{
		CHARA enemy;
		enemy.position.x = rand() % (WND_W - C_SIZE);	//ランダム出現位置を設定
		enemy.position.y = -C_SIZE;
		enemy.state = enCharaState::Living; //生存中
		enemy.ExpAnimCnt = 0;
		m_Enemy[i] = new CEnemy(enemy);

	}

	//背景スクロール初期配置
	m_back_y = WND_H;

	m_Score = 0;

}
//構築関数
bool CGame::Create()
{

	//乱数の初期化
	srand((unsigned int)time(nullptr));

	m_pXInput = new CXInput(0);	//コントローラークラスのインスタンスを生成

	//メモリDCの作成.
	m_hMemDC = CreateCompatibleDC(nullptr);

	AddFontResourceEx("Data\\Font\\BoldPixels.ttf", FR_PRIVATE, 0);

	//フォントの作成.
	m_hFont = CreateFont(
		60,	//文字高さ.
		20,	//文字幅.
		0,	//角度.
		0,	//ベースライン角度.
		FW_REGULAR,	//太さ(太字:FW_BOLD,通常:FW_REGULAR).
		FALSE,		//斜体(TRUE:有効).
		FALSE,		//下線(TRUE:有効).
		FALSE,		//打ち消し線(TRUE:有効).
		DEFAULT_CHARSET,	//文字セット.
		OUT_DEFAULT_PRECIS,	//出力精度.
		CLIP_DEFAULT_PRECIS,//クリッピング精度.
		DEFAULT_QUALITY,	//出力品質.
		VARIABLE_PITCH | FF_ROMAN,	//可変ピッチとフォントファミリ.
		"BoldPixels");		//書体(nullptr:現在使われている書体).


	//背景の読み込み.
	if (LoadBmp(&m_hBack,		"Data\\BMP\\background.bmp")	== false)	return false;
	//キャラクターの読み込み.
	if (LoadBmp(&m_hChara,		"Data\\BMP\\chara.bmp")			== false)	return false;
	//爆発の読み込み
	if (LoadBmp(&m_hExplosion,	"Data\\BMP\\explosion.bmp")		== false)	return false;
	//タイトル画像読み込み
	if (LoadBmp(&m_hTitle,		"Data\\BMP\\title.bmp")			== false)	return false;
	//	ゲームオーバー画像読み込み
	if (LoadBmp(&m_hGameOver,	"Data\\BMP\\gameover.bmp")		== false)	return false;

	if (LoadBmp(&m_hEnding,		"Data\\BMP\\ending.bmp")		== false)	return false;

	//サウンドのインスタンス生成
	m_pBGM1 = nullptr;	//nullpterで初期化
	m_pBGM1 = new CSound;	//インスタンス生成

	//MIDIまたはmp3ファイルのオープン.
	if(m_pBGM1->Open("Data\\Sound\\BGM\\title.mp3", "BGM1", m_pGameWnd->hWnd) == false ) return false;
	//if (sound_Open("Data\\Sound\\BGM\\title.mp3", "BGM1",		pGS->gameWnd->hWnd) == false) return false;
	if (sound_Open("Data\\Sound\\BGM\\gamemain.mp3", "BGM2",	m_pGameWnd->hWnd) == false) return false;
	if (sound_Open("Data\\Sound\\BGM\\gameover.mp3", "BGM3",	m_pGameWnd->hWnd) == false) return false;
	if (sound_Open("Data\\Sound\\BGM\\ending.mp3", "BGM4",		m_pGameWnd->hWnd) == false) return false;
	if (sound_Open("Data\\Sound\\SE\\explosion.mp3", "SE1",		m_pGameWnd->hWnd) == false) return false;


	//BGMの再生.
	m_pBGM1->PlayLoop();	//ループ再生.
	//sound_Play("BGM1", m_gameWnd->hWnd);

	// BGMフラグを設定.
	m_BGMNo = 1;

	//CSVデータ受け取り用
	ENEMY_CSV* pEnemyCSV = nullptr;
	//データの最大数
	int EnemyCSV_Max = 10;


	if (LoadCSV(
		"Data\\CSV\\enemy_list.csv",
		&pEnemyCSV,
		EnemyCSV_Max ) == false)
	{
		return false;
	}
	//CSVデータの削除
	DeleteCSV(&pEnemyCSV);


	return true;
}
//破棄関数
void CGame::Destroy()
{
	//mp3のクローズ.
	sound_Close("SE1",	m_pGameWnd->hWnd);
	sound_Close("BGM4", m_pGameWnd->hWnd);
	sound_Close("BGM3", m_pGameWnd->hWnd);
	sound_Close("BGM2", m_pGameWnd->hWnd);
	m_pBGM1->Close();	//BGM1のクローズ.
	//sound_Close("BGM1", m_gameWnd->hWnd);

	//BGM1の解放.
	if( m_pBGM1 != nullptr) 	{
		delete m_pBGM1;
		m_pBGM1 = nullptr;
	}

	//BITMAPの解放.
	DeleteObject(m_hEnding);
	DeleteObject(m_hGameOver);
	DeleteObject(m_hTitle);
	DeleteObject(m_hExplosion);
	DeleteObject(m_hChara);
	DeleteObject(m_hBack);


	//フォントの解放.
	DeleteObject(m_hFont);
	RemoveFontResourceEx(("Data\\Font\\BoldPixels.ttf"), FR_PRIVATE, 0);

	//メモリDCの解放.
	DeleteDC(m_hMemDC);

	//コントローラークラスの破棄
	if (m_pXInput != nullptr)
	{
		delete m_pXInput;
		m_pXInput = nullptr;
	}
}
//更新関数
void CGame::Update()
{
	m_pXInput->Update();	//コントローラの入力情報を更新
	HandleInput();

	//BGM無音する
	if (IsMuted()) {
		if (m_BGMNo != -1) {
			char buffer[5] = "BGM";
			buffer[3] = m_BGMNo + '0';
			buffer[4] = '\0';
			sound_Stop(buffer, m_pGameWnd->hWnd);
			sound_Close(buffer,m_pGameWnd->hWnd);
		}
		m_BGMNo = -1;
	}

	switch (m_BGMNo) {
	case -1:
		break;
	case 1:
		//曲の状態を取得する.
		m_pBGM1->PlayLoop();
		//sound_PlayLoop("BGM1", m_pGameWnd->hWnd);
		break;
	case 2:
		//曲の状態を取得する.
		sound_PlayLoop("BGM2", m_pGameWnd->hWnd);
		break;
	case 3:
		//曲の状態を取得する.
		sound_PlayLoop("BGM3", m_pGameWnd->hWnd);
		break;
	case 4:
		//曲の状態を取得する.
		sound_PlayLoop("BGM4", m_pGameWnd->hWnd);
		break;
	}

	//シーンごとの処理
	switch (m_Scene) {

	case enScene::Title:

		m_Alpha++;
		if (m_Alpha >= 255) {
			m_Alpha = 0;
		}

		if (IsKeyDown("Enter") ||
			m_pXInput->IsDown(CXInput::START))
		{
			m_Alpha = 0;

			m_Scene = enScene::GameMain;	//ゲームメイン

			//BGMの変更.
			m_pBGM1->Stop();	//BGM1の停止.
			//sound_Stop("BGM1", m_pGameWnd->hWnd);	//BGM1の停止
			sound_PlayLoop("BGM2", m_pGameWnd->hWnd);	//BGM1の停止
			m_BGMNo = 2;

			//初期化処理
			InitializeGame();
		}

		if (GetAsyncKeyState('P') & 0x8000)//0x0001[遅延連射], 0x8000[即連射].
		{
			Mute();
		}

		break;
	
	case enScene::GameMain:
	{
		//Get cursor position
		if (GetCursorPos(&m_CursorPos))
		{
			ScreenToClient(m_pGameWnd->hWnd, &m_CursorPos);
			m_pPlayer->UpdatePosToCursor(m_CursorPos);
		}
		else
		{
			m_CursorPos.x = 0;
			m_CursorPos.y = 0;
		}

		//F1キー.
		if (IsKeyDown("F1")) {
			//ウィンドウを閉じる通知を送る.
			PostMessage(m_pGameWnd->hWnd, WM_CLOSE, 0, 0);
		}

		//↑.
		if (IsKeyDown("Up"))
		{
			m_pPlayer->MoveUp();
		}
	
		//↓.
		if (IsKeyDown("Down"))
		{
			m_pPlayer->MoveDown();

		}
		//←.
		if (IsKeyDown("Left"))
		{
			m_pPlayer->MoveLeft();
		}

		//→.
		if (IsKeyDown("Right"))
		{
			m_pPlayer->MoveRight();
		}

		//Zキー.
		if (IsKeyDown("Z"))
		{
			m_pPlayer->Shoot();
		}

		//Enterキー.
		if (IsKeyDown("Enter"))
		{
		}

		m_pPlayer->PlayerBulletsMove();

		//敵機の動作
		for (int i = 0; i < E_MAX; i++) {
			m_Enemy[i]->Move();
		}

		//自機の生存確認.
		if (m_pPlayer->GetPlayerState() == enCharaState::Living)
		{
			for (int i = 0; i < E_MAX; i++)
			{
				VEC2 playerPos = m_pPlayer->GetPlayerPos();
				VEC2 enemyPos = m_Enemy[i]->GetPos();
				//敵機が生存している
				if (m_Enemy[i]->GetState() == enCharaState::Living) {
					if (CollsionDetection(
						playerPos.x, playerPos.y, C_SIZE, C_SIZE,
						enemyPos.x, enemyPos.y, C_SIZE, C_SIZE))
					{
						//自機
						m_pPlayer->SetPlayerState(enCharaState::Dying);	//状態を死亡中に設定.
						m_pPlayer->SetPlayerAnimCnt(0);					//爆発アニメーションカウンタを０に設定

						m_Enemy[i]->SetState(enCharaState::Dying);

						break;		//敵機と当たればfor文を抜ける
					}
					else if (m_Enemy[i]->GetPos().y >= 600) {
						//敵機が画面外に出たら
						m_Enemy[i]->ResetEnemy();
						m_pPlayer->UpdateLife(-1);	//ライフを1減らす
					}
				}

			}

		}
		//自機の爆発処理
		else if (m_pPlayer->GetPlayerState() == enCharaState::Dying)
		{
			m_pPlayer->Destroy();

			if (m_pPlayer->GetPlayerAnimCnt() > 15) {
				//死亡しているのでゲームオーバー
				m_Scene = enScene::GameOver;

				//BGMの変更.
				sound_Stop("BGM2", m_pGameWnd->hWnd);	//BGM1の停止
				m_pBGM1->PlayLoop();	//BGM1の停止
				m_BGMNo = 1;
			}
		}

		for (int eNo = 0; eNo < E_MAX; eNo++) {
			//敵機の生存確認
			if (m_Enemy[eNo]->GetState() == enCharaState::Living) {
				//自機の弾が発射されている・
				VEC2 enemyPos = m_Enemy[eNo]->GetPos();
				for (int psNo = 0; psNo < PS_MAX; psNo++) {
					if (m_pPBulletsFlags[psNo]) {
						//自機の弾と敵機の当たり判定
						if (CollsionDetection(
							m_pPBullets[psNo].x, m_pPBullets[psNo].y, C_SIZE, C_SIZE,
							enemyPos.x, enemyPos.y, C_SIZE, C_SIZE))
						{
							//命中した時
							m_Enemy[eNo]->SetState(enCharaState::Dying);

							//自機の弾の着弾後の処理
							m_pPBulletsFlags[psNo] = false;
							m_pPBullets[psNo].x = WND_W;
							m_pPBullets[psNo].y = WND_H;

							//スコア加算
							m_Score += 1;

							//爆発音を鳴らす
							//sound_PlaySE("SE1", m_pGameWnd->hWnd); *****************SET TO MUTE*********************

							break;
						}
					}
				}
			}

			//敵機の爆発処理
			else if (m_Enemy[eNo]->GetState() == enCharaState::Dying) {
				m_Enemy[eNo]->DestroyAnim();
			}
			else if (m_Enemy[eNo] != nullptr)
			{
				delete m_Enemy[eNo];
				m_Enemy[eNo] = nullptr;
			}
				

		}

		//背景スクロール処理.
		if (m_back_y > -WND_H) {

			m_back_y--;	//スクロールを減らす.
		}
		else {
			m_back_y = WND_H;	//初期値に戻す.
		}

		if (m_Score >= 300)
		{
			m_Scene = enScene::Ending;
			//BGMの変更.
			sound_Stop("BGM2", m_pGameWnd->hWnd);	//BGM1の停止
			sound_PlayLoop("BGM1", m_pGameWnd->hWnd);	//BGM1の停止
			m_BGMNo = 1;
		}
	}
	break;

	case enScene::GameOver:

		if (IsKeyDown("Enter"))
		{
			m_Scene = enScene::Title;	//ゲームメイン
		}

		break;
	case enScene::Ending:
		if (IsKeyDown("Enter"))
		{
			m_Scene = enScene::Title;	//ゲームメイン
		}
		break;

	default:		//上記以外
		break;
	}
}

void CGame::Draw()
{
	
	//シーンごとの処理
	switch (m_Scene) {

	case enScene::Title:
		//タイトル画像を表示.
		SelectObject(m_hMemDC, m_hTitle);
		BitBlt(m_pGameWnd->hScreenDC,		//デバイスコンテキスト.
			0, 0,							//表示位置x,y座標.
			WND_W, WND_H,					//画像幅、高さ.
			m_hMemDC,						//メモリDC.
			0, 0,							//元画像x,y座標.
			SRCCOPY);						//コピーする.

		//タイトル画像のPressEnterKeyを半透明で表示させるために
		//タイトル画像のピンク部分を半透明にして表示.
		AlBlend(
			m_pGameWnd->hScreenDC,			//デバイスコンテキスト
			0, 430,							//表示位置ｘ、ｙ座標
			WND_W, 32,						//画像幅、高さ
			m_hMemDC,						//メモリＤＣ
			0, 0,							//元画像ｘ、ｙ座標
			m_Alpha);						//アルファ値

		break;
	case enScene::GameMain:
	{

		//背景の画像をメモリDCへコピー.
		SelectObject(m_hMemDC, m_hBack);
		BitBlt(m_pGameWnd->hScreenDC,		//デバイスコンテキスト.
			0, 0,							//表示位置x,y座標.
			WND_W, WND_H,					//画像幅、高さ.
			m_hMemDC,						//メモリDC.
			0, m_back_y,					//元画像x,y座標.
			SRCCOPY);						//コピーする.

		

		//背景の表示(サブスクロール)
		if (m_back_y < 0) {
			BitBlt(m_pGameWnd->hScreenDC,		//デバイスコンテキスト.
				0, 0,							//表示位置x,y座標.
				WND_W, -(m_back_y),				//画像幅、高さ.
				m_hMemDC,						//メモリDC.
				0,								//元画像x座標.
				(WND_H * 2 + m_back_y),			//元画像y座標.
				SRCCOPY);						//コピーする.
		}
		
		for (int i = 0; i < PS_MAX; i++)
		{
			if (m_pPBulletsFlags[i] == true)
			{
				//キャラクターの画像をメモリDCへコピー
				SelectObject(m_hMemDC, m_hChara);
				//自機の弾の表示
				TransBlt(m_pGameWnd->hScreenDC,	//デバイスコンテキスト
					m_pPBullets[i].x, m_pPBullets[i].y,		//表示位置ｘ、ｙ座標
					C_SIZE, C_SIZE,				//画像幅、高さ
					m_hMemDC,					//メモリＤＣ
					0,							//元画像ｘ座標
					C_SIZE * 1);				//元画像ｙ座標
			}
		}
		
		m_pPlayer->Draw(m_pGameWnd->hScreenDC, m_hMemDC, m_hChara, m_hExplosion);

		for (int i = 0; i < E_MAX; i++) {
			if (m_Enemy[i] == nullptr)
				continue;
			//敵機が生きている
			if (m_Enemy[i]->GetState() == enCharaState::Living) {
				//キャラクターの画像をメモリDCへコピー
				SelectObject(m_hMemDC, m_hChara);
				VEC2 enemyPos = m_Enemy[i]->GetPos();
				//敵機の表示
				TransBlt(m_pGameWnd->hScreenDC,			//デバイスコンテキスト
					enemyPos.x, enemyPos.y,			//表示位置ｘ、ｙ座標
					C_SIZE, C_SIZE,						//画像幅、高さ
					m_hMemDC,							//メモリＤＣ
					C_SIZE * 1,
					0);									//元画像ｘ、ｙ座標
			}

			//敵機が死亡中
			else if (m_Enemy[i]->GetState() == enCharaState::Dying) {
				//爆発の画像をメモリDCへコピー
				SelectObject(m_hMemDC, m_hExplosion);
				VEC2 enemyPos = m_Enemy[i]->GetPos();
				int enemyAnimCnt = m_Enemy[i]->GetAnimCnt();
				//爆発の表示
				TransBlt(m_pGameWnd->hScreenDC,	//デバイスコンテキスト
					enemyPos.x, enemyPos.y,						//表示位置ｘ、ｙ座標
					C_SIZE, C_SIZE,				//画像幅、高さ
					m_hMemDC,						//メモリＤＣ
					(enemyAnimCnt % 8) * C_SIZE,		//元画像ｘ、ｙ座標:8で割った余りを出す
					(enemyAnimCnt / 8) * C_SIZE);		//元画像ｘ、ｙ座標:8で割った値をだす
			}
			
		}
	
	//*************************************
	// Screen UI
	//*************************************
		RECT rect;
		rect.top = 0;
		rect.left = 0;
		rect.right = WND_W/2;
		rect.bottom = WND_H/2;

		//SCORE TEXT
		char scoreText[128] = "";
		wsprintf(scoreText, "SCORE:%d", m_Score);
		HFONT hOldFont = (HFONT)SelectObject(m_pGameWnd->hScreenDC, m_hFont);
		SetBkMode(m_pGameWnd->hScreenDC, TRANSPARENT); // No background fill
		DrawTextA(m_pGameWnd->hScreenDC, scoreText, -1, &rect, DT_CENTER | DT_SINGLELINE);

		//HP TEXT
		rect.right = WND_W;
		rect.right = WND_H;
		char lifeText[128] = "";
		wsprintf(lifeText, "HP:%d", m_pPlayer->UpdateLife(0));
		DrawTextA(m_pGameWnd->hScreenDC, lifeText, -1, &rect, DT_CENTER | DT_SINGLELINE);

		SelectObject(m_pGameWnd->hScreenDC, hOldFont);


	}

	break;
	case enScene::GameOver:
		//ゲームオーバー画像を表示.
		SelectObject(m_hMemDC, m_hGameOver);
		BitBlt(m_pGameWnd->hScreenDC,		//デバイスコンテキスト.
			0, 0,			//表示位置x,y座標.
			WND_W, WND_H,	//画像幅、高さ.
			m_hMemDC,			//メモリDC.
			0, 0,			//元画像x,y座標.
			SRCCOPY);		//コピーする.
	break;
	case enScene::Ending:
		//エンディング画像を表示.
		SelectObject(m_hMemDC, m_hEnding);
		BitBlt(m_pGameWnd->hScreenDC,		//デバイスコンテキスト.
			0, 0,			//表示位置x,y座標.
			WND_W, WND_H,	//画像幅、高さ.
			m_hMemDC,			//メモリDC.
			0, 0,			//元画像x,y座標.
			SRCCOPY);		//コピーする.
		break;

	default:		//上記以外
		break;
	}
}

void CGame::HandleInput()
{
	//F1キー.
	if (GetAsyncKeyState(VK_F1) & 0x0001) {
		//ウィンドウを閉じる通知を送る.
		m_keyBoardInput["F1"] = true;
		PostMessage(m_pGameWnd->hWnd, WM_CLOSE, 0, 0);
	}
	else
		m_keyBoardInput["F1"] = false;

	//↑.
	if (GetAsyncKeyState(VK_UP) & 0x8000)//0x0001[遅延連射], 0x8000[即連射].
	{
		m_keyBoardInput["Up"] = true;
		m_pPlayer->MoveUp();
	}
	else
		m_keyBoardInput["Up"] = false;
	//↓.
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)//0x0001[遅延連射], 0x8000[即連射].
	{
		m_keyBoardInput["Down"] = true;
		m_pPlayer->MoveDown();

	}
	else
		m_keyBoardInput["Down"] = false;
	//←.
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)//0x0001[遅延連射], 0x8000[即連射].
	{
		m_keyBoardInput["Left"] = true;
		m_pPlayer->MoveLeft();
	}
	else
		m_keyBoardInput["Left"] = false;

	//→.
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)//0x0001[遅延連射], 0x8000[即連射].
	{
		m_keyBoardInput["Right"] = true;
		m_pPlayer->MoveRight();

	}
	else
		m_keyBoardInput["Right"] = false;

	//Zキー.
	if (GetAsyncKeyState('Z') & 0x8000)//0x0001[遅延連射], 0x8000[即連射].
	{
		m_keyBoardInput["Z"] = true;
		m_pPlayer->Shoot();
	}
	else
		m_keyBoardInput["Z"] = false;

	//Enterキー.
	if (GetAsyncKeyState(VK_RETURN) & 0x0001)//0x0001[遅延連射], 0x8000[即連射].
	{
		m_keyBoardInput["Enter"] = true;
	}
	else
		m_keyBoardInput["Enter"] = false;
}

bool CGame::IsKeyDown(std::string id)
{
	if (m_keyBoardInput[id] == true) return true;

	return false;
}

//矩形同士
bool CGame::CollsionDetection(
	int Ax, int Ay, int Aw, int Ah,	//矩形Aのｘ、ｙ座標と幅高さ.
	int Bx, int By, int Bw, int Bh)	//矩形Ｂのｘ、ｙ座標と幅高さ.
{
	//自機の弾と敵機の当たり判定.
	if ((Ax <= Bx + C_SIZE) && (Bx <= Ax + C_SIZE) &&
		(Ay <= By + C_SIZE) && (By <= Ay + C_SIZE))
	{
		//命中したとき.
		return true;
	}
	//外れた時.
	return false;
}

