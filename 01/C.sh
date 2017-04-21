#!/bin/sh
# 課題B
# ループ seq 1 10
for i in `seq -f %02g 0 99`
do
  #echo ${i}
  wget "http://pf.is.s.u-tokyo.ac.jp/jp/class/syspro/kadai1/1.pdf.${i}"
done

cat `seq -f 1.pdf.%02g 0 99` > 1.pdf
printf "combined into 1.pdf"
