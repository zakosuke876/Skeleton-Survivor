#pragma once
#include <vector>

// 分割して読み込むスプライトシートの種類
enum class SheetType {
	FIRE_SPELL,             // 炎魔法
	SHEET_TYPE_MAX,         // 配列サイズとして扱う
};

// スプライトシート1枚当たりの分割設定
struct SheetData {
	const char* graphPath;  // シートのファイルパス
	int allNum;             // 総コマ数
	int divX;               // 横の分割数
	int divY;               // 縦の分割数
	int frameW;             // 1コマの横幅
	int frameH;             // 1コマの縦幅
};

// エフェクト1種類ごとの再生設定
struct EffectData {
	SheetType sheet;        // 使用するスプライトシート
	int startFrame;         // 再生開始コマ番号
	int endFrame;           // 再生終了コマ番号
	float frameInterval;    // 1コマの表示時間(秒)
	float scale;            // 描画時の拡大倍率
	bool loop;              // ループ再生するか
};


extern const EffectData EFFECT_DATA_TABLE[];
extern const SheetData   SHEET_DATA_TABLE[];

class EffectManager {
public:

	EffectManager();
	~EffectManager();
	void Reset();

	void Update();
	void Draw() const;

private:

	void PlayEffect();

	// 再生中のエフェクトを管理する
	//std::vector<Effect> effects;

	// シートごとの分割済み画像ハンドル
	//std::vector<int> sheetHandles[ToInt(SheetType::SHEET_TYPE_MAX)];
};