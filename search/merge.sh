list=(`ls ./exet`)
valu=${#list[*]}

while [ $valu -gt 1 ]
do

`cat ./exet/${list[0]} >> ./exet/${list[1]}`
`rm ./exet/${list[0]}`
list=(`ls ./exet`)
valu=${#list[*]}

done
