#do Splitting
mkdir Wiki_Split_Files
cd Wiki_Split_Files
split -b 100M --suffix-length=4 ../$1
cd ..
python quantizing.py "Wiki_Split_Files/"

#compile files
g++ invIndex.cpp -O3 -w -o invIndex
