# Root41

岐阜ブロック瑞浪ノード Root41の開発用リポジトリ

どんどん開発していこう！

## 開発すべきもの

### プログラム

* オフェンス用
* ディフェンス・キーパー用
* テスト用プログラム

### ハードウェア

* 設計
* 実験データ

## やることリスト

* プログラム制作
* ハードウェア設計・補修
* エントリー
* バックアップ

## 開発するときのルール

一応、お互いのGitの使い方がばらばらだと、

わけわからんことになるので、ソース開発時のルールを定めておきます。

### Gitの使い方

無駄なコミットはやめよう。管理しにくくなる。

### コンフリクト

コンフリクトが発生した場合、二人で相談し、

どちらをマージするか決めましょう。

### 開発時のブランチ

開発するときは自分用のブランチを切って開発していこう。

絶対にmasterに直接プッシュしないこと！（バージョン管理の意味がなくなる）

安定して動くプログラムができたら、プルリクエストしてください。

コンフリクトしていなければ自身でマージ可とします。（2019/3/29改正）

開発するときの大きな変更

（関数を新しく作る、内容を大幅に変える、ファイルを新しく作る等）は

一回安定して動く最終版をプルリクエスト-->マージして、

それからブランチを切って開発しよう。

ブランチ名は、

自分の名前（英語）- 開発する部分+数字

という感じにしよう。

<例>tanukichi41がボールのプログラムを作成する場合

tanukichi41-ball001

一応GitHubはインターネット上のサービスなので、

自分の名前の部分はアカウント名で統一しよう。

### コーディング

コーディングルールは別に定めた、コーディング規則.mdを読んでください。
