#!/bin/sh
# 課題D カレント内の*.cppファイルを、名前・メアドがあれば変更し、trailing whitespaceを削除し、*.ccに変更するスクリプト。
for file in `find . -name "*.cpp"`
do
  cat ${file} | sed -e '2s/NEET the 3rd/Ryo Nakabayashi/g' -e '2s/neet3@example.com/ryonakabayashi@gmail.com/g' -e 's/[<space><tab>]*$//g' > ${file%%.cpp}.cc
  rm ${file}
done
