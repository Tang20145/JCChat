#! /bin/bash
year=$2
month=$1
#将month转换为数字

case $month in
January)
	month=1
;;
February)
	month=2
;;
March)
	month=3
;;
April)
	month=4
;;
May)
	month=5
;;
June)
	month=6
;;
July)
	month=7
;;
August)
	month=8
;;
September)
	month=9
;;
October)
	month=10
;;
November)
	month=11
;;
December)
	month=12
;;
esac

#得到日期格式的月份
res_date=$(date -d "${year}-${month}-1" )

#下一个月
res_next_month=$(date -d "$res_date 1 month" )

#计算本月多少天
count=$(( ($(date -d "$res_next_month" +%s) - $(date -d "$res_date" +%s))/(24*60*60) ))
week_day=`date -d "$res_date" +%w`

#echo $count
#echo $res_date
#echo $res_next_month
#echo $week_day

#开始输出月历
echo ""
echo -e "SUN\tMON\tTUE\tWEN\tTHU\tFRI\tSAT"
if [ $week_day -gt 0 ];then
for((i=1;i<=$week_day;i++))
do
	echo -n -e "\t"
done
fi


for((i=1;i<=$count;i++))
do
	echo -n $i
	echo -n -e "\t"
	if [ $((($week_day+$i)%7)) == 0 ];then
	echo  ""
	fi
done
echo ""
echo ""
