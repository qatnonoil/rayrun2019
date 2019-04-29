# レイトレ徒競走 ～交差エンジン編～

## ルール
- 入力データに対するAO画像を素早く出力するrayRun()を実装してください。
- 必ずリファレンス画像から一定以内のRMSEに抑えてください。
- ビルド済みのdllを提出してください。

## 実装に関して
- rayRun()関数は何度も呼び出されます。
- 本番はimageポインタを観れるUIで観戦します。途中結果もできるだけimageに書き込んでください。

## 禁止事項
- GPUは使用しないでください
- Embreeなどサードパーティの交差判定ライブラリは使用しないでください。
- 交差判定以外であればサードパーティライブラリは指定しても構いません。

## 評価方法
- 全てのrayRun()の合計値の短さを競いいます。
- 許容されるRMSEの値は今後指定がかかります。

## 未定事項
- 評価方法。テスト時間の平均？MSEは評価に加える？
- 使用可能なSIMD拡張命令セットは未定です。

## 参考実装のビルド方法

```
mkdir rayrun
cd rayrun
git clone https://github.com/qatnonoil/rayrun2019.git
git submodule update --init
build.bat
```