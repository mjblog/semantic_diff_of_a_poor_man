#!/bin/bash
#A=`./tt ./tt.c |awk '{print $4,$7,$9,$14}'`
CLEAN_TEMPFILE()
{
	rm ${old_parse}
	rm ${new_parse}
	rm ${old_buf}
	rm ${new_buf}
	rm ${diff_result}
}

#set -x
trap 'CLEAN_TEMPFILE' INT
old_parse=`mktemp`
new_parse=`mktemp`
old_buf=`mktemp`
new_buf=`mktemp`
diff_result=`mktemp`
old_file=$1
new_file=$2
parser=`dirname $0`/parse
${parser} ${old_file} > $old_parse 2>/dev/null
${parser} ${new_file} > $new_parse 2>/dev/null
last_line_in_old=1
last_line_in_new=1
while read line
do
#	echo $line
	set $line
	func_name=$4
	file_name=$7
	start=$9
	end=${14}
	#echo "n:${func_name} f:${file_name} s:${start} e:${end}"
	cur_start_line=${start%%:*}
	sed -n "${last_line_in_old},$((cur_start_line-1))p" ${old_file} > ${old_buf}

	while read line_new
	do
		set $line_new
		func_name_new=$4
#		file_name=$7
		start_new=$9
		cur_start_line_new=${start_new%%:*}
		if [ "${func_name_new}" == "${func_name}" ] && [ ${cur_start_line_new} -ge ${last_line_in_new} ] ;then
			sed -n "${last_line_in_new},$((cur_start_line_new-1))p" ${new_file} > ${new_buf}
			#echo "oldxxxx" 
			#cat ${old_buf}
			#echo "newxxxx" 
			#cat ${new_buf}
			sdiff ${old_buf} ${new_buf}  >> ${diff_result}
			last_line_in_new=$((cur_start_line_new))
			break
		fi
		#echo "new yyyyyyyyyyyyyyy"
	done < ${new_parse}

	last_line_in_old=$((cur_start_line))
done < ${old_parse}



sed -n "${last_line_in_old},$$p" ${old_file} > ${old_buf}
sed -n "${last_line_in_new},$$p" ${new_file} > ${new_buf}
sdiff ${old_buf} ${new_buf}  >> ${diff_result}
cat ${diff_result}
CLEAN_TEMPFILE
