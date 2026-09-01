# プロジェクト概要
DxLib (C++) を使った 3Dアクション（サバイバー系）。JSONでデータ駆動。

# 技術スタック
- 言語: C++
- ライブラリ: DxLib（3D/2D）、JSONは nlohmann/json（include/json.hpp）
- IDE: Visual Studio（ソリューションでビルド、x64）
- 文字コード: ソースコードは Shift_JIS。UTF-8化しない。新規ソースも Shift_JIS で揃える
  （このCLAUDE.md自体はUTF-8で問題ない）

# 設計・作風（私の既存コードに合わせる）
- 依存は「参照渡し（Type&）」でコンストラクタ注入する。生ポインタや new/delete は避け、
  基本はスタック管理。DxLibのハンドルは int で持つ
- const 正しさを守る：描画/getterは const、受け取りは const Type&
- 状態は enum + switch のステートマシンで管理する
- シーンは Scene 基底クラス（OnEnter/OnExit/Update/Draw）を継承して多態で扱う
- オブジェクトはマネージャー（EnemyManager / ItemManager / SoundManager 等）で集約管理する
- 調整値（プレイヤー/敵/スポーン/ドロップ設定等）は JSON に外出しし、
  専用 DataLoader / Config構造体（PlayerStatus 等）で読む
- マジックナンバー/文字列は static constexpr の定数にまとめる
- ヘッダは前方宣言で include を減らす

# 命名
- 型・関数・メソッド: PascalCase
- メンバ変数・ローカル変数: camelCase（_プレフィックス無し）
- 定数: UPPER_SNAKE_CASE（static constexpr）
- 列挙: 新規は enum class（値は PascalCase）。既存のC形式 enum（PLAYER_XXX 等）に合わせる箇所はそれに従う

# DxLib固有の注意
- エントリは WinMain。DxLib_Init と DxLib_End は必ず対。
  ゲーム本体（Game）は {} スコープで包み、DxLib_End より前に破棄する
- 画像/音/モデルのハンドル(int)は使用後に解放する
  （DeleteGraph / DeleteSoundMem / MV1DeleteModel 等）。未ロードは -1 で表す
- 3D描画はZバッファ切替（SetUseZBuffer3D/SetWriteZBuffer3D）で挟み、2D UIは切ってから描く

# 補足
- 全プロジェクト共通の進め方・提案の出し方・目標は ~/.claude/CLAUDE.md 側にある
