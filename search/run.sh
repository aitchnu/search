#Takes the path of input xml file and output index file as arguments and builds the index on the output file

# run on all the files
for file in `ls Wiki_Split_Files/`
do
    ./invIndex Wiki_Split_Files/$file $2
done

