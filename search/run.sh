#Takes the path of input xml file and output index file as arguments and builds the index on the output file

# run on all the files
cp invIndex $1
cd $1
for file in `ls`
do
      ./invIndex $file $2
done

