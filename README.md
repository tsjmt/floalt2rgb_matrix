# floalt2rgb_matrix
## Function
浮動小数点数をM5 Atom Matrixの5x5のRGB LED matrixに2桁の整数で表示します。
以下の方法で-9x10^9 ~ 9x10^9までの値を表示可能です。
- 正数は赤色で表示、負数は青色で表示
- 0.0fのみ緑色で0と表示
- 絶対値が10未満の数は小数点を表示して整数部と小数部を1桁ずつ表示
- 絶対値が100未満の数は整数部のみを2桁で表示
- 絶対値が1*10^10未満の数は最上位一桁と、指数部（緑色）1桁で指数表示
- 範囲外の数が渡された場合その符号に合わせた色で"inf"と表示
## Usage
.inoファイルと同じディレクトリに.hファイルと.cファイルを追加して、includeしてください。
表示したい数と表示用のバッファを渡せばバッファを埋めて返します。
詳しくは'examples/simple_usage.ino'を参照してください。
## Target Board
ATOM Matrix
## Licence
MIT